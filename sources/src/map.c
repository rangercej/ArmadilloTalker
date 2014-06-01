#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "armadillo.h"

int initmap()
{
	int i=0, j=0;

	for (i=0; i < MAX_AREAS; i++) {
		map[i].name[0]=-1;
		map[i].status=UNUSED;
		for (j=0; j < MAX_AREAS+1; j++)
			map[i].links[j]=-1;
		for (j=0; j < 15; j++)
			map[i].desc[j][0]='~';
		for (j=0; j < 3; j++)
			map[i].messages[j][0]='~';
		map[i].type=PUBLIC;
		map[i].board=FALSE;
	}
	return 0;
}

/*** Okay...so it's a bit long... ***/
int readmap ()
{
	char index[MAX_AREAS][255];
	char links[100][50];
	char fname[255];
	char aline[255], aoriginal[255];
	char out[255];
	char *line=aline, *original=aoriginal;
	char *token;
	FILE *mapfile;
	int  i,j,k,mapindex=0;

	logmessage ("MAP: Reading system map");
	debug ("Reading map");
	for (i=0; i < MAX_AREAS; i++)
		index[i][0]=-1;

	for (i=0; i < 100; i++)
		links[i][0]=0;

	sprintf (fname,"%s/%s/map",ARMADILLO_HOME,SYSTEM_DIR);
	mapfile=fopen(fname,"r");
	if (mapfile==NULL) {
		logmessage ("MAP FATAL: Could not read room data");
		server.status=FATAL;
	}

	i=0;
	debug ("Buliding index...");
	fgets (line,255,mapfile);
	while (!feof(mapfile)) {
		if (strstr(line,"ROOM")==line) {
			while (line[0]!=' ') line++;
			line++;
			stripcrlf(line);
			strlower(line);
			debug ("Found room #%d: %s",i,line);
			strcpy (index[i],line);
			i++;
		}
		fgets (line,255,mapfile);
	}
	rewind(mapfile);

	debug ("Reading all room data");
	fgets (line,255,mapfile);
	while (!feof(mapfile)) {
		if (server.status==FATAL) {
			fclose(mapfile);
			return 0;
		}
		for (i=0; i < 100; i++)
			links[i][0]=0;
		if (line[0]!='#') {
			if (strstr(line,"ENDDEF")==line) {
				mapindex++;
			}
			if (strstr(line,"ROOM")==line) {
				while (line[0]!=' ') line++;
				debug ("\nFinding rooms...");
				line++;
				stripcrlf(line);
				strlower(line);
				debug (line);
				strcpy (map[mapindex].name,line);
				map[mapindex].status=USED;
			}
			if ((strstr(line,"LINKS")==line) || (strstr(line,"HIDDEN")==line)) {
				if (map[mapindex].status != USED) {
					sprintf (out,"MAP ERROR: LINKS or HIDDEN tag before NAME defined. Tag ignored.");
					puts (out);
					logmessage (out);
					continue;
				}

				strcpy (original,line);
				while (line[0]!=' ') line++;
				line++;
				linestripcrlf(line);
				strlower(line);
				i=0;
				token=strtok(line," ");

				debug ("Finding links...");
				while (token!=NULL) {
					strcpy (links[i],token);
					debug ("%d: %s",i,links[i]);
					i++;
					token=strtok(NULL," ");
				}
				i=0; j=0; k=0;
				while (map[mapindex].links[k]!=-1) k++;
				while (links[i][0]!=0) {
					if (!strcmp(links[i],"*none")) { i++; continue; }
					while ((index[j][0]!=0) && (strcmp (links[i],index[j]))) {
						debug ("i=%d, j=%d, links[i]=%s, index[j]=%s",i,j,links[i],index[j]);
						j++;
					}
					if (index[j][0]!=0) {
						if (strstr(original,"HIDDEN")==original)
							map[mapindex].links[k]=j+100;
						else
							map[mapindex].links[k]=j;
						k++;
					}
					else
					{
						logmessage ("MAP FATAL: Map definition error defining LINKS links");
						puts ("FATAL: Map file error");
						server.status=FATAL;
					}	
					i++; j=0;
					
				}
			}
			if (strstr(line,"EMPTY")==line) {
				if (map[mapindex].status != USED) {
					sprintf (out,"MAP ERROR: EMPTY tag before NAME defined. Tag ignored.");
					puts (out);
					logmessage (out);
					continue;
				}

				while (line[0]!=' ') line++;
				debug ("Getting empty message...");
				line++;
				linestripcrlf(line);
				debug (line);
				if (strlen(line) < 80)
					strcpy (map[mapindex].messages[EMPTY],line);
				else {
					sprintf (out,"MAP WARNING: Room %s: EMPTY tag: Line more than 80 characters",map[mapindex].name);
					puts (out);
					logmessage (out);
				}
			}
			if (strstr(line,"OTHERS")==line) {
				if (map[mapindex].status != USED) {
					sprintf (out,"MAP ERROR: OTHERS tag before NAME defined. Tag ignored.");
					puts (out);
					logmessage (out);
					continue;
				}

				while (line[0]!=' ') line++;
				debug ("Getting others message...");
				line++;
				linestripcrlf(line);
				debug (line);
				if (strlen(line) < 80)
					strcpy (map[mapindex].messages[OTHERS],line);
				else {
					sprintf (out,"MAP WARNING: Room %s: OTHERS tag: Line more than 80 characters",map[mapindex].name);
					puts (out);
					logmessage (out);
				}
			}
			if (strstr(line,"INVIS")==line) {
				if (map[mapindex].status != USED) {
					sprintf (out,"MAP ERROR: INVIS tag before NAME defined. Tag ignored.");
					puts (out);
					logmessage (out);
					continue;
				}

				while (line[0]!=' ') line++;
				debug ("Getting invis message...");
				line++;
				linestripcrlf(line);
				debug (line);
				if (strlen(line) < 80)
					strcpy (map[mapindex].messages[INVIS],line);
				else {
					sprintf (out,"MAP WARNING: Room %s: EMPTY tag: Line more than 80 characters",map[mapindex].name);
					puts (out);
					logmessage (out);
				}
			}
			if (strstr(line,"DESC")==line) {
				if (map[mapindex].status != USED) {
					sprintf (out,"MAP ERROR: DESC tag before NAME defined. Tag ignored.");
					puts (out);
					logmessage (out);
					continue;
				}

				while (line[0]!=' ') line++;
				debug ("\nGetting description...");
				line++;
				linestripcrlf(line);
				debug (line);
				i=0;
				while (map[mapindex].desc[i][0]!='~') i++;
				if (i < 15)
					strcpy (map[mapindex].desc[i],line);
				else {
					puts ("WARNING: Room description longer than 15 lines");
					logmessage ("MAP WARNING: Room description longer than 15 lines");
				}
			}				
			if (strstr(line,"BOARD")==line) {
				if (map[mapindex].status != USED) {
					sprintf (out,"MAP ERROR: BOARD tag before NAME defined. Tag ignored.");
					puts (out);
					logmessage (out);
					continue;
				}

				while (line[0]!=' ') line++;
				debug ("\nGetting board...");
				line++;
				stripcrlf(line);
				strlower(line);
				debug (line);
				if (!strcmp(line,"yes"))
					map[mapindex].board=TRUE;
				else
					map[mapindex].board=FALSE;
			}
			if (strstr(line,"TYPE")==line) {
				if (map[mapindex].status != USED) {
					sprintf (out,"MAP ERROR: TYPE tag before NAME defined. Tag ignored.");
					puts (out);
					logmessage (out);
					continue;
				}

				while (line[0]!=' ') line++;
				debug ("\nGetting type...");
				line++;
				stripcrlf(line);
				strlower(line);
				if ((strcmp(line,"public")) && (strcmp(line,"fixedpub")) && (strcmp(line,"private"))) {
					puts ("FATAL: Error reading TYPE in map file");
					logmessage ("MAP FATAL: Error reading TYPE in map definition file");
					server.status=FATAL;
				}
				debug (line);
				if (!strcmp(line,"public")) {
					map[mapindex].type=PUBLIC;
					map[mapindex].status=PUBLIC;
				}				
				if (!strcmp(line,"fixedpub")) {
					map[mapindex].type=FIXEDPUB;
					map[mapindex].status=PUBLIC;
				}
				if (!strcmp(line,"private")) {
					map[mapindex].type=FIXEDPRV;
					map[mapindex].status=PRIVATE;
				}
			}
		}
		fgets (line,255,mapfile);
	}
	fclose (mapfile);
#ifdef DEBUG
	displaymap();
#endif

	return 0;
}

