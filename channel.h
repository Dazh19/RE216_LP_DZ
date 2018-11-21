#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "constant.h"

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

char* getChannelList(struct myBag* bag);

#endif /* CHANNEL_H_ */
