#ifndef SRC_API_H_
#define SRC_API_H_

struct stream *makeSteam(struct netTest *test);
void freeStream(struct stream *sp);
void catchSigend(void (*handler)(int));
void getSigend(struct netTest *test);

#endif /* SRC_API_H_ */
