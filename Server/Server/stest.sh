#!/bin/sh

#  stest.sh
#  Server
#
#  Created by Mohan Liu on 4/23/15.
#  Copyright (c) 2015 Mohan Liu. All rights reserved.

./ProxyServer 8889
./telnet 127.0.0.1 8889
