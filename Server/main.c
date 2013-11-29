#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "net.h"

void react(int sock, char buf[], struct sockaddr_in si_other, int slen)
{
  printf("Received packet from %s:%d\nData: %s\n\n", 
      inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);


}

int main(int argc, char *argv[])
{
  struct sockaddr_in sockaddserver, si_other;
  int s, slen=sizeof(si_other);
  char buf[BUFLEN];

  if (argc < 2)
    diep("Not enough arguments.");

  s = mksocket();

  sockaddserver = mksockaddserver(s, argv[1]);

  while(1)
  {
    if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr * restrict) &si_other, (socklen_t * restrict) &slen)==-1)
      diep("recvfrom()");
    int pid = fork();
    if(pid < 0)
      diep("fork");
    if(pid == 0)
    {
      react(s, buf, si_other, slen);
      sleep(5);
      printf("sending ack.\n");
      sprintf(buf, "recieved your message!\n");
      if (sendto(s, buf, strlen(buf), 0, (struct sockaddr*) &si_other, slen)==-1)
        diep("failed to send");
      exit(0);
    }
    /*printf("Received packet from %s:%d\nData: %s\n\n", 
                inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);*/
  }

  close(s);
  return 0;
}
