//
//  m.c
//  Server
//
//  Created by Mohan Liu on 4/22/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#include "m.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rio.h"
#include <netinet/in.h>
#include "open_listenfd.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define MAXLINE 100
#define MAXBUF 100

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int  parse_uri(char *uri, char * filename, char *cgiars);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char * filename, char * filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char * shortmsg, char *longmsg);

int main (int argc, char **argv)
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    // check cmd args
    if(argc != 2)
    {
        fprintf(stderr, "usage: %s <port> \n", argv[0]);
        exit(1);
    }
    
    port = atoi(argv[1]);
    
    listenfd = open_listenfd(port);
    while(1)
    {
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd,(void *) &clientaddr,&clientlen);
        doit(connfd);
        close(connfd);
    }
}
void doit(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;
    
    // read request line and headers
    rio_readinitb(&rio,fd);
    rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf,"%s %s %s", method, uri, version);
    if(strcasecmp(method, "GET"))
    {
        clienterror(fd, method, "501", "NOT IMPLEMENTED", "tiny does not implement this method ");
        return;
    }
    read_requesthdrs(&rio);
    
    // parse uri from GET requests
    is_static = parse_uri(uri, filename, cgiargs);
    if(stat(filename,&sbuf) < 0)
    {
        clienterror(fd, filename, "404", "NOT FOUND", "tiny cou;t not find this file");
        return;
    }
    
    if(is_static){ // serve static content
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR& sbuf.st_mode)) {
            clienterror(fd, filename, "403", "FORBIDDEN", "Tiny could not read the file");
            return;
        }
        serve_static(fd, filename, (int)sbuf.st_size);
    }
    else{
        if(! (S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode))
        {
            clienterror(fd, filename, "403", "FORBIDDEN", "tiny could not run the CGI program");
            return;
        }
        serve_dynamic(fd,filename,cgiargs);
    }
}

int parse_uri(char * uri, char *filename, char * cgiargs)
{
    char *ptr;
    
    if(!strstr(uri, "cgi-bin"))
    {
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename,uri);
        
        if(uri[strlen(uri) - 1] == '/')
            strcat(filename,"home.html");
        return 1;
    }
    else  // dynamic content
    {
        ptr = index(uri,'?');
        if(ptr)
        {
            strcpy(cgiargs,ptr+1);
            *ptr = '\0';
        }
        else
            strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}

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
// need to implement dup2
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


// get_filetype - derive file type from file name
void get_filetype(char *filename, char * filetype)
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else
        strcpy(filetype, "text/plain");
}
/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];
    
    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);
    
    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));
}
/* $end clienterror */


