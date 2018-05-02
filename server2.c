#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int socket_desc, client_sock, c, hostN, rc, option_len;
	int optval, optlen, optval1;
	struct sockaddr_in server, client;
	char client_message[2000], host[2000], clientname[2000], 
myIP[50], clientIP[20], forMTU[20];
	int value = 1, novalue = 0;	
	struct ifreq ifr;
	
	socket_desc = socket(AF_INET, SOCK_STREAM,0);
	if(socket_desc == -1)
	{
		printf("Could not create");
	}
	puts("Socket created\n");

	hostN = gethostname(host, sizeof(host));
	
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(2345);

	if(bind(socket_desc, (struct sockaddr *) &server, 
sizeof(server))<0)
	{
		perror("bind failed. error");
	}
	puts("Bind done\n");

	listen(socket_desc, 3);
	
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);

	client_sock = accept(socket_desc, (struct sockaddr *) 
&client, (socklen_t *) &c);

	if(client_sock<0)
	{
		perror("accept failed");
	}
	puts("Connection accepted");
	
	bzero(&server, sizeof(server));
	int len = sizeof(server);
	getsockname(client_sock, (struct sockaddr *) &server, &len);	
	inet_ntop(AF_INET, &server.sin_addr, myIP, sizeof(myIP));
	int myPort = ntohs(server.sin_port);
	char pa[100];
	sprintf(pa, "%d", myPort);
	int lei = sizeof(client);
	getpeername(client_sock, (struct sockaddr *) &client, &lei);
	inet_ntop(AF_INET, &client.sin_addr, clientIP, sizeof(clientIP));
						
	while(1)
	{
	recv(client_sock, client_message, 200, 0);
	char s[] = "Assalamualaikum";
	char a[2000];
	strcpy(a, client_message);

	if(strcasecmp(a,s) == 0)
	{
		strcpy(client_message, "Waalaikumslam");		
	}
	else if(strcasecmp(a, "hostname") == 0)
	{
		strcpy(client_message, host);
	}
	else if(strcasecmp(a, "hostip") == 0)
	{
		strcpy(client_message, myIP);
	}
	else if(strcasecmp(a, "clientip") == 0)
	{
		strcpy(client_message, clientIP);
	}
	else if(strcasecmp(a, "port") == 0)
	{
		strcpy(client_message, pa);
	}
	else if(strcasecmp(a, "getmtu") == 0)
	{
		strcpy(ifr.ifr_name, "wm0");
		if(!ioctl(socket_desc, SIOCGIFMTU, &ifr))
		{
			ifr.ifr_mtu;
		}
		sprintf(forMTU, "%d", ifr.ifr_mtu);
		strcpy(client_message, forMTU);
	}
	else if(strcasecmp(a, "setmtu") == 0)
	{
		strcpy(client_message, "Please enter new MTU");
		send(client_sock, client_message, 200, 0);
		recv(client_sock, client_message, 200, 0);
		strcpy(ifr.ifr_name, "wm0");
		if(!ioctl(socket_desc, SIOCGIFMTU, &ifr))
		{
			ifr.ifr_mtu;
		}
		int intmtu = atoi(client_message);
		ifr.ifr_mtu = intmtu;
		if(!ioctl(socket_desc, SIOCSIFMTU, &ifr))
		{
				//success
		}
	}
	else if(strcasecmp(a, "getoob") == 0)
	{
		optlen = sizeof(optval);
		getsockopt(socket_desc, SOL_SOCKET, SO_OOBINLINE, 
(char *)&optval, &optlen);
		if(optval == 0)
			strcpy(client_message, "out of normal queue/disable");
		else
			strcpy(client_message, "in normal queue/enable");
	}
	else if(strcasecmp(a, "setoob") == 0)
	{
		strcpy(client_message, "set out or in queue?");
		send(client_sock, client_message, 200, 0);
		recv(client_sock, client_message, 200, 0);
		char j[200];
		strcpy(j, client_message);
		if(strcasecmp(j, "in") == 0)
		{
		optval1 = 1;
		setsockopt(socket_desc, SOL_SOCKET, SO_OOBINLINE, 
(char *) &optval1, sizeof(int));
		}
		if(strcasecmp(j, "out") == 0)
		{
		optval1 = 0;
		setsockopt(socket_desc, SOL_SOCKET, SO_OOBINLINE, 
(char *) &optval1, sizeof(int));
		}
	}
	else if((strcasecmp(a, "exit")) == 0 || (strcasecmp(a, "quit") 
== 0))
	{
		break;
	}
	else if(strcasecmp(a, "getbc") == 0)
	{
		optlen = sizeof(optval);
		getsockopt(socket_desc, SOL_SOCKET, SO_BROADCAST, 
(char *) &optval, &optlen);
		
		if(optval == 0)
			strcpy(client_message, "disable");
		else
			strcpy(client_message, "enable");
	}
	else if(strcasecmp(a, "setbc") == 0)
	{
		strcpy(client_message, "enable or disable?");
		send(client_sock, client_message, 200, 0);
		recv(client_sock, client_message, 200, 0);
		char t[200];
		strcpy(t, client_message);
		if(strcasecmp(t, "en") == 0)
		{
			optval1 = 1;
			setsockopt(socket_desc, SOL_SOCKET, 
SO_BROADCAST, (char *) &optval1, sizeof(int));
		}
		if(strcasecmp(t, "dis") == 0)
		{
			optval1 = 0;
			setsockopt(socket_desc, SOL_SOCKET, 
SO_BROADCAST, (char *) &optval1, sizeof(int));
		}
	}
	else if(strcasecmp(a, "gettcp") == 0)
	{
		optlen = sizeof(optval);
		rc = getsockopt(socket_desc, IPPROTO_TCP, TCP_NODELAY, 
(char *) &optval, &optlen);
		if(optval == 0)
			strcpy(client_message, "TCP NODELAY is disable");
		else
			strcpy(client_message, "TCP NODELAY is enable");
	}
	else if(strcasecmp(a, "settcp") == 0)
	{
		strcpy(client_message, "enable or disable??");
		send(client_sock, client_message, 200, 0);
		recv(client_sock, client_message, 200, 0);
		char i[200];
		strcpy(i, client_message);
		if(strcasecmp(i, "en") == 0)
		{
			optval1 = 1;
			setsockopt(socket_desc, IPPROTO_TCP, 
TCP_NODELAY, (char *) &optval1, sizeof(int));
		}
		else if(strcasecmp(i, "dis") == 0)
		{
			optval1 = 0;
			setsockopt(socket_desc, IPPROTO_TCP, 
TCP_NODELAY, (char *) &optval1, sizeof(int));
		}
	}
	else if(strcasecmp(a, "getalive") == 0)
	{
		optlen = sizeof(optval);
		rc = getsockopt(socket_desc, SOL_SOCKET, SO_KEEPALIVE, 
(char *)&optval, &optlen);
		if(optval == 0)
			strcpy(client_message, "KEEPALIVE IS DISABLE");
		else
			strcpy(client_message, "KEEPALIVE IS ENABLE");
	}
	else if(strcasecmp(a, "setalive") == 0)
	{
		strcpy(client_message, "enable or disable?");
		send(client_sock, client_message, 200, 0);
		recv(client_sock, client_message, 200, 0);
		char p[200];
		strcpy(p, client_message);
		if(strcasecmp(p, "en") == 0)
		{
			optval1 = 1;
			setsockopt(socket_desc, SOL_SOCKET, 
SO_KEEPALIVE, (char *) &optval1, sizeof(int));
		}
		if(strcasecmp(p, "dis") == 0)
		{
			optval1 = 0;
			setsockopt(socket_desc, SOL_SOCKET, 
SO_KEEPALIVE, (char *) &optval1, sizeof(int));
		}
	}
	else
	{
		strcpy(client_message, "invalid");
	}
	send(client_sock, client_message, 200, 0);
	
	}
	return 0;
}
