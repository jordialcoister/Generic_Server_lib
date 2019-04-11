/* This header include all the routines used by the server
to to its job, like send files, etc... */

#include <limits.h>

#define PROMPT ">\0"
#define GOODBYE "Què passi un bon dia..."
#define UNKNOWN "No entenc aquesta ordre"
#define CD_ERROR "Error en CD"
#define MKDIR_ERROR "Error al crear el directori"
#define RMDIR_ERROR "Error a l'esborrar el directori"
#define ROOT "/home/jordi/share/pub"

typedef enum transtype {ASCII, BINARY} t_transtype;

typedef struct server_status{
     unsigned char root[PATH_MAX];
     enum transtype transmission;
};

typedef struct  session_t{
     char user[128];
     unsigned char logged_in;
};
     
int send_textfile(const char *path, t_host *c);
int server_prompt(t_host *c);
int auth_prompt(t_host *c);
int sys_auth_user (const char*username, const char*password);
int ftp_cd(t_host *c, char *dirname);
int ftp_cdup(t_host *c);
int ftp_mkdir(t_host *c, char *dirname);
int ftp_rmdir(t_host *c, char *dirname);
int ftp_ls(t_host *c, char *dirname);

