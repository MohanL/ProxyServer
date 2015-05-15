

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
    int sock, n;
    struct hostent *hp;
    struct sockaddr_in server;
    char server_reply[MAXBUF];
    
    if( (sock = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        printf("could not create client socket\n");
        return -1;
    }

    if ((hp = gethostbyname(hostname))== NULL) {
        printf("invalid host no match in DNS\n");
        return -2;
    }
    // configure the server
    //copy the first IP address in the host entry (which is already in network byte order) to the server’s socket address structure
    bzero((char *)&server,sizeof(server));
    server.sin_family = AF_INET;
    bcopy((char *) hp->h_addr_list[0],
          (char *)&server.sin_addr.s_addr,hp->h_length);
    
    /* second two to configure the server
    *  memset(&server, 0, sizeof(server));
    *  server.sin_family = AF_INET;
    *  server.sin_addr.s_addr= inet_addr(hp->h_addr_list[0]);
    */
    
    // convert between host to network byte order.
    server.sin_port=htons(port);
    
    // connect to the remote server
    if(connect(sock,(void *)&server,sizeof(server)) < 0)
    {
        printf("connect failed. Error\n");
        return -1;
    }
    
    //going to send the server things that real client send to this server
    if( send(sock,request, strlen(request) , 0) < 0)
    {
            puts("Send failed");
            return 1;
    }
    
    bzero(server_reply,MAXBUF);
    /* modification 1 */
    while(1)
    {
        n = (int)recv(sock,server_reply,MAXBUF,0);
        if(n == 0) // fixed chunked transfer encoding.
        {
            close(sock);
            break;
        }
        send(fd,server_reply,n,0);
    }
    close(sock);
    puts("connection closed");
    return 0;
    
}

 /* modification 1 */
 /* original version of recv and send function
 recv(sock,server_reply,MAXBUF,0);
 if(strstr(server_reply,"Transfer-Encoding: chunked"))
 {
 //puts("we are working on chunked transfer encoding");
 
 // the difference between the java version and the C version is the string concatenation here, char[] is not allowed to concatenate over here, but in java, it is fine.
 write(fd, server_reply , sizeof(server_reply));
 while(!strstr(server_reply,"0\r\n\r\n")&& (recv(sock, server_reply, MAXBUF, 0)<0))
 {
 write(fd, server_reply , sizeof(server_reply));
 bzero(server_reply,MAXBUF);
 // recv(sock,server_reply,MAXBUF,0);
 }
 // I didn't notice here that I used strlen, which may affect the behavior
 write(fd, server_reply , sizeof(server_reply));
 
 // ERROR message : FAILED IncompleteRead(0 bytes read), did I not read everything ?
 
 }
 
 else{ //100%
 //puts("normal encoding");
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
 */

