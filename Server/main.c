#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "net.h"
#include "message.h"

#define HASHLEN 1000000

unsigned int qhash(char *str)
{
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash%HASHLEN;
}

void ack(int s, struct sockaddr_in const * const sockadd)
{
  Message mes = {0, ACK, "null", "null", "null"};
  char buf[BUFLEN];
  mestoa(&buf, &mes);

  if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) sockadd, sizeof(*sockadd))==-1)
    diep("failed to ack");
}

void storemessage(Message const * const mes)
{  
  FILE *fp;
  fp = fopen(mes->destination, "a+");
  fprintf(fp, "%s%c%s%c", mes->source, UNITSEP, mes->message, RECORDSEP);
  fclose(fp);
}

void forwardmessage(int s, unsigned int const hash, unsigned int *seqhash, struct sockaddr_in const * const sockadd, Message const mes)
{
  char buf[BUFLEN];
  mestoa(&buf, &mes);

  while(1)
  {
    if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) sockadd, sizeof(*sockadd))==-1)
      diep("failed to send.");

    sleep(2);

    if(*(seqhash+hash) >= mes.seq)
      break;
  }
}

void walkstoredmessages(int s, char const * const destination, struct sockaddr_in const * const sockadd, unsigned int *seqhash)
{
  FILE *fp;
  Message mes;
  fp = fopen(destination, "r");

  char parsestring[BUFLEN];
  sprintf(parsestring, "%%%d[^%c]%c%%%d[^%c]%c", BUFLEN, UNITSEP, UNITSEP, BUFLEN, RECORDSEP, RECORDSEP);

  for (int i = 0; fscanf(fp, parsestring, mes.source, mes.message) != EOF; ++i)
  {
    mes.seq = i;
    mes.type = SEND;
    strcpy(mes.destination, destination);
    unsigned int hash = qhash(destination);

    int pid = fork();
    if(pid<0)
      diep("fork");
    if(pid==0)
    {
      forwardmessage(s, hash, seqhash, sockadd, mes);
      exit(0);
    }
    printf("%d | %s : %s\n", i, mes.source, mes.message);
  }

  fclose(fp);
}

void actGet(Message mes, int s, struct sockaddr_in sockadd, unsigned int *seqhash)
{
  printf("Get request!\n\tFrom: %s@%s:%d\n"
      , mes.source
      , inet_ntoa(sockadd.sin_addr)
      , ntohs(sockadd.sin_port)
      );

  unsigned int hash = qhash(mes.source);
  *(seqhash+hash) = 0;

  walkstoredmessages(s, mes.source, &sockadd, seqhash);
  ack(s, &sockadd);
}

void actSend(Message mes, int s, struct sockaddr_in sockadd)
{
  printf("Message!\n\tFrom: %s@%s:%d\n\tTo: %s\n\t%s\n"
      , mes.source
      , inet_ntoa(sockadd.sin_addr)
      , ntohs(sockadd.sin_port)
      , mes.destination
      , mes.message
      );

  storemessage(&mes);
  ack(s, &sockadd);
}

int main(int argc, char *argv[])
{
  struct sockaddr_in sockaddserver, sockaddclient;
  int s, slen=sizeof(sockaddclient);
  char buf[BUFLEN];
  unsigned int *seqhash = malloc(HASHLEN*sizeof(int));

  if (argc < 2)
    diep("Not enough arguments.");

  s = mksocket();

  sockaddserver = mksockaddserver(s, argv[1]);

  while(1)
  {
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr * restrict) &sockaddclient, (socklen_t * restrict) &slen)==-1)
      diep("Failed on recieving.");
    int pid = fork();
    if(pid < 0)
      diep("fork");
    if(pid == 0)
    {
      printf("bs1\n");
      Message mes = atomes(buf);
      printf("%d%d%d\n", mes.type, GET, SEND);
      if(mes.type == GET)
        actGet(mes, s, sockaddclient, seqhash);
      else if(mes.type == SEND)
        actSend(mes, s, sockaddclient);
      else if(mes.type == ACK)
      {
        unsigned int hash = qhash(mes.source);
      }

      exit(0);
    }
    /*printf("Received packet from %s:%d\nData: %s\n\n", 
                inet_ntoa(sockaddclient.sin_addr), ntohs(sockaddclient.sin_port), buf);*/
  }

  free(seqhash);
  close(s);
  return 0;
}
