
CFLAGS=-Wall -Wextra -pedantic -Iinclude
LDFLAGS=

all: src/main.c
	gcc $(CFLAGS) -o main src/main.c $(LDFLAGS)

clean:
	rm -frv main
