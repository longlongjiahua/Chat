#include "message.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>

char delims[1] = {UNITSEP};

void mestoa(char* const buf, Message const * const message)
{
  char num[20];

  strcpy(buf, "");

  sprintf(num, "%d", htonl(message->seq));
  strcat(buf, num);
  strcat(buf, delims);

  sprintf(num, "%d", htonl(message->type));
  strcat(buf, num);
  strcat(buf, delims);

  strcat(buf, message->source);
  strcat(buf, delims);

  strcat(buf, message->destination);
  strcat(buf, delims);

  strcat(buf, message->message);
  strcat(buf, delims);
}

Message atomes(char const * const mes)
{
  Message message;
  char* src = malloc(strlen(mes));
  strcpy(src, mes);

  message.seq = ntohl(atoi(strtok(src, delims)));
  message.type = ntohl(atoi(strtok(NULL, delims)));
  strcpy(message.source, strtok(NULL, delims));
  strcpy(message.destination, strtok(NULL, delims));
  printf("bs2\n");
  strcpy(message.message, strtok(NULL, delims));

  free(src);

  return message;
}

