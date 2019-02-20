#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "server_side.h"

t_host server;


int main(int argc, char *argv[]){
     /*t_host server;*/
     int res;
     const char address[]="127.0.0.1";
     int status;
     pid_t pid, sid;


     pid = fork ();
     if (pid < 0)
     {
	  /* The fork failed.  Report failure.  */
	  perror("Error forking: ");
	  status = -1;
	  exit(EXIT_FAILURE);     
     }
     else if (pid > 0){
	  printf("Daemon created successfully\n");
	  exit(EXIT_SUCCESS);
     }
     else {
	  /* Change the file mode mask */
	  umask(0);
	  /* Open any logs here */
	
	  /* Create a new SID for the child process */
	  sid = setsid();
	  if (sid < 0) {
	       /* Log any failure */
	       exit(EXIT_FAILURE);
	  }

	  /* Close out the standard file descriptors */
	  close(STDIN_FILENO);
	  /*close(STDOUT_FILENO);
	    close(STDERR_FILENO);

	    Ara per ara els utilitzem per a debugging*/
	  
	  res=create_server(&server, 56789, NULL,0);
	  if (!res){
	       printf("Servidor creat correctament\n");
	  } else {
	       printf("Error al crear el servidor: %i\n",res);
	  }
	  res=run_server(&server);
	  if (!res){
	       printf("Servidor iniciant correctament\n");
	  } else {
	       printf("Error a l'iniciar el servidor: %i\n",res);
	  }
	  /*
	   *This code doesen't make sense...
	   */
	   res=close_server(&server);
	   if (!res){
	   printf("Servidor destruït correctament\n");
	   } else {
	   printf("Error al destruïr el servidor: %i\n",res);
	   }

	  pthread_exit(NULL); /* Where I put this?*/

     }
     /*exit(0);*/
}
