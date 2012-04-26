CC=gcc
CFLAGS=-W -Wextra -Wall -std=c99 -c

hunt: hunt.o animals.o mylibrary.o
	$(CC) -o hunt hunt.o animals.o mylibrary.o

hunt.o: hunt.c animals.h weapons.h info.h mylibrary.h
	$(CC) $(CFLAGS) hunt.c

animals.o: animals.c animals.h
	$(CC) $(CFLAGS) animals.c

mylibrary.o: mylibrary.c mylibrary.h
	$(CC) $(CFLAGS) mylibrary.c

clean: 
	rm -fr *\.o hunt
