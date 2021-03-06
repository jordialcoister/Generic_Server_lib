#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
/*#include <security/pam_appl.h>*/
#include "include/server_side.h"
#include "include/server_routines.h"
#include "include/ftpreplycodes.h"
#include "include/auth.h"

//enum commands = { "USER", "PASS", "LS", "CD" };

DIR *d;
struct dirent *dir;
struct session_t session;

void termination_handler(int signum){
     printf("Signal captured!");

     /* Do the cleaning... */
     /* Terminate all the threads */
     /*for (i=0; i < 5; i++){
	  
       }*/
/* Now reraise the signal.  We reactivate the signal’s
     default handling, which is to terminate the process.
     We could just call exit or abort,
     but reraising the signal sets the return status
     from the process correctly. */

}


unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int ftp_cd(t_host *c, char *dirname){
     int res;

     res=chdir(dirname);
     if(res==-1){
	  perror("Error: ");
	  return -1;
     }
     else {
	  getcwd(c->outbuf,strlen(c->outbuf));
	  /* res=send(c->sd,"\r\n",2,0); */
	  getcwd(c->outbuf,strlen(c->outbuf));
	  res=send(c->sd,c->outbuf,strlen(c->outbuf),0);
	  return 0;
     }
}

int ftp_rmdir(t_host *c, char *dirname){
     int res;

     if((res=rmdir(dirname))==-1){
	  perror("Error: ");
	  return -1;
     }
     else {
	  sprintf(c->outbuf,"Directori %s eliminat satisfactòriament\r\n",dirname);
	  res=send(c->sd,c->outbuf,strlen(c->outbuf),0);
	  return 0;
     }
}

int ftp_mkdir(t_host *c, char *dirname){
     int res;
     
     res=mkdir(dirname,0755);
     if(res==-1){
	  perror("Error: ");
	  return -1;
     }
     else {
	  sprintf(c->outbuf,"Directori %s creat satisfactòriament\r\n",dirname);
	  getcwd(c->outbuf,strlen(c->outbuf));
	  res=send(c->sd,c->outbuf,strlen(c->outbuf),0);
	  return 0;
     }
}

int ftp_ls(t_host *c, char *dirname){
     int res;

     if (dirname != NULL)
       d = opendir(dirname);
     else
       d = opendir(".");
     /* d=opendir("."); */
     /* memset(c->outbuf,0,strlen(c->outbuf)); */
     if (d)
     {
	  while ((dir = readdir(d)) != NULL)
	  {
	       strcat(c->outbuf,dir->d_name);
	       strcat(c->outbuf,"\r\n");
	  }
	  closedir(d);
     }
     printf("LLISTAT FITXERS: \r\n%s",c->outbuf);
     res=send(c->sd,c->outbuf,strlen(c->outbuf),0);
     return(0);
}

int auth_prompt(t_host *c){
     int res;
     char *token;
     
     /* for (;;){ */
     res=send(c->sd,PROMPT,1,0);
     /* printf("Enviats %d caràcters\n",res); */
     if (res == -1){
       perror("Sending error: ");
       return -1;
     }
     res=recv(c->sd,c->inbuf,MAXBUFSIZE,0);
     /* printf("Rebuts %d caràcters\n",res); */
     if (res == -1){
       perror("Receiving error: ");
       return -2;
     }
	  
     token=strtok(c->inbuf," \r\n");
     if (strcmp("user",c->inbuf)==0){
       /* hi ha que comprovar si hi ha una sessió en marxa, si
	* la hi ha, hauré de finalitzar la sessió, però deixar
	* que les descàrregues s'acaben, si no n'hi ha,
	* esperem la contrassenya*/
       token=strtok(NULL," \r\n");
       if (token){
	 /* Check if the user exists first */
	 /* Check if there is an active session and act
	  * accordingly, by now we just copy the username
	  * in a variable*/
	 strcpy(session.user,token);
	 send(c->sd,USEROK,strlen(USEROK),0);
	 printf("Usuari rebut: %s\nEsperant la pass\n",token);
	 /* We send another prompt, in this case waiting
	  * for the password */
	 res=send(c->sd,PROMPT,1,0);
	 printf("Enviats %d caràcters\n",res);
	 if (res == -1){
	   perror("Sending error: ");
	   return -1;
	 }
	 res=recv(c->sd,c->inbuf,MAXBUFSIZE,0);
	 printf("Rebuts %d caràcters\n",res);
	 if (res == -1){
	   perror("Receiving error: ");
	   return -2;
	 }
	 token=strtok(c->inbuf," \r\n");
	 if (strcmp("pass",c->inbuf)==0){
	   token=strtok(NULL," \r\n");
	   if (token){
	     printf("Rebuda contrasenya: %s",token);
	     if (sys_auth_user(session.user, token)==0){
	       send(c->sd,LOGINOK,strlen(LOGINOK),0);
	       /* We change the state of the
		* session to "started"*/
	       session.logged_in=1;
	       return 0;
	       /* Everything went fine, so we
		* return 0 in order to server_core
		* to call server_prompt */
	     } else {
	       send(c->sd,ENOTLOGIN,strlen(ENOTLOGIN),0);
	     } /* sys_auth_user */
	   }
	 } else {
	   send(c->sd,BADSEQ,strlen(BADSEQ),0);
	 }/* pass */
       }
     } else {
       send(c->sd,BADSEQ,strlen(BADSEQ),0);
     } /* user */
}      /* function */
     
