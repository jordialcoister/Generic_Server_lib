/* 
   We want a function that given a socket, an IP address and a port
   make a connnection to that address and on that port.  I must return
   a descriptor that we could use to send and receive data.
*/


/* 
   In other order of things, we need a create_server function that
   given a local address, and a port, create a server ready to wait
   for connections (later on its own thread)
*/ 
#include <netinet/in.h>
#include <stdio.h>


#define PORT 5678
#define BACKLOG 2
#define ERROR_1 "1: Error al crear el socket\n"
#define ERROR_2 "2: Error durante el bind\n"
#define ERROR_3 "3: Error durante el lisen\n"
#define DEF_ERR "Error desconocido\n"
#define MAXBUFSIZE 512


typedef struct {
     int sd; 			/* The socket descriptor for the server */
     struct sockaddr_in addr;   /* The address to listen at */
     char outbuf[MAXBUFSIZE];	/* The buffer to send to client */
     char inbuf[MAXBUFSIZE];	/* The buffer to receive from client */
} t_host;

/* typedef struct { */
/*      int sd; */
/*      struct sockaddr_in addr; */
/*      char outbuf[MAXBUFSIZE];	/\* The buffers to send to/receive from				 * the server *\/ */
/*      char inbuf[MAXBUFSIZE]; */
/* } t_client; */

const char* print_error();
/*int create_server(int *fd, const char* address, unsigned int port);*/
int create_server(t_host *s, int port, const char *address, char options);
int run_server(t_host *s);
int close_server(t_host *s);
void *server_core(void *client);
void *cleaner(void *pool);


