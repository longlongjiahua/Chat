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
  struct sockaddr_in si_other;
  int s, slen=sizeof(si_other);
  char buf[BUFLEN];
  char err[BUFLEN];

  if(argc < 4)
    diep("Not enough arguments.");

      if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        diep("socket");

  memset((char *) &si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(atoi(argv[2]));
  if (inet_aton(argv[1], &si_other.sin_addr)==0) {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  if (strlen(argv[3]) >= BUFLEN) {
    sprintf(err, "Please make your message smaller than %zu (from %i)\n", strlen(argv[3]), BUFLEN);
    diep(err);
  }

  sprintf(buf, "%s\n", argv[3]);

  printf("%s\n", buf);
  if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen)==-1)
    diep("message failed");

  for (int i=0; i<10; i++) {
    sleep(1);
    printf("Sending packet %d\n", i);
    sprintf(buf, "This is packet %d\n", i);
    if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &si_other, slen)==-1)
      diep("sendto()");
  }

  while(1)
  {
    if(recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1)
    {}
    else {
      printf("%s\n", buf);
      break;
    }
  }

  close(s);
  return 0;
}
