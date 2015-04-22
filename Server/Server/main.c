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

#define MAXLINE 1000
int main(int argc, const char * argv[]) {
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
}
