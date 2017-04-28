CC=gcc

cient/client.o : client/client.c $(headers)
	$(CC) -c client/client.c

server/server.o : server/server.c $(headers)
	$(CC) -c server/server.c

