CC=gcc
CFLAGS=-Wall -Wextra --std=gnu99 -pedantic -Werror
PROJECT_SERVER=server
PROJECT_CLIENT=client

all: server client

server: server.o settings.h status_server.h
	$(CC) server.o settings.h status_server.h -o $(PROJECT_SERVER) $(CFLAGS)

server.o: server.c
	$(CC) -c server.c $(CFLAGS)

client: client.o settings.h status_server.h
	$(CC) client.o settings.h status_server.h -o $(PROJECT_CLIENT) $(CFLAGS)

client.o: client.c
	$(CC) -c client.c $(CFLAGS)

clean:
	rm -f *.o $(PROJECT_SERVER)
	rm -f *.o $(PROJECT_CLIENT)