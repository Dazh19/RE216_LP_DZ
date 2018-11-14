#ifndef CHANNEL_H_
#define CHANNEL_H_


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

struct myBag{
  struct channel *premier;
};

struct channel{
  char* channel_name;
  int is_create;
  int nb_users;
  int users_fd[NB];
  struct channel *next;
};



struct myBag* channel_init(char * name);


void insert_channel(struct myBag *bag,char* name);

void delete_channel(struct myBag *bag, char* name);

int get_nb_user_channel(struct myBag *bag,char* name);

void inc_nb_user_channel(struct myBag *bag,char* name);

void dec_nb_user_channel(struct myBag *bag,char* name);

void set_iscreate(struct myBag* bag, char* name , int inte);

int get_iscreate(struct myBag* bag, char* name);

void set_channel_table(struct myBag* bag, char* name, int i, int inte);

int get_nbchannel(struct myBag *bag);

void set_premierName(struct myBag* bag,char* name);


#endif /* CHANNEL_H_ */
