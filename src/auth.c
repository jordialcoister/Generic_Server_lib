/* 
File: auth.c
Author: Jordi Garcia Soler
Date: around 2017 spring

In a nutshell: This peace of code contains a set of routines to
authentication with a buch of different methods.  It was thought as a
module to build my server set.  */

#include <string.h>
#include <pwd.h>
#include <shadow.h>
#include <crypt.h>
#include <stdio.h>
#include "include/auth.h"

int sys_auth_user (const char *username, const char *password)
{
  struct passwd *pw; 		/*  structure containing the broke-out fields of /etc/passwd */
  struct spwd *sp;
  char *encrypted, *correct;

  if (strcmp(username,"anonymous")){
  
  pw = getpwnam (username);
  endpwent();

  if (!pw) return 1; //user doesn't really exist

  setspent();
  sp = getspnam (pw->pw_name);
  endspent();
  if (sp)
     correct = sp->sp_pwdp;
  else
     correct = pw->pw_passwd;
  
  if(correct){
       encrypted = crypt(password, correct);
       if (encrypted)
	    return strcmp (encrypted, correct) ? 2 : 0;  // bad pw=2, success=0
       else{
	    perror("Error encrypting the typed password");
	    
       }       
  } else 
  perror("Error retrieving the encrytped password");
  } else {
       return 0;
  }
}



