/*
 CS252 Assignment 5
 Team: Katarzyna Ruszowska kruszows@u.rochester.edu
 Mohan Liu           mliu26@u.rochester.edu
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <pthread.h>
#include "open_listenfd.h"
#include "rio.h"
#include "doit.h"
// thread function
void *connection_handler(void *);

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
    
    //  I need to modify this, currently it is not multithreading but iterative.
    /*
    while(1)
    {
        clientlen = sizeof(clientaddr);
        if((connfd = accept(listenfd,(void *) &clientaddr,&clientlen)) < 0)
        {
            perror("accept failed\n");
            return -1;
        }
        puts("connection accepted\n");
        doit(connfd);
        close(connfd);
    }
     */
    clientlen = sizeof(clientaddr);
    while ((connfd = accept(listenfd,(void *)&clientaddr,&clientlen)) > 0) {
        puts("New connection accepted");
        
        pthread_t t;
        int *new_sock = malloc(1);
        *new_sock = connfd;
        
        if(pthread_create(&t,NULL,connection_handler,(void*)new_sock)< 0)
        {
            perror("coudld not create thread");
            return 1;
        }
        //puts("handler assigned");
    }
    if(connfd < 0 )
    {
        perror("accept failed");
        return 1;
    }
    
    return 0;
}
// handler for each connection
// parameter is the connected socket descriptor
void * connection_handler(void * socket_in)
{
    // get the socket descriptor
    int sock = *(int *)socket_in;
    doit(sock);
    free(socket_in);
    return 0;
}









