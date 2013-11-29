#ifndef SGET_H
#define SGET_H

#include <netinet/in.h>

#include "message.h"

void forwardmessage(int s, unsigned int const hash, unsigned int *seqhash, struct sockaddr_in const * const sockadd, Message const mes);
void walkstoredmessages(int s, char const * const destination, struct sockaddr_in const * const sockadd, unsigned int *seqhash);
void actGet(Message mes, int s, struct sockaddr_in sockadd, unsigned int *seqhash);

#endif
