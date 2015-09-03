#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "debug.h"

void debug(const char * format, ...)
{
	if(debugging){
		va_list args;
		fprintf(stdout, "[Debug] ");
		va_start(args, format);
		vfprintf(stdout, format, args);
		va_end(args);
	}
}
void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
