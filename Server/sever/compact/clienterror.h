/*
 CS252 Assignment 5
 Team: Katarzyna Ruszowska kruszows@u.rochester.edu
 Mohan Liu           mliu26@u.rochester.edu
*/

#ifndef __Server__clienterror__
#define __Server__clienterror__

#include <stdio.h>
extern void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
#endif /* defined(__Server__clienterror__) */
