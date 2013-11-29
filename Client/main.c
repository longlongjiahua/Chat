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

//  0       1           2       3         4       5               6
//./client clientName serverIP serverPORT GET
//./client clientName serverIP serverPORT SEND destinationName message
int main(int argc, char *argv[])
{
  struct sockaddr_in sockadd;
  int s, slen=sizeof(sockadd);
  char buf[BUFLEN];
  char err[BUFLEN];
  Message mes;

  //Yong's change
  if(argc <5) 
    diep("Not enough arguments.");

  s = mksocket();
  sockadd = mksockaddclient(argv[2], argv[3]);

  //case for get message from server
  if(argc == 5 && strncmp(argv[4], "GET",3)==0  ){
    printf("get\n");
    mes = (Message)
      { 0
      , GET
      , argv[1]
      , ""
      , ""
      };
    mestoa(&buf, &mes);
    printf("bs1:%s\n",buf);
    if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &sockadd, slen)==-1)
      diep("Can not send message to server");
    while(1) {  //receive message from client 1;
      sleep(2);
      if(recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*) &sockadd, (socklen_t * restrict) &slen)==-1)
      {}
      else {
        printf("%s\n", buf);
      }
      break;
    }
  }

  else if(argc ==7 && strncmp(argv[4], "SEND",4)==0) {
    printf("send\n");
    if (strlen(argv[6]) >= BUFLEN) {
      sprintf(err, "Please make your message smaller than %zu (from %i)\n", strlen(argv[3]), BUFLEN);
      diep(err);
    }

    mes = (Message)
    { 0
    , SEND
    , argv[1]
    , argv[5]
    , argv[6]
    };
    mestoa(&buf, &mes);

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
  } //done for case 'SEND';

  else {
    diep("input is not correct");
  }
  return 0;
}
