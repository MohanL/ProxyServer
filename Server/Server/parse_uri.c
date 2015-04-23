//
//  parse_uri.c
//  Server
//
//  Created by Mohan Liu on 4/23/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#include "parse_uri.h"
#include <string.h>

int parse_uri(char * uri, char *filename, char * cgiargs)
{
    char *ptr;
    
    if(!strstr(uri, "cgi-bin"))
    {
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename,uri);
        
        if(uri[strlen(uri) - 1] == '/')
            strcat(filename,"home.html");
        return 1;
    }
    else  // dynamic content
    {
        ptr = index(uri,'?');
        if(ptr)
        {
            strcpy(cgiargs,ptr+1);
            *ptr = '\0';
        }
        else
            strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}
