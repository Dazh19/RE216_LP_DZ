#include "user.h"
#include "channel.h"
//___________________________________________________________________________________________
//___________________________________________________________________________________________
void init_serv_addr(char* port, struct sockaddr_in* serv_addr)
{
  int portno;
  memset(serv_addr, 0, sizeof(*serv_addr));
  //cast the port from a string to an int
  portno   = atoi(port);

  serv_addr->sin_family = AF_INET;

  serv_addr->sin_addr.s_addr = inet_addr(INADDR_ANY);

  serv_addr->sin_port = htons(portno);

}


int do_socket(int domain, int type, int protocol)
{
  int sock;
  sock = socket(domain, type, protocol);
  if (sock == -1)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  return sock;
}


int do_bind(int sock, const struct sockaddr_in serv_addr)
{

  if (bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)
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
  int addrlen = sizeof(struct sockaddr_in*);
  int sock_client = accept(sock, (struct sockaddr *)serv_addr,(socklen_t*) &addrlen);
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
  int nb = read(sock_client, buf, MX_SIZE); // \0
  char *quit     = "/quit";
  char *nickname = "/nick";
  char *who      = "/who";
  char *whois    = "/quiest";
  char *msgall   = "/msgall";
  char *tothem   = "/tothem";
  char* create   = "/create";
  char* join     = "/join";
  char* leave    = "/leave";
  char* chanlist = "/chanlist";
  char* chanmember = "/chanmember";

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
  if(strncmp(buf,msgall,7)==0){
    return 5;
  }
  if(strncmp(buf,tothem,7)==0){
    return 6;
  }
  if(strncmp(buf,create,7)==0){
    return 7;
  }
  if(strncmp(buf,join,5)==0){
    return 8;
  }
  if(strncmp(buf,leave,6)==0){
    return 9;
  }
  if(strncmp(buf,chanlist,9)==0){
    return 10;
  }
  if(strncmp(buf,chanmember,10)==0){
    return 11;
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
  while( msg[i*sizeof(char)] != '\n'){
    i++;
  }
  msg[i*sizeof(char)]= '\0';
  return msg;
}

char* myCpy1(char* msg){
  //TAKE THE WORD AFTER THE FIRST SPACE


  int i=0;
  int p=0;
  char* msg1=malloc(CENT*sizeof(char));
  memset(msg1,0,CENT*sizeof(char));
  char* msg2=malloc(CENT*sizeof(char));
  memset(msg2,0,CENT*sizeof(char));

  while(msg[i*sizeof(char)] != ' '){
    i++;
  }
  msg1 = msg + (i+1)*sizeof(char);
  while(msg1[p] != ' ' && msg1[p]!= '\0' ){
    msg2[p]= msg1[p];
    p++;
  }
  return msg2;
}

char* myCpy2(char* msg){
  //TAKE THE STRING AFTER THE SECOND SPACE
  int i=0;
  int p=0;
  char* msg1=malloc(100*sizeof(char));
  memset(msg1,0,100*sizeof(char));

  char* msg2=malloc(100*sizeof(char));
  memset(msg2,0,100*sizeof(char));

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

char* msg_is_From(char* msg){
  char* tempo = malloc(100*sizeof(char));
  memset(tempo, 0, 100*sizeof(char));

  strcpy(tempo,"-->[");
  strcat(tempo,msg);
  strcat(tempo,"]: ");
  return tempo;
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
  int b,dope = 0;

  int sock=do_socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in sin;
  init_serv_addr(argv[1], &sin);

  struct sockaddr_in addr_client;

  struct pollfd fds[NB];
  memset (fds, 0, sizeof(fds));
  fds[0].fd = sock;
  fds[0].events = POLLIN;
  struct Liste *maListe;
  struct myBag *myBag;
  myBag=channel_init("NONAME");


  //___________________________________________________________________________________________
  //___________________________________________________________________________________________

  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("ERROR setting socket options");
    exit(EXIT_FAILURE);
  }// Control sock not already opened
  //we bind on the tcp port specified
  do_bind(sock, sin);
  //specify the socket to be a server socket and listen for at most NB concurrent client
  do_listen(sock, NB);
  int i;

  for (i =1;i<NB;i++){
    fds[i].fd = 0;
  }

  for (;;)
  {
    if (poll(fds, NB, -1) == -1)
    {
      perror("poll");
      exit(EXIT_FAILURE);
    }
    for (i=1;i<NB;i++)
    {

      if (fds[0].revents == POLLIN)
      {
        if(fds[i].fd == 0){ //looking for a fds[i] that doesn't contain socket

          printf("new connexion:%d\n",i);
          fflush(stdout);
          fds[i].fd = do_accept(sock, &addr_client);
          fds[i].events = POLLIN;


          if(b ==1){
            insert_client(maListe,fds[i].fd,&addr_client);
          }
          else if( b==0){
            maListe=client_init(fds[i].fd,&addr_client);
            b=1;
          }

          if(i==NB-1){

            // close the last connexion because server full
            printf("socket i:%d is closed\n",i);
            fflush(stdout);
            char kick[20]="connexion refused";
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
        char* currentUser=getPseudo(maListe,fds[i].fd);
        int is_inchannel = get_userisChannel(maListe,fds[i].fd);


        if( get_userisChannel(maListe,fds[i].fd) == 1){//si celui qui parle est dans un salon
          if( a!=1 && a!=2 && a!=3  && a!=4 && a!=5 && a!=6 && a!=7 && a!=8 && a!=9 && a!=10 && a!=11){
            int j=1;
            char* msgfromUser = msg_is_From(currentUser);
            char* userChannel = malloc(30*sizeof(char));
            strcpy(userChannel,get_userchannel(maListe,fds[i].fd));

            struct channel  *chacha = myBag->premier;
            while(chacha != NULL && strcmp(chacha->channel_name,userChannel)!=0){
              chacha = chacha->next;
            }
            if(chacha != NULL){

              char* temporary = malloc(MX_SIZE*sizeof(char));
              memset(temporary, 0, MX_SIZE*sizeof(char));

              strcpy(temporary,"CHANNEL: ");
              strcat(temporary,chacha->channel_name);
              strcat(temporary,"/// message from: ");
              strcat(temporary,msgfromUser);
              strcat(temporary,buf);

              for (j=0;j < NB;j++){
                if(chacha->users_fd[j] > 0){

                  do_write(chacha->users_fd[j],temporary);

                }
              }
            }
          }
        }



      if ( a == 1)
      //if user deconnects
      {


        printf("%s has deconnected\n",currentUser);
        fflush(stdout);
        deleteK(maListe,fds[i].fd);
        fds[i].fd = 0;
        fds[i].events=0;
        fds[i].revents=0;
        memset(buf, 0, MX_SIZE);
        break;
      }


      else if( a == 2 ){
        //if the client types /nick myName...
        char *usrname=malloc(CENT*sizeof(char));
        memset(usrname,0,CENT);

        strcpy(usrname,monBuffer(buf)); // copy name
        usrname = usrname + 6*sizeof(char);
        set_pseudo(maListe,usrname,fds[i].fd);

        }


      else if( a ==3 ){
        //if /who
        do_write(fds[i].fd,getUserList(maListe));
      }

      else if( a == 4){
        // if /quiest
        char* special_boeuf=malloc(MX_SIZE*sizeof(char));
        memset(special_boeuf, 0, MX_SIZE*sizeof(char));

        strcpy(special_boeuf,monBuffer(buf));

        special_boeuf = special_boeuf + 8*sizeof(char);// copy name
        int fd=getfd(maListe,special_boeuf);
        if(fd == -1){

          char* unfound;
          unfound = " User not found";
          strcat(special_boeuf,unfound);
          do_write(fds[i].fd,special_boeuf);
        }
        else { //User is found
          char* buff =malloc(CENT*sizeof(char));
          memset(buff, 0, CENT*sizeof(char));

          strcat(buff,special_boeuf);
          strcat(buff, " connecté sur le port " );
          strcat(buff,portToString(maListe,fd));
          strcat(buff, " avec l'adresse IP " );
          strcat(buff,getIP(maListe,fd));
          strcat(buff," connecté depuis le ");
          strcat(buff,getTime(maListe,fd));
          do_write(fds[i].fd,buff);

        }
      }

      else if (a ==5){
        //broadcast /msgall
        int j=1;
        char* lapin = malloc(MX_SIZE*sizeof(char));
        char* temporary = malloc(MX_SIZE*sizeof(char));
        memset(lapin, 0, MX_SIZE*sizeof(char));
        memset(temporary, 0, MX_SIZE*sizeof(char));


        char* msgIsFrom = msg_is_From(currentUser);

        strcpy(temporary,msgIsFrom);
        strcpy(lapin,buf);
        lapin = lapin +8*sizeof(char);
        strcat(temporary,lapin);

        for (j=1;j <NB ;j++){
          if(fds[j].fd != 0){
            if(j != i){
              do_write(fds[j].fd,temporary);
            }
          }
        }

      }

      else if( a== 6){
        //tothem username msg[]
        char  *receiver = myCpy1(buf);
        int idx=getfd(maListe,receiver);

        if (idx == -1){
          char* noexist = malloc(CENT*sizeof(char));
          memset(noexist,0,CENT*sizeof(char));

          strcpy(noexist,receiver);
          strcat(noexist," doesn't exist !!!");
          do_write(fds[i].fd,noexist);
          free(noexist);
        }
        else{
          char* temporary1 = malloc(MX_SIZE*sizeof(char));
          memset(temporary1, 0, MX_SIZE*sizeof(char));

          char* chevre = malloc(MX_SIZE*sizeof(char));
          memset(chevre,0,MX_SIZE*sizeof(char));

          chevre = myCpy2(monBuffer(buf));
          temporary1 = msg_is_From(currentUser);
          strcat( temporary1,chevre);
          do_write(idx,temporary1);

        }
      }

      else if( a == 7 ){ //create channel
        if( get_userisChannel(maListe,fds[i].fd) == 0){
          char* channelName =myCpy1(monBuffer(buf));
          int cache= get_iscreate(myBag,channelName);

          if( cache == 1){
            char* lievre = malloc(50*sizeof(char));
            memset(lievre,0,50*sizeof(char));
            strcpy(lievre,channelName);
            strcat(lievre," channel already exists");
            do_write(fds[i].fd,lievre);
          }

          else if( cache <0 ){


            if ( dope ==0){
              //first create
              set_premierName(myBag,channelName);

              dope=1;
            }
            else if (dope == 1){
              insert_channel(myBag,channelName);
            }

            char* dauphin = malloc(MX_SIZE*sizeof(char));
            memset(dauphin,0,MX_SIZE*sizeof(char));
            strcpy(dauphin,channelName);
            strcat(dauphin," channel is created");
            do_write(fds[i].fd,dauphin);
          }
        }
        else{
          do_write(fds[i].fd,"you can't create a new channel, leave your channel and then create one ");
        }


      }

      else if(a == 8){//user can't join another channel if he is already in one
      //leave the current channel
        if( get_userisChannel(maListe,fds[i].fd) == 0){
          char* channelName1= myCpy1(monBuffer(buf));
          int val = get_iscreate(myBag,channelName1);
          if(val <0){
            //if channel doesnt exist
            // cant join no exist channel
            char* baleine = malloc(CENT*sizeof(char));
            memset(baleine,0,CENT*sizeof(char));
            strcpy(baleine,channelName1);
            strcat(baleine," channel doesnt exist");
            do_write(fds[i].fd,baleine);
          }
          else if (val>0){
            //if channel exists: join it

            //change user struct
            set_userIsChannel(maListe,fds[i].fd,1);
            set_userChannel(maListe,fds[i].fd,channelName1);

            //change channel struct
            inc_nb_user_channel(myBag,channelName1);
            set_channel_table(myBag,channelName1,fds[i].fd,fds[i].fd);

            char* cachalot = malloc(CENT*sizeof(char));
            memset(cachalot,0,CENT*sizeof(char));
            strcpy(cachalot,currentUser);
            strcat(cachalot,", WELCOME IN ");
            strcat(cachalot,channelName1);
            strcat(cachalot, " !!!");

            do_write(fds[i].fd,cachalot);
          }
        }
        else{
          do_write(fds[i].fd,"you can't join a new channel, leave your channel and then join one ");
        }



      }

      else if( a == 9){ //if /leave

        //user can leave channel if user is in one...
        if(is_inchannel >0){
          char* channelName2 = malloc(30*sizeof(char));
          memset(channelName2,0,30*sizeof(char));
          strcpy(channelName2,get_userchannel(maListe,fds[i].fd));
          //modify channel struct
          dec_nb_user_channel(myBag,channelName2);
          set_channel_table(myBag,channelName2,fds[i].fd,-1);

          //modify user struct
          set_userChannel(maListe,fds[i].fd,"NOCHANNEL");
          set_userIsChannel(maListe,fds[i].fd,0);

          int empty=get_nb_user_channel(myBag,channelName2);
          if(empty == 0){
            delete_channel(myBag,channelName2);
          }
        }

      }

      else if( a == 10){
        int channelnumber = get_nbchannel(myBag);
        if (channelnumber>0){
          if(channelnumber == 1 && strcmp(myBag->premier->channel_name,"NONAME")==0){
            do_write(fds[i].fd,"there is no created channel");
          }
          else{
            do_write(fds[i].fd,getChannelList(myBag));
          }
        }
        else{
            do_write(fds[i].fd,"there is no created channel");
        }
      }
      else if(a == 11){
        if(get_userisChannel(maListe,fds[i].fd)>0){ //verify user is in channel
          struct channel *cha1 = malloc(sizeof(*cha1));
          cha1 = myBag->premier;
          while(strcmp(cha1->channel_name,get_userchannel(maListe,fds[i].fd))!=0){
            cha1 = cha1->next;
          }
          if(cha1!= NULL){
            int indice=0;
            char* usr_in_channel = malloc(CENT*sizeof(char));
            strcpy(usr_in_channel,"the list of users in your channel ");
            strcat(usr_in_channel,cha1->channel_name);
            strcat(usr_in_channel," is:\n");
            for(indice=0;indice<NB;indice++){
              if(cha1->users_fd[indice]!= -1 ){
                strcat(usr_in_channel,"-");
                strcat(usr_in_channel,getPseudo(maListe,cha1->users_fd[indice]));
                strcat(usr_in_channel,"\n");
              }
            }
            do_write(fds[i].fd,usr_in_channel);
          }

        }
        else{
          do_write(fds[i].fd,"you aren't in a channel");
        }
      }

        printf("%s [%s] a dit: %s\n",get_timeHHDD(),currentUser,buf);
        fflush(stdout);


          //we write back to the client
        if( a!=1 && a!=2 && a!=3  && a!=4 && a!=5 && a!=6 && a!=7 && a!=8 && a!=9 && a!=10 && a!=11){
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
