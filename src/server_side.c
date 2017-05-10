#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "server_side.h"

/* int init_server(int *fd, const char* address, unsigned int port){ */
/*      struct sockaddr_in server, client; */
  
/*      int sin_size; */
   
/*      if (!(*fd=socket(AF_INET, SOCK_STREAM, 0))) */
/* 	  return 1; //Returns a one, because failed in the first step */

/*      /\*Now we setup the server info struct *\/ */
/* 	     server.sin_family=AF_INET; */
/*      if ((port<=1024) || (port > 65535)){ */
/* 	  server.sin_port=htons(PORT); */
/*      } */
/*      else */
/* 	  server.sin_port=htons(port); */
/*      if (!address) */
/* 	  server.sin_addr.s_addr = INADDR_ANY; */
/*      else */
/* 	  server.sin_addr.s_addr = inet_addr(address); */

/*      memset(&(server.sin_zero),0,8); */

/*      /\* Time to associate the socket with the server info *\/ */
/*      if (bind(*fd,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){ */
/* 	  return 2; //Error in the second step, the bind() */
/*      } */

/*      if  (listen(*fd,BACKLOG)==-1){ */
/* 	  return 3; */
/*      } //Error in the third step, listen() */

  
/*      return 0; //Everything is ok at home */

/*      /\* The main loop has to be done in other function, run_server *\/ */
/* }  */

/*int create_server(int *fd, const char* address, unsigned int port){*/
int create_server(t_server *s, int port, const char *address, char options){
     
     int sin_size,ret=0;
     
     memset(s->inbuf,0,MAXBUFSIZE);
     memset(s->outbuf,0,MAXBUFSIZE);
     s->sd=socket(AF_INET, SOCK_STREAM, 0);
     if (s->sd<0)
	  exit(1); //Returns a one, because failed in the first step
     
/*Now we setup the server info struct */
     s->addr.sin_family=AF_INET;
     if (port<=1024 || port > 65535){
	  s->addr.sin_port=htons(PORT); 	/* default port, to revise */
     }
     else
	  s->addr.sin_port=htons(PORT);
     if (!address)
	  s->addr.sin_addr.s_addr = INADDR_ANY;
     else
	  /*inet_aton(address, s->addr.sin_addr.s_addr);*/
     memset(s->addr.sin_zero,0,8); 

/* Time to associate the socket with the server info */
     ret=bind(s->sd,(struct sockaddr*)&(s->addr),sizeof(struct sockaddr));
     if (ret<0){
	  perror("Error en bind: ");
	  exit(2); //Error in the second step, the bind()
     }

     if (listen(s->sd,BACKLOG)==-1){
	  exit(3);
     } //Error in the third step, listen()

  
     return 0; //Everything is ok at home

/* The main loop has to be done in other function, run_server */
}

int start_server(t_server *s){
     /* socklen_t sin_size; */
     int fd2,numbytes,sin_size;
     t_client client;
     /*struct sockaddr_in client;*/
     char buf[MAXBUFSIZE];

     sin_size=sizeof(struct sockaddr_in);
     /*printf("sin_size= %i",sin_size);*/
     for(;;){
	  client.sd=accept(s->sd,(struct sockaddr *)&client.addr, &sin_size);
	  if (client.sd==-1){
	       perror("Accept error: ");
	       return 4; /*Eror in the fourth step, accept*/
	  }
	  /*Print a message to the log and send a welcome message to the
	    other side, before closing the connection*/
	  printf("Conexió rebuda des de %s\n",inet_ntoa(client.addr.sin_addr));
	  /*send(client.sd,"Sigues benvingut al meu reialme.\r\nContestaré a les teves preguntes.\r\n",69,0);
	  send(client.sd,"> ",2,0);
	  while ((strcmp(buf,"quit\r\n")!=0)){
	       if ((numbytes=recv(client.sd,buf,MAXBUFSIZE,0))>=0){
		    buf[numbytes]=0;
		    send(client.sd,buf,strlen(buf),0);
	       }
	       else
		    break;
		    }*/
	  send(client.sd,"Have a nice day!\r\n",16,0);
	  close (client.sd); 
     }
  
     return 0;
}

/* Do we really need a funcion to destroy the server? Absolutely yes */

int close_server(t_server *s){
     if (!close(s->sd))		/* Close the socket, but there's a lot
				 * * * more to do here */
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
