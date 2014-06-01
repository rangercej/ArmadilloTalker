#include <stdio.h>
#ifdef _AIX
#include <sys/select.h>
#endif
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "armadillo.h"

int initnetwork()
{
	struct sockaddr_in bind_addr;
	int listen_sock,on,size;

	printf ("Setting up network...");

	size=sizeof(struct sockaddr_in);
	if ((listen_sock=socket(AF_INET,SOCK_STREAM,0))==-1) {
        	perror("\nNETWORK FATAL: Couldn't open listen socket");
	       	server.status=FATAL;
		logmessage ("NETWORK FATAL: Couldn't open listen socket");
		return 0;
        }
	on=1;
	setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,(char *)&on,sizeof(on));
	bind_addr.sin_family=AF_INET;
	bind_addr.sin_addr.s_addr=INADDR_ANY;
	bind_addr.sin_port=htons(server.port);
	if (bind(listen_sock,(struct sockaddr *)&bind_addr,size)==-1) {
        	perror("\nNETWORK FATAL: Couldn't bind to port");
	      	server.status=FATAL;
		logmessage ("NETWORK FATAL: Couldn't bind to port");
  		return 0;
        }
	if (listen(listen_sock,20)==-1) {
        	perror("\nNETWORK FATAL: Listen error"); 
		server.status=FATAL;
		logmessage ("NETWORK FATAL: Listen error");
		return 0;
        }
	fcntl(listen_sock,F_SETFL,O_NDELAY);

	debug ("Server using socket: %d",listen_sock);
	server.socket=listen_sock;

	puts ("done.");

	return 0;
}

char *getsite (struct sockaddr_in *socket_in)
{
	char site_num[255];
	unsigned int addr;
	struct hostent *host;
	static char site[255];

	strcpy(site_num,(char *)inet_ntoa(socket_in->sin_addr));
	addr=inet_addr(site_num);
	if ((host=gethostbyaddr((char *)&addr,4,AF_INET)))
		strcpy(site,host->h_name);
	else 
		strcpy(site,site_num);

	return site;
}

int broadcast (char *message,int override)
{
	int i;

	for (i=0; i < MAX_USERS; i++) {
		if ((users[i].status == ACTIVE) || ((users[i].status!=FREE) && override))
			tell (i,message);
	}
	return 0;
}

int message (int user, char *mess, int touser)
{
	int i;

	for (i=0; i < MAX_USERS; i++) {
		if ((users[i].status == ACTIVE) && (users[i].area==users[user].area)) {
			if ((i!=user) || ((i==user) && (touser==YES)))
				tell (i,mess);
		}
	}
	return 0;
}

int tell (int user, char *mess)
{
	write (users[user].socket,mess,strlen(mess));

	return 0;
}

int clean_user (int user)
{
	close (users[user].socket);
	users[user].status=FREE; 
	users[user].socket=-1;

	return 0;
}

int donenetwork()
{
	printf ("Closing network...");
	close(server.socket);
	puts ("done.");

	return 0;
}
