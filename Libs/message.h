#ifndef MESSAGE_H
#define MESSAGE_H

#include "net.h"

typedef struct Message
{
  int seq;
  char source[20];
  char destination[20];
  char message[BUFLEN];
} Message;

char* mestoa(Message message);
Message atomes(char* mes);

#endif
