//
//  m.h
//  Server
//
//  Created by Mohan Liu on 4/22/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#ifndef __Server__m__
#define __Server__m__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include "open_listenfd.h"
#include "rio.h"
#include "client.h"

extern void doit(int fd);
extern void read_requesthdrs(rio_t *rp);
extern int  parse_uri(char *uri, char * filename, char *cgiars);
extern void serve_static(int fd, char *filename, int filesize);
extern void get_filetype(char * filename, char * filetype);
extern void serve_dynamic(int fd, char *filename, char *cgiargs);
extern void clienterror(int fd, char *cause, char *errnum, char * shortmsg, char *longmsg);
#endif /* defined(__Server__m__) */
