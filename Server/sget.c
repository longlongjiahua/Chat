#include "sget.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shelp.h"

void forwardmessage(int s, unsigned int const hash, int shmkey, struct sockaddr_in const * const sockadd, Message const mes)
{
  char buf[BUFLEN];
  mestoa(&buf, &mes);
  unsigned int *seqhash;
  int shmid;

  if ((shmid = shmget(shmkey, HASHLEN*sizeof(int), 0666)) < 0) {
    perror("shmget");
    exit(1);
  }
  if ((seqhash = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    exit(1);
  }

  while(1)
  {
    if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) sockadd, sizeof((*sockadd)))==-1)
      diep("failed to send.");

    sleep(2);
    
    printf("curack: %d | %d >= %d\n", (seqhash+hash), *(seqhash+hash), mes.seq);

    if(*(seqhash+hash) >= mes.seq)
      break;
  }
}

void walkstoredmessages(int s, char const * const destination, struct sockaddr_in const * const sockadd, int shmkey)
{
  FILE *fp;
  Message mes;
  fp = fopen(destination, "r");

  char parsestring[BUFLEN];
  sprintf(parsestring, "%%%d[^%c]%c%%%d[^%c]%c", BUFLEN, UNITSEP, UNITSEP, BUFLEN, RECORDSEP, RECORDSEP);

  unsigned int *seqhash;
  int shmid;

  if ((shmid = shmget(shmkey, HASHLEN*sizeof(int), 0666)) < 0) {
    perror("shmget");
    exit(1);
  }
  if ((seqhash = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    exit(1);
  }

  unsigned int hash = qhash(destination);
  char buf[BUFLEN];
  int i;
  for (i = 0; fscanf(fp, parsestring, mes.source, mes.message) != EOF; ++i)
  {
    mes.seq = i;
    mes.type = SEND;
    strcpy(mes.destination, destination);
/*
  printf("Message!\n\tFrom: %s@%s:%d\n\tTo: %s\n\t%s\n"
      , mes.source
      , inet_ntoa(sockadd->sin_addr)
      , ntohs(sockadd->sin_port)
      , mes.destination
      , mes.message
      );*/

    int pid = fork();
    if(pid<0)
      diep("fork");
    if(pid==0)
    {
      forwardmessage(s, hash, shmkey, sockadd, mes);
      exit(0);
    }
    printf("%d | %s : %s\n", i, mes.source, mes.message);
  }

  fclose(fp);

  while(*(seqhash+hash) < i-1)
  {
    printf("waiting for %d < %d\n", *(seqhash+hash), i-1);
    sleep(1);
  }
  mes.type = ACK;
  mes.seq = 0;
  strcpy(mes.message, "null");
  mestoa(&buf, &mes);
  printf("KILLCILENTACK\n");
  sendto(s, buf, BUFLEN, 0, (struct sockaddr*) sockadd, sizeof((*sockadd)));

}

void actGet(Message mes, int s, struct sockaddr_in sockadd, int shmkey)
{
  printf("Get request!\n\tFrom: %s@%s:%d\n"
      , mes.source
      , inet_ntoa(sockadd.sin_addr)
      , ntohs(sockadd.sin_port)
      );
  unsigned int *seqhash;
  int shmid;

  if ((shmid = shmget(shmkey, HASHLEN*sizeof(int), 0666)) < 0) {
    perror("shmget");
    exit(1);
  }
  if ((seqhash = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    exit(1);
  }

  unsigned int hash = qhash(mes.source);
  *(seqhash+hash) = 0;

  walkstoredmessages(s, mes.source, &sockadd, shmkey);
}

