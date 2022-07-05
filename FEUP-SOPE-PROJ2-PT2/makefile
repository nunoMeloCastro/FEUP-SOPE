CC=gcc
CFLAGS=-Wall

SRC_DIR=src
S_DIR=server
C_DIR=client
L_DIR=lib

FILES = $(SRC_DIR)/$(S_DIR)/server.c $(SRC_DIR)/utils.c $(SRC_DIR)/$(S_DIR)/input_check.c $(SRC_DIR)/$(S_DIR)/fifo.c $(SRC_DIR)/$(S_DIR)/buffer.c $(SRC_DIR)/$(S_DIR)/thread.c

all: server client

server: ${FILES} $(SRC_DIR)/$(L_DIR)/lib.o $(SRC_DIR)/$(L_DIR)/delay.c $(SRC_DIR)/$(L_DIR)/delay.h
	gcc -Wall -DDELAY=100 -o s $(SRC_DIR)/$(L_DIR)/delay.c $(SRC_DIR)/$(L_DIR)/lib.o ${FILES} -pthread

client: $(SRC_DIR)/$(C_DIR)/client.c $(SRC_DIR)/$(C_DIR)/common.h
	$(CC) $(CFLAGS) -o c $(SRC_DIR)/$(C_DIR)/client.c -pthread

# server.o: server.c common.h
#  	$(CC) $(CFLAGS) -c -o server.o server.c

# lib.o: lib.c lib.h
#  	$(CC) $(CFLAGS) -c -o lib.o lib.c

clean:
	rm -f s c