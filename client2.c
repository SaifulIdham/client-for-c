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

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("192.168.232.135");
	server.sin_port = htons(2345);
		
	if(connect(sock, (struct sockaddr *) &server, sizeof(server))<0)
	{
		perror("Connect failed. error");
		return 1;
	}
	puts("Connected\n");

	while(1){
	printf("Enter message : ");
	scanf("%s", message);
	
	char a[200];
	strcpy(a,message);
	
	if(strcasecmp(a, "exit")==0 || (strcasecmp(a, "quit") == 0))
	{
		break;
	}
	else;
	
	send(sock, message, 200, 0);
	
	recv(sock, server_reply, 200, 0);
	
	puts("Server reply : ");
	puts(server_reply);
	}
	close(sock);
	return 0;


}
