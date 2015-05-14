

#ifndef __Server__clienterror__
#define __Server__clienterror__

#include <stdio.h>
extern void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
#endif /* defined(__Server__clienterror__) */
