#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <time.h>
#define CENT 100


char* myCpy1(char* msg){
  int i=0;
  int p=0;
  char* msg1=malloc(CENT*sizeof(char));
  memset(msg1,0,CENT*sizeof(char));
  char* msg2=malloc(CENT*sizeof(char));
  memset(msg2,0,CENT*sizeof(char));
  while(msg[i] != ' '){
    i++;
  }
  msg1 = msg + (i+1)*sizeof(char);
  while(msg1[p] != ' '){
    msg2[p]= msg1[p];
    p++;
  }
  msg2[p+1]='\0';
  return msg2;
}

char* myCpy2(char* msg){
  int i=0;
  int p=0;
  char* msg1=malloc(CENT*sizeof(char));
  memset(msg1,0,CENT*sizeof(char));

  char* msg2=malloc(CENT*sizeof(char));
  memset(msg2,0,CENT*sizeof(char));

  while(msg[i] != ' '){
    i++;
  }

  msg1 = msg + (i+1)*sizeof(char);
  while(msg1[p] != ' '){
    p++;
  }
  msg2 = msg1 + (p+1)*sizeof(char);
  return msg2;
}

char* return_time(){
  char * timeString = malloc(20*sizeof(char));
  time_t secs = time(0);
  char *last = malloc(20*sizeof(char));
  struct tm *local = localtime(&secs);

  sprintf(timeString, "GMT +1: %02d:%02d %02d/%02d/%02d", local->tm_hour, local->tm_min,local->tm_mday, local->tm_mon +1, local->tm_year +1900);
  strcpy(last,timeString);

  return last;
}

int main(int argc, char** argv){

     printf("%s\n",return_time());


  return 0;
}
