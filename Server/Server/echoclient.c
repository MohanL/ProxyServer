//
//  echoclient.c
//  Webserver
//
//  Created by Mohan Liu on 4/20/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#include "echoclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "client.c"
#include "client.h"
#include "open_listenfd.h"
#include "open_listenfd.c"
#include <unistd.h>
#include "rio.h"
#include "rio.c"
#define MAXLINE 5000

int something(int argc, char ** argv)
{
    int clientfd, port;
    char *host, buf[MAXLINE];
    rio_t rio;

    if(argc!=3)
    {
        fprintf(stderr,"usage :%s <host> <port>\n",argv[0]);
        exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);

    clientfd = open_clientfd(host,port);
    rio_readinitb(&rio,clientfd);
    
    while (fgets(buf, MAXLINE, stdin)!= NULL) {
        rio_writen(clientfd,buf,strlen(buf));
        rio_readlineb(&rio,buf,MAXLINE);
        fputs(buf, stdout);
    }
    close(clientfd);
    exit(0);
    
}
  