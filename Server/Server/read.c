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
#define MAXLINE 100
/* read and ignore request headers */
void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];
    rio_readlineb(rp, buf,MAXLINE);
    
    while (strcmp(buf,"\r\n")) {
        rio_readlineb(rp,buf,MAXLINE);
        printf("%s",buf);
    }
    return;
}
