CC=gcc
CFLAGS=-Wall

SRC_DIR=src

FILES = $(SRC_DIR)/main.c $(SRC_DIR)/xmod.c $(SRC_DIR)/filesystem.c $(SRC_DIR)/utils.c $(SRC_DIR)/inputcheck.c $(SRC_DIR)/logger.c $(SRC_DIR)/signals.c

make: ${FILES}
	$(CC) $(CFLAGS) -o xmod ${FILES}

clean:
	rm -f xmod

