#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>


#define buflen 1000


void main(int argc, char* argv[]) {

	int server_id, len, recv_len, port;
	struct sockaddr_in server_sock, client_sock;
	char buffer[buflen], message[1024];


	//Check if argument has been passed
	if( argc != 2 ) {
		printf("ERROR: Incorrect arguments\n");
		printf("USAGE: ./server <port>\n");
		exit(1);
	} else {
		port = atoi(argv[1]);
	}

	
	//Create a socket
	if((server_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		perror("ERROR: Socket could not be created");
		exit(1);
	}

	
	//Setting up the sockaddr_in struct
	memset((char *) &server_sock, 0, sizeof(server_sock));
	server_sock.sin_family = AF_INET;
	server_sock.sin_port = htons(port);
	server_sock.sin_addr.s_addr = htonl(INADDR_ANY);

	
	//Bind the socket
	if(bind(server_id, (struct sockaddr *) &server_sock, sizeof(server_sock)) == -1) {
		perror("ERROR: Bind failed");
		exit(1);
	}


	//Infinite loop to accept incoming packets
	while(1) {
		memset(buffer, '\0', buflen);
		printf("Waiting for data");
		fflush(stdout);

		//Recieve packets
		len = sizeof(client_sock);
		if( (recv_len = recvfrom(server_id, buffer, buflen, 0, (struct sockaddr*) &client_sock, &len ))  == -1) {
			printf("ERROR: Could not Recieve data\n");
			exit(1);
		}
		printf("Received packet from %s:%d\n", inet_ntoa(client_sock.sin_addr), ntohs(client_sock.sin_port));
		// printf("REACHED HERE\n");
        printf("Data: %d\n" , atoi(buffer));
        int max = atoi(buffer);
        // memset(message, '\0', 1024);
        // sprintf(message, "%d : %p", )

        for(int i=0; i<max; i++){
        	memset(message, '\0', 1024);
	        sprintf(message, "%d : %s", i, buffer);
        	printf("%s\n", message);
        	if( (sendto(server_id, message, 1024, 0, (struct sockaddr*) &client_sock, len)) == -1 ) {
                	printf("ERROR: Could not Recieve data\n");
        			exit(1);
        	}
        }
	}

	close(server_sock);
}
