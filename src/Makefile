IDIR=./include/
CC=gcc
CFLAGS=-g -W -I$(IDIR)
LIBS=-pthread -lcrypt

OBJ=server.o server_side.o server_routines.o

all: server

server: $(OBJ)
	clear
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

server.o: server.c server_side.h
	$(CC) $(CFLAGS) -c -o $@ server.c

server_side.o: server_side.c server_side.h
	$(CC) $(CFLAGS) -c -o $@ server_side.c

server_routines.o: server_routines.c server_side.h
	$(CC) $(CFLAGS) -c -o $@ server_routines.c

.PHONY: clean

clean:
	$(RM) *.o core 
