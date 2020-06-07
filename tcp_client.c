//TCP Client Program:
///////////////////////////
// This code build through following step for TCP client:
//	1. Create the TCP Socket.
//	2. Connect Newly created client socket to server.
//
//////////////////////////

//Code Begins Here:

#include<stdio.h>
#include<sys/socket.h>		//for sockets
#include<stdlib.h>
#include<netinet/in.h>		//for the AF_INT(Address Family)
#include<sys/types.h>
#include <string.h>
#include<netdb.h>			//Includes definition of the network database operation.

//Parameters Definition:
#define BUF_SIZE  200
#define PORT 8080
#define SA struct sockaddr	

//Function for the chat between client and server:
void ser_cln_chat( int sockfd)
{
	char buff[BUF_SIZE]; 
	int n; 
	for (;;) 
	{ 
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff));
		printf("From Server: %s \n To Server:",buff);
		bzero(buff, sizeof(buff));
		n = 0; 
		while ((buff[n++] = getchar()) != '\n'); 
		write(sockfd, buff, sizeof(buff)); 
 
		if ((strncmp(buff, "exit", 4)) == 0) 
		{ 
			printf("Client Exit...\n"); 
			break; 
        	} 
    	}
}

// Main function:
int main(int argc,char const *argv[])
{
	int client_fd, conn_fd, len;
	struct sockaddr_in servaddr, cli; 
	char * serverAddr;
	int conn_ack;
	
	//Create Socket:
	client_fd = socket(AF_INET, SOCK_STREAM, 0);		//create socket file descriptor
	if(client_fd < 0)
	{
		printf("ERROR : while creating socket.\n");
		exit(1);
	}
	else
		printf("Socket Created.\n");
	bzero(&servaddr, sizeof(servaddr)); 
	
	//Assign IP and PORTS:
	servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
	
	//Connect the client socket to server socket:
	conn_ack = connect(client_fd,(SA*)&servaddr, sizeof(servaddr));
	if(conn_ack < 0)
	{
		printf("ERROR : while connecting to the server.\n");
		exit(1);
	}
	else
		printf("Connected to the server.\n");
	
	//Call for chat function created above :
	ser_cln_chat(client_fd);
	
	//After chatting close the socket
	close(client_fd);
	
	return 0;
}
