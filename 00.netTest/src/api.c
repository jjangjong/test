#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include "netTest.h"

struct stream *makeSteam(struct netTest *test){
	int i;
	struct stream *sp;
	char template[] = "/tmp/netTest.XXXXXX";
	char A='A',z='z';

	sp = (struct stream *)malloc(sizeof(struct stream));
	if(!sp) {
		printf("Can't make stream\n");
		return NULL;
	}
	memset(sp, 0, sizeof(struct stream));

	sp->buffer_fd=mkstemp(template);
	if(sp->buffer_fd == -1){
		free(sp);
		return NULL;
	}
	if (unlink(template) < 0) {
		free(sp);
		return NULL;
	}
	if (ftruncate(sp->buffer_fd, test->blksize) < 0) {
		free(sp);
		return NULL;
	}

	sp->buffer = (char *)mmap(NULL,test->blksize,
			PROT_READ | PROT_WRITE, MAP_PRIVATE, sp->buffer_fd,0);
	if(sp->buffer == MAP_FAILED){
		free(sp);
		return NULL;
	}

	srandom(time(NULL));
	for (i = 0; i < test->blksize; ++i){
		//sp->buffer[i] = random();
		sp->buffer[i] = (random()%(A-z))+A; //ASCII range
		printf("sp->buffer[%d] = %d\n",i, sp->buffer[i]);
	}

	return sp;
}
