//
//  client.c
//  Server
//
//  Created by Mohan Liu on 4/24/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

// basically create a intermediate client and send the real server the real stuff
#include "client.h"
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h>
#include <unistd.h>

#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
int interclient(char * hostname,int port, char request[])
{
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
    
    //configure the server
    // some sort of initlization
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
    
    //going to send the server things that real client send to this server
    
    // over here the design is that this sock should exist until the real client is gone.
    while(1)
    {
        
        //Send the modified request to the real server
        if( send(sock , request, MAXBUF , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
        
        //Receive a reply from the real server
        if( recv(sock , server_reply , MAXBUF , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        
        // over here we need to send this message out back to the real client.
        
        puts("Server reply :");
        puts(server_reply);
    }
    close(sock);
    return 0;
    
}