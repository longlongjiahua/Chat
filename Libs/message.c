#include "message.h"

#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>

char* mestoa(Message message)
{}

Message atomes(char* mes)
{
  Message message;
  char* src = malloc(strlen(mes));
  strcpy(src, mes);

  char delims[1] = {7};

  message.seq = ntohl(atoi(strtok(src, delims)));
  strcpy(message.source, strtok(NULL, delims));
  strcpy(message.destination, strtok(NULL, delims));
  strcpy(message.message, strtok(NULL, delims));

  return message;
}
