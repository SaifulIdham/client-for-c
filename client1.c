#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1000], server_reply[2000];

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		printf("Could not created socket\n");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr("192.168.232.135");
	server.sin_family = AF_INET;
	server.sin_port = htons(36808);

	if(connect(sock, (struct sockaddr *) &server, sizeof(server))<0)
	{
		perror("COnnect failed. error");
		return 1;
	}
	puts("Connected\n");

	printf("Enter message : ");
	scanf("%s", message);

	send(sock, message, strlen(message),0);
	
	recv(sock, server_reply, 2000, 0);
	
	puts("Server reply : ");
	puts(server_reply);	
	close(sock);
	return 0;


}
