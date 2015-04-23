//
//  read.c
//  Server
//
//  Created by Mohan Liu on 4/23/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#include "read.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rio.h"
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */
/* read and ignore request headers */

/* I may need to modify this one */
void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];
    rio_readlineb(rp, buf,MAXLINE);
    
    printf("request is : %s\n",buf);
    
    while (strcmp(buf,"\r\n")) {
        rio_readlineb(rp,buf,MAXLINE);
        printf("%s",buf);
    }
    return;
}
