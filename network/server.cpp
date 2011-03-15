/*
    Demonstration of a client-server application that communicates via sockets
    This code is a modification of Beej's example code
    http://beej.us/guide/bgnet/

    This server
        1) sends a message to the client
        2) recieves a message from the client
        3) exits
*/
// IMMA TEST COMMENT
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

const int BACKLOG=1; // the max number of pending connections to be queued

int main(int argc, char *argv[])
{
    int port;
    int listen_fd; // listen on sock_fd
    int client_fd; // new connections (from accept)
    struct sockaddr_in my_addr;    // my address information
    struct sockaddr_in client_addr; // connector's address information
    struct sigaction sa;

    if (argc != 2)
    {
	    cerr << "No port specified; terminating" << endl;
	    exit(1);
    }
    // listens on port specified in argv[1]
    port = atoi(argv[1]); 

    // STEP 1: create a socket to listen on
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
        perror("socket");
        exit(1);
    }

    // make socket reuseable so when we restart the problem we won't
    // get a socket in use error
    int yes = 1;
    if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) == -1) 
    {
        perror("setsockopt SO_REUSEADDR");
        exit(1);
    }

    // configure socket information
    my_addr.sin_family = AF_INET;         // host byte order
    my_addr.sin_port = htons(port);       // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct


    // STEP 2: bind the socket to a specific port
    if (bind(listen_fd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    // STEP 3: tell the socket to start listening for connections
    if (listen(listen_fd, BACKLOG) == -1) 
    {
        perror("listen");
        exit(1);
    }

    // STEP 4: wait for a connection request from a client
    cout << "server blocked waiting for client" << endl;
    socklen_t sin_size = sizeof(struct sockaddr_in);
    if ((client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, 
         &sin_size)) == -1) 
    {
            perror("accept client");
            exit(1);
    }

    // STEP 5: the communication channel is complete
    // client and server can communicate via send() and recv()
    cout << "server connected to client: "
         << inet_ntoa(client_addr.sin_addr)
         << endl;

    cout << "server sending \"connected\" message to client" << endl;
    char msg[] = "connected";
    if (send(client_fd, msg, strlen(msg), 0) == -1)
    {
        perror("server send connected to client 1");
        exit(1);
    }
    cout << "server: send to client worked" << endl;

    for (;;)
    {
    const int MAX = 1000;
    char buf[MAX];
    int size;
    if ((size = recv(client_fd, buf, MAX-1, 0)) == -1)
    {
        perror("server recv from client 1");
        exit(1);
    }

    // the buffer is not null terminated
    buf[size] = 0;
    cout << "server received: " << buf << endl;
//    close(client_fd);

//    cout << "server exiting" << endl;
	sleep(1);
	}
//    return 0;
}
