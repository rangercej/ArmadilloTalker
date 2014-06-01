#include <stdio.h>
#include <string.h>
#include "armadillo.h"

/***********************************************************************
**
** Area related commands
**
***********************************************************************/

int expand_room (char *room,char *newroom)
{
	int i=0,fflag=0;

	for (i=0; i < MAX_AREAS; i++) {
		if (strstr(map[i].name,room) == map[i].name) {
			if (!fflag) {
				strcpy (newroom,map[i].name);
			}			
			fflag++;
		}
	}
	debug ("Room = %s\nFflag = %d\nnewroom = %s",room,fflag,newroom);
	if (fflag == 0)
		strcpy (newroom,".ERROR");
	else
	if (fflag > 1)
		strcpy (newroom,".AMBIGUOUS"); 


	return 0;
}

int go (int user, char *input)
{
	char newroom[ARR_SIZE];
	char out[ARR_SIZE];
	int i=0,j=0,fflag=0;
	int link;

	if (input[0]==0) {
		tell (user,"--> Syntax: .go <room name>\n");
		return 0;
	}

	stripcrlf (input);
	strlower (input);

	expand_room(input,newroom);
	if (!strcmp(newroom,".ERROR")) {
		tell (user,"--> Oi ya dafty! That room does not exist!\n");
		return 0;
	}

	if (!strcmp(newroom,".AMBIGUOUS")) {
		tell (user,"--> Error: That room could be one of several!\n       Be more specific, please!\n");
		return 0;
	}

	strcpy (input,newroom);

	if (!strcmp(map[users[user].area].name,newroom)) {
		tell (user,"--> You're already in this room!!\n");
		return 0;
	}

	while ((strcmp(map[i].name,input)) && (map[i].status!=UNUSED)) i++;

	if (map[i].status==UNUSED)
		tell (user,"--> Oi ya dafty! That room does not exist!\n");
	else
	{
		while (((map[users[user].area].links[j]%100) != i) && (map[users[user].area].links[j] != -1 )) j++;

		if (map[users[user].area].links[j] == -1) {
			tell (user,"--> That room is not connected to this one!\n");
			j=0;
			while (map[users[user].area].links[j] != -1) {
				link=map[users[user].area].links[j];
				if (link < 100) {
					if (!fflag)
						tell (user,"   Rooms connected to this one:\n");
					sprintf (out,"        %s\n",map[link].name);
					tell (user,out);
					fflag=1;
				}
				j++;
			}
			if (!fflag)
				tell (user,"   You can't see any connecting rooms here!\n");

			return 0;
		}

		sprintf (out,"%s has gone to the %s\n",users[user].name,input);
		message (user,out,NO);
		users[user].area=i;
		sprintf (out,"%s arrives in a blaze of glory and goes immediately for the bar!\n",users[user].name);
		message (user,out,NO);

		look (user);
	}
	return 0;
}

int look (int user)
{
	int i=0,fflag=0,link;
	char out[ARR_SIZE];

	debug ("Looking!!");
	debug ("User Id: %d",user);
	debug ("User: %s - Area %d",users[user].name,users[user].area);

	sprintf (out,"You are in the %s\n",map[users[user].area].name);
	tell (user,"\n\n----------------------------------------------------------------------\n");
	tell (user,out);
	while (map[users[user].area].desc[i][0]!='~') {
		tell (user,map[users[user].area].desc[i]);
		tell (user,"\n");
		i++;
	}
	tell (user,"\n");
	for (i=0; i < MAX_USERS; i++) {
		if ((users[i].area == users[user].area) && (i != user) && (users[i].status == ACTIVE)) {
			if (!fflag) {
				tell (user,map[users[user].area].messages[OTHERS]);
				tell (user,"\n");
			}
			tell (user,"    ");
			tell (user,users[i].name);
			tell (user,"\n");
			fflag=1;
		}
	}
	if (!fflag) {
		tell (user,map[users[user].area].messages[EMPTY]);		
		tell (user,"\n");
	}
	fflag=0;
	i=0;
	while (map[users[user].area].links[i] != -1) {
		link=map[users[user].area].links[i];
		if (link < 100) {
			if (!fflag) {
				tell (user,"\nYou can see the following rooms nearby:\n     ");
				sprintf (out,"%s",map[link].name);
			}
			else
				sprintf (out,", %s",map[link].name);
			tell (user,out);
			fflag=1;
		}
		else 
			if (fflag==0) fflag=2;
		i++;
	}
	if (fflag!=1)
		tell (user,"\nYou can't see any connecting rooms");
	if (fflag==2)
		tell (user,", yet you feel there may be a room\nclose by...\n");
	else
		tell (user,"\n");
	tell (user,"\n\n"); 

	debug ("Done look!");

	return 0;
}

int areas(int user)
{
	int i;
	char *status[]={"Unused","Public","Private","Public (fixed)","Private (fixed)"};

	tell (user,"You may find the following rooms here...\n");

	tell (user,"Room name       Status\n");
	for (i=0; i < MAX_AREAS; i++) {
		if (map[i].status!=UNUSED) {
			sprintf (users[user].scratch,"%-15s %s\n",map[i].name,status[map[i].status]);
			tell (user,users[user].scratch);
		}
	}
	tell (user,"\n"); 

	return 0;
}
