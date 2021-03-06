#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "init.h"
#include "netTest.h"
#include "net.h"
#include "usage.h"

int main(int argc, char **argv) {
	struct netTest *test;
	struct stream *sp;

	printf("tttt branch\n");
	printf("ttt branch\n");

	printf("test branch\n");
	printf("merge remind\n");
	printf("merge 1\n");
	printf("test branch\n");
	printf("master\n");
	printf("merge3\n");
	printf("merge2\n");
	printf("add main.c\n");
	if(!(test = initTest()))
		debug("Init error\n");

	if (parseArguments(test, argc, argv) < 0) {
		debug("Error arguments\n");
		usage();
		exit(1);
	}

	if (test->role == 's') {
		if (serverRun(test) < 0) {
			debug("Can't run Server");
			exit(1);
		}
	} else {
		if (!(sp = makeSteam(test))) {
			printf("Error makeStream\n");
			exit(1);
		}
		test->sp = sp;

		if (client(test, sp) < 0)
			exit(1);
	}

//
//
//
//	if(!(sp = makeSteam(test))){
//		printf("Error makeStream\n");
//		exit(1);
//	}
//	test->sp = sp;
//	sp->test = test;
//
//	freeStream(test->sp);

//
//	if(test->role == 's'){
//		server(test, sp);
//	}else
//		client(test, sp);

	free(test);
	return 0;
}

