#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define maxsize 256
#define KNRM  "\x1B[0m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"


void *send_message(void *file_descriptor);
void *recv_message(void *file_descriptor);
int send_id, recv_id;
pthread_t send_msg, recv_msg;
    

void main(int argc, char* argv[])
{
    int port, client_id;
    struct sockaddr_in client;
    socklen_t len;
    
    if (argc < 3){
        printf("USAGE: %s server_ip port\n", argv[0]);
        return;
    }
    port = atoi(argv[2]);
    client_id = socket(AF_INET, SOCK_STREAM, 0);
    if(client_id < 0) {
        printf("ERROR CREATING SOCKET\n");
        return;
    }
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    client.sin_addr.s_addr = inet_addr(argv[1]);
    len = sizeof(client);
    if(connect(client_id, (struct sockaddr*) &client, len) < 0) {
        printf("ERROR CONNECTING\n");
        return;
    }
    send_id = pthread_create(&send_msg, NULL, send_message, (void *)&client_id);
    recv_id = pthread_create(&recv_msg, NULL, recv_message, (void *)&client_id);
    pthread_join(send_msg, NULL);
    pthread_join(recv_msg, NULL);

    exit(0);
}

void *send_message(void *file_descriptor) {
    int fd = *((int *) file_descriptor);
    char message[maxsize];
    
    while(1) {
        memset(message, '\0', maxsize);
        fgets(message, maxsize, stdin);
        if (strcmp(message, "quit\n") == 0){
            exit(0);
            close(fd);
            pthread_cancel(recv_msg);
            pthread_cancel(send_msg);
            break;
        }
        if(send(fd, message, maxsize, 0) <= 0) {
            printf("ERROR IN SENDING MESSAGE\n");
            close(fd);
            pthread_cancel(recv_msg);
            pthread_cancel(send_msg);
            break;
        }
    }
    return NULL;
}

void *recv_message(void *file_descriptor) {
    int fd = *((int *) file_descriptor);
    char message[maxsize];
    
    while(1) {
        memset(message, '\0', maxsize);
        if(recv(fd, message, maxsize, 0) <= 0) {
            close(fd);
            pthread_cancel(send_msg);
            pthread_cancel(recv_msg);
            break;
        }
        printf("%s\n", message);
    }
}


