#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>

#define BUFLEN 512
#define PORT 7777

void die(char *sock)
{
	perror(sock);
	exit(1);
}

int main(void)
{
	struct sockaddr_in server, client;
	int sock, i, clen = sizeof(client), recv_len;
	char buf[BUFLEN];

	if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		die("socket");
	}

	memset((char *) &server, 0, sizeof(server));
	
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sock, (struct sockaddr *) &server, sizeof(server)) ==-1)
	{
		die("bind");
	}
	
	time_t n;
	struct tm ts;
	time(&n);
	char a[200];
	char bu[200];	
		
	ts = *localtime(&n);
	strftime(a, sizeof(a), "%a %Y-%m-%d %H:%M:%S %2", &ts);
	printf("%s\n", a);

	recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *) &client, &clen);

	strcpy(buf, a);		
	sendto(sock, buf, 200, 0, (struct sockaddr *) &client, clen);
		
	close(sock);
	return 0;
}