int server_prompt(t_host *c){
     /*S'hauria de copiar al buffer quan no se sap la longitud a
     priori del missatge, per exemple, si s'ha d'enviar un llistat de
     fitxers /*memcpy(c->outbuf,PROMPT,1);*/
     int res=0;
     struct server_status status;
     DIR *dirp;
     struct dirent *direntry;
     char *token;
     struct sigaction new_action, old_action;
     char user[32];

     new_action.sa_handler = termination_handler;
     sigemptyset (&new_action.sa_mask);
     new_action.sa_flags = 0;
     sigaction (SIGINT, NULL, &old_action);
     if (old_action.sa_handler != SIG_IGN)
	  sigaction (SIGINT, &new_action, NULL);
     sigaction (SIGTERM, NULL, &old_action);
     if (old_action.sa_handler != SIG_IGN)
	  sigaction (SIGTERM, &new_action, NULL);

     
     chdir(ROOT);
     /*if (chroot(ROOT)!=0){
	  perror("chroot: ");
	  return 1;
	  }
	  strcpy(status.root,"/");*/
     strcpy(status.root,ROOT);
     status.transmission=ASCII;
     for(;;){
	  res=send(c->sd,PROMPT,1,0);
	  /* printf("Enviats %d caràcters\n",res); */
	  if (res == -1){
	       perror("Sending error: ");
	       return -1;
	  }
	  res=recv(c->sd,c->inbuf,MAXBUFSIZE,0);
	  /* printf("Rebuts %d caràcters\n",res); */
	  if (res == -1){
	       perror("Receiving error: ");
	       return -1;
	  }
	  
	  token=strtok(c->inbuf," \r\n");
	  printf("Hash: %ld", hash(token));
	  switch (hash(token)){
	  case 2090806916: /* user */
	       token=strtok(NULL," \r\n");
	       if (token){
		    send(c->sd,USEROK,strlen(USEROK),0);
		    printf("Usuari rebut: %s",token);
		    strcpy(user,token);
		    res=recv(c->sd,c->inbuf,MAXBUFSIZE,0);
		    printf("Rebuts %d caràcters\n",res);
		    if (res == -1){
			 perror("Receiving error: ");
			 return -1;
		    }
		    token=strtok(c->inbuf," \r\n");
		    if (strcmp("pass",token)){
			 token=strtok(NULL," \r\n");
			 if (token==0){
			      printf("Rebuda contrasenya: %s",token);
			      if (sys_auth_user(user, token)){
				   send(c->sd,LOGINOK,strlen(LOGINOK),0);
			      }
			 }
			 else{
			      perror("Error en CD: ");
			      send(c->sd,CD_ERROR,strlen(CD_ERROR),0);
			 }
		    }
		    else{
			 send(c->sd,BADSEQ,strlen(BADSEQ),0);
			 memset(user,0,strlen(user));
		    }
	       }
	       
	       break;
	       
	  case 2090608092:  	/* pass */
	       send(c->sd,BADSEQ,strlen(BADSEQ),0);
	       break;
	  case 6385632776 :	/* quit */
	       res=send(c->sd,GOODBYE,strlen(GOODBYE),0);
	       return 1;
	  case 5863588:		/* ls */
	       token=strtok(NULL," \r\n");
	       if (token){
		    printf("Llistant el directori: %s",token);
		    ftp_cd(c,token);
	       }
	       else{
		 printf("Llistant el directori\n");
		 ftp_cd(c, NULL);
	       }
	       ftp_ls(c, token); 
	       break;
	  case 5863276: 	/* cd */
	       token=strtok(NULL," \r\n");
	       if (token){
		    printf("Canviant al directori: %s",token);
		    ftp_cd(c,token);
	       }
	       else{
		    perror("Error en CD: ");
		    send(c->sd,CD_ERROR,strlen(CD_ERROR),0);
	       }
	       break;
	  case 6385111537:	/* cdup */
	       ftp_cd(c,"..");
	       break;
	  case 210720772860: 	/* mkdir */
	       token=strtok(NULL," \r\n");
	       if(token){
		    printf("Creant el directori: %s",token);
		    ftp_mkdir(c,token);
	       }
	       else{
		    perror("Error en CD: ");
		    send(c->sd,MKDIR_ERROR,strlen(MKDIR_ERROR),0);
	       }
	       break;
	  case 210726774339: 	/* rmdir */
	       token=strtok(NULL," \r\n");
	       if(token){
		    printf("Esborrant el directori: %s",token);
		    ftp_rmdir(c,token);
	       }
	       else{
		    perror("Error a l'esborrar el directori: ");
		    send(c->sd,RMDIR_ERROR,strlen(RMDIR_ERROR),0);
	       }
	       break;
	  default:
	       res=send(c->sd,UNKNOWN,strlen(UNKNOWN),0);
	       break;

	  }
     }
     return 0;
}

int send_textfile(const char *path, t_host *c){
     /* We MUST make this function thread-safe, for God sake! */
     FILE *fd;
     int r=0,s=0,sumr=0,sums=0;
     
     fd=fopen(path,"r");
     if (!fd){
	  perror("Error opening the file: ");
	  return -1;
     }
     do{
	  r=fread(c->outbuf,1,MAXBUFSIZE,fd);
	  sumr+=r;
	  s=send(c->sd,c->outbuf, r,0);
	  sums+=s;
     } while ( r>0 && s>0 );
     if (feof){
          printf("%i characters read from file\n%icharacters sent\n",sumr,sums);
	  fclose(fd);
	  return sums;
     }
     else if (ferror){
	  perror("An error occured while reading the welcome file:");
	  printf("%i chars read",sumr);
	  fclose(fd);
	  return -1;
     }
}

