#ifndef NET_H
#define NET_H

#define BUFLEN 512

#include <sys/socket.h>

void diep(char *s);
int mksocket();
struct sockaddr_in mksockaddclient(char* serveraddr, char* port);
struct sockaddr_in mksockaddserver(int s, char* port);

#endif
