/***************************************************************************
**
** Armadillo - chat/conference server for UNIX system
**
** Chris Johnson, 1996
**
***************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <netinet/in.h>

/*** Check for GNU CC ***/
#ifndef __USE_GNU
#endif

/*** Version ***/
#define VERSION "1.0"

/*** Server paths ***/
#define ARMADILLO_HOME "/home/chris/src/Armadillo"	/* Armadillo top directory */
#define SYSTEM_DIR "system"				/* System files go here */
#define RUNTIME_DIR "work"				/* Runtime files go here */
#define USER_DIR "users"				/* User files go here */

/*** Admin email addy ***/
#define EMAIL_ADDR "cjohnso0@pine.shu.ac.uk"		/* Talker admin email address */

/*** Server setup ***/
#define ARR_SIZE 2000			/* Used in a lot of stuff */
#define MAX_AREAS 100			/* Max. # of areas */
#define MAX_USERS 50			/* Max # of logged on users */
#define NAME_LEN 15			/* Max length of name */
#define DESC_LEN 30			/* Max length of desc */
#define PRO_LINES 10			/* Max lines for profile */		
#define MAIL_LINES 10			/* Max lines for mail */
#if PRO_LINES > MAIL_LINES		/* Don't modify this bit */
#define MAX_LINES PRO_LINES		/*            |             */
#else					/*            |             */
#define MAX_LINES MAIL_LINES		/*            |             */
#endif					/*            |             */
#ifndef FD_SETSIZE			/*            |             */
#define FD_SETSIZE 256			/*            |             */
#endif					/* End of the do not modify bit */
#define SALT "AB"			/* Encryption key */

/*** Boolean stuff - shouldn't need modifying ***/
#define NO 0
#define YES 1
#define FALSE 0
#define TRUE -1
#define FAILED 0
#define SUCCESS 1

/*** Ranks - feel free to play ***/
#define NUMBEROFRANKS 4
#define NEWBIE 1
#define USER 2
#define WIZARD 3
#define GOD 4

/*** Genders - feel free to play ***/
#define NUMBEROFGENDERS 3
#define FEMALE 0
#define NEUTER 1
#define MALE 2

/*** Date time calculations - shouldn't need modifying ***/
#define ALL_DT 0
#define NODATE 1
#define NOTIME 2

/*** Server status codes - shouldn't need modifying ***/
/* Active defined in user status codes */
#define FATAL 0
#define STARTING 2
#define QUIT 3

/*** Room status codes - shouldn't need modifying ***/
#define USED -1
#define UNUSED 0
#define PUBLIC 1
#define PRIVATE 2
#define FIXEDPUB 3
#define FIXEDPRV 4

#define OTHERS 0
#define EMPTY 1
#define INVIS 2

/*** User status codes - shouldn't need modifying ***/
#define FREE -1
#define DEAD 0
#define ACTIVE 1
#define LOGNAME 2
#define LOGPASS 3
#define LOGNEW 4
#define TERMDETECT 5
#define EDITOR 6
#define MAIL 7
#define MAILER 8

/*** Editor quit codes ***/
#define ED_SAVE 0
#define ED_ABORT 1

/*** Some prototypes ***/
/* armadillo.c */
int main (void);

/* cmd_area.c */
int expand_room (char *room,char *newroom);
int go (int user, char *input);
int look (int user);
int areas(int user);

/* cmd_mail.c */
char *get_mbox_name(int user);
int open_mailbox (int user, char *user_name, char *mode, FILE **mailbox);
int rmail (int user, char *input);
int dmail (int user);
int smail (int user, char *input);
int mailer (int user, char *input);
int mailer_menu(int user);
int mailer_send (int user, char *inpstr);
int mailer_read (int user, char *inpstr);

/* cmd_user.c */
int quituser (int user);
int say (int user, char *input);
int emote (int user, char *input);
int who (int user);
int shout (int user, char *input);

