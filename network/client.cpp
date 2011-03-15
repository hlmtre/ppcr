/*
    Demonstration of a client-server application that communicates via sockets
    This code is a modification of Beej's example code
    http://beej.us/guide/bgnet/

    This client
        1) receives a message from the server
        2) sends a message to the server
        3) exits
*/
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    struct hostent *remote_hostent;
    struct sockaddr_in remote_machine_addr; // connector's address information 
    int port;
    const char *hostname;

	if (argc != 3)
	{
		cerr << "Usage: ./client 'internet address' 'port'" << endl;
		exit(1);
	}
    // default port, should change so you don't conflict w/other students
    port = atoi(argv[2]); 

    // attempts to connect to listening server at internet address specified by argument 1
	hostname = argv[1];

    // the name of the remote host is specified
    // need to get the address of the machine
    if ((remote_hostent=gethostbyname(hostname)) == NULL) {  
        perror("gethostbyname");
        exit(1);
    }

    // STEP 1: create the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // specify the ordering of the bytes
    remote_machine_addr.sin_family = AF_INET;  

    // we have the port as an unsigned short integer
    // need to convert it to network byte order
    remote_machine_addr.sin_port = htons(port);  // short, network byte order 

    // need to specify which host this socket is to connect to
    remote_machine_addr.sin_addr = *((struct in_addr *)remote_hostent->h_addr);

    // set the other members to zero 
    memset(&(remote_machine_addr.sin_zero), '\0', 8);  

    // STEP 2: connect to the socket
    if (connect(sockfd, (struct sockaddr *)&remote_machine_addr, 
    	sizeof(struct sockaddr)) == -1) 
    {
        perror("connect");
        exit(1);
    }
    cout << "client now connected to server" << endl;
    // sleep to make the demonstration easier to see
    sleep(3);

    // STEP 3: the communication channel is complete
    // client and server can communicate via send() and recv()
    const int MAX = 100;
    char message_buf[MAX];
    if ((numbytes=recv(sockfd, message_buf, MAX-1, 0)) == -1) 
    {
         perror("receive from server error");
         exit(1);
    }

    // the buffer is not null terminated
    message_buf[numbytes] = 0;
    cout << "client received: " << message_buf << endl;

    string message;
    cout << "enter text to send to server: ";
    while (cin)
    {
    getline(cin,message);

    cout << "client sending <" << message << "> to server" << endl;
    
	if (send(sockfd, message.c_str(), message.size(), 0) == -1)
	{
        perror("send to server error");
        exit(1);
    }
    cout << "client: send to server worked" << endl;
    }
//    cout << "client exiting" << endl;
//    close(sockfd);
}
