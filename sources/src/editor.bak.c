#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

load (int user, char *filename, char **text, int maxlines)
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
		return;
	}

	work=*text;
	initedit (&work,maxlines);
	if (work==NULL) {
		sprintf (out,"EDITOR: Could not get memory for editor in load()");
		logmessage (out);
		work=NULL;
		return;
	}
	home=work;

#ifdef DEBUG
	printf ("Memory is: %p, %p\n",work,home);
#endif

	fgets (out,80,fedit);
#ifdef DEBUG
	puts ("Loading file...");
#endif
	while (!feof(fedit)) {
		linestripcrlf(out);
		strcpy (work,out);
		i++;
		if ((i > MAX_LINES) && (!warn)) {
			sprintf (out2,"EDITOR: Too many lines for edit (max: %d) in file %s. Truncated.",MAX_LINES,filename);
			logmessage (out2);
			tell (user,"WARNING! Too many lines for edit - text has been truncated\n");
			warn=1;
			linesfound=i-1;
		}
		if (!warn) {
			work=work+strlen(out)+1;
			*work=0; *(work+1)=0;
		}
		fgets (out,80,fedit);
	}
	if (linesfound=-1)
		linesfound=i;

#ifdef DEBUG
	printf ("i=%d   linesfound=%d\n",i,linesfound);
	work=home;
	puts ("Loaded text:");
	for (i=0; i < linesfound; i++) {
		printf ("%d: %s\n",i,getline (work,i));
	}
#endif
	close (fedit);
	*text=home;
	return linesfound;
}

initedit (char **text, int maxlines)
{
	char *memptr;

	memptr=calloc(maxlines,80);
	if (memptr==NULL) {
		logmessage ("EDITOR: Memory allocation failed in initedit()");
	}
	*text=memptr;
}

edit (int user, char *inpstr)
{
	char buffer[256];
	char *textedit;
	int i,maxlines,numlines;

	textedit=users[user].editor.textbuffer;
	maxlines=users[user].editor.maxlines;
	numlines=users[user].editor.numlines;	

#ifdef DEBUG
	printf ("Text buffer is %p\n",textedit);
#endif
	if (users[user].status != EDITOR) {
		showtext (user,textedit,1,numlines,SHOWLINES);
		users[user].oldstatus=users[user].status;
		users[user].oldmode=users[user].mode;
		users[user].status=EDITOR;
		users[user].mode=NEWEDIT;
	} else {
		switch (users[user].mode) {
			case PROMPT: gotoroutine (user,inpstr); break;
			case SEARCH: search (user,inpstr); break;
			case DELETE: deleteline (user,inpstr); break;
			case EDIT  : editline (user,inpstr);
				     if (users[user].mode == PROMPT) 
					tell (user,"\n=> Enter ? for help, or p, s, d, e, i, w, q: ");
				     break;
			case INSERT: insertline (user,inpstr);
				     if (users[user].mode == PROMPT) 
					tell (user,"\n=> Enter ? for help, or p, s, d, e, i, w, q: ");
				     break;	
			case WRITE : writefile (user,inpstr); break;
			case AEQUIT: quitfile (user,inpstr); break;
		}
	}

	if (users[user].mode==NEWEDIT) {
		if (numlines < 1) {
			insertline (user,textedit,"i 1");
		}
		tell (user,"\n=> Enter ? for help, or p, s, d, e, i, w, q: ");
		users[user].mode=PROMPT;
	}
}

gotoroutine (int user, char *inpstr)
{
	int line;
	char out[256],cmd;

	puts ("In gotoroutine()");
	linestripcrlf(inpstr);
	line=atoi(inpstr);
	inpstr[0]=toupper(inpstr[0]);
	cmd=inpstr[0];
	inpstr++; inpstr++;

	switch (cmd) {
		case '?': editorhelp(user); break;
		case 'Q': quitfile(user); break;
		case 'W': writefile(user); break;
		case 'D': deleteline(user,inpstr); break;
		case 'P': displaylines(user); break;
		case 'S': search(user); break;
		case 'E': editline(user,inpstr); break;
		case 'I': insertline(user,inpstr); break;
		default : tell (user,"Editor - unknown command. Please try again.\n");
			  break;
	}
	if (users[user].mode==PROMPT) {
		if (cmd != 'Q')
			tell (user,"\n=> Enter ? for help, or p, s, d, e, i, w, q: ");
		else {
			users[user].mode=users[user].oldmode;
			tell (user,"=> Welcome back :-)\n");
			look (user);
		}	
	}
}

