#include "user.h"

// THIS FILE CONTAINS LINKED LISTS
//___________________________________________________________________________________________
//___________________________________________________________________________________________


struct Liste *client_init(int fd,struct sockaddr_in* serv_addr){
  struct Liste *liste=malloc(sizeof(*liste));
  struct Client *client=malloc(sizeof(*client));
  if (liste == NULL || client == NULL)
   {
     perror("client_init");
     exit(EXIT_FAILURE);
   }
   client->ip = inet_ntoa(serv_addr->sin_addr);
   client->port = ntohs(serv_addr->sin_port);
   client->client="Unknown";
   client->fd=fd;
   client->next=NULL;
   liste->premier=client;
  return liste;
}


void insert_client(struct Liste *liste,int fd,struct sockaddr_in* serv_addr){
  struct Client *new=malloc(sizeof(*new));

  if(liste ==NULL || new==NULL){
    perror("insert_client");
    exit(EXIT_FAILURE);
  }
  new->ip=inet_ntoa(serv_addr->sin_addr);
  new->port = ntohs(serv_addr->sin_port);
  new->client ="Unknown";
  new->next =liste->premier;
  new->fd = fd;

  liste->premier=new;
  printf("sucess insertion ");
  fflush(stdout);
}


void set_pseudo(struct Liste* liste, char* pseudo,int fd){
  if(liste ==NULL ){
    perror("set_pseudo");
    exit(EXIT_FAILURE);
  }
  struct Client *actuel = liste->premier;
  while((actuel != NULL) && (actuel->fd!=fd)){
    actuel=actuel->next;
  }
  if(actuel->fd ==fd){
    actuel->client = pseudo;
  }
}


void deleteK(struct Liste * liste,int fd){

  if(liste==NULL){
    perror("delete");
    exit(EXIT_FAILURE);
  }
  if(liste->premier != NULL){
    struct Client *prev=liste->premier;

    if(prev->fd == fd){ // if the head of the list is fd
      liste->premier = prev->next;
      free(prev);
    }else{
      while( (prev != NULL) && (prev->next->fd != fd) ){
        prev=prev->next;
      }
      //prev is the previous element of fd

      struct Client *curr=prev->next;//nt has fd =fd;
      prev->next = curr->next;

      free(curr);
    }
  }

}


char *retournerUN(struct Liste* liste,int fd){
  if (liste == NULL){
      perror("print list");
      exit(EXIT_FAILURE);
  }

  struct Client *actuel = liste->premier;
  while(actuel != NULL && actuel->fd != fd){
    actuel=actuel->next;
  }
  return actuel->client;
}


int getfd(struct Liste* liste, char* pseudo){
  if (liste == NULL){
      perror("print list");
      exit(EXIT_FAILURE);
  }

  struct Client *actuel = liste->premier;
  while(actuel != NULL && strcmp(actuel->client,pseudo)!=0 ){
    actuel=actuel->next;
  }
  if(actuel !=NULL){ //pseudo exists
    return actuel->fd;
  }
  else{
    return -1;
  }

}


char* getIP(struct Liste* liste, int fd){
  if (liste == NULL){
      perror("print list");
      exit(EXIT_FAILURE);
  }

  struct Client *actuel = liste->premier;
  while(actuel != NULL && actuel->fd != fd){
    actuel=actuel->next;
  }
  return actuel->ip;
}


int getport(struct Liste *liste,int fd){
  if (liste == NULL){
      perror("print list");
      exit(EXIT_FAILURE);
  }

  struct Client *actuel = liste->premier;
  while(actuel != NULL && actuel->fd != fd){
    actuel=actuel->next;
  }
  return actuel->port;//ntohs
}