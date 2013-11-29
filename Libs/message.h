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

void mestoa(char* const buf, Message const * const message);
Message atomes(char const * const mes);

char delims[1] = {7};

#endif
