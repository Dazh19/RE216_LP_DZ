#ifndef USER_H_
#define USER_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define NB 20
#define MX_SIZE 1024
#define size 150


struct Liste{
  struct Client *premier;
};


struct Client{
  char* client;
  int port;
  char* ip;
  int fd;
  struct Client *next;
};


struct Liste *client_init(int fd,struct sockaddr_in6* serv_addr);
void insert_client(struct Liste *liste,int fd,struct sockaddr_in6* serv_addr);
void set_pseudo(struct Liste* liste, char* pseudo,int fd);
void deleteK(struct Liste * liste,int fd);
char *retournerUN(struct Liste* liste,int fd);
int getfd(struct Liste* liste, char* pseudo);
char* getIP(struct Liste* liste, int fd);
int getport(struct Liste *liste,int fd);


#endif /* USER_H_ */
