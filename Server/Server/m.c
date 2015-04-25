//
//  m.c
//  Server
//
//  Created by Mohan Liu on 4/22/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

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
#include "doit.h"


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
    
    // prepare a server socket which is ready to connect to client
    listenfd = open_listenfd(port);
    
    while(1)
    {
        clientlen = sizeof(clientaddr);
        if((connfd = accept(listenfd,(void *) &clientaddr,(socklen_t *)&clientlen)) < 0)
        {
            perror("accept failed\n");
            return -1;
        }
        puts("connection accepted\n");
        doit(connfd);
        close(connfd);
    }
}









