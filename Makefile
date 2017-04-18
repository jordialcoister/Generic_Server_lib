CC=gcc
CFLAGS=-g

server: server.o server_side.o
	gcc -o server server.c server_side.c
