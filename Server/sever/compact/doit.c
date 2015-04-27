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
#include "interclient.h"
#include "clienterror.h"
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */
void doit(int fd)
{

    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE],nbuf[MAXLINE];
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
    bzero(nbuf,MAXBUF);
    
    // parse the request and reconstruct
    // trouble shooting: suburi is not parsed correctly.
    if(strstr(uri, "http")) // case the server has GET http://www.cycle1.csug.rochester.edu/home.html HTTP/1.1
    {
        p = strtok(uri, "/");
        server = strtok (NULL, "/");
        suburi = p;
        while (p != NULL)
        {
            suburi = p;
            p= strtok (NULL, "/");
            
        }
        if(strcmp(suburi, "http:")== 0)
            sprintf(nbuf,"GET /home.html HTTP/1.1\r\nhost:%s\r\n\r\na",server);

        else
            sprintf(nbuf,"GET /%s HTTP/1.1\r\nhost:%s\r\n\r\na",suburi,server);
        
        //printf("%s",nbuf);

    }
    else// case the server doesn't have http : GET www.cycle1.csug.rochester.edu/home.html HTTP/1.1
    {
        p = strtok(uri, "/");
        server = p;
        suburi = p;
        while (p != NULL)
        {
            suburi = p;
            p= strtok (NULL, "/");
            
        }
        
        if(strcmp(suburi, server) == 0)
            sprintf(nbuf,"GET /home.html HTTP/1.1\r\nhost:%s\r\n\r\na",server);
        //combine the request into (method suburi version\r\n host: server\r\n"
        else
            sprintf(nbuf,"GET /%s HTTP/1.1\r\nhost:%s\r\n\r\na",suburi,server);

    }
    
    
    // at this point we have variable char * server, char nbuf and port = 80
    
    unsigned long len = strlen(nbuf)-1;
    char request[len];
    bzero(request, len);
    strncpy(request, nbuf, len);
    printf("%s",request);
    puts("REMOTE SERVER OUTPUT");
    interclient(server,80,request,fd);
}
