/*
 CS252 Assignment 5
 Team: Katarzyna Ruszowska kruszows@u.rochester.edu
 Mohan Liu           mliu26@u.rochester.edu
*/

#ifndef __Webserver__rio__
#define __Webserver__rio__


#include <stdio.h>

#define RIO_BUFSIZE 8192
typedef struct {
    int  rio_fd;                /* descriptor for this internal buffer */
    int  rio_cnt;               /* unread bytes in internal buf */
    char *rio_bufptr;          /* next unread byte in the internal buf */
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;

extern void rio_readinitb(rio_t *rp, int fd);
extern ssize_t rio_readn(int fd, void *usrbuf, size_t n);
extern ssize_t rio_writen(int fd, void *usrbuf, size_t n);
extern ssize_t rio_readlineb(rio_t *rp, void * usrbuf, size_t maxlen);
extern ssize_t rio_readnb(rio_t *rp,void *usrbuf,size_t n);


#endif /* defined(__Webserver__rio__) */

