//
//  main.c
//  Server
//
//  Created by Mohan Liu on 4/21/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#include <stdio.h>
#include "client.h"
#include "open_listenfd.h"
#include "rio.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "echo.h"
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  1024  /* Second argument to listen() */

/* for future reference here is the prototype for the accept function
 * int accept (int listenfd, struct sockaddr *addr, int *addrlen)
 *
 *
 *
 */
int main(int argc, char * argv[]) {
    
    int clientfd, port;
    char *host, buf[MAXLINE];
    rio_t rio;
    
    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }
    host = argv[1];
    port = atoi(argv[2]);
    
    clientfd = open_clientfd(host, port);
    rio_readinitb(&rio, clientfd);
    
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        printf("1so I am actually working: %s\n", buf);
        rio_writen(clientfd, buf, strlen(buf));
        printf("2so I am actually working: %s\n", buf);
        rio_readlineb(&rio, buf, MAXLINE);
        //fputs(buf, stdout);
        printf("3so I am actually working: %s\n", buf);
    }
    close(clientfd); //line:netp:echoclient:close
    exit(0);

}

/* readinitb, rio_readlineb and riowriten test
 // insert code here...
 printf("Hello, World!\n");
 
 int n ;
 rio_t rio;
 char buf [MAXLINE];
 
 rio_readinitb(&rio,STDIN_FILENO);
 while ((n = (int)rio_readlineb(&rio,buf,MAXLINE))) {
 rio_writen(STDOUT_FILENO,buf,n);
 }
 
 
 return 0;
 */