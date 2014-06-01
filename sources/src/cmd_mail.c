#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include "armadillo.h"

/** Mailer modes **/
#define MENU 0
#define M_SEND 1
#define M_READ 2

/** Submodes **/
#define S_TO 1
#define S_SUBJECT 2
#define S_BODY 3
#define S_COMMIT 4
#define R_MENU 4

/*************************************************************************
**
** The Mailing system
**
*************************************************************************/

char *get_mbox_name(int user)
{
	static char mbox_name[ARR_SIZE];
	FILE *mbox;

	bzero(mbox_name,sizeof(mbox_name));
	sprintf (mbox_name,"%s/%s/mail/%s",ARMADILLO_HOME,USER_DIR,users[user].name);
	if ((mbox=fopen(mbox_name,"r")) == NULL) {
		mbox_name[strlen(mbox_name)] = '*';
	} else {
		fclose(mbox);
	}

	return mbox_name;
}

/**************************************************************************
** Generic open mailbox routine:
**	user		userid of user opening mailbox
**	user_name	name of user whos mailbox should be opened, or NULL
**			if opening [user]'s own mailbox
**	mode		Same as mode in fopen(). "a", "r", "w", &tc...
**	mailbox		Pointer (to pointer) to mail file variable
**************************************************************************/
int open_mailbox (int user, char *user_name, char *mode, FILE **mailbox)
{
	FILE *mbox;
	char mbox_name[255];
	char user_mail[255];
	char out[255];
	int retcode = FAILED;

	if (user_name == NULL) {
		strcpy (user_mail,users[user].name);
	} else {
		strcpy (user_mail,user_name);
	}
	sprintf (mbox_name,"%s/%s/mail/%s",ARMADILLO_HOME,USER_DIR,user_mail);
	if ((mbox=fopen(mbox_name,mode))==NULL) {
		tell (user,"--> Could not open mailbox\n");
		sprintf (out,"CMD_MAIL: Could not open %s (user %s) for mode %s in open_mailbox()",mbox_name,user_mail,mode);
		logmessage (out);
		retcode = FAILED;
	} else {
		*mailbox = mbox;
		retcode = SUCCESS;
	}

	return retcode;
}

int rmail (int user, char *input)
{
	char tempbox[255];
	char out[ARR_SIZE];
	char date[80],subject[61],from[NAME_LEN+1],message[256];
	time_t tm;
	long posn=0;
	FILE *mbox;
	FILE *temp;

	if (users[user].status!=MAIL) {
		if (open_mailbox(user,NULL,"r",&mbox) == FAILED) {
			logmessage ("CMD_MAIL: Mailbox open failed in rmail()");
			return 0;
		}
	
		sprintf (tempbox,"%s/%s/mail-%s.XXXXXX",ARMADILLO_HOME,RUNTIME_DIR,users[user].name);
		strcpy(tempbox,mktemp(tempbox));
		if ((temp=fopen(tempbox,"w"))==NULL) {
			tell (user,"--> ERROR: Can't create temporary file for reading mail. Aborted.\n");
			sprintf (out,"CMD_MAIL: Couldn't open temporary file %s (user %s) for writing in rmail()",tempbox,users[user].name);
			logmessage (out);
			return 0;
		}
		users[user].status=MAIL;
		strcpy (users[user].fileview.file,get_mbox_name(user));
		users[user].fileview.position=0;
		users[user].fileview.fptr=mbox;
		strcpy (users[user].fileview.temp,tempbox);
		users[user].fileview.tposn=0;
		users[user].fileview.tptr=temp;
	}

	debug ("Temp file is: %s",users[user].fileview.temp);

	linestripcrlf(input);
	input[0]=toupper(input[0]);

	mbox=users[user].fileview.fptr;
	temp=users[user].fileview.tptr;
	if (input[0] != 'D') {
		posn=ftell(mbox);
		if (posn != users[user].fileview.position) {
			fseek(mbox,users[user].fileview.position,SEEK_SET);
			fgets (out,255,mbox);
			while (strcmp (out,".%%\n")) {
				fputs (out,temp);
				fgets (out,255,mbox);
			}
			fputs (".%%\n",temp);
		}
	}

	users[user].fileview.position=ftell(mbox);
	fgets (date,79,mbox);
	if (!feof(mbox)) {
		fgets (subject,60,mbox); linestripcrlf(subject);
		fgets (from,NAME_LEN,mbox); linestripcrlf(from);
		tm=atol(date);
		clearscreen(user);
		sprintf (out,"From: %s\nDate: %s\nSubject: %s\n\n",from,mklocaltime(user,tm,ALL_DT),subject);
		tell (user,"---------------------------------------------------------------------------\n");
		tell (user,out);
		fgets (message,255,mbox); linestripcrlf(message);
		while (strcmp (message,".%%")) {
			tell (user,message);
			tell (user,"\n");
			fgets (message,255,mbox);
			linestripcrlf(message);
		}
		tell (user,"\n=> Hit <ENTER> for next message, or press <D> and hit <ENTER> to delete.\n");
	}
	
	if (feof(mbox)) {
		fclose (mbox);
		fclose (temp);
		unlink (users[user].fileview.file);
		rename (users[user].fileview.temp, users[user].fileview.file);
		tell (user,"\n--> No more mail.\n");
		users[user].status=ACTIVE;
		look (user);
	}
	return 0;
}

