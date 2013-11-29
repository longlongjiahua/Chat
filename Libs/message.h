#ifndef MESSAGE_H
#define MESSAGE_H

#include "net.h"

enum msgtype
{
  GET,
  SEND,
  ACK
};

typedef struct Message
{
  int seq;
  enum msgtype type;
  char source[20];
  char destination[20];
  char message[BUFLEN];
} Message;

void mestoa(char* const buf, Message const * const message);
Message atomes(char const * const mes);

#endif
