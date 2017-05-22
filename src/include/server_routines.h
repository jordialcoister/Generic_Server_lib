/* This header include all the routines used by the server
to to its job, like send files, etc... */

#include <limits.h>

#define PROMPT ">\0"
#define GOODBYE "Què passi un bon dia..."
#define UNKNOWN "No entenc aquesta ordre"
#define CD_ERROR "Error en CD"
#define ROOT "/home/jordi/share/pub"

typedef enum transtype {ASCII, BINARY};

typedef struct server_status {
     unsigned char root[PATH_MAX];
     enum transtype transmission;
};
     
int send_textfile(const char *path, t_client *c);
int server_prompt(t_client *c);
int sys_auth_user (const char*username, const char*password);
int ftp_cd(t_client *c, char *dirname);
int ftp_cdup(t_client *c);
int ftp_mkdir(t_client *c, char *dirname);
int ftp_rmdir(t_client *c, char *dirname);
int ftp_ls(t_client *c);

