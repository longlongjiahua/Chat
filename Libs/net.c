#include "net.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct sockaddr_in mksockaddclient(char* serveraddr, char* port)
{
  struct sockaddr_in sockadd;

  memset((char *) &sockadd, 0, sizeof(sockadd));
  sockadd.sin_family = AF_INET;
  sockadd.sin_port = htons(atoi(port));
  if (inet_aton(serveraddr, &sockadd.sin_addr)==0)
    diep("Failed on server address.");

  return sockadd;
}

struct sockaddr_in mksockaddserver(int s, char* port)
{
  struct sockaddr_in sockserver;
  memset((char *) &sockserver, 0, sizeof(sockserver));
  sockserver.sin_family = AF_INET;
  sockserver.sin_port = htons(atoi(port));
  sockserver.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(s, (const struct sockaddr *) &sockserver, sizeof(sockserver))==-1)
    diep("Failed to bind server.");

  return sockserver;
}

int mksocket()
{      
  int s;
  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("Failed to make socket.");
  return s;
}

void diep(char *s)
{
  perror(s);
  exit(1);
}
