all: 16029_server.c 16029_client.c
	clear
	gcc 16029_server.c -pthread -o server
	gcc 16029_client.c -pthread -o client

server: 
	gcc 16029_server.c -pthread -o server

client: 
	gcc 16029_client.c -pthread -o client

fresh: clean server client

clean: 
	rm server client
