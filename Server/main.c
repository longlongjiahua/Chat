#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "net.h"

void react(int sock, char buf[], struct sockaddr_in sockaddclient, int slen)
{
  printf("Received packet from %s:%d\nData: %s\n\n", 
      inet_ntoa(sockaddclient.sin_addr), ntohs(sockaddclient.sin_port), buf);


}

int main(int argc, char *argv[])
{
  struct sockaddr_in sockaddserver, sockaddclient;
  int s, slen=sizeof(sockaddclient);
  char buf[BUFLEN];

  if (argc < 2)
    diep("Not enough arguments.");

  s = mksocket();

  sockaddserver = mksockaddserver(s, argv[1]);

  while(1)
  {
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr * restrict) &sockaddclient, (socklen_t * restrict) &slen)==-1)
      diep("recvfrom()");
    int pid = fork();
    if(pid < 0)
      diep("fork");
    if(pid == 0)
    {
      react(s, buf, sockaddclient, slen);
      sleep(5);
      printf("sending ack.\n");
      sprintf(buf, "recieved your message!\n");
      if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &sockaddclient, slen)==-1)
        diep("failed to send");
      exit(0);
    }
    /*printf("Received packet from %s:%d\nData: %s\n\n", 
                inet_ntoa(sockaddclient.sin_addr), ntohs(sockaddclient.sin_port), buf);*/
  }

  close(s);
  return 0;
}
