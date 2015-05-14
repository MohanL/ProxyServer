
#include "doit.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "rio.h"
#include <string.h>
#include "clienterror.h"
#include "interclient.h"
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
    if(strstr(uri, "http")) // case the server has GET http://www.cycle1.csug.rochester.edu/home.html HTTP/1.1
    {
        //puts("case1");
        p = strtok(uri, "/");
        server = strtok (NULL, "/");
        suburi = p;
        char a[1000];
        bzero(a, sizeof(a));
        while ((p=strtok(NULL, "/"))!= NULL){
            strcat(a,"/");
            strcat(a, p);
            suburi = p;
        }
        if(strcmp(suburi, "http:")== 0)
            sprintf(nbuf,"GET /home.html HTTP/1.1\r\nhost: %s\r\n\r\n",server);
        else
            sprintf(nbuf,"GET %s HTTP/1.1\r\nhost: %s\r\n\r\n",a,server);
    }
    else// case the server doesn't have http : GET www.cycle1.csug.rochester.edu/home.html HTTP/1.1
    {
        //puts("case2");
        p = strtok(uri, "/");
        suburi = p;
        server = p;
        char a[50];
        while ((p=strtok(NULL, "/"))!= NULL){
            strcat(a,"/");
            strcat(a, p);
            suburi = p;
        }
        if(strcmp(suburi, server) == 0)
            sprintf(nbuf,"GET /home.html HTTP/1.1\r\nhost: %sa\r\nb\r\n",server);
        else
            sprintf(nbuf,"GET %s HTTP/1.1\r\nhost: %s\r\n\r\n",a,server);
    }

    unsigned long len = strlen(nbuf);
    char request[len];
    bzero(request, len);
    strncpy(request, nbuf, len);
    //printf("%s",request);
    interclient(server,80,request,fd);
}
