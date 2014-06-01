/*****************************************************************************
**
** Debug routines!
**
*****************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

void debuginit(void)
{
#ifndef DEBUG
	close(0);
	close(1);
	close(2);
#endif
}

void debug(char *fmt, ...)
{
#ifdef DEBUG
	char buf[2048];
	va_list args;

	va_start(args,fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	printf ("DEBUG: %s\n",buf);
#endif
}

void debug_bytes(char *fmt, ...)
{
#ifdef DEBUG
	unsigned char buf[2048];
	va_list args;
	unsigned int i;

	va_start(args,fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	printf ("DEBUG BYTES: ");
	for (i=0; i < strlen(buf); i++)
		printf ("%d ",buf[i]);
	printf ("\n");
#endif
}
