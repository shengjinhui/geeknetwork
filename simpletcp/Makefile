
all:client server

logging:logging.c logging.h simpletcp.h
	gcc -c logging.c

server:server.c logging simpletcp.h
	gcc -o server server.c logging.o

client:client.c logging
	gcc -o client client.c logging.o

clean:
	rm client server logging.o