IDIR=./include/
CC=gcc
CFLAGS=-g -W -I$(IDIR)
LIBS=-pthread -lcrypt

OBJ=server.o server_side.o server_routines.o auth.o

all: server

server: $(OBJ)
	#clear
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

server.o: server.c $(IDIR)server_side.h
	$(CC) $(CFLAGS) -c -o $@ server.c

server_side.o: server_side.c $(IDIR)server_side.h
	$(CC) $(CFLAGS) -c -o $@ server_side.c

server_routines.o: server_routines.c $(IDIR)server_side.h
	$(CC) $(CFLAGS) -c -o $@ server_routines.c

auth.o: auth.c $(IDIR)auth.h
	$(CC) $(CFLAGS) -c -o $@ auth.c

.PHONY: clean

clean:
	$(RM) *.o core 
