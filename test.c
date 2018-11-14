#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
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


int main(int argc, char** argv){
  char* mamie=malloc(100*sizeof(char));
  mamie="/un deuuuux trois quatre";
  printf("%s %s %lu hehe ",myCpy1(mamie), myCpy2(mamie),strlen(myCpy1(mamie)));
  char buffer[100]="alola";
  printf("%s %d  %c",buffer,sizeof(buffer),buffer[92]);
  //printf("%s hehe ",myCpy2(mamie));

  return 0;
}
