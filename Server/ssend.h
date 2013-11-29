#ifndef SSEND_H
#define SSEND_H

#include <netinet/in.h>

#include "message.h"

void ack(int s, struct sockaddr_in const * const sockadd);
void storemessage(Message const * const mes);
void actSend(Message mes, int s, struct sockaddr_in sockadd);

#endif