displaylines (int user)
{
		showtext (user,users[user].editor.textbuffer,1,users[user].editor.numlines);
}

search (int user)
{
	tell (user,"=> Not yet implemented\n");
}

deleteline (int user, char *inp)
{
	char out[256],*blocksource,*blockdest,*blocktemp1,*blocktemp2;
	int i=0,num;

	num=getlinefrominput(user,inp);
	if (num != -1) {
		blocksource=users[user].editor.textbuffer;
		initedit (&blockdest,users[user].editor.maxlines);

		blocktemp1=blocksource;
		blocktemp2=blockdest;
		for (i=0; i < users[user].editor.numlines; i++) {
			if (i != (num-1)) {
				strcpy (blockdest,blocksource);
				blockdest=blockdest+strlen(blocksource)+1;
			}
			blocksource=blocksource+strlen(blocksource)+1;
		}

		sprintf (out,"=> Deleted line %d\n",num);
		tell (user,out);
		users[user].editor.numlines--;
		users[user].editor.textbuffer=blocktemp2;
		free(blocktemp1);
	}
}

editline (int user, char *inp)
{
	int line,i;
	char *source, *dest, *w1, *w2, buffer[256];

	if (users[user].mode==PROMPT) {
		line=getlinefrominput(user,inp);
		if (line != -1) {
			users[user].mode=EDIT;
			users[user].editor.line=line;
			sprintf (buffer,"\n=> Editing line %d. Enter a '.' to leave the line as it is.\n",line);
			tell (user,buffer);
			sprintf (buffer,"%2d:%s\n%2d>",line,getline(user[users].editor.textbuffer,line-1),line);
			tell (user,buffer);
		}
		else
			users[user].mode=PROMPT;
		return;
	}
	line=users[user].editor.line;
	source=users[user].editor.textbuffer;
	initedit (&dest,users[user].editor.maxlines);
	w1=source; w2=dest;

	strcpy (buffer,inp);
	linestripcrlf(buffer);
	if (strlen(buffer) > LINELENGTH) {
		sprintf (buffer,"Line to long. Max %d chars (last line was %d chars).\nTry again...\n",LINELENGTH,strlen(inp));
		tell (user,buffer);
		sprintf (buffer,"%2d:%s\n%2d>",line,getline(user[users].editor.textbuffer,line-1),line);
		tell (user,buffer);
		return;
	}
	if (!strcmp(buffer,".")) {
		users[user].mode=PROMPT;
		return;
	}
	for (i=0; i < (users[user].editor.line-1); i++) {
		strcpy (w2,w1);
		w2=w2+strlen(w1)+1;
		w1=w1+strlen(w1)+1;
	}
	strcpy (w2,buffer);
	w2=w2+strlen(buffer)+1;
	w1=w1+strlen(w1)+1;
	for (i=users[user].editor.line; i < users[user].editor.numlines; i++) {
		strcpy (w2,w1);
		w2=w2+strlen(w1)+1;
		w1=w1+strlen(w1)+1;
	}
	users[user].editor.textbuffer=dest;
	free (source);
	users[user].mode=PROMPT;
}

