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

#define PORT 5678
#define BACKLOG 2
#define ERROR_1 "1: Error al crear el socket\n"
#define ERROR_2 "2: Error durante el bind\n"
#define ERROR_3 "3: Error durante el lisen\n"
#define DEF_ERR "Error desconocido\n"

const char* print_error();
int create_server(int *fd, const char* address, unsigned int port);
int run_server(int *fd);
int destroy_server(int *fd);
