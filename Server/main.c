#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
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
  struct sockaddr_in si_me, si_other;
  int sock, i, slen=sizeof(si_other);
  char buf[BUFLEN];

  if (argc < 2)
    diep("Not enough arguments.");

  if ((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");

  memset((char *) &si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(atoi(argv[1]));
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(sock, &si_me, sizeof(si_me))==-1)
    diep("bind");

  while(1)
  {
    if (recvfrom(sock, buf, BUFLEN, 0, &si_other, &slen)==-1)
      diep("recvfrom()");
    int pid = fork();
    if(pid < 0)
      diep("fork");
    if(pid == 0)
    {
      react(sock, buf, si_other, slen);
      sleep(5);
      printf("sending ack.\n");
      sprintf(buf, "recieved your message!\n");
      if (sendto(sock, buf, strlen(buf), 0, (struct sockaddr*) &si_other, slen)==-1)
        diep("failed to send");
      exit(0);
    }
    /*printf("Received packet from %s:%d\nData: %s\n\n", 
                inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);*/
  }

  close(sock);
  return 0;
}
