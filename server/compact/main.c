

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

//resource : http://www.cs.dartmouth.edu/~campbell/cs50/socketprogramming.html
//resource : https://stackoverflow.com/questions/9137965/regarding-a-simple-tcp-based-echo-server-using-sockets-api

void *connection_handler(void *);

int main (int argc, char **argv)
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    /*ignore the SIGPIPE signal*/
    //signal(SIGPIPE,SIG_IGN);
    
    // check cmd args
    if(argc != 2)
    {
        fprintf(stderr, "usage: %s <port> \n", argv[0]);
        exit(1);
    }
    
    port = atoi(argv[1]);
    
    // prepare a server socket which is ready to connect to client
    if((listenfd = open_listenfd(port)) < 0){
        perror("Problem in creating the socket");
        exit(2);
    }
    
    printf("%s\n","Server running ... waiting for incoming connections");

    clientlen = sizeof(clientaddr);
    while(1) {
        
        if((connfd = accept(listenfd,(void *)&clientaddr,&clientlen)) <0)
        {
            puts("connection error");
            continue;
        }
        puts("New connection accepted");
        
        /* multi-threading */
        pthread_t t;
        int *new_sock = malloc(1);
        *new_sock = connfd;
        
        if(pthread_create(&t,NULL,connection_handler,(void*)new_sock)< 0)
        {
            perror("could not create thread");
            return 1;
        }
        //puts("handler assigned");
    }
    if(connfd < 0)
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









