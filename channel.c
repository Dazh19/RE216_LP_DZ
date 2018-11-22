#include "channel.h"

struct myBag* channel_init(char * name){
  struct myBag *bag=malloc(sizeof(*bag));
  struct channel *channel=malloc(sizeof(*channel));
  if (bag == NULL || channel == NULL)
   {
     perror("channel_init");
     exit(EXIT_FAILURE);
   }
   bag->premier = channel;
   channel->channel_name = name;
   channel->is_create = 1;
   channel->nb_users = 0;
   int i3=0;
   for ( i3=0;i3<NB ;i3++){
     channel->users_fd[i3] = -1;
   }

   channel->next = NULL;
   return bag;
}


void insert_channel(struct myBag *bag,char* name){
  struct channel *new=malloc(sizeof(*new));

  if(bag ==NULL || new==NULL){
    perror("insert_channel");
    exit(EXIT_FAILURE);
  }

  new->channel_name = name;
  new->is_create = 1;
  new->nb_users = 0;
  int i=0;
  for ( i=0;i<NB ;i++){
    new->users_fd[i] = -1;
  }
  new->next =bag->premier;
  bag->premier=new;
  printf("sucess channel insertion ");
  fflush(stdout);
}

void delete_channel(struct myBag *bag, char* name){
  if (bag == NULL){
    perror("delete channel");
    exit(EXIT_FAILURE);
  }
  if( bag->premier!= NULL){
    struct channel *prev = bag->premier;
    if( strcmp(prev->channel_name,name)==0){
      bag->premier = prev->next;
      free(prev);
    }
    else{
      //tant qu'on ne retrouve pas le bon channel
      while(prev!=NULL && strcmp(prev->next->channel_name,name)!=0 ){
        prev=prev->next;
      }
      //si on l'a trouvé et que c'est le suivant
      struct channel* curr=prev->next;
      prev->next = curr ->next;
      free(curr);
    }
  }

}

int get_nb_user_channel(struct myBag *bag,char* name){
  if (bag == NULL){
    perror("get nb user channel failed");
    exit(EXIT_FAILURE);
  }
  struct channel *actuel = bag->premier;
  while(actuel != NULL && strcmp(name,actuel->channel_name) !=0){
    actuel = actuel -> next;
  }
    return actuel->nb_users;

}

void inc_nb_user_channel(struct myBag *bag,char* name){
  if (bag == NULL){
    perror("inc nb user channel failed");
    exit(EXIT_FAILURE);
  }
  struct channel *actuel = bag->premier;
  while(actuel != NULL && strcmp(name,actuel->channel_name) !=0){
    actuel = actuel -> next;
  }
    actuel->nb_users ++;

}
void dec_nb_user_channel(struct myBag *bag,char* name){
  if (bag == NULL){
    perror("dec nb user channel failed");
    exit(EXIT_FAILURE);
  }
  struct channel *actuel = bag->premier;
  while(actuel != NULL && strcmp(name,actuel->channel_name) !=0){
    actuel = actuel -> next;
  }
    actuel->nb_users --;

}


void set_iscreate(struct myBag* bag, char* name , int inte){
  if (bag == NULL){
    perror("set is create channel failed");
    exit(EXIT_FAILURE);
  }
  struct channel *actuel = bag->premier;
  while(actuel != NULL && strcmp(name,actuel->channel_name) !=0){
    actuel = actuel -> next;
  }
    actuel->is_create = inte;

}

int get_iscreate(struct myBag* bag, char* name){
  if (bag == NULL){
    perror("get is create channel failed");
    exit(EXIT_FAILURE);
  }
  struct channel *actuel = bag->premier;
  while(actuel != NULL && strcmp(name,actuel->channel_name) !=0){
    actuel = actuel -> next;
  }
  if( actuel !=NULL){
    return actuel->is_create;
  }
  else
    return -1;

}




void set_channel_table(struct myBag* bag, char* name, int i, int inte){
  if (bag == NULL){
    perror("set channel table failed");
    exit(EXIT_FAILURE);
  }
  struct channel *actuel = bag->premier;
  while(actuel != NULL && strcmp(name,actuel->channel_name) !=0){
    actuel = actuel -> next;
  }

    actuel->users_fd[i] = inte;

}

int get_nbchannel(struct myBag *bag){
  if (bag == NULL){
    perror("nb_channel failed");
    exit(EXIT_FAILURE);
  }
  int c=0;
  struct channel *actuel = bag->premier;
  while(actuel != NULL){
    actuel = actuel->next;
    c++;
  }
  return c;
}

void set_premierName(struct myBag* bag,char* name){
  if (bag == NULL){
    perror("set_premierName failed");
    exit(EXIT_FAILURE);
  }
  if( bag-> premier != NULL){
    bag->premier->channel_name=name;
  }
}


char* getChannelList(struct myBag* bag){
  if(bag == NULL){
    perror("getChannelList failed");
    exit(EXIT_FAILURE);
  }

  char* buffer=malloc(MX_SIZE*sizeof(char));
  memset(buffer, 0, MX_SIZE);
  strcpy(buffer,"La liste des salons créés:");

  struct channel *actuel= bag->premier;
  while(actuel != NULL){
      strcat(buffer,"\n-");
      strcat(buffer,actuel->channel_name);
      actuel=actuel->next;
  }
  return buffer;
}
