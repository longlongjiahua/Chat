#ifndef SGET_H
#define SGET_H

#include <netinet/in.h>

#include "message.h"

void forwardmessage(int s, unsigned int const hash, int shmkey, struct sockaddr_in const * const sockadd, Message const mes);
void walkstoredmessages(int s, char const * const destination, struct sockaddr_in const * const sockadd, int shmkey);
void actGet(Message mes, int s, struct sockaddr_in sockadd, int shmkey);

#endif
