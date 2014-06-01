#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "armadillo.h"

#define SHOWLINES 1
#define NOLINES 0
#define LINELENGTH 75

/* Modes for editor */
#define PROMPT 0
#define INSERT 1
#define SEARCH 2
#define DELETE 3
#define EDIT   4
#define WRITE  5
#define AEQUIT 6
#define NEWEDIT 7

/*******************************************************************************
**
** editor.c - A line based text editor...loosely based on ex
**
*******************************************************************************/

int load (int user, char *filename, char **text, int maxlines)
{
	FILE *fedit;
	char out[ARR_SIZE];
	char out2[ARR_SIZE];
	char *home,*work;
	int i=0, warn=0, linesfound=-1;

	if ((fedit=fopen(filename,"r"))==NULL) {
		sprintf (out,"EDITOR: Could not open %s for reading in load()",filename);
		logmessage (out);
		text=NULL;
		return 0;
	}

	work=*text;
	initedit (&work,maxlines);
	if (work==NULL) {
		sprintf (out,"EDITOR: Could not get memory for editor in load()");
		logmessage (out);
		work=NULL;
		return 0;
	}
	home=work;

	debug ("Memory is: %p, %p",work,home);

	fgets (out,80,fedit);
	debug ("Loading file...");
	while (!feof(fedit)) {
		linestripcrlf(out);
		strcpy (work,out);
		i++;
		if ((i > MAX_LINES) && (!warn)) {
			sprintf (out2,"EDITOR: Too many lines for edit (max: %d) in file %s. Truncated.",MAX_LINES,filename);
			logmessage (out2);
			tell (user,"--> WARNING! Too many lines for edit - text has been truncated\n");
			warn=1;
			linesfound=i-1;
		}
		if (!warn) {
			work=work+strlen(out)+1;
			*work=0; *(work+1)=0;
		}
		fgets (out,80,fedit);
	}
	if (linesfound == -1)
		linesfound=i;

#ifdef DEBUG
	debug ("i=%d   linesfound=%d",i,linesfound);
	work=home;
	debug ("Loaded text:");
	for (i=0; i < linesfound; i++) {
		debug ("%d: %s",i,getline (work,i));
	}
#endif
	fclose (fedit);
	*text=home;
	return linesfound;
}

int initedit (char **text, int maxlines)
{
	char *memptr;
	int buffer_length;
	int retcode = 0;

	buffer_length = maxlines * 80;

	memptr=malloc(buffer_length);
	if (memptr==NULL) {
		logmessage ("EDITOR: Memory allocation failed in initedit()");
		retcode = 0;
	} else {
		bzero(memptr,buffer_length);
		*text=memptr;
		retcode = 1;
	}

	return retcode;
}

int edit (int user, char *inpstr)
{
	char buffer[256];
	char cmd;
	int retcode = ED_ABORT;

	debug ("Text buffer is %p",users[user].editor.textbuffer);
	if (users[user].status != EDITOR) {
		backup_user(user);
		showtext (user,users[user].editor.textbuffer,1,users[user].editor.numlines,NOLINES);
		users[user].status=EDITOR;
		if (users[user].editor.numlines == 0) {
			users[user].mode=EDIT;
			users[user].editor.inspoint = users[user].editor.textbuffer;
			tell (user,"--> Enter text. Use a single dot by itself to end.\n");
			tell (user,"> ");
		} else {
			users[user].mode=PROMPT;
			displayprompt(user);
		}
	} else {
		linestripcrlf(inpstr);
		switch (users[user].mode) {
			case PROMPT:	cmd = toupper(inpstr[0]);
					switch(cmd) {
						case 'X':	restore_user(user);
								retcode = ED_SAVE;
								users[user].data.async = 1;
								break;
						case 'A':       restore_user(user);
								retcode = ED_ABORT;
								users[user].data.async = 1;
								break;
						case 'S':	users[user].mode = EDIT;
								users[user].editor.inspoint = users[user].editor.textbuffer;
								tell (user,"--> Enter text. Use a single dot by itself to end.\n");
								tell (user,"> ");
								break;
						default:	tell (user,"--> Sorry, I don't know that command\n");
								displayprompt(user);
					}
					break;
			case EDIT:	if ((inpstr[0] != '.') || (inpstr[1] != 0)) {
						strcpy(users[user].editor.inspoint,inpstr);
						users[user].editor.inspoint = users[user].editor.inspoint + strlen(inpstr) + 1;
						*(users[user].editor.inspoint-1) = '\n';
						users[user].editor.numlines++;
						if (users[user].editor.numlines > users[user].editor.maxlines) {
							sprintf(buffer,"--> Maximum number of lines (%d) reached.\n",users[user].editor.maxlines);
							tell (user,buffer);
							displayprompt(user);
							users[user].mode = PROMPT;
						} else {
							tell (user,"> ");
						}
					} else {
						tell (user,"--> Okay - no more text.\n");
						displayprompt(user);
						users[user].mode = PROMPT;
					}
					break;
			default:	sprintf(buffer,"EDITOR: Unknown mode '%d' detected in edit()",users[user].mode);
					logmessage (buffer);
		}
	}
	return retcode;
}

char *getline (char *text, int linenum)
{
        int i;
        char *block=text;

        for (i=0; i < linenum; i++) {
                block=block+strlen(block)+1;
        }
        return block;
}

int showtext (int user, char *text, int startline, int endline, int numbers)
{
	int i;
	char *block=text;
	char out[256];

	for (i=0; i < (startline-1); i++) {
		block=block+strlen(block)+1;
	}

	for (i=(startline-1); i < endline; i++) {
		if (numbers)
			sprintf (out,"%2d: %s\n",i+1,block);
		else 
			sprintf (out,"%s\n",block);
		tell (user,out);
		block=block+strlen(block)+1;
	}

	return 0;
}

void displayprompt (int user)
{
	tell (user,"--> Valid commands are e'X'it, 'A'bort, 'S'tart over\n");
	tell (user,"--> Command: ");
}
