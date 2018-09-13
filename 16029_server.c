#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define KNRM  "\x1B[0m"
#define KGRN  "\x1B[32m"
#define KWHT  "\x1B[37m"
#define max_clients 10

int clients[max_clients];
pthread_t client_threads[max_clients];
int current_clients = 0;
int accept_id, input_thread_id;
pthread_t accept_thread, input_thread;
struct sockaddr_in master, client;

void *accept_conn(void *file_descriptor);
void *server_input(void *file_descriptor);
void *client_listener(void *index_no);

void main(int argc, char* argv[])
{
    int master_id, server_port, client_id, len;
    
    char message[256];
    // socklen_t len;

    //Check if command line arguments have been provided
    //Extract port number
    if (argc < 2) {
        fprintf(stdout, "USAGE : %s port\n", argv[0]);
        return;
    }
    server_port = atoi(argv[1]);
    

    //Create the socket
    master_id = socket(AF_INET, SOCK_STREAM, 0);
    if (master_id < 0) {
        fprintf(stdout, "COULD NOT OPEN SOCKET\n");
        return;
    }
    //Address for the socket
    memset(&master, 0, sizeof(master));
    master.sin_family = AF_INET;
    master.sin_port = htons(server_port);
    master.sin_addr.s_addr = INADDR_ANY;
    len = sizeof(master);
    if (bind(master_id, (struct sockaddr*) &master, len) == 1) {
        fprintf(stdout, "ERROR IN BINDING SOCKET\n");
        return;
    }
    for(int i=0; i<max_clients; i++)
        clients[i] = -1;

    accept_id = pthread_create(&accept_thread, NULL, accept_conn, (void *)&master_id);
    input_thread_id = pthread_create(&input_thread, NULL, server_input, (void *)&master_id);

    pthread_join(accept_thread, NULL);
    pthread_join(input_thread, NULL);
    printf("EXITING\n");
    exit(0);
    // if(listen(master_id, 4) < 0) {
    //     fprintf(stdout, "COULD NOT LISTEN ON SOCKET\n");
    //     return;
    // }
    // len = sizeof(client);
    // client_id = accept(master_id, (struct sockaddr*)&master, &len);
    // recv(client_id, message, 256, 0);

}

void *accept_conn(void *file_descriptor) {
    
    int fd = *((int *) file_descriptor);
    char message[256];
    int client_id;
    if(listen(fd, 3) < 0) {
        printf("COULD NOT LISTEN ON SOCKET\n");
        return NULL;
    }
    while (1) {
        int len = sizeof(master);
        client_id = accept(fd, (struct sockaddr*)&master, &len);
        if(current_clients<max_clients) {
            for (int i=0; i<max_clients; i++) { 
                if (clients[i] <0) {
                    clients[i] = client_id;
                    if(pthread_create(&client_threads[i], NULL, client_listener, (void *)&i) == 0) {
                        current_clients++;
                    }
                    break;
                }
            }
        }
        else {
            // close(client_id);
            current_clients--;
        }
    }

}

void *server_input(void *file_descriptor) {

    int fd = *((int *) file_descriptor);
    char input[30];
    while (1) {
        memset(input, '\0', 30);
        fgets(input, 30, stdin);
        if(strcmp(input, "quit_server\n") == 0) {
            printf("SHUTTING DOWN THE SERVER\n");
            pthread_cancel(accept_thread);
            pthread_cancel(input_thread);
            break;
        }
        else{
            printf("TYPE : %squit%s to exit.\n", KGRN, KNRM);
        }
    }
    return NULL;
}

void *client_listener(void *index_no) {
    int i = *((int *) index_no);
    char input[256];
    printf("Creating new thread id : %d\n", clients[i]);
    while(1) {
        memset(input, '\0', 256);
        int ret = recv(clients[i], input, 256, 0);
        printf("%d\n", ret);
        if ( ret <= 0 ) {
            int tmp = clients[i];
            clients[i] = -1;
            close(tmp);
            printf("Cancel pthread\n");
            pthread_cancel(pthread_self());
            pthread_exit(NULL);
            break;
        }
        else {
            printf("%s\n", input);
            for(int i=0; i<max_clients; i++) {
                if(clients[i] > 0){
                    send(clients[i], input, 256, 0);
                }
            }
        }
    }
}
