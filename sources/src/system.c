#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <ctype.h>
#include "armadillo.h"

/********************************************************************
**
** Armadillo internal system routines
**
********************************************************************/

int resetconfig()
{
	server.port=0;
	server.syslog=-1;
	server.mailquota=0;

	return 0;
}

int readconfig()
{
	char out[255];
	char fname[255];
	char cline[255];
	char *line=cline;
	FILE *conf;

	sprintf (fname,"%s/%s/armadillo.conf",ARMADILLO_HOME,SYSTEM_DIR);
	if ((conf=fopen(fname,"r"))==NULL) {
		sprintf (out,"SYSTEM FATAL: Could not open configuration file");
		logmessage (out);
		puts ("=> Armadillo FATAL Error: Could not open configuration file");
		server.status=FATAL;
		return 0;
	}

	logmessage ("SYSTEM: Reading system configuration");
	resetconfig();
	
	fgets (line,255,conf);
	while (!feof(conf)) {
		if (server.status==FATAL) {
			fclose (conf);
			return 0;
		}
		if (line[0]!='#') {
			if (strstr(line,"PORT")==line) {
				debug ("Found port...");
				while (line[0]!=' ') line++;
				line++;
				stripcrlf(line);

				if ((atoi(line) < 1024) || (atoi(line) > 65535)) {
					logmessage ("SYSTEM FATAL: Port number invalid (1024 -> 65535)");
					puts ("=> Armadillo FATAL Error: Port number invalid (1024 -> 65535)");
					server.status=FATAL;
				}
				else {
					server.port=atoi(line);
				}
			}
			if (strstr(line,"SYSLOG")==line) {
				debug ("Got syslog info...");
				while (line[0]!=' ') line++;
				line++;
				stripcrlf(line);

				if ((atoi(line) < LOG_EMERG) || (atoi(line) > LOG_DEBUG)) {
					logmessage ("SYSTEM FATAL: System log level invalid (0 -> 7)");
					puts ("=> Armadillo FATAL Error: System log level invalid (0 -> 7)");
					server.status=FATAL;
				}
				else {
					server.syslog=atoi(line);
				}
			}
			if (strstr(line,"MAILQUOTA")==line) {
				while (line[0]!=' ') line++;
				line++;
				stripcrlf(line);
				if (atoi(line) < 1) {
					logmessage ("SYSTEM FATAL: Mail quota invalid. Must be > 0 or non-existant");
					puts ("=> Armadillo FATAL Error: Mail quota invalid. Must be > 0.");
					server.status=FATAL;
				}
				else {
					server.mailquota=atoi(line);
				}
			}
		}
		fgets (line,255,conf);
	}
	if (server.port==0) {
		logmessage ("SYSTEM FATAL: PORT not defined in configuration file");
		puts ("Armadillo FATAL Error: Port number not defined.");
		server.status=FATAL;
	}
	return 0;
}


int logmessage(char *logmess)
{
	char logfile[255];
	char mess[ARR_SIZE];
	FILE *logf;
	sprintf (logfile,"%s/%s/messages",ARMADILLO_HOME,SYSTEM_DIR);
	logf=fopen (logfile,"a");
	if (logf==NULL) {
		sprintf (mess,"=> Armadillo Error: Could not open system log for writing: %s",logfile);
		perror (mess);
		return -1;
	}
	sprintf (mess,"%s: %s\n",getdateline(ALL_DT),logmess);
	fputs (mess,logf);
	fclose (logf);

	return 0;
}

char *getdateline(int flags)
{
	time_t tm;
	char timestr[255];
	static char outtime[255];
	
	time(&tm); strcpy (timestr,ctime(&tm));
	timestr[strlen(timestr)-1]=0;

	switch (flags) {
		case NODATE: strmidcpy (outtime,timestr,12,5); break;
		case NOTIME: strmidcpy (outtime,timestr,5,6); break;
		case ALL_DT: strmidcpy (outtime,timestr,5,12); break;
	}
	return outtime;
}

char *mklocaltime (int user, time_t tm, int flags)
{
	char timestr[255];
	static char outtime[255];

	tm=tm+(users[user].data.timezone*60*60);
	strcpy (timestr,ctime(&tm));
	timestr[strlen(timestr)-1]=0;

	switch (flags) {
		case NODATE: strmidcpy (outtime,timestr,12,5); break;
		case NOTIME: strmidcpy (outtime,timestr,5,6); break;
		case ALL_DT: strmidcpy (outtime,timestr,5,12); break;
	}
	return outtime;
}
	
int strmidcpy (char *out, char *in, int index, int count)
{
	char *p=in;

	p=p+index-1;
	strncpy (out,p,count);

	return 0;
}

int strlower (char *str)
{
	int i=-1;

	do {
		i++;
		str[i]=tolower(str[i]);
	} while (str[i]!=0);

	return 0;
}

int strupper (char *str)
{
	int i=-1;

	do {
		i++;
		str[i]=toupper(str[i]);
	} while (str[i]!=0);

	return 0;
}


