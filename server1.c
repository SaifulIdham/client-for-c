#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int socket_desc, client_sock, c, read_size;
	struct sockaddr_in server, client;
	char client_message[2000];
	
	socket_desc = socket(AF_INET, SOCK_STREAM,0);
	if(socket_desc == -1)
	{
		printf("Could not create");
	}
	puts("Socket created\n");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(36808);

	if(bind(socket_desc, (struct sockaddr *) &server, 
sizeof(server))<0)
	{
		perror("bind failed. error");
	}
	puts("Bind done\n");

	listen(socket_desc, 3);

	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);

	client_sock = accept(socket_desc, (struct sockaddr *) &client, 
(socklen_t*) &c);
	if(client_sock<0)
	{
		perror("accept failed");
	}
	puts("Connection accepted");

	read_size = recv(client_sock, client_message, 2000, 0);
	char s[] = "Assalamualaikum";
	char a[2000];
	strcpy(a, client_message);
	if(strcasecmp(a,s)==0)
	{
		strcpy(client_message, "Waalaikumslam\n");
	}
	else
	{
		strcpy(client_message, "Are you a muslim?\n");
	}
	write(client_sock, client_message, strlen(client_message));
	if(read_size==0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size ==-1)
	{
		perror("recv failed");
	}	
	return 0;



}
