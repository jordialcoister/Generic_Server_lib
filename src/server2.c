#include "server_side.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
     t_server server;
     int res;
     const char address[]="192.168.1.200";

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
     res=close_server(&server);
     if (!res){
	  printf("Servidor destruït correctament\n");
     } else {
	  printf("Error al destruïr el servidor: %i\n",res);
     }
          
     exit(0);
}
