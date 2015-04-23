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
#define MAXLINE 1000

/* for future reference here is the prototype for the accept function
 * int accept (int listenfd, struct sockaddr *addr, int *addrlen)
 *
 *
 *
 */
int main(int argc, char * argv[]) {
    
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);
    
    listenfd = open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);
        
        /* Determine the domain name and IP address of the client */
        hp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                           sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr);
        printf("server connected to %s (%s)\n", hp->h_name, haddrp);
        
        echo2(connfd);
        close(connfd);
    }
    exit(0);
}
/* interative echo server
 */

/* simple echo server main routine
 int clientfd, port;
 char *host, buf[MAXLINE];
 rio_t rio;
 
 if(argc!=3){
 fprintf(stderr,"usage: %s <host> <port> \n",argv[0]);
 exit(0);
 }
 
 host = argv[1];
 port = atoi(argv[2]);
 
 clientfd = open_clientfd(host, port);
 rio_readinitb(&rio, clientfd);
 
 while(fgets(buf, MAXLINE, stdin) != NULL)
 {
 rio_writen(clientfd, buf, strlen(buf));
 rio_readlineb(&rio, buf, MAXLINE);
 fputs(buf, stdout);
 }
 close(clientfd);
 exit(0);
 
 */


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
