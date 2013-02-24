CC=gcc
CFLAGS=-W -Wextra -Wall -std=c99 -c

hunt: hunt.o animals.o misc.o
	$(CC) -o hunt hunt.o animals.o misc.o

hunt.o: hunt.c animals.h weapons.h misc.h info.h
	$(CC) $(CFLAGS) hunt.c

animals.o: animals.c animals.h
	$(CC) $(CFLAGS) animals.c

misc.o: misc.c misc.h
	$(CC) $(CFLAGS) misc.c

clean:
	rm -fr *\.o hunt
