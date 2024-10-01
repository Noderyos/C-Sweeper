
CFLAGS=-Wall -Wextra -Iinclude -Iraylib/include
LDFLAGS=-Lraylib/lib -l:libraylib.a -lm

all: src/main.c
	gcc $(CFLAGS) -o main src/main.c $(LDFLAGS)

clean:
	rm -frv main
