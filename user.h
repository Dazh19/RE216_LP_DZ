#ifndef USER_H_
#define USER_H_

#include "constant.h"


struct Liste{
  struct Client *premier;
};


struct Client{
  char* client;
  int port;
  char* ip;
  int fd;
  int inChannel;
  char* channel_name;
  char* time;
  struct Client *next;
};


struct Liste *client_init(int fd,struct sockaddr_in* serv_addr);
void insert_client(struct Liste *liste,int fd,struct sockaddr_in* serv_addr);
void set_pseudo(struct Liste* liste, char* pseudo,int fd);
char* getUserList(struct Liste* liste);
void deleteK(struct Liste * liste,int fd);
char *getPseudo(struct Liste* liste,int fd);
int getfd(struct Liste* liste, char* pseudo);
char* getIP(struct Liste* liste, int fd);
char* return_time();
char* getTime(struct Liste* liste, int fd);
int getport(struct Liste *liste,int fd);
char* portToString(struct Liste* liste,int fd);
void set_userIsChannel(struct Liste* liste, int fd, int un);
void set_userChannel(struct Liste* liste, int fd, char* channel);
char* get_userchannel(struct Liste* liste, int fd);
int get_userisChannel(struct Liste* liste, int fd);


#endif /* USER_H_ */
