#ifndef MESSAGE_H
#define MESSAGE_H

#include "net.h"

typedef struct Message
{
  char destination[20];
  char source[20];
  int seq;
  char message[BUFLEN];
} Message;

char* mestoa(Message message);
Message atomes(char* mes);

#endif