int dmail (int user)
{
	tell (user,"=> Not yet implemented...\n");

	return 0;
}

int smail (int user, char *input)
{
	int i;
	time_t tm;
	char recipient[NAME_LEN+1];
	char out[ARR_SIZE];
	char buffer[NAME_LEN+1];
	FILE *mbox;

	debug ("In mailer (smail)");

	if (input[0]==0) {
		tell (user,"--> Syntax: .smail <user> <message>\n");
		tell (user,"         Also see .mailer for advanced mailing\n\n");
		return 0;
	}
	linestripcrlf (input);
	strlower(input);
	
	for (i=0; (i < NAME_LEN) && (*input != ' '); i++, input++) {
		recipient[i]=*input;
	}
	recipient[i]=0;
	while (*input == ' ')
		input ++;

	if (input[0] == 0) {
		tell (user,"--> Syntax: .smail <user> <message>\n");
		tell (user,"         Also see .mailer for advanced mailing\n\n");
		return 0;
	}

	strlower(recipient);
	recipient[0]=toupper(recipient[0]);
	strcpy (buffer,userexist(recipient));
	if (!strcmp(buffer,"")) {
		tell (user,"--> Error: User does not exist.\n\n");
	} else {
		if (!strcmp(buffer,".AMBIGUOUS")) {
			tell (user,"--> That could be more than one user!\n\n");
		} else {
			strcpy (recipient,buffer);
			if (open_mailbox(user,recipient,"a",&mbox) == FAILED) {
				logmessage ("CMD_MAIL: Mailbox open failed in smail()");
			} else {
				time(&tm);
				fprintf (mbox,"%ld\n[no subject - sent using .smail]\n%s\n%s\n.%%%%\n",tm,users[user].name,input);
				fclose (mbox);
				sprintf (out,"CMD_MAIL: %s mailed %s with smail()",users[user].name,recipient);
				logmessage (out);
				sprintf (out,"--> Mail sent to %s\n",recipient);
				tell (user,out);
			}
		}
	}
	return 0;
}

