//
//  serve.c
//  Server
//
//  Created by Mohan Liu on 4/23/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#include "serve.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "getfiletype.h"
#include "rio.h"

#define MAXLINE 100
#define MAXBUF 100
extern char **environ;
void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char * srcp, filetype[MAXLINE], buf[MAXBUF];
    
    get_filetype(filename, filetype);
    sprintf(buf,"HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    rio_writen(fd, buf, strlen(buf));
    
    /* Send response body to client */
    srcfd = open(filename, O_RDONLY, 0);
    srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    close(srcfd);
    rio_writen(fd, srcp, filesize);
    munmap(srcp, filesize);
}
void serve_dynamic(int fd, char *filename, char *cgiargs) {
    char buf[MAXLINE], *emptylist[] = { NULL };
    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    rio_writen(fd, buf, strlen(buf));
    if (fork() == 0)
    {   /* child */
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1);
        dup2(fd, STDOUT_FILENO);  /*Redirect stdout to client */
        
        // need to populate the environ variable
        execve(filename, emptylist,environ); /* Run CGI program */
    }
    wait(NULL); /* Parent waits for and reaps child */
}
