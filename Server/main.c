#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "net.h"
#include "message.h"

#include "sget.h"
#include "ssend.h"
#include "shelp.h"

int main(int argc, char *argv[])
{
  struct sockaddr_in sockaddserver, sockaddclient;
  int s, slen=sizeof(sockaddclient);
  char buf[BUFLEN];
  unsigned int *seqhash = malloc(HASHLEN*sizeof(int));

  if (argc < 2)
    diep("Not enough arguments.");

  s = mksocket();

  sockaddserver = mksockaddserver(s, argv[1]);

  while(1)
  {
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr * restrict) &sockaddclient, (socklen_t * restrict) &slen)==-1)
      diep("Failed on recieving.");
    int pid = fork();
    if(pid < 0)
      diep("fork");
    if(pid == 0)
    {
      printf("bs1\n");
      Message mes = atomes(buf);
      printf("%d%d%d\n", mes.type, GET, SEND);
      if(mes.type == GET)
        actGet(mes, s, sockaddclient, seqhash);
      else if(mes.type == SEND)
        actSend(mes, s, sockaddclient);
      else if(mes.type == ACK)
      {
        unsigned int hash = qhash(mes.source);
        if(*(seqhash+hash) < mes.seq)
          *(seqhash+hash) = mes.seq;
      }

      exit(0);
    }
    /*printf("Received packet from %s:%d\nData: %s\n\n", 
                inet_ntoa(sockaddclient.sin_addr), ntohs(sockaddclient.sin_port), buf);*/
  }

  free(seqhash);
  close(s);
  return 0;
}
