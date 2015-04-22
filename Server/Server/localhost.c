//
//  localhost.c
//  Webserver
//
//  Created by Mohan Liu on 4/20/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, const char * argv[]) {
    char ** pp;
    struct in_addr addr;
    struct hostent *hostp;
    
    if(argc!=2)
    {
        fprintf(stderr, "usage:%s <domain name or dotted-decimal>\n",argv[0]);
        exit(0);
    }
    
    if (inet_aton(argv[1], &addr)!= 0) {
        hostp = gethostbyaddr(&addr, sizeof(addr), AF_INET);
    } else {
        hostp = gethostbyname(argv[1]);
    }
    
    printf("official hostname:%s\n",hostp->h_name);
    
    for(pp = hostp->h_aliases;*pp!=NULL;pp++)
        printf("alias: %s\n",*pp);
    
    for(pp = hostp->h_addr_list;*pp!=NULL;pp++){
        addr.s_addr = ((struct in_addr *)*pp) -> s_addr;
        printf("address:%s\n",inet_ntoa(addr));
    }
    exit(0);
}
