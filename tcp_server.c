//TCP Server Program:
///////////////////////////
// This code build through following step for TCP Server:
//	1. Use create() : Create the TCP socket.
//	2. Use bind()	: Bind the socket to server address.
//	3. Use listen()	: Put server socket in Passive mode to wait for client to apporach.
//	4. Use accept()	: Build connection between client and server.
//	5. Go Back to STEP-3.
///////////////////////////

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
#define CLADDR_LEN 100
#define SA struct sockaddr

//Function for the chat between client and server:
void ser_cln_chat( int sockfd)
{
	char buff[BUF_SIZE]; 
	int n; 
    	// infinite loop for chat 
	for (;;) 
	{ 
		bzero(buff, sizeof(buff)); 
	  	printf("Enter the string : "); 
        	n = 0; 	
		while ((buff[n++] = getchar()) != '\n'); 	// copy server message in the buffer 
	  	write(sockfd, buff, sizeof(buff)); 		// and send that buffer to client 

		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Client : %s", buff); 
		if ((strncmp(buff, "exit", 4)) == 0) 
		{ 
		    printf("Server Exit...\n"); 
		    break; 
        	} 
        } 

}

// Main function:
int main()
{
	int server_fd, conn_fd, len;
	struct sockaddr_in servaddr, cli; 
	int bind_ack;
	int client_ack;
	char clientAddr[CLADDR_LEN];
	
	//Create Socket:
	server_fd = socket(AF_INET, SOCK_STREAM, 0);		////create socket file descriptor
	if(server_fd < 0)
	{
		printf("ERROR : while creating socket.\n");
		exit(1);
	}
	else
		printf("Socket created.\n");
	
	bzero(&servaddr,sizeof(servaddr));
	
	//Assign IP and PORTS:
	servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT);
	
	//Binding created socket to given IP and verification:
	bind_ack = bind(server_fd, (SA*)&servaddr, sizeof(servaddr));
	if(bind_ack < 0)
	{
		printf("ERROR : while binding socket to server.\n");
		exit(1);
	}
	else
		printf("Binding Done.\n");
	
	//Listen Server:
	if ((listen(server_fd, 5)) != 0) 
	{ 
        printf("Listen failed...\n"); 
        exit(1); 
    } 
    else
        printf("Server listening..\n"); 
	
	//Accepting MSG from the client:
	len = sizeof(cli);
	client_ack =accept(server_fd, (SA*)&cli, &len); 
	if(client_ack < 0)
	{
		printf("ERROR : while accepting connection.\n");
		exit(1);
	}
	else
		printf("Server Accept the Clinet.\n");
	
	inet_ntop(AF_INET, &(cli.sin_addr), clientAddr, CLADDR_LEN);	//convert IPv4 addresses between binary and text form
	printf("Connection accepted from %s...\n", clientAddr);
	
	
	//Call for chat function created above :
	ser_cln_chat(client_ack);
	
	//After chatting close the socket
	close(server_fd);
	
	return 0;
}

