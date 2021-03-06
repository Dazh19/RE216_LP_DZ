#include "constant.h"
#include "user.h"


void get_addr_info(char* address, char* port, struct sockaddr_in* serv_addr)
{
  int portno, addressno;

  //clean the serv_add structure
  memset(serv_addr, 0, sizeof(*serv_addr));// Initialisation of memory blocs

  //cast the port from a string to an int
  portno   = atoi(port);
  //internet family protocol
  serv_addr->sin_family = AF_INET;


  serv_addr->sin_port = htons(portno);

  serv_addr->sin_addr.s_addr = inet_addr(address);
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

void do_connect(int sock, const struct sockaddr_in serv_addr)
{
  if ( connect(sock, (struct sockaddr *)& serv_addr, sizeof(serv_addr)) == -1)
  {
    perror("connect");
    exit(EXIT_FAILURE);
  }
}

/*void do_gets(char* msg){
  gets(msg);
}*/


char* do_read_client(int sock_client, char* buf)
{

  if (read(sock_client, buf, MX_SIZE)   == -1 )
  {

    perror("recv");
    exit(EXIT_FAILURE);
  }


  return 0;
}

int handle_client_message(int sock, char *msg)
{
  int to_send = sizeof(char)*strlen(msg);
  int sent = 0;
  char *quit = "/quit";
  do
  {
    if ( sent == -1 )
    {
      perror("send");
      exit(EXIT_FAILURE);
    }
    else
    {
      sent += write(sock, msg + sent, to_send - sent); // Be sure of that all the message is sent
    }
  }while (sent != to_send);

    if ( strncmp(msg,quit,5) == 0)
    {

          return 1;

    }


  return 0;
}

int main(int argc,char** argv)
{

  char msg[MX_SIZE];
  char servermsg[MX_SIZE];
  struct sockaddr_in sin;
  //get address info from the server
  get_addr_info(argv[2], argv[1],  &sin);

  if (argc != 3)
  {
    fprintf(stderr,"usage: RE216_CLIENT hostname port\n");
    return 1;
  }

  //get the socket
  int sock = do_socket(AF_INET, SOCK_STREAM, 0);

  //connect to remote socket
  do_connect(sock, sin);

  struct pollfd fds[2];
  memset (fds, 0, sizeof(fds));
  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;
  fds[1].fd = sock;
  fds[1].events = POLLIN;


  for(;;)
  {
    if (poll(fds, NB, -1) == -1)
    {
      perror("poll client.c");
      exit(EXIT_FAILURE);
    }
    if(fds[0].revents == POLLIN){
      memset(msg, 0, MX_SIZE);
      //get user input
      do_read_client(STDIN_FILENO,msg);

      //send message to the server
      if (handle_client_message(sock, msg) == 1){
        printf("connexion finished\n");
        fflush(stdout);
        break;
      }
    }
    else if(fds[1].revents == POLLIN){
      memset(servermsg,0,MX_SIZE);
      do_read_client(sock,servermsg);
      printf("%s [SERVER]: %s\n",get_timeHHDD(),servermsg);
      fflush(stdout);

    }

  }
  close(sock); // Closing client's program socket
  return 0;
}
