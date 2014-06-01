#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/telnet.h>
#include "armadillo.h"

/******************************************************************************
**
** login.c - Handle logins
**
******************************************************************************/

int login (int user, char *instr, int where)
{
	int i; 
	char outstr[ARR_SIZE];

	instr[strlen(instr)-2]=0;

	switch (where) {
		case LOGNAME: 
				if (strlen(instr) > NAME_LEN) {
					tell (user,"Name to long - try again\n");
					tell (user,"\nLogin name? ");
					break;
				}
				i=-1;
				do {
					i++;
					instr[i]=tolower(instr[i]);
				} while ((instr[i]!=0) && (isalnum(instr[i])));

				if (instr[i]!=0) {
					tell (user,"Name can only contain letters and numbers - try again\n");
					tell (user,"\nLogin name? ");
					break;
				}
				instr[0]=toupper(instr[0]);

				debug ("Got name for %i: %s",user,instr);
				strcpy (users[user].name,instr);
				users[user].status=LOGPASS;

				tell (user,"Password? ");
				echo_off(user);
				break;

		case LOGPASS:	if (users[user].status != DEAD) {	
					debug ("Got password for %i (%s)",user,users[user].name);
					if (strlen(instr) < 6) {
						tell (user,"Password too short. Please try again\n");
						tell (user,"\nPassword? ");
						return 0;
					}

					if (!checkpassword (user,users[user].name,crypt(instr,SALT))) {
						tell (user,"\nLogin incorrect...please try again\n");
						tell (user,"\nLogin name? ");
						users[user].status=LOGNAME;
						echo_on (user);
						return 0;
					}
					if (users[user].status==LOGPASS)
						users[user].status=ACTIVE;
					if (users[user].status==LOGNEW) 
						strcpy (users[user].scratch,crypt(instr,SALT));
				}
				break;

		case LOGNEW:
				debug ("Got password re-entry for newbie %i (%s)",user,users[user].name);
				if (strcmp(users[user].scratch,crypt(instr,SALT))) {
					tell (user,"Passwords don't match. Try again.\n");
					tell (user,"\nLogin name? ");
					users[user].status=LOGNAME;
					echo_on (user);
					return 0;
				}
				addpassword (user,users[user].name,users[user].scratch);
				users[user].status=ACTIVE;
				break;

	}
	if (users[user].status==ACTIVE) {
		set_user (user);
		echo_on (user);
		look (user);
		sprintf (outstr,"--> User: %s joined\n",users[user].name);
		message (user,outstr,YES);
		sprintf (outstr,"USERS: User %s logged on from %s",users[user].name,users[user].site);
		logmessage (outstr);

	}
	return 0;
}

void echo_off (int user)
{
	char out[10];
	sprintf (out,"%c%c%c",IAC,WILL,TELOPT_ECHO);
	tell (user,out);
}

void echo_on (int user)
{
	char out[10];
	sprintf (out,"%c%c%c",IAC,WONT,TELOPT_ECHO);
	tell (user,out);
}
