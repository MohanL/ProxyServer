//
//  clienterror.h
//  Server
//
//  Created by Mohan Liu on 4/23/15.
//  Copyright (c) 2015 Mohan Liu. All rights reserved.
//

#ifndef __Server__clienterror__
#define __Server__clienterror__

#include <stdio.h>
extern void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
#endif /* defined(__Server__clienterror__) */
