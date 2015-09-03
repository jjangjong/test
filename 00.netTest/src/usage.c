#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "netTest.h"


const char usage_shortstr[] = "Usage: hanTest [-s|-c host] [-p port]\n"
                           "Try `netTest --help' for more information.\n";

void usage() {
	fputs(usage_shortstr, stderr);
}

int parseArguments(struct netTest *test, int argc, char **argv){
	static struct option longopts[] = {
			{"port", required_argument, NULL, 'p'},
			{"client", required_argument, NULL, 'c'},
			{"server", no_argument, NULL, 's'},
			{"bandwidth", required_argument, NULL, 'b'},
			{"debug", no_argument, NULL, 'd'},
			{NULL, 0, NULL, 0}
	};
	int flag;

	while ((flag = getopt_long(argc, argv, "p:sc:b:d",longopts, NULL)) != -1){
		switch(flag) {
		case 'p':
			test->serverPort = atoi(optarg);
			break;
		case 's':
			test->role = 's';
			break;
		case 'c':
			test->role='c';
			test->serverHostname = strdup(optarg);
			break;
		case 'b':
			test->blksize = atoi(optarg);
			break;
		case 'd':
			debugging = 1;
			break;
		default :
			return -1;
		}
	}
	return 0;
}