int mailer (int user, char *input)
{
	if (users[user].status != MAILER) {
		users[user].status=MAILER;
		users[user].mode=MENU;
	} else {
		if (users[user].mode == MENU) {
			linestripcrlf(input);
			input[0]=toupper(input[0]);
			if (input[1] == 0) {
				switch (input[0]) {
					case '1': users[user].mode = M_SEND; break;
					case '2': users[user].mode = M_READ; break;
					case '0': users[user].status = ACTIVE; break;
				}
			}
		}
	}
		
	if (users[user].status == MAILER) {
		switch (users[user].mode) {
			case MENU:	mailer_menu(user);
					break;
			case M_SEND:	mailer_send(user,input);
					if (users[user].mode == MENU)
						mailer_menu(user);
					break;
			case M_READ:	mailer_read(user,input);
					if (users[user].mode == MENU)
						mailer_menu(user);
					mailer_menu(user);
					break;
		}
	}

	if (users[user].status == ACTIVE) {
		clearscreen(user);
		tell (user,"=> You are now back in the room...\n");
		look (user);
	}
	return 0;
}

int mailer_menu(int user)
{
		clearscreen(user);
		tell (user,"---------------------------------------------------------------------------\n");
		tell (user,"ARMADILLO MAIL SYSTEM\n");
		tell (user,"---------------------------------------------------------------------------\n");
		tell (user,"     1. Send mail\n");
		tell (user,"     2. Read and delete recieved mail\n");
		tell (user,"\n     0. Leave mailer\n");
		tell (user,"---------------------------------------------------------------------------\n");
		tell (user,"Select option: ");
		users[user].submode=MENU;

		return 0;
}

int mailer_send (int user, char *inpstr)
{
	char recipient[255];
	char buffer[ARR_SIZE];
	char *text;
	time_t tm;
	FILE *mbox;

	if (users[user].submode == MENU) {
		clearscreen(user);
		tell (user,"-- Sending mail -----------------------------------------------------------\n");
		tell (user,"Recipient: ");
		users[user].submode = S_TO;
	} else {
		linestripcrlf (inpstr);
		strlower(inpstr);
		switch (users[user].submode) {
			case S_TO: if (inpstr[0] != 0) {
					strlower (inpstr);
					inpstr[0]=toupper(inpstr[0]);
					strcpy (recipient,userexist(inpstr));
					if (!strcmp(recipient,"")) {
						tell (user,"--> That user does not exist! Try again...\n\n");
						tell (user,"Recipient: ");
					} else {
						if (!strcmp(recipient,".AMBIGUOUS")) {
							tell (user,"--> That could be more than one user! Try again...\n\n");
							tell (user,"Recipient: ");
						} else {
							strcpy (users[user].mail.recipient,recipient);
							tell (user,"Subject: ");
							users[user].submode=S_SUBJECT;
						}
					}
				}
				break;
			case S_SUBJECT: linestripcrlf(inpstr);
				strncpy (users[user].mail.subject,inpstr,sizeof(users[user].mail.subject)-3);
				users[user].submode=S_BODY;
				users[user].data.async = 1;
				tell (user,"Enter text:\n");
				break;
			case S_BODY: initedit (&text,MAIL_LINES);
				if (text==NULL) {
					tell (user,"--> Error allocating memory");
					users[user].submode=MENU;
				} else {
					users[user].editor.textbuffer=text;
					users[user].editor.maxlines=MAIL_LINES;
					users[user].editor.numlines=0;
					users[user].submode = S_COMMIT;
					edit(user,inpstr);
				}
				break;
			case S_COMMIT:
					debug ("==> EDITOR RETURNED THIS TEXT!");
					debug (users[user].editor.textbuffer);

					if (open_mailbox(user,users[user].mail.recipient,"a",&mbox) == FAILED) {
						logmessage ("CMD_MAIL: Mailbox open failed in mailer_send()");
					} else {
						time (&tm);
						fprintf (mbox,"%ld\n%s\n%s\n%s\n.%%%%\n",tm,users[user].mail.subject,users[user].name,users[user].editor.textbuffer);
						fclose (mbox);
						sprintf (buffer,"CMD_MAIL: Mailer: %s sent %s mail (time stamp: %ld)",users[user].name, users[user].mail.recipient, tm);
						logmessage (buffer);
					}
					users[user].mode = MENU;
					break;
		}
	}

	return 0;
}

int mailer_read (int user, char *inpstr)
{
	clearscreen(user);
	users[user].mode = MENU;

	return 0;
}
