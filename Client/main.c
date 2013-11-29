#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "net.h"

int main(int argc, char *argv[])
{
  struct sockaddr_in sockadd;
  int s, slen=sizeof(sockadd);
  char buf[BUFLEN];
  char err[BUFLEN];

  if(argc < 4)
    diep("Not enough arguments.");

  s = mksocket();

  sockadd = mksockaddclient(argv[1], argv[2]);

  if (strlen(argv[3]) >= BUFLEN) {
    sprintf(err, "Please make your message smaller than %zu (from %i)\n", strlen(argv[3]), BUFLEN);
    diep(err);
  }

  sprintf(buf, "%s\n", argv[3]);

  printf("%s\n", buf);
  if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &sockadd, slen)==-1)
    diep("message failed");

  while(1)
  {
    if(recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*) &sockadd, (socklen_t * restrict) &slen)==-1)
    {}
    else {
      printf("%s\n", buf);
      break;
    }
  }

  close(s);
  return 0;
}