/* commands.c */
int inchar (char *str, unsigned char ch);
int shortcut (int user, char *instr,char *newcmd);
int abbr (char *instr,char *newcmd);
int docommand (int user, char *cmdline);
int help (int user);

/* debug.c */
void debuginit(void);
void debug (char *fmt, ...);
void debug_bytes (char *fmt, ...);

/* editor.c */
int load (int user, char *filename, char **text, int maxlines);
int initedit (char **text, int maxlines);
int edit (int user, char *inpstr);
char *getline (char *text, int linenum);
int showtext (int user, char *text, int startline, int endline, int numbers);
void displayprompt (int user);

/* login.c */
int login (int user, char *instr, int where);
void echo_off (int user);
void echo_on (int user);

/* main.c */
int init (void);
int run (void);
int done (void);

/* map.c */
int initmap(void);
int readmap (void);
int displaymap(void);

/* network.c */
int initnetwork(void);
char *getsite (struct sockaddr_in *socket_in);
int broadcast (char *message,int override);
int message (int user, char *mess, int touser);
int tell (int user, char *mess);
int clean_user (int user);
int donenetwork(void);

/* signals.c */
int initsignals(void);

/* system.c */
int resetconfig(void);
int readconfig(void);
int logmessage(char *logmess);
char *getdateline(int flags);
char *mklocaltime (int user, time_t tm, int flags);
int strmidcpy (char *out, char *in, int index, int count);
int strlower (char *str);
int strupper (char *str);
int checkpassword (int user, char *name, char *pass);
int addpassword (int user, char *name, char *password);
char *userexist (char *username);
int stripcrlf (char *str);
int linestripcrlf (char *str);
int go_shutdown(int user, char *message);
int term_detect(int user, char *instr);
int init_syslog (void);
int write_syslog (char *mess, int pri);
int done_syslog (void);
int clearscreen(int user);
int process_telnet(int user, char *instr);
int initusers(void);
int add_user(int socket, struct sockaddr_in *socket_in);
int split (int user, char *data);
unsigned long merge (int user);
int set_user (int user);
int save_user (int user);
int input (int user, char *instr);
int backup_user(int user);
int restore_user(int user);

/*** Gloabal stuff ***/
struct srvstruct {
	int port;
	int socket;
	int status;
	int syslog;
	int mailquota;
};

struct usrstruct {
	char name[NAME_LEN];
	char desc[DESC_LEN];
	char scratch[256];
	char site[256];
	struct {
		char *textbuffer;
		char *inspoint;
		int numlines;
		int maxlines;
		int line;
	} editor;
	struct {
		FILE *fptr;
		FILE *tptr;
		char file[256];
		char temp[256];
		long position;
		long tposn;
	} fileview;
	struct {
		unsigned tzdir		: 1;
		unsigned timezone	: 3;
		unsigned charmode	: 1;
		unsigned bold		: 1;
		unsigned igexamine	: 1;
		unsigned igbeeps	: 1;
		unsigned iglogin	: 1;
		unsigned igtell		: 1;
		unsigned earmuffs	: 1;
		unsigned rank		: 3;
		unsigned gender		: 1;
		unsigned listen		: 1;
		unsigned async		: 1;
		unsigned ewtoo		: 1;
	} data;
	struct {
		char recipient[NAME_LEN+1];
		char subject[256];
	} mail;
	int area;
	int oldstatus;
	int oldmode;
	int oldsubmode;
	int status;
	int mode;
	int submode;
	int socket;
	int lines;
	time_t lastlogin;
	time_t lastmailread;
	time_t datecreate;
	time_t totaltime;
	struct usrstruct *backup;
};

struct mapstruct {
	char name[81];
	char messages[3][81];
	char desc[15][81];
	int links[MAX_AREAS+1];
	int type;
	int board;
	int status;
};

extern struct usrstruct users[];
extern struct srvstruct server;
extern struct mapstruct map[];

extern char *ranknames[NUMBEROFRANKS][NUMBEROFGENDERS];
extern char *genddesca[];
extern char *genddescb[];

extern char xxx[ARR_SIZE];
