//
//  doit.c
//  Server
//
//  Created by Mohan Liu on 4/23/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#include "doit.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "rio.h"
#include <string.h>
#include "parse_uri.h"
#include "read.h"
#include "serve.h"
#include "clienterror.h"
#include "interclient.h"
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */
void doit(int fd)
{
    //int is_static;
    //struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE],nbuf[MAXLINE];
    //char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;
    
    char * server, *p, *suburi;
    
    // read request line and headers
    rio_readinitb(&rio,fd);
    rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf,"%s %s %s", method, uri, version);
    if(strcasecmp(method, "GET")) //POST www.cycle1.csug.rochester.edu/home.html HTTP/1.1
    {
        clienterror(fd, method, "501", "NOT IMPLEMENTED", "tiny does not implement this method");
        return;
    }
    
    // parse the request and reconstruct
    if(strstr(uri, "http")) // case the server has GET http://www.cycle1.csug.rochester.edu/home.html HTTP/1.1
    {
        p = strtok(uri, "/");
        server = strtok (NULL, "/");
        suburi = p;
        //printf("%s\n",p);
        while (p != NULL)
        {
            suburi = p;
            p= strtok (NULL, "/");
            
        }
        if(strcmp(suburi, "http:")== 0)
            sprintf(nbuf,"GET /home.html HTTP/1.1\r\nhost:%s\r\n\r\n",server);

        else
            sprintf(nbuf,"GET /%s HTTP/1.1\r\nhost:%s\r\n\r\n",suburi,server);
        printf("%s",nbuf);

    }
    else// case the server doesn't have http : GET www.cycle1.csug.rochester.edu/home.html HTTP/1.1
    {
        //printf("case 2: there is no http\n");
        p = strtok(uri, "/");
        server = p;
        suburi = p;
        //printf("%s\n",p);
        while (p != NULL)
        {
            suburi = p;
            p= strtok (NULL, "/");
            
        }
        
        if(strcmp(suburi, server) == 0)
            sprintf(nbuf,"GET /home.html HTTP/1.1\r\nhost:%s\r\n\r\n",server);
        //combine the request into (method suburi version\r\n host: server\r\n"
        else
            sprintf(nbuf,"2.GET /%s HTTP/1.1\r\nhost:%s\r\n\r\n",suburi,server);
        
        printf("%s",nbuf);
    }
    
    
    // at this point we have variable char * server, char nbuf and port = 80
    
    //interclient(server,80,nbuf,fd);
    
    // this line of code doesn't really do anything, does it ?
    //read_requesthdrs(&rio);

    
    /* Here we are suppose to send the request to the dest server and retrieve whatever we need */
    
    // parse uri from GET requests
    /* we don't need to parse the URI
    is_static = parse_uri(uri, filename, cgiargs);
    if(stat(filename,&sbuf) < 0)
    {
        clienterror(fd, filename, "404", "NOT FOUND", "tiny coult not find this file");
        return;
    }
    
    if(is_static){ // serve static content
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
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
     */
}
