#include "user.h"


//___________________________________________________________________________________________
//___________________________________________________________________________________________
void init_serv_addr(char* port, struct sockaddr_in* serv_addr)
{
  int portno;
  //clean the serv_add structure
  memset(serv_addr, 0, sizeof(struct sockaddr_in));
  //cast the port from a string to an int
  portno   = atoi(port);

  //internet family protocol
  serv_addr->sin_family = AF_INET;

  //we bind to any ip form the host
  serv_addr->sin_addr.s_addr = INADDR_ANY;

  //we bind on the tcp port specified
  serv_addr->sin_port = htons(portno);
}


int do_socket(int domain, int type, int protocol)
{
  int sock;
  int yes = 1;
  sock = socket(domain, type, protocol);
  if (sock == -1)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  return sock;
}


int do_bind(int sock, const struct sockaddr_in* serv_addr)
{

  if (bind(sock, (struct sockaddr *) serv_addr, sizeof(*serv_addr)) == -1)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  return 0;
}


int do_listen(int sock, int nb)
{
  if(listen(sock, nb) == -1)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  return 0;
}


int do_accept(int sock, struct sockaddr_in* serv_addr)
{
  int addrlen = sizeof(struct sockaddr_in); // this settle the "bad address" problem
  int sock_client = accept(sock, (struct sockaddr *)serv_addr, &addrlen);
  if (sock_client == -1)
  {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  return sock_client;
}


//___________________________________________________________________________________________
//___________________________________________________________________________________________
int do_read(int sock_client, char* buf)
{
  int k;
  int nb = read(sock_client, buf, MX_SIZE); // \0
  char *quit     = "/quit";
  char *nickname = "/nick";
  char *who      = "/who";
  char *whois    = "/quiest";

  if ( nb == -1 )
  {
    perror("recv");
    exit(EXIT_FAILURE);
  }
  if ( strncmp(buf,quit,5) == 0){
    close(sock_client);
    return 1;
  }
  if (strncmp(buf,nickname,5)==0){
    return 2;
  }
  if (strncmp(buf,who,4)==0){
    return 3;
  }
  if(strncmp(buf,whois,7)==0){
    return 4;
  }



  return 0;
}


int do_write(int sock_client, char* buf)
{

  int to_send = sizeof(char)*strlen(buf) ;
  int sent = 0;
  do
  {
    if ( sent == -1 )
    {
      perror("send");
      exit(EXIT_FAILURE);
    }
    else
    {
      sent += write (sock_client, buf + sent, to_send - sent); // Be sure of that all the message is sent
    }
  }while (sent != to_send);
  return 0;
}


char* monBuffer(char* msg){ //copies string without /n (used for user pseudo copy)
  int i=0;
  while( msg[i] != '\n'){
    i++;
  }
  msg[i]= '\0';
  return msg;
}



//___________________________________________________________________________________________
//___________________________________________________________________________________________
int main(int argc, char** argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "usage: RE216_SERVER port\n");
    return 1;
  }
  //NEW VARIABLES & INITIALISATION
  //___________________________________________________________________________________________
  //___________________________________________________________________________________________
  char buf[MX_SIZE];
  int yes= 1;
  int a = 0;
  int b = 0;

  int sock=do_socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in sin;
  init_serv_addr(argv[1], &sin);

  struct sockaddr_in addr_client;

  struct pollfd fds[NB];
  memset (fds, 0, sizeof(fds));
  fds[0].fd = sock;
  fds[0].events = POLLIN;
  struct Liste *maListe;


  //___________________________________________________________________________________________
  //___________________________________________________________________________________________

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("ERROR setting socket options");
    exit(EXIT_FAILURE);
  }// Control sock not already opened
  //we bind on the tcp port specified
  do_bind(sock, &sin);
  //specify the socket to be a server socket and listen for at most 20 concurrent client
  do_listen(sock, 0);


  for (;;)
  {
    if (poll(fds, NB, -1) == -1)
    {
      perror("poll");
      exit(EXIT_FAILURE);
    }
    int i;
    for (i=1;i<NB;i++)
    {

      if (fds[0].revents == POLLIN)
      {
        if(fds[i].fd == 0){ //looking for a fds[i] that doesn't contain socket

          printf("new connexion:%d\n",i);
          fflush(stdout);
          fds[i].fd = do_accept(sock, &addr_client);
          fds[i].events = POLLIN;

          if( b==0){
            maListe=client_init(fds[i].fd,&addr_client);
            b=1;
          }
          else if(b ==1){
            insert_client(maListe,fds[i].fd,&addr_client);
          }

          if(i==NB-1){

            // close the last connexion because server full
            printf("socket i:%d is closed\n",i);
            fflush(stdout);
            char kick[MX_SIZE]="connexion refused";
            do_write(fds[i].fd,kick);
            close(fds[i].fd);
            fds[i].fd = 0;
            fds[i].events=0;
            fds[i].revents=0;
          }
          break;

        }
      }


    if (fds[i].revents == POLLIN && fds[i].fd !=0 ) {
      //serv receiving client msg
      a=do_read(fds[i].fd, buf);

      if ( a == 1)
      //if client quits
      {
        printf("%s has deconnected\n",retournerUN(maListe,fds[i].fd));
        fflush(stdout);
        deleteK(maListe,fds[i].fd);
        fds[i].fd = 0;
        fds[i].events=0;
        fds[i].revents=0;
        break;
      }


      else if( a == 2 ){
        //if the client types /nick myName...
        char *usrname=malloc(MX_SIZE*sizeof(char));
        memset(usrname,0,MX_SIZE);

        usrname = strcpy(usrname,monBuffer(buf)); // copy name
        usrname = usrname + 6*sizeof(char);
        set_pseudo(maListe,usrname,fds[i].fd);

        }


      else if( a ==3 ){

        char* special_buf=malloc(MX_SIZE*sizeof(char));
        memset(special_buf, 0, MX_SIZE);
        special_buf=strcat(special_buf,"La liste des utilisateurs connectés sont:");
        struct Client *actuel= maListe->premier;
        while(actuel != NULL){
          if(actuel->fd != 0){
            special_buf=strcat(special_buf,"\n-");
            special_buf=strcat(special_buf,actuel->client);
            actuel=actuel->next;
          }

        }
        do_write(fds[i].fd,special_buf);

      }

      else if( a == 4){
        char* special_boeuf=malloc(size*sizeof(char));
        char* buff =malloc(10*size*sizeof(char));

        memset(special_boeuf, 0, size*sizeof(char));
        memset(buff, 0, 10*size*sizeof(char));

        special_boeuf = strcat(special_boeuf,monBuffer(buf)); // copy name

        special_boeuf = special_boeuf + 8*sizeof(char);
        int fd=getfd(maListe,special_boeuf);
        if(fd == -1){

          char* unfound;
          unfound = " User not found";
          unfound = strcat(special_boeuf,unfound);
          do_write(fds[i].fd,unfound);
        }
        else { //User is found
          char* IPc = getIP( maListe,fd);
          char* numport= malloc(100*sizeof(char));
          memset(numport,0,100*sizeof(char));
          sprintf(numport,"%d",getport(maListe,fd));


          buff = strcat(buff,special_boeuf);
          buff = strcat(buff, " connecté sur le port " );
          buff = strcat(buff,numport);
          buff = strcat(buff, " avec l'adresse IP " );
          buff = strcat(buff,IPc);

        do_write(fds[i].fd,buff);

        }


      }


        printf("[%s]: %s\n",retournerUN(maListe,fds[i].fd),buf);
        //printfs on server stdout
        fflush(stdout);


          //we write back to the client
        if(a!=3 && a!=1 && a!=4){
          do_write(fds[i].fd, buf);

        }


        memset(buf, 0, MX_SIZE);
      }

  }
}

//clean up server socket
close(sock);

return 0;
}
