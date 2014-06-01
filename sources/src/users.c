#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "armadillo.h"

int initusers()
{
	int i;	

	for (i=0; i<MAX_USERS; i++) {
		users[i].name[0]=0;
		users[i].socket=-1;
		users[i].status=FREE;
	}
	return 0;
}

int add_user(int socket, struct sockaddr_in *socket_in)
{
	int slot=0;
	char name[10];

	while (users[slot].status != FREE) slot++;
	
	users[slot].socket=socket;
	strcpy (users[slot].site,getsite(socket_in));
	sprintf (name,"User_%d",slot);
	strcpy (users[slot].name,name);
	users[slot].status=LOGNAME;
	tell (slot,"Login name? ");

	return slot;
}

int split (int user, char *data)
{
	unsigned long datavals;

	datavals=atol (data);

	users[user].data.ewtoo=(datavals >> 15) & 1;
	users[user].data.gender=(datavals >> 14) & 1;
	users[user].data.rank=(datavals >> 11) & 7;
	users[user].data.earmuffs=(datavals >> 10) & 1;
	users[user].data.igtell=(datavals >> 9) & 1;
	users[user].data.iglogin=(datavals >> 8) & 1;
	users[user].data.igbeeps=(datavals >> 7) & 1;
	users[user].data.igexamine=(datavals >> 6) & 1;
	users[user].data.bold=(datavals >> 5) & 1;
	users[user].data.charmode=(datavals >> 4) & 1;
	users[user].data.timezone=(datavals >> 1) & 7;
	users[user].data.tzdir=(datavals & 1);

	return 0;
}

/** Bits are:
	0: Tzdir (1 = + gmt, 0 = - gmt)		8: Ignore logins
	1: Timezone				9: Ignore tells
	4: Character/line mode			10: Ignore shouts
	5: Bold on/off				11: Rank
	6: Ignore .ex's				14: Gender
	7: Ignore beeping			15: Ewtoo
**/	

unsigned long merge (int user)
{
	unsigned long datavals=0;

	debug ("Merging details: User rank: %d   nasty bit: %ld",users[user].data.rank,(long)users[user].data.rank << 11);
	datavals=((users[user].data.ewtoo << 15) | datavals);
	datavals=((users[user].data.gender << 14) | datavals);
	datavals=((users[user].data.rank << 11) | datavals);
	datavals=((users[user].data.earmuffs << 10) | datavals);
	datavals=((users[user].data.igtell << 9) | datavals);
	datavals=((users[user].data.iglogin << 8) | datavals);
	datavals=((users[user].data.igbeeps << 7) | datavals);
	datavals=((users[user].data.igexamine << 6) | datavals);
	datavals=((users[user].data.bold << 5) | datavals);
	datavals=((users[user].data.charmode << 4) | datavals);
	datavals=((users[user].data.timezone << 1) | datavals);
	datavals=(users[user].data.tzdir | datavals);

	debug ("Datavals=%ld",datavals);

	return datavals;
}

int set_user (int user)
{
	FILE *profile;
	char fname[255];
	char out[ARR_SIZE];
	char in[256];
	float version;
	time_t createtime;

	users[user].area=0;
	users[user].data.gender=NEUTER;
	users[user].data.rank=NEWBIE;
	users[user].data.earmuffs=0;
	users[user].data.igtell=0;
	users[user].data.iglogin=0;
	users[user].data.igbeeps=0;
	users[user].data.igexamine=0;
	users[user].data.bold=0;
	users[user].data.charmode=0;
	users[user].data.timezone=0;
	users[user].data.tzdir=0;
	users[user].data.listen=1;
	users[user].data.async=0;
	users[user].data.ewtoo=1;
	time (&createtime);
	users[user].datecreate=createtime;
	users[user].lines=24;
	users[user].backup=NULL;

	sprintf (fname,"%s/%s/profile/%s",ARMADILLO_HOME,USER_DIR,users[user].name);
	if ((profile=fopen(fname,"r"))==NULL) {
		tell (user,"No profile found...using defaults\n");
		sprintf (out,"USERS: No profile found for user %s",users[user].name);
		logmessage (out);
		return 0;
	}
	debug ("Reading profile");

	tell (user,"Reading profile...\n");

	fgets (in,255,profile);		/* Version */
	version=atof(in);
	fgets (in,255,profile);		/* Data */
	split (user,in);	
	fgets (in,255,profile);		/* Last log in */
	users[user].lastlogin=atol(in);
	fgets (in,255,profile);		/* Last mail read */
	users[user].lastmailread=atol(in);
	fgets (in,255,profile);		/* Date of creation */
	users[user].datecreate=atol(in);
	fgets (in,255,profile);		/* Total login time */
	users[user].totaltime=atol(in);
	fgets (in,255,profile);		/* Description */
	strcpy (users[user].desc,in);
	fgets (in,255,profile);		/* Last login site */

	fclose (profile);

	debug ("Done reading profile");
	debug ("User area is: %d",users[user].area);

	return 0;
}

int save_user (int user)
{
	FILE *profile;
	char fname[255];
	char out[255];
	unsigned long data;
	time_t tm;

	sprintf (fname,"%s/%s/profile/%s",ARMADILLO_HOME,USER_DIR,users[user].name);
	if ((profile=fopen(fname,"w"))==NULL) {
		tell (user,"SYSTEM ERROR: Could not create profile for writing.\n");
		sprintf (out,"Please email the system admin for this system at %s\n",EMAIL_ADDR);
		tell (user,out);

		sprintf (out,"USERS WARNING: Could not create profile for user %s",users[user].name);
		logmessage (out);
		return 0;
	}

	fputs (VERSION,profile);	/* Save version */
	data=merge (user);		/* Merge users.data into integer */
	time (&tm);
	fprintf (profile,"\n%ld\n%ld\n%ld\n%ld\n",data,tm,users[user].lastmailread,users[user].datecreate);
	fprintf (profile,"%ld\n%s\n%s\n",users[user].totaltime,users[user].desc,users[user].site);
	fclose (profile);

	return 0;
}

int input (int user, char *instr)
{
	char shortcmd[ARR_SIZE];
	char outstr[ARR_SIZE];
	int i=0;

	if (strlen(instr)==0)
		return 0;

	while (instr[i]==' ')
		i++;

	if (instr[i] < ' ')
		return 0;	

	if (users[user].data.ewtoo) {
		memmove(instr+1,instr,strlen(instr));
		instr[0] = '.';
	} else {
		if (instr[0] != '.') {
			memmove(instr+5,instr,strlen(instr));
			memcpy(instr,".say ",5);
		}
	}

	debug ("Command before shortcut(): %s",instr);
	shortcut (user,instr,shortcmd);

	debug ("shortcmd = %s",shortcmd);

	if (strcmp(shortcmd,".ERROR"))
		strcpy (instr,shortcmd);

	debug ("Command after shortcut(): %s",instr);

	switch (docommand (user,instr)) {
		case 0:   break;
		case -1:  sprintf (outstr,"%s: %s",users[user].name,instr);
			  message (user,outstr,YES);
			  break;
		case -2:  break;
	}
	return 0;
}

int backup_user(int user)
{
	struct usrstruct *old_user;

	old_user = malloc(sizeof(struct usrstruct));
	bcopy(&users[user],old_user,sizeof(struct usrstruct));
	users[user].backup = old_user;

	return 0;
}

int restore_user(int user)
{
	struct usrstruct *old_user;

	old_user = users[user].backup;
	bcopy(old_user,&users[user],sizeof(struct usrstruct));
	users[user].backup = NULL;
	free (old_user);

	return 0;
}
