CC=gcc

client/client.o : client/client.c $(headers)
	$(CC) -c client/client.c

server/server.o : server/server.c $(headers)
	$(CC) -c server/server.c
	
clean: 
	rm prog client/client.o server/server.o