#ifdef DEBUG
int displaymap()
{
	int i=0, j=0;
 
	puts ("\nKnown Map:");
	for (i=0; i < MAX_AREAS; i++) {
		if (map[i].status != UNUSED) {
			printf ("Room #%d: %s\n",i,map[i].name);
			printf ("Status: %d\n",map[i].status);
			printf ("Links:\n");
			while (map[i].links[j]!=-1) {
				printf ("     %s",map[(map[i].links[j]%100)].name);
				if (map[i].links[j] > 99)
					puts (" (hidden)");
				else
					puts ("");
				j++;
			}
			printf ("Messages:\n");
			for (j=0; j < 3; j++) {
				switch (j) {
					case EMPTY:  printf ("Empty:  "); break;
					case OTHERS: printf ("Others: "); break;
					case INVIS:  printf ("Invis:  "); break;
				}
				if (map[i].messages[j][0]=='~')
					puts ("No message defined");
				else
					puts (map[i].messages[j]);
			}
			j=0;
			printf ("Description:\n");
			while (map[i].desc[j][0]!='~') {
				puts (map[i].desc[j]);
				j++;
			}
			printf ("Board: ");
			if (map[i].board) 
				puts ("YES");
			else
				puts ("NO");
			printf ("Type of room: ");
			switch (map[i].type) {
				case PUBLIC: puts ("Public, toggleable to private"); break;
				case FIXEDPUB: puts ("Fixed public room"); break;
				case PRIVATE:  puts ("Fixed private room"); break;
				default: puts ("");
			}
			j=0;
			puts ("");
		}
	}
	if (server.status != FATAL)
		logmessage ("MAP: Map read without fatal error");
	else
		logmessage ("MAP: server.status is FATAL. Error in map read.");

	return 0;
}
#endif
