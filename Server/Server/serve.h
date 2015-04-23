
//
//  serve.h
//  Server
//
//  Created by Mohan Liu on 4/23/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#ifndef __Server__serve__
#define __Server__serve__

#include <stdio.h>
extern void serve_static(int fd, char *filename, int filesize);
extern void serve_dynamic(int fd, char *filename, char *cgiargs);
#endif /* defined(__Server__serve__) */
