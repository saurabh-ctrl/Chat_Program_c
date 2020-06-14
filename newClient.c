//New Client try

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
#include<arpa/inet.h>
#include<pthread.h>

//Parameters Definition:
#define BUF_SIZE  200
#define PORT 8080
#define SA struct sockaddr	

//Global variables:
int exit_flag = 0;
int sockfd = 0;

//Send message to the Server:
void send_msg_server()
{
	char msg[BUF_SIZE] = {};
	while(1)
	{
		bzero(msg,sizeof(msg));
		printf("Client : ");
		int idx = 0;
		while((msg[idx++] = getchar()) != '\n');
		send(sockfd,msg,strlen(msg),0);
		if(strcmp(msg,"exit") == 0)
		{
			printf("Client Exit..\n");
			exit_flag = 1;
			break;
		}
	}
}

//Received message from Server:
void recv_msg_server()
{
	char msg[BUF_SIZE] ={};
	while(1)
	{
		bzero(msg,sizeof(msg));
		int recv_msg;
		recv_msg = recv(sockfd,msg,strlen(msg),0);
		if(recv_msg > 0)
		{
			printf("Server : %s\n",msg);
			
		}
		else if(recv_msg == 0)
		{
			printf("[-] Server exired ..\n");
			exit_flag = 1;
			break;
		}
		
		else
		{
			printf("[-] ERROR : received msg from Server.\n");
			break;
			
		}
	}
}

int main()
{
	int len,connect_fd;
	int conn_ack;
	struct sockaddr_in servaddr;
	pthread_t send_msg_thread,recv_msg_thread;
	
	//Create clinet socket:
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
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
	conn_ack = connect(sockfd,(SA*)&servaddr, sizeof(servaddr));
	if(conn_ack < 0)
	{
		printf("ERROR : while connecting to the server.\n");
		exit(1);
	}
	else
		printf("Connected to the server.\n");
	
	if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_server, NULL) != 0)
	{
		printf("[-]ERROR : pthread_create ..\n");
		return -1;
	}
	
	if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_server, NULL) != 0)
	{
		printf("[-]ERROR : pthread_create ..\n");
		return -1;
	}
	
	while(1)
	{
		if(exit_flag == 1)
		{
			printf("[-] Server Disconncted..\n");
			break;
		}
	}
	
	close(sockfd);
	
	return 0;
	
}