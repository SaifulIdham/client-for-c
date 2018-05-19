#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define SERVER "192.168.232.135"
#define BUFLEN 512
#define PORT 7777

void die(char *sock)
{
	perror(sock);
	exit(1);
}
int main(void)
{
	struct sockaddr_in client;
	int sock, i, clen = sizeof(client);
	char buf[BUFLEN];
	char message[BUFLEN];

	if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}

	memset((char *) &client, 0, sizeof(client));
	client.sin_family = AF_INET;
	client.sin_port = htons(PORT);
	
	if(inet_aton(SERVER, &client.sin_addr) == 0)
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	printf("Enter message : ");
	gets(message);
	
	sendto(sock, message, strlen(message), 0, (struct sockaddr *) &client, clen);
		
	
	recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *) &client, &clen);
	printf("Server reply : ");
	puts(buf);
	close(sock);
	return 0;
}
