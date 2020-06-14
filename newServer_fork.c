//Server using client socketas a struct :
//TCP server code: 	tcp_serv.c using threads:

#include<stdio.h>
#include<sys/socket.h>		//for sockets
#include<stdlib.h>
#include<netinet/in.h>		//for the AF_INT(Address Family)
#include<sys/types.h>
#include <string.h>
#include<netdb.h>			//Includes definition of the network database operation.
#include<arpa/inet.h>
#include<pthread.h>			//to operates on the threads:


//Parameters Definition:
#define BUF_SIZE  200
#define PORT 8080
#define SA struct sockaddr	
#define MAX_CLIENT 10

typedef struct{
	struct sockaddr_in addr;
	int sockfd;
	int uid;
} client_struct;

//global declaration:
client_struct *clients[MAX_CLIENT];
unsigned int clnt_count = 0;
int static uid = 10;

//Add clients to the queue:
void queue_add(client_struct *cli)
{
	for(int idx = 0;idx < MAX_CLIENT; idx++)
	{
		if(!clients[idx])
		{
			clients[idx] = cli;
			break;
		}
	}
}

//Remove the client from the queue:
void queue_remove(int uid)
{
	for(int idx = 0;idx < MAX_CLIENT; idx++)
	{
		if(clients[idx])
		{
			if(clients[idx]->uid == uid)
			{
				clients[idx] = NULL;
				break;
			}
		}
	}
}

//Send msg to all clients expect the sender:
vois send_msg(char *buffer,int uid)
{
	for(int idx = 0;idx < MAX_CLIENT; idx++)
	{
		if(clients[idx])
		{
			if(clients[idx]->uid == uid)
			{
				if(write(clients[idx]->sockfd,buffer,strlen(buffer)) < 0)
				{
					printf("[-] ERROR : While send_msg.\n");
					break;
				}
			}
		}
	}
}

//Handle all the clients connection:
void * handle_client(void *arg)
{
	char buffer[BUF_SIZE];
	int exit_flag = 0;
	int recv_msg;
	clnt_count++;
	client_struct *cli = (client_struct *)arg;
	
	bzero(buffer,sizeof(buffer));
	while(1)
	{
		if(exit_flag)
			break;
		recv_msg = recv(cli->sockfd,buffer,sizeof(buffer),0);
		if(recv_msg > 0)		//Return some byte_size
		{
			if(strlen(buffer)>0)
			{
				send_msg(buffer,cli->uid);
			}
		}
		
		else if(recv_msg == 0 || (strcmp(buffer,"exit")) < 0)
		{
			printf("[-] Disconnected client : %d \n",uid);
			send_msg(buffer,cli->uid);
			printf("%s \n",buffer);
			exit_flag = 1;
			
		}
		
		else
		{
			printf("[-] ERROR : recevied_msg.\n");
			exit_flag = 1;
		}
		
		bzero(buffer,sizeof(buffer));
	}
	
	//Close and terminate the client:
	close(cli->sockfd);
	queue_remove(cli->uid);
	free(cli);
	clnt_count--;
	return NULL;
}

int main()
{
	int server_fd,bind_ack,len;
	struct sockaddr_in serverAddr;
	int client_fd;
	struct sockaddr_in newAddr;
	char buffer[BUF_SIZE] = {0};
	int idx=0;
	pthread_t tid;
	
	//Creating server socket:
	server_fd = socket(AF_INET,SOCK_STREAM,0);
	if(server_fd < 0)
	{
		printf("[-] ERROR : While creating the client socket.\n");
		exit(1);
	}
	printf("[+] Server socket Created Successfully.\n");
	
	 memset(&serverAddr,'\0',sizeof(serverAddr));
	//bzero(&serverAddr,sizeof(serverAddr));
	
	//Assign IP and PORTS:
	serverAddr.sin_family = AF_INET; 
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	serverAddr.sin_port = htons(PORT); 
	
	//Binding socket to address provided:
	bind_ack = bind(server_fd,(SA*)&serverAddr,sizeof(serverAddr));
	if(bind_ack < 0)
	{
		printf("[-] ERROR : while binding server.\n");
		exit(1);
	}
	
	printf("[+] Successfully Bind the server to port :%d\n",PORT);
	
	//Listening the call by clients:
	if(listen(server_fd,5)==0)				//Can allow upto 5 client in the pending queue:
	{
		printf("[+] Listening..\n");
	}
	else
	{
		printf("[-] ERROR : while Listening client.\n");
		exit(1);
	}
	
	len = sizeof(newAddr);
	while(1)
	{
		client_fd = accept(server_fd,(SA*)&newAddr,&len);
		
		//Check for maximum limit:
		if(clnt_count + 1 == MAX_CLIENT)
		{
			printf("Reached max limit.\n");
			close(client_fd);
			continue;
			
		}
		
		client_struct *cli = (client_struct*)malloc(sizeof(client_struct));
		cli->addr = newAddr;
		cli->sockfd = client_fd;
		cli->uid = uid++;
		
		//Add client to the queue:
		queue_add(cli);
		
		//create Threads:
		pthread_create(&tid,NULL,&handle_client,(void*)cli);
		
		sleep(1);
		
	}
	
	return 0;
}