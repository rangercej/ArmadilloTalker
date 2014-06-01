#include <time.h>
#include "armadillo.h"

struct srvstruct server;
struct mapstruct map[MAX_AREAS];
struct usrstruct users[MAX_USERS];

/* Rank names in female -> neuter -> male order */
char *ranknames[NUMBEROFRANKS][NUMBEROFGENDERS]={ 
			{ "Newbie","Newbie","Newbie" },
			{ "User","User","User" },
			{ "Witch","Wizitch","Wizard" },
			{ "Goddess","Godds","God" }
};

/* Gender descriptors */
char *genddesca[]={ "she","it","him" };
char *genddescb[]={ "her","it","his" };

char xxx[ARR_SIZE];

