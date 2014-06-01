#include <stdio.h>
#include <string.h>
#include "armadillo.h"

/***********************************************************************
**
** User related commands
**
***********************************************************************/

int quituser (int user)
{
	char outstr[ARR_SIZE];

	users[user].status=DEAD;
	tell (user,"Saving profile...\n");
	save_user (user);
	tell (user,"Goodbye!!!\n");
	clean_user (user);
	sprintf (outstr,"--> User: %s quit\n",users[user].name);
	message (user,outstr,NO);
	sprintf (outstr,"CMD_USER: User quit: %s",users[user].name);
	logmessage (outstr);

	return 0;
}

int say (int user, char *input)
{
	char out[ARR_SIZE];

	if (input[0]==0) {
		tell (user,"--> Syntax: .say <stuff>\n");
		return 0;
	}

	linestripcrlf (input);
	sprintf (out,"%s: %s\n",users[user].name,input);
	message (user,out,YES);

	return 0;
}

int emote (int user,char *input)
{
	char out[ARR_SIZE];

	if (input[0]==0) {
		tell (user,"--> Syntax: .emote <stuff>\n");
		return 0;
	}

	linestripcrlf (input);
	sprintf (out,"%s %s\n",users[user].name,input);
	message (user,out,YES);

	return 0;
}

int who (int user)
{
	int i=0,fflag=0;
	char out[ARR_SIZE];

	for (i=0; i < MAX_USERS; i++) {
		if ((user != i) && (users[i].status == ACTIVE)) {
			if (!fflag)
				tell (user,"Other users currently logged on:\n");
			sprintf (out,"%-20s   %-50s\n",users[i].name,map[users[i].area].name);
			tell (user,out);
			tell (user,"\n");
			fflag=1;
		}
	}
	if (!fflag)
		tell (user,"You're the only one logged in at the moment...\n");

	return 0;
}

int shout (int user, char *input)
{
	int i;
	char out[ARR_SIZE];

	if (input[0]==0) {
		tell (user,"--> Syntax: .shout <stuff>\n");
		return 0;
	}

	strupper (input);
	sprintf (out,"%s shouts: %s\n",users[user].name,input);
	for (i=0; i < MAX_USERS; i++) {
		if ((users[i].status == ACTIVE) && (!users[i].data.earmuffs) && (users[i].data.listen))
			tell (i,out);
	}

	return 0;
}
