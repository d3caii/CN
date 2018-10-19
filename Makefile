all: server.c client.c
	gcc 16029_server.c -o server
	gcc 16029_client.c -o client

server: 
	gcc 16029_server.c -o server

client: 
	gcc 16029_client.c -o client

clean: 
	rm server client
