CC=gcc
CFLAGS=-Wall

SRC_DIR=src
S_DIR=server
C_DIR=client

FILES = $(SRC_DIR)/$(C_DIR)/client.c $(SRC_DIR)/utils.c $(SRC_DIR)/$(C_DIR)/input_check.c $(SRC_DIR)/$(C_DIR)/thread.c $(SRC_DIR)/$(C_DIR)/fifo.c

all: server client

server: $(SRC_DIR)/$(S_DIR)/server.o $(SRC_DIR)/$(S_DIR)/lib.o $(SRC_DIR)/$(S_DIR)/delay.c $(SRC_DIR)/$(S_DIR)/delay.h
	gcc -Wall -DDELAY=100 -o s $(SRC_DIR)/$(S_DIR)/delay.c $(SRC_DIR)/$(S_DIR)/lib.o $(SRC_DIR)/$(S_DIR)/server.o -pthread

client: ${FILES} $(SRC_DIR)/$(S_DIR)/common.h
	$(CC) $(CFLAGS) -o c ${FILES} -pthread

# server.o: server.c common.h
# 	gcc -Wall -c -o server.o server.c
#
# lib.o: lib.c lib.h
# 	gcc -Wall -c -o lib.o lib.c

clean:
	rm -f c s