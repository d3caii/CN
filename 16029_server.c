#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


void main(int argc, char* argv[])
{
    int sock_id, sock_port;
    struct sockaddr_in sock_addr;
    
    //Check if command line arguments have been provided
    //Extract port number
    if (argc < 2) {
        fprintf(stdout, "USAGE : %s port", argv[0]);
    }
    sock_port = atoi(argv[2]);
    

    //Create the socket
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id < 0) {
        fprintf(stderr, "COULD NOT OPEN SOCKET\n");
        return;
    }

    //Address for the socket
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = htons(sock_port);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock_id, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) == 1) {
        fprintf(stderr, "ERROR IN BINDING SOCKET");
    }

    
    
}
