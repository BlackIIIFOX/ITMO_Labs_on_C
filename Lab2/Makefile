CC=gcc
CFLAGS=-Wall -Wextra --std=gnu99 -pedantic -Werror
PROJECT_CAT=my_cat
PROJECT_XARGS=my_xargs

all: my_cat my_xargs

my_cat: my_cat.o sb.o
	$(CC) my_cat.o sb.o -o $(PROJECT_CAT) $(CFLAGS)

my_xargs: my_xargs.o
	$(CC) my_xargs.o -o $(PROJECT_XARGS) $(CFLAGS)

my_cat.o: my_cat.c
	$(CC) -c my_cat.c $(CFLAGS)

sb.o: sb.c
	$(CC) -c sb.c $(CFLAGS)

my_xargs.o:  my_xargs.c
	$(CC) -c my_xargs.c $(CFLAGS)

clean:
	rm -f *.o $(PROJECT_CAT)
	rm -f *.o $(PROJECT_XARGS)