#include <stdio.h>
#include "armadillo.h"

char *ident="@(#) Armadillo UNIX talk/conference server. Chris Johnson.";

int main ()
{
	init();
	if (server.status!=FATAL) {
		debug ("Server status before run: %d\n",server.status);
		run();
	}
	done();

	return 0;
}
