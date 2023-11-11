SHELL := /bin/sh
CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
EXEC = keygen encrypt decrypt
OBJS = ss.o numtheory.o randstate.o set.o

all: $(EXEC)

decrypt: $(OBJS) decrypt.o
	$(CC) -o decrypt $(OBJS) decrypt.o `pkg-config --libs gmp`

decrypt.o: decrypt.c
	$(CC) $(CFLAGS) -c decrypt.c

encrypt: $(OBJS) encrypt.o
	$(CC) -o encrypt $(OBJS) encrypt.o `pkg-config --libs gmp`

encrypt.o: encrypt.c
	$(CC) $(CFLAGS) -c encrypt.c

keygen: $(OBJS) keygen.o
	$(CC) -o keygen $(OBJS) keygen.o `pkg-config --libs gmp`

keygen.o: keygen.c
	$(CC) $(CFLAGS) -c keygen.c

ss.o: ss.c
	$(CC) $(CFLAGS) -c ss.c

numtheory.o: numtheory.c
	$(CC) $(CFLAGS) -c numtheory.c

randstate.o: randstate.c
	$(CC) $(CFLAGS) -c randstate.c

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

clean:
	rm -f $(OBJS) $(EXEC) keygen.o encrypt.o decrypt.o

format:
	clang-format -i -style=file *.[ch]

scan-build: clean
	scan-build --use-cc=$(CC) make
