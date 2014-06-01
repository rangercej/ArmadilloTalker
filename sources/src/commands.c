#include <stdio.h>
#include <string.h>
#include "armadillo.h"

/************************************************************************
**
** Armadillo Command Dispatcher + help
**
************************************************************************/

typedef struct {
	char *command;
	int  rank;
	int  class;
	char *shortcuts;
} comstr;

/* Commands Structure: command_name,level,classification,shortcut_chars */
comstr commands[]={
	{"quit",	NEWBIE,	0,	NULL},
	{"emote",	NEWBIE,	0,	";:"},
	{"go",		NEWBIE,	0,	NULL},
	{"who",		NEWBIE, 0,	NULL},
	{"look",	NEWBIE,	0,	NULL},
	{"areas",	NEWBIE, 0,	NULL},
	{"rooms",	NEWBIE, 0,	NULL},
	{"help",	NEWBIE, 0,	"?"},
	{"load",	NEWBIE, 0,	NULL},		/* Dummy command */
	{"smail",	NEWBIE, 0,	NULL},
	{"rmail",	NEWBIE, 0,	NULL},
	{"dmail",	NEWBIE, 0,	NULL},
	{"mailer",	NEWBIE, 0,	NULL},
	{"shout",	NEWBIE,	0,	"!"},
	{"say",		NEWBIE, 0,	"'\""},
	{NULL,0,0,NULL} 
};

int inchar (char *str, unsigned char ch)
{
	int i=0;

	if (str==NULL)
		return -1;

	while ((ch!=str[i]) && (str[i]!=0)) {
		i++;
	}

	if (ch==str[i])
		return i;
	else
		return -1;
}
		
int shortcut (int user, char *instr,char *newcmd)
{
	unsigned char c;
	int i=0;

	if (instr[0] == '.') {
		if (users[user].data.ewtoo) {
			instr++;
		} else {
			instr=instr+5;
		}
	}

	c=instr[0];
	while ((commands[i].command!=NULL) && (inchar(commands[i].shortcuts,c)==-1))
	{
		i++;
	}

	debug ("commands[i].command is: %i -> %p",i,commands[i].command);

	if (commands[i].command==NULL) {
		strcpy (newcmd,".ERROR");
		return 0;
	}

	debug ("Grabbed shortcut");

	strcpy (newcmd,".");
	strcat (newcmd,commands[i].command);
	instr++;
	if (instr[0]==' ') instr++;
	strcat (newcmd," ");
	strcat (newcmd,instr);

	return 0;
}

int abbr (char *instr,char *newcmd)
{
	char raw_word[ARR_SIZE];
	char *word=raw_word;
	int i=0,fflag=0;

	sscanf (instr,"%s",word);
	word++;
	
	debug ("word: %s",word);
	debug ("instr: before while: %s",instr);

	while ((instr[0]!=' ') && (instr[1]!=0)) instr++;
	debug ("after: %s",instr);


	while ((commands[i].command!=NULL)) {
		if (strstr(commands[i].command,word) == commands[i].command) {
			if (!fflag) {
				strcpy (newcmd,".");
				strcat (newcmd,commands[i].command);
				strcat (newcmd,instr);
			}
			fflag++;
		}
		i++;
	}
	debug ("After expansion:");
	debug ("fflag: %d; newcmd: %s",fflag,newcmd);

	if (fflag == 0)
		strcpy (newcmd,".ERROR");
	else
	if (fflag != 1)
		strcpy (newcmd,".AMBIGUOUS"); 

	return 0;
}

int docommand (int user, char *cmdline)
{
	int i=0;
	char newcmd[ARR_SIZE];

	/* Not a command */
	if (cmdline[0]!='.')
		return -1;
	debug ("Found possible command: %s",cmdline);
	abbr(cmdline,newcmd);

	if (strcmp (newcmd,".ERROR"))
		if(!strcmp(newcmd,".AMBIGUOUS")) {
			tell (user,"--> Error: Abbreviation too vague - could be more than one command\n");
			return -2;
		}
		else {
			strcpy (cmdline,newcmd);
		}

	cmdline++;
	while ((commands[i].command!=NULL) && (strstr(cmdline,commands[i].command)!=cmdline)) {
		i++;
	}

	if (commands[i].command==NULL) {
		tell (user,"--> Error: Not a command\n");
		return -2;
	}

	debug ("Got command: %s",cmdline);
	while ((cmdline[0]!=' ') && (cmdline[1]!=0)) cmdline++;
	cmdline++;
/*	while (cmdline[0] <= ' ') cmdline++; */
	if (cmdline[0]==13)
		cmdline[0]=0;
	debug ("Parameters: %s   ([0] = %d)",cmdline,cmdline[0]);

	switch (i) {
		case 0:	quituser(user); break;
		case 1: emote (user,cmdline); break;
		case 2: go (user,cmdline); break;
		case 3: who (user); break;
		case 4: look (user); break;
		case 5: /* .areas */
		case 6: areas (user); break;	
		case 7: help (user); break;
		case 8: tell (user,"=> This is a test command for ambiguity detection [try .lo] .\n"); break;
		case 9: smail (user,cmdline); break;
		case 10: rmail (user,cmdline); break;
		case 11: dmail (user); break;
		case 12: mailer (user,cmdline); break;
		case 13: shout (user,cmdline); break;
		case 14: say (user,cmdline); break;
		default: tell (user,"--> Command not yet implemented.\n");
	}
	return 0;
}

int help (int user)
{
	int i,j;

	tell (user,"\nCommands currently implemented...\n");

	i=0;
	while (commands[i].command!=NULL) {
		j=0;
		while ((commands[i].command!=NULL) && (j < 5)) {
			sprintf (users[user].scratch,".%-10s   ",commands[i].command);
			tell (user,users[user].scratch);
			j++; i++;
		}
		tell (user,"\n");
	}
	tell (user,"\n");

	return 0;
}
