
#include "open_listenfd.h"
#include <sys/socket.h> // listen and bind
#include <netinet/in.h>
#include <string.h> // for the bzero function
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define	MAXLINE	 8192  /* Max text line length */
#define MAXBUF   8192  /* Max I/O buffer size */
#define LISTENQ  2000  /* Maximum number of client connections*/


int open_listenfd(int port)
{
    int listenfd,optval = 1;
    struct sockaddr_in serveraddr;
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
        return -1;
    // setsockopt function : The setsockopt function sets a socket option.
    /* protocal type of the setsockopt function 
     * int setsockopt( SOCKET s, int level, int optname, const char *optval, int optlen );
     * about the parameters : SOCKET S : A descriptor that identifies a socket. In our case, listenfd is the socekt descriptor
     *                        int level : The level at which the option is defined (for example, SOL_SOCKET)
     *                        int optname : The socket option for which the value is to be set (for example, SO_BROADCAST). The optname  
     *                                      parameter must be a socket option defined within the specified level, or behavior is undefined.
     *                        optval [in] : A pointer to the buffer in which the value for the requested option is specified.
     *                        optlen [in] : The size, in bytes, of the buffer pointed to by the optval parameter.
     */
    
    /* about the real arugments we pass into the function 
     * SO_REUSEADDR : Allows the socket to be bound to an address that is already in use. For more information, see bind. Not applicable on
     *                ATM sockets.
     *
     */
    
    /*  explanation for the purpose
     *  the setsockopt function (not described here) to configure the server so 
     *  that it can be terminated and restarted immediately. By default,
     *  a restarted     server will deny connection requests from clients for
     *  approximately 30 seconds, which seriously hinders debugging.
     */
    
    // Eliminate "Address already in use "error from bind
    //Modified by Mohan Liu
    /* if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)))
        return -1;
    */
    // Listenfd will be an end point for all request to port on any IP address for this host
    bzero((char *)&serveraddr, sizeof(serveraddr));
    memset(&serveraddr,0,sizeof(serveraddr));
    
    /* we initialize the server’s socket address structure in preparation for 
     * calling the bind function. In this case, we have used the INADDR_ANY wild-
     * card address to tell the kernel that this server will accept requests to any
     * of the IP addresses for this host and to well-known port port
     */
    
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl((INADDR_ANY));
    //we use the htonl and htons functions to convert the IP address and port number from host byte order to network byte order.
    serveraddr.sin_port = htons((unsigned short)port);
    
    // the purpose of bind function is for the server to establish connenctions with clients
    /* the protocal of bind function
     * int bind (int sockfd, struct sockaddr *my_addr, int addrlen);
     * the bind function tells the server's kernel to associate the server's socket address
     * stored in my_addr with the socket descriptor sockfd, the addrlen argument is sizeof(sockaddr_in)
     */
    if(bind(listenfd, (void *)&serveraddr, sizeof(serveraddr)) < 0){
        perror("bind failed, Error");
        return -1;
    }
    
     /* about listen function
      * context knowledge : Clients are active entities that initiate connection requests. Servers are passive entities that wait for 
      *                     connection requests from clients. By default, the kernel assumes that a descriptor created by the socket 
      *                     function corresponds to an active socket that will live on the client end of a connection. A server calls 
      *                     the listen function to tell the kernel that the descriptor will be used by a server instead of a client.
      * therefore, listen is a function that can convert a active socket to a passive socket that can accept connection requests from clients.
      * The backlog argument is a hint about the number of outstanding connection requests that the kernel should queue up before
      * it starts to refuse requests, based on cycle1 and cycle1's limit, we set this number to be relatively small comparing to a real webserver.
      */
     // make it a listening socket ready to accept connection requests
    if(listen(listenfd,LISTENQ) < 0){
        perror("listen failed, Error");
        return -1;
    }
    return listenfd;
}

