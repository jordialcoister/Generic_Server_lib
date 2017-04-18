#include <stdio.h>
#include <stdlib.h>
#include "server_side.h"

int main(int argc, char *argv[]){
     int fd;
     int res;
  
     if((res=create_server(&fd, NULL, 5678)))
	  printf("Server created successfully!");
     else
	  printf("Error número: %i",res);

     if (run_server(&fd)){
	  printf("Error ejecutando el servidor\n.");
	  if (init_server(&fd))
	       printf("Server destroyed successfully!");
	  else 
	       printf("There was an error while destroying the server...");
	  exit(-1);
     }
  
     if (destroy_server(&fd))
	  printf("Server destroyed successfully!");
     else 
	  printf("There was an error while destroying the server...");

     exit(0);
}
