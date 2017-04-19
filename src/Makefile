IDIR=./include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
#LDIR=../lib
LIBS=-pthread

_DEPS=server_side.h
DEPS= $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ= server.o server_side.o
OBJ= $(patsubst %,$(ODIR)/%,$(_OBJ))

#$(ODIR)/%.o: %.c #$(DEPS)
#	$(CC) -c -o $@ %< $(CFLAGS)

obj/server.o: server.c
	$(CC) -c -o $(ODIR)/server.o server.c  $(CFLAGS)

obj/server_side.o: server_side.c $(IDIR)/server_side.h
	$(CC) -c -o $(ODIR)/server_side.o server_side.c $(CFLAGS)

server: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#debug: server.o server_side.o
#	$(CC) -c -o server server.o server_side.o $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