int checkpassword (int user, char *name, char *pass)
{
	FILE *passfile;
	char fname[255];
	char outstr[ARR_SIZE];
	char line[256];
	char pfname[100],pfpass[100];

	sprintf (fname,"%s/%s/passwd",ARMADILLO_HOME,SYSTEM_DIR);
	passfile=fopen (fname,"a+");
	if (passfile==NULL) {
		tell (user,"SYSTEM ERROR: Cannot open password file\n");
		sprintf (outstr,"Please email the system admin for this system at %s\n",EMAIL_ADDR);
		tell (user,outstr);
		logmessage ("SYSTEM: Cannot open password file in checkpassword()");
		users[user].status=DEAD;

		return 0;
	}
	rewind(passfile);

	fgets (line,255,passfile);
	sscanf (line,"%s %s",pfname,pfpass);

	while ((strcmp (pfname,name)) && (!feof(passfile))) {
		fgets (line,255,passfile);
		sscanf (line,"%s %s",pfname,pfpass);
	}

	if (feof(passfile)) {
		tell (user,"New user - re-enter password: ");
		users[user].status=LOGNEW;
		fclose (passfile);
		return 1;
	}

	fclose (passfile);	
	if (strcmp (pfpass,pass))
		return 0;
	else
		return 1;
}

int addpassword (int user, char *name, char *password)
{
	FILE *passfile;
	char fname[255];
	char outstr[ARR_SIZE];

	sprintf (fname,"%s/%s/passwd",ARMADILLO_HOME,SYSTEM_DIR);
	passfile=fopen (fname,"a");
	if (passfile==NULL) {
		tell (user,"SYSTEM ERROR: Cannot open password file\n");
		sprintf (outstr,"Please email the system admin for this system at %s\n",EMAIL_ADDR);
		tell (user,outstr);
		logmessage ("SYSTEM: Cannot open password file in addpassword()");
		users[user].status=DEAD;

		return 0;
	}
	sprintf (outstr,"%s %s\n",name,password);
	fputs (outstr,passfile);
	fclose (passfile);

	return 0;
}

/*** Does user exist? ***/
char *userexist (char *username)
{
	char passname[NAME_LEN+1];
	char passfile[256];
	static char otheruser[NAME_LEN+1];
	int flag=0;
	FILE *pfile;

	debug ("userexist(): Looking for %s",username);

	bzero(passname,sizeof(passname));
	bzero(otheruser,sizeof(otheruser));
	sprintf (passfile,"%s/%s/passwd",ARMADILLO_HOME,SYSTEM_DIR);
	if ((pfile=fopen(passfile,"r")) == NULL) {
		logmessage ("SYSTEM: Cannot open password file in userexist()");
		return NULL;
	}
	fscanf (pfile,"%s %s",passname,passfile);
	while (!feof(pfile) && (flag != 2)) {
		debug ("userexist(): %s %s %s",username, passname, passfile);
		if (strstr(passname,username)==passname) {
			switch (flag) {
				case 0: flag=1;
					strcpy(otheruser,passname);
					break;
				case 1: flag=2;
				default:strcpy(otheruser,".AMBIGUOUS");
			}
		}
		fscanf (pfile,"%s %s",passname,passfile);
	}
	fclose (pfile);

	debug ("userexist(): Flag = %d, Otheruser = %s",flag,otheruser);

	return otheruser;
}


/*** Strip a CRLF of a word only ***/
int stripcrlf (char *str)
{
	int i=0;
	while (isalnum(str[i])!=0)
		i++;

	str[i]=0;

	return 0;
}

/*** Strip a CRLF of a complete line ***/
int linestripcrlf (char *str)
{
	int i=0;
	while ((str[i] >= ' ') || (isblank (str[i])))
		i++;

	str[i]=0;

	return 0;
}

/*** SHUTDOWN ***/
int go_shutdown(int user, char *message)
{
	int i;
	char out[ARR_SIZE];

	server.status=QUIT;
	
	logmessage ("SYSTEM: Shutting down server");
	broadcast ("\n\n\nArmadillo is being shut down!! Sorry!!\n",1);
	if (message[0]!=0) {
		broadcast (message,1);
		broadcast ("\n\n",1);
	}

	if (user!=-1)
		tell (user,"Quitting users...");
	for (i=0; i < MAX_USERS; i++) {
		if ((users[i].status!=FREE) && (user != i))
			quituser (i);
	}

	if (user != -1) {
		tell (user,"Now quitting you...");
		quituser (user);
	}

	strcpy (out,"Server shutting down: ");
	strcat (out,message);
	write_syslog (out,LOG_NOTICE);

	return 0;
}

/* Detect a terminal */
int term_detect(int user, char *instr)
{
	char termtype[32];

	if (users[user].status != TERMDETECT) {
		users[user].status=TERMDETECT;
		tell (user,"\033[c");
		return 0;
	}

	strcpy (termtype,"UNKNOWN");
	if (!strcmp(instr,"\033[11c"))
		strcpy (termtype,"PT250");
	else if (!strncmp(instr,"\233?62;",5))
		strcpy (termtype,"VT220");
	else if (!strncmp(instr,"\033?62;",5))
		strcpy (termtype,"VT220");

	debug ("User %s is using termtype %s (%s)",users[user].name,termtype,instr);

	users[user].status=ACTIVE;

	return 0;
}

int init_syslog ()
{
	char out[ARR_SIZE];
	if (server.syslog != -1) {
		openlog ("armadillo",LOG_PID,LOG_USER);
		sprintf (out,"System logging to syslogd enabled. Log level: %d",server.syslog);
		logmessage (out);
		write_syslog(out,LOG_NOTICE);
	}
	return 0;
}

int write_syslog (char *mess, int pri)
{
	if ((server.syslog != -1) && (pri <= server.syslog)) {
		syslog (pri,mess);
	}
	return 0;
}

int done_syslog ()
{
	if (server.syslog != -1)
		closelog();

	return 0;
}

int clearscreen(int user)
{
	int i;

	for (i=0; i < 50; i++)
		tell (user,"\n");

	return 0;
}

int process_telnet(int user, char *instr)
{
	return 0;
}
