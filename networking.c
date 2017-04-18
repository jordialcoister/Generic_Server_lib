#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "networking.h" 

int create_server(int *fd, const char* address, unsigned int port){
  struct sockaddr_in server, client;
  
  int sin_size;
  
  if ((*fd=socket(AF_INET, SOCK_STREAM, 0)))
    return 1; //Returns a one, because failed in the first step
  
  /*Now we setup the server info struct */
  server.sin_family=AF_INET;
  if (port<=1024 || port > 65535){
    server.sin_port=PORT;
  }
  else
    server.sin_port=port;
  if (!address)
    server.sin_addr.s_addr = INADDR_ANY;
  else
    server.sin_addr.s_addr = inet_addr(address);
  memset(&(server.sin_zero),0,8); 

  /* Time to associate the socket with the server info */
  if (bind(*fd,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){
    return 2; //Error in the second step, the bind()
  }

  if (listen(*fd,BACKLOG)==-1){
    return 3;
  } //Error in the third step, listen()

  
  return 0; //Everything is ok at home

  /* The main loop has to be done in other function, run_server */
}

int run_server(int *fd){
  int sin_size=sizeof(struct sockadd_in);
  int fd2;
  
  for(;;){
    if (fd2=accept(*fd,(struct sockaddr*)&client, &sin_size)=-1)
      return 4; /*Eror in the fourth step, accept*/
    /*Print a message to the log and send a welcome message to the
      other side, before closing the connection*/
    printf("Conexió rebuda des de %s\n",inet_ntoa(client.sin_addr));
    send(fd2,"Sigues benvingut al meu reialme.\n",32);
    close (fd2); 
  }
  
  return 0;
}

/* Do we really need a funcion to destroy the server? Absolutely yes */

int destroy_server(int *fd){
  if (close(*fd))
    return 0;
  else
    return 1;
}

/*const char* print_error(){
  //  static int error;
  int error;
  
  switch (error){
  case 1:
    return ERROR_1;
  case 2: 
    return ERROR_2; 
  case 3:
    return ERROR_3;
  default:
    return DEF_ERR;
    }*/
