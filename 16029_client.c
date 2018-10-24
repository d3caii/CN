#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#define buflen 1024



//Functions
// void *send_message(void *args);
// void *recv_message(void *args);

//Structs
// struct multiple_args{
// 	int id;
// 	struct sockaddr_in server;
// };

//Global Variables
// pthread_t send_msg, recv_msg;



 
void main(int argc, char* argv[]) {
    struct sockaddr_in server;
    int client_id, len, port, seq_no;
    char buffer[buflen];
    // struct multiple_args arguments;
 
    if (argc != 3) {
    	perror("USAGE: ./client <ip> <port>\n");
    	exit(1);
    } else {
    	port = atoi(argv[2]);
    }


    if ( (client_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("ERROR: Creating socket");
        exit(1);
    }
 
    memset((char *) &server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(argv[1]);	
 	len = sizeof(server);

 	//Setting up the argument structure
 	// arguments.id = client_id;
 	// arguments.server = server;

 	// int send_id = pthread_create(&send_msg, NULL, send_message, (void *)&arguments);
 	// int recv_id = pthread_create(&recv_msg, NULL, recv_message, (void *)&arguments);
 	// pthread_join(send_msg, 0);
 	// pthread_join(recv_msg, 0);

    while(1) {
        memset(buffer,'\0', buflen);
        printf("Enter message : ");
        fgets(buffer, 1024, stdin);
        // gets(buffer);

        //send the message
        if ((sendto(client_id, buffer, strlen(buffer) , 0 , (struct sockaddr *) &server, len)) == -1) {
            perror("ERROR: Sending data");
        	exit(1);
        }

     	// if ((recvfrom(client_id, buffer, buflen, 0, (struct sockaddr *) &server, &len)) == -1) {
      //       perror("ERROR : Receiving data");
      //       // printf("%d\n", i);
      //   	exit(1);
      //   }
        int max = atoi(buffer);
        int count = 0;
        int i = 0;
        do{
        	//receive a reply and print it
            //clear the buffer by filling null, it might have previously received data
            memset(buffer,'\0', buflen);
            //try to receive some data, this is a blocking call
            if ((recvfrom(client_id, buffer, buflen, 0, (struct sockaddr *) &server, &len)) == -1) {
                perror("ERROR : Receiving data");
                // printf("%d\n", i);
            	exit(1);
            }
            // puts(buffer);
        	count++;
        	i++;
            char * ch;
            ch = strtok(buffer, " : ");
         	seq_no = atoi(ch);
         	while(ch != NULL){
         		ch = strtok(NULL, " : ");
         	}
            printf("%d : count = %d\n", seq_no, count);
        }while(i<max);
    }
 
    close(client_id);
}
