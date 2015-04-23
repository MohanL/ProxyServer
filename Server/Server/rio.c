//
//  rio.c
//  Webserver
//
//  Created by Mohan Liu on 4/21/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#include "rio.h"
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void rio_readinitb(rio_t *rp, int fd){
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;
    while (nleft > 0)
    {
        if ((nread = read(fd, bufp, nleft)) < 0)
        {
            if (errno == EINTR) /* interrupted by sig handler */
                nread = 0;      /* and call read() again */
            else
                return -1;      /* errno set by read() */
        }
        else if (nread == 0)    /* EOF */
                break;
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);             /* return >= 0 */
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;
    
    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if(errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        bufp  += nwritten;
        
    }
    return n;
}
/* Book's code for edbugging
/*
 * rio_read - This is a wrapper for the Unix read() function that
 *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
 *    buffer, where n is the number of bytes requested by the user and
 *    rio_cnt is the number of unread bytes in the internal buffer. On
 *    entry, rio_read() refills the internal buffer via a call to
 *    read() if the internal buffer is empty.
 */
/* $begin rio_read */
/*
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    printf("rio_read is called\n");
    int cnt;
    
    while (rp->rio_cnt <= 0) {  // Refill if buf is empty
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf,
                           sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno != EINTR) // Interrupted by sig handler return
                return -1;
        }
        else if (rp->rio_cnt == 0)  // EOF
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf; // Reset buffer ptr
    }
    
    // Copy min(n, rp->rio_cnt) bytes from internal buf to user buf
    cnt = n;
    if (rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    printf("rio_read ends\n");

    return cnt;
}
*/
/* $end rio_read */

//Mohan's version of code

static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
    printf("rio_read is called\n");
    int cnt;
    while (rp -> rio_cnt <= 0) { // refill if buf is empty
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if(rp -> rio_cnt < 0)
        {
            if(errno != EINTR)
                return -1;
        }
        else if (rp->rio_cnt == 0)
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf;
    }
    cnt = (int)n;
    if(rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(usrbuf,rp->rio_bufptr,cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -=cnt;
    printf("rio_read ends\n");
    return cnt;
}


//Mohan's version of code
/*ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    int n, rc;
    char c, *bufp = usrbuf;
    
    for (n = 1; n < maxlen; n++) {
        if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n') {
                n++;
                break;
            }
        } else if (rc == 0) {
            if (n == 1)
                return 0; // EOF, no data read
            else
                break;    // EOF, some data was read
        } else
            return -1;	  // Error
    }
    *bufp = 0;
    return n-1;
}
*/
/* $end rio_readlineb */
// rio_readlineb - robustly read a text line (buffered)
ssize_t rio_readlineb(rio_t *rp, void * usrbuf, size_t maxlen)
{
    printf("rio_readlineb is called \n");
    int n, rc;
    char c, *bufp = usrbuf;
    
    for(n = 1; n < maxlen;n++)
    {
        if((rc = (int)rio_read(rp, &c, 1)) == 1)
        {
            *bufp++ = c;
            if(c == '\n')
                break;
        }
        else if (rc == 0){
            if(n == 1)
                return 0;
            else
                break;
        }
        else
            return -1;
    }
    *bufp = 0;
    printf("rio_readlineb ends \n");
    return n;
}

ssize_t rio_readnb(rio_t *rp,void *usrbuf,size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;
    
    while (nleft > 0) {
        if ((nread = rio_read(rp, bufp,nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;
            } else {
                return -1;
            }
        } else if ( nread == 0) {
            break;
        }
        nleft -= nread;
        bufp += nread;
    }
    return (n-nleft);
}




