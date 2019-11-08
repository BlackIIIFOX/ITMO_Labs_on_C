CC=gcc
CFLAGS=-Wall -Wextra --std=gnu99 -pedantic -Werror -lpthread
PROJECT_INVERT=invert

all: invert

invert:
	$(CC) invert.c -o $(PROJECT_INVERT) $(CFLAGS)

clean:
	rm -f *.o $(PROJECT_INVERT)