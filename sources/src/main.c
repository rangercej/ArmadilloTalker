#ifdef _AIX
#include <sys/select.h>
#endif
#include <sys/time.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "armadillo.h"

/***********************************************************************
**
** Everything centres round the routines in this file
**
***********************************************************************/

int init ()
{
	char log[ARR_SIZE];

	printf ("Armadillo version %s\n",VERSION);
	sprintf (log,"=> BOOTING ARMADILLO VERSION %s",VERSION);
	logmessage ("========================================================");
	logmessage (log);
	logmessage ("--------------------------------------------------------");
	logmessage ("BOOT: Started init()");
	server.status=STARTING;
	server.syslog=1;
	umask (0007);
	if (server.status!=FATAL) readconfig();
	if (server.status!=FATAL) init_syslog();
	if (server.status!=FATAL) initmap();
	if (server.status!=FATAL) initsignals();
	if (server.status!=FATAL) readmap();
	if (server.status!=FATAL) logmessage ("BOOT: Map data read");
	if (server.status!=FATAL) initnetwork();
	if (server.status!=FATAL) logmessage ("BOOT: Connected to network");
	if (server.status!=FATAL) initusers();
	if (server.status!=FATAL) logmessage ("BOOT: Reset user structure");
	if (server.status!=FATAL) logmessage ("BOOT: Passing control to run()");
	if (server.status==STARTING) {
		logmessage ("BOOT: Server status set to ACTIVE");
		server.status=ACTIVE;
	}

	debuginit();

	return 0;
}

int run ()
{
	fd_set readmask;
	struct sockaddr_in acc_addr;
	int accept_sock,size,id,i,inlen;
	char welcome[]="\n\nWelcome to Armadillo !!\n\n";
	char info[50];
	unsigned char instr[ARR_SIZE],outstr[ARR_SIZE];	

	while (server.status!=QUIT) {
	        FD_ZERO(&readmask);
		for (i=0;i < MAX_USERS;i++) {
			if (users[i].socket != -1)
				FD_SET(users[i].socket,&readmask);
		}
		FD_SET(server.socket,&readmask);
		if (select(FD_SETSIZE,&readmask,0,0,0)==-1)
			continue;
		if (FD_ISSET(server.socket,&readmask)) {
			size=sizeof(acc_addr);
			accept_sock=accept(server.socket,(struct sockaddr *)&acc_addr,&size);
			fcntl(accept_sock,F_SETFL,O_NONBLOCK);
			write (accept_sock,welcome,strlen(welcome));
			debug("Got connection on socket %d: ",server.socket);
			id=add_user (accept_sock,&acc_addr);
			debug ("User ID: %d   Socket: %d   Site: %s",id,users[id].socket,users[id].site);
		}		

/*		term_detect (id,instr); */

		for (i=0; i < MAX_USERS; i++) {
			if (users[i].status != FREE ) {
				if (users[i].status==DEAD) {
					quituser(i);
					continue;
				}
				debug ("ASYNC REPORT: User %d has flag %d",i,users[i].data.async);
				if (!users[i].data.async) {
					/* No data? Next user... */
					if (!FD_ISSET(users[i].socket,&readmask))
						continue;
				}

				/* see if client (eg telnet) has closed socket */
				instr[0]=0;
			
				memset (instr,0,sizeof(instr));
				if (!(inlen=read(users[i].socket,instr,sizeof(instr)))) {
					if (!users[i].data.async) {
						close (users[i].socket);
						users[i].socket=-1;
						users[i].status=FREE;
						debug ("Lost connection: %d - connection closed",i);
						sprintf (outstr,"MAIN: Lost connection to %s (%s). Socket closed.",users[i].name,users[i].site);
						logmessage (outstr);
						sprintf (info,"--> User: %s quit (connection lost)\n",users[i].name);
						message (i,info,NO);
					} else {
						strcpy (instr,"\n");
						inlen = 1;
					}
				} else {
					if (users[i].data.async) {
						users[i].data.async = 0;
						debug ("%s has had async flag reset",users[i].name);
					}
					instr[inlen]=0;
					debug ("Got input from %i (%s): %s",i,users[i].name,instr);
					if (instr[0] == 255)
						debug_bytes(instr);

					if (instr[0] != 255) {
						switch (users[i].status) {
							case ACTIVE: input (i,instr); break;
							case LOGNAME: login (i,instr,LOGNAME); break;
							case LOGPASS: login (i,instr,LOGPASS); break;
							case LOGNEW: login (i,instr,LOGNEW); break;
							case EDITOR: edit (i,instr); break;
							case MAIL: rmail (i,instr); break;
							case MAILER: mailer (i,instr); break;
							case DEAD: quituser (i); break;
/*							case TERMDETECT: term_detect (i,instr); break; */
						}
					} else {
						process_telnet(i,instr);
					}
					if (users[i].data.async) {
						i--;
					}
				}
			}
		}
	}
	return 0;
}

int done ()
{
	donenetwork();
	done_syslog();

	return 0;
}

