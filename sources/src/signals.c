#include <stdio.h>
#include <signal.h>
#include "armadillo.h"

/******************************************************************
**
** Handle signals, SIGTERM etc...
**
******************************************************************/

void sig_handler (int sig);

int initsignals()
{
	logmessage ("SIGNALS: Fixing signal handlers");
	signal (SIGTERM,sig_handler);
	signal (SIGINT,sig_handler);
	signal (SIGSEGV,sig_handler);
	logmessage ("SIGNALS: Done fixing signal handler");

	return 0;
}

void sig_handler (int sig)
{
	char out[255];

	if (sig==SIGTERM) {
		logmessage ("SIGNALS: Caught SIGTERM...exiting...");
		go_shutdown(-1,"=> ARMADILLO SYSTEM: Caught terminate signal from host");
		return;
	}
	if (sig==SIGINT) {
		logmessage ("SIGNALS: Caught SIGINT...exiting...");
		go_shutdown(-1,"=> ARMADILLO SYSTEM: Caught interrupt signal from host");
		return;
	}
	if (sig==SIGSEGV) {
		logmessage ("SIGNALS: Caught SIGSEGV...exiting...");
		go_shutdown(-1,"=> ARMADILLO SYSTEM: Armadillo caused segmentation fault...");
	} 
	
	sprintf (out,"SIGNALS: Detected signal %d\n",sig);
	logmessage (out);
}
