//
//  client.c
//  Server
//
//  Created by Mohan Liu on 4/24/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

// basically create a intermediate client and send the real server the real stuff
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h>
#include <unistd.h>
#include <stdlib.h>

#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
int interclient(char * hostname,int port, char request[],int fd)
{
    //printf("*%s*\n",hostname);
    int sock;
    struct hostent *hp;
    struct sockaddr_in server;
    char server_reply[MAXBUF];
    
    // create a client socket
    if( (sock = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        printf("could not create client socket\n");
        return -1;
    }
    
    
    // get our host's information
    // fill in the server's IP address and port
    if ((hp = gethostbyname(hostname))== NULL) {
        printf("invalid host no match in DNS\n");
        return -2;// can't get correct Domain name from the DNS system
    }
    
    // configure the server
    bzero((char *)&server,sizeof(server));
    
    //copy the first IP address in the host entry (which is already in network byte order) to the server’s socket address structure
    server.sin_family = AF_INET;
    bcopy((char *) hp->h_addr_list[0],
          (char *)&server.sin_addr.s_addr,hp->h_length);
    
    // convert between host to network byte order.
    server.sin_port=htons(port);
    
    // connect to the remote server
    if(connect(sock,(void *)&server,sizeof(server)) < 0)
    {
        printf("connect failed. Error\n");
        return -1;
    }
    
    //printf("intermediate connection successful\n");
    
    //going to send the server things that real client send to this server
    if( send(sock,request, MAXBUF , 0) < 0)
    {
            puts("Send failed");
            return 1;
    }
    bzero(server_reply,MAXBUF);
    recv(sock,server_reply,MAXBUF,0);
    if(strstr(server_reply,"chunked"))
    {
        puts("we are working on chunked transfer encoding");
        while(!strstr(server_reply,"0\r\n\r\n"))
        {
        	write(fd, server_reply , sizeof(server_reply));
    		bzero(server_reply,MAXBUF);
            recv(sock,server_reply,MAXBUF,0);
        }
        write(fd, server_reply , strlen(server_reply)-4);
    }	    
    else{ //100%
        puts("normal encoding");
        write(fd, server_reply , sizeof(server_reply));
        bzero(server_reply,MAXBUF);
    	while(recv(sock,server_reply , MAXBUF , 0) > 0)
    	{
        	
        	write(fd, server_reply , sizeof(server_reply));
        	bzero(server_reply,MAXBUF);
    	}
    	const char end[] = "\r\n";
    	write(fd, end, sizeof(end));
    }
    close(sock);
    return 0;
    
}
