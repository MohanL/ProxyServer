//
//  client.c
//  Webserver
//
//  Created by Mohan Liu on 4/20/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//


/* this is a file defines all the necessary functions for a client socket */
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
// where the generic and specific socket address structure are stored
// where the connect function could be referenced
#include <sys/socket.h>


// socket function
#include <netinet/in.h>
#include <string.h> // for the bzero function
#include <netinet/in.h>

#include <arpa/inet.h>
#include <netdb.h>

/* for future reference here is the structure of the sockeaddr_in( internet style socket address structure ) 
struct socketaddr_in{
    unsigned short sin_family;  // Address family (always AF_INET)
    unsigned short sin_port;    // port number in network byte order
    struct in_addr sin_addr;    // IP address in network byte order
    unsigned char  sin_zero[8]; // padding to sizeof(struct socketaddr)
}
*/

/* for future reference here is the structure of the hostent (DNS host entry structure)
struct hostent{
    char *h_name;               // official name of the host
    char **h_aliases;           // NULL terminated array of domain names;
    int h_addrtype;             // Host address type (AF_INET)
    int h_length;               // length of address in bytes;
    char ** h_addr_list;        // NULL terminated array of in_addr structs
}
*/

/* for future reference here is the interent address strucutre 
struct in_addr{
 unsigned int s_addr;           // network byte order
 }
 */


//self-defined wrapper function for connect and socket function
//returns descriptor if OK, -1 on Unix error and -2 on DNS error
int open_clientfd(char * hostname, int port)
{
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in serveraddr;
    
    // whether the host is a client or a server, a socket descriptor is necessary
    // the socket function returns -1 if error, nonnegative socket descriptor if correct
    // AF_INET indicates that we are using the Internet
    // SOCK_STREAM indicates that the socket will be an end point for an Internet connection.
    // clientfd descriptor returned by socket is only partially opened and cannot yet be used for reading and writing.
    if (clientfd  == socket(AF_INET, SOCK_STREAM, 0)<0) {
        return -1; // could not create a socket for interenet connection
    }
    
    
    // fill in the server's IP address and port
    if ((hp = gethostbyname(hostname))== NULL) {
        return -2;// can't get correct Domain name from the DNS system
    }
    
    
    //The bzero()function copies n bytes, each with a value of zero, into string s.
    
    // I am not sure what this line does to the code
    bzero((char *)&serveraddr,sizeof(serveraddr));
    
    //copy the first IP address in the host entry (which is already in network byte order) to the server’s socket address structure
    serveraddr.sin_family = AF_INET;
    bcopy((char *) hp->h_addr_list[0],
          (char *)&serveraddr.sin_addr.s_addr,hp->h_length);
    
    // convert between host to network byte order.
    serveraddr.sin_port=htons(port);
    
    if(connect(clientfd,(void *)&serveraddr,sizeof(serveraddr))<0)
        return -1;
    return clientfd;
}
