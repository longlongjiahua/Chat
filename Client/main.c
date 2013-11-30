#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

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
    /*printf("get\n");*/

    mes.seq = 0;
    mes.type = GET;
    strcpy(mes.source, argv[1]);
    strcpy(mes.destination, "null");
    strcpy(mes.message, "null");

    mestoa(&buf, &mes);

    if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &sockadd, slen)==-1)
      diep("Can not send message to server");

    int curseq = 0;
    while(1) {  //receive message from client 1;
      if(recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*) &sockadd, (socklen_t * restrict) &slen)==-1)
        diep("failed to recieve message");

      mes = atomes(&buf);
      /*printf("got %d\n", mes.seq);*/
      if(mes.type == ACK)
        break;
      if(mes.seq == curseq)
      {
        printf("%d | From: %s\n%s\n\n", mes.seq, mes.source, mes.message);

        mes.type = ACK;
        strcpy(mes.source, argv[1]);

        mestoa(&buf, &mes);
        if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &sockadd, slen)==-1)
          diep("Couldn't ack.");
        ++curseq;
      }
    }
  }

  else if(argc ==7 && strncmp(argv[4], "SEND",4)==0) {
    /*printf("send\n");*/
    if (strlen(argv[6]) >= BUFLEN) {
      sprintf(err, "Please make your message smaller than %zu (from %i)\n", strlen(argv[3]), BUFLEN);
      diep(err);
    }

    mes.seq = 0;
    mes.type = SEND;
    strcpy(mes.source, argv[1]);
    strcpy(mes.destination, argv[5]);
    strcpy(mes.message, argv[6]);

    mestoa(&buf, &mes);

    while(1)
    {
      if (sendto(s, buf, BUFLEN, 0, (struct sockaddr*) &sockadd, slen)==-1)
        diep("message failed");

      int pid = fork();
      if(pid==0)
      {
        if(recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*) &sockadd, (socklen_t * restrict) &slen)==-1)
          diep("failed recieval of ack.");
        /*printf("%s\n", buf);*/
        exit(0);
      }

      int timerpid = fork();
      if(timerpid==0)
      {
        sleep(2);
        exit(0);
      }

      int firsttodie = wait(NULL);
      if(firsttodie==pid)
      { // before timer, got the ack
        break;
      }
      else
      { // after timer, kill waiting child and resend
        kill(pid, SIGTERM);
      }

      break;
    }

    close(s);
  } //done for case 'SEND';

  else {
    diep("input is not correct");
  }
  return 0;
}
