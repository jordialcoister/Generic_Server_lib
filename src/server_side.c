#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include "include/server_side.h"
#include "include/server_routines.h"

volatile sig_atomic_t fatal_error_in_progress = 0;
pthread_t servers[5];
pthread_t joiners[5];



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

int run_server(t_server *s){
     int fd2,numbytes,sin_size;
     t_client client[5];
     char buf[MAXBUFSIZE];
     int i=0,res=0;
     

     /*Associem una funció per a manejar l'arribada d'una SIGINT*/
	  /*signal(SIGINT, termination_handler);
	    signal(SIGTERM,termination_handler); */
     
     sin_size=sizeof(struct sockaddr_in); 
     for(;;){
	  client[i].sd=accept(s->sd,(struct sockaddr *)&client[i].addr, &sin_size);
	  if (client[i].sd==-1){
	       perror("Accept error: ");
	       return 4; /*Eror in the fourth step, accept*/
	  }
	  /*If we do this out of the thread we make it automatically
	    thread-safe*/
	  send_textfile("./welcome.txt",&client[i]);
          /* We should create two threads per client: 
           * -The client's attendant
	   * -The thread's cleaner*/
	  res=pthread_create(&servers[i], NULL, server_core, (void *)&client[i]);
	  if (res){
	       printf("ERROR: Return code from pthread_create is %ld",res);
	       
	       return res;
	  }
	  /*res=pthread_create(&joiners[i], cleaner, (void *)&servers[i],NULL);
	  if (res){
	       printf("ERROR: Return code from pthread_create is %ld",res);
	       return res;
	       }*/
	  /* If anythig went right increment the index */
	  i++;
	  printf("Connectat al client %d\n",i);
     }
  
     return 0;
}

void *cleaner(void *pool){     
     pthread_t *p=(pthread_t *)pool;
     int ret;
     
     pthread_join(*p,(void *)&ret);
     if ((int )ret){
	  /* We should remove all resources here */
	  printf("Conection closed successfully");
     } else {
	  perror("Error joining the thread");
     }
     
     pthread_exit(NULL);
}

void *server_core(void *client){
     /*Print a message to the log and send a welcome message to the
       other side, before closing the connection*/
     t_client *c=(t_client *)client;
     int res;
	  
     printf("Conexió rebuda des de %s\n",inet_ntoa(c->addr.sin_addr));
     res=server_prompt(c);
     if (close(c->sd)<0)
	  perror("Error al tancar la connexió: ");
     /*pthread_exit(NULL);
      * The call to pthread_exit is done implicitly when
      * the function executed in it returns, so the call isn't
      * necessary...
      */
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
