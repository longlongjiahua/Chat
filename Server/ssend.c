#include "ssend.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

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

void actSend(Message mes, int s, struct sockaddr_in sockadd)
{
  /*printf("Message!\n\tFrom: %s@%s:%d\n\tTo: %s\n\t%s\n"*/
      /*, mes.source*/
      /*, inet_ntoa(sockadd.sin_addr)*/
      /*, ntohs(sockadd.sin_port)*/
      /*, mes.destination*/
      /*, mes.message*/
      /*);*/

  storemessage(&mes);
  ack(s, &sockadd);
}
