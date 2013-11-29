#include "sget.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void forwardmessage(int s, unsigned int const hash, unsigned int *seqhash, struct sockaddr_in const * const sockadd, Message const mes)
{
  char buf[BUFLEN];
  mestoa(&buf, &mes);

  while(1)
  {
    if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) sockadd, sizeof((*sockadd)))==-1)
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
}