insertline (int user, char *inp)
{
	int line,i;
	char *source, *dest, *w1, *w2, buffer[256];

	if (users[user].mode==PROMPT) {
		users[user].mode=INSERT;
		line=getlinefrominput(user,inp);
		if (line != -1) {
			users[user].editor.line=line;
			sprintf (buffer,"=> Inserting text above line %d.\n   Enter a single '.' at the start of a line to finish.\n",line);
			tell (user,buffer);
			sprintf (buffer,"%2d>",line);
			tell (user,buffer);
		}
		else
			users[user].mode=PROMPT;
		return;
	}

	line=users[user].editor.line;
	source=users[user].editor.textbuffer;
	initedit (&dest,users[user].editor.maxlines);
	w1=source; w2=dest;

	strcpy (buffer,inp);
	linestripcrlf(buffer);
	if (strlen(buffer) > LINELENGTH) {
		sprintf (buffer,"Line to long. Max %d chars (last line was %d chars).\nTry again...\n",LINELENGTH,strlen(inp));
		tell (user,buffer);
		sprintf (buffer,"%2d>",line);
		tell (user,buffer);
		return;
	}
	if (!strcmp(buffer,".")) {
		users[user].mode=PROMPT;
		return;
	}
	for (i=0; i < (users[user].editor.line-1); i++) {
		strcpy (w2,w1);
		w2=w2+strlen(w1)+1;
		w1=w1+strlen(w1)+1;
	}
	strcpy (w2,buffer);
	w2=w2+strlen(buffer)+1;
	for (i=(users[user].editor.line-1); i < users[user].editor.numlines; i++) {
		strcpy (w2,w1);
		w2=w2+strlen(w1)+1;
		w1=w1+strlen(w1)+1;
	}
	users[user].editor.numlines++;
	users[user].editor.textbuffer=dest;
	users[user].editor.line++;
	free (source);
	if (users[user].editor.numlines == users[user].editor.maxlines) {
		tell (user,"Editor - buffer has been filled, returning you to prompt\n");
		users[user].mode=PROMPT;
	} else {
		sprintf (buffer,"%2d>",users[user].editor.line);
		tell (user,buffer);
	}
}

writefile (int user)
{
	tell (user,"=> Not yet implemented.\n");
}

quitfile (int user)
{
	users[user].status=users[user].oldstatus;
}

getlinefrominput (int user,char *inp)
{
	char word[256];
	int num,i=0;

	if (users[user].editor.numlines==0) {
		tell (user,"Editor - error: there are no lines to edit.\n");
		return -1;
	}
	if (inp[0]==0) {
		tell (user,"Editor - error: command is missing line number.\n");
		return -1;
	}

	while (isdigit(inp[i])) {
		word[i]=inp[i];
		i++;
	}

	word[i]=0;
	num=atoi(word);
	if (num==0) {
		tell (user,"Editor - error: parameter given is not a line number.\n");
		return -1;
	}
	if (num > users[user].editor.numlines) {
		tell (user,"Editor - error: that line does not exist.\n");
		return -1;
	}

	return num;
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

showtext (int user, char *text, int startline, int endline, int numbers)
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
}

/* Searches field for needle and replcaes with haystack */
char *strsub (char *field, char *haystack, char *needle)
{
	char buffer[16384];
	char *work;
	int i=0,j=0;

	work=field;
	while (*(work+j) != 0) {
		if (strstr(work+j,needle) == work+j) {
			*(work+j)=0;
			strcpy(buffer,field);
			strcat(buffer,haystack);
			strcat(buffer,work+strlen(needle));
		}
		j++;
	}
	strcpy (field,buffer);
	return field;
}

editorhelp(int user)
{
#ifdef DEBUG
	puts ("Editor help!!");
#endif
	tell (user,"Editor help\n");
	tell (user,"-----------\n");
	tell (user,"The editor in armadillo is designed to be more functional than editors found\n");
	tell (user,"on other systems. Commands are entered into the editor in the form:\n");
	tell (user,"          command [line#]\n\n");
	tell (user,"Valid commands are:\n");
	tell (user,"          p      Displays the current buffer\n");
	tell (user,"          d #    Delete a specified line\n");
	tell (user,"          s      Search and replace text\n");
	tell (user,"          e #    Edit a line\n");
	tell (user,"          i #    Insert a line above the specified line\n");
	tell (user,"          w      Save the text\n");
	tell (user,"          q      Quit\n\n");
	tell (user,"Note: the '#' in the list above shows where the line number will go.\n\n");
	tell (user,"Examples:\n");
	tell (user,"To insert a line above line 3: i 3\n");
	tell (user,"To delete line 1: d 1\n");
	tell (user,"To show the entire buffer: p\n");
}
