#!/usr/bin/python

import urllib2
import hashlib
import socket
import sys

if len(sys.argv) != 3:
	print 'Usage: ' + sys.argv[0] + ' <ip or hostname> <port>\n'
	print 'Example: ' + sys.argv[0] + ' cycle1.csug.rochester.edu 8080'
	print 'Example: ' + sys.argv[0] + ' 128.151.69.85 8081'
	sys.exit()

# command line arguments
# iphost: is either IP address or hostname of the machine on which your proxy server is running
# port: is the port number to which your proxy server is listening
iphost = sys.argv[1]
port = int(sys.argv[2])

# urllib2 is used to send requests to your proxy server
proxy = urllib2.ProxyHandler({"http":"http://" + iphost + ":" + `port`})
opener = urllib2.build_opener(proxy)
urllib2.install_opener(opener)

def test_fetch_concurrent2():
    clientsock = [None] * 2
    try:
        clientsock[0] = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        clientsock[1] = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    #print 'ckpt1'
    clientsock[0].connect((iphost, port))
    #print 'ckpt2'
    # no exception on this.
    except Exception, e:
        print 'Concurrent Fetch 2'+ ': FAILED ' +  str(e)
        return None

    try:
        html = urllib2.urlopen("http://cs.rochester.edu/u/hedayati/csc252/lorem.html").read()
        print ('Concurrent Fetch ' + ': ') + 'PASSED' if hashlib.md5(html).hexdigest() == '5ac3495fa2ffab9e97d519ce8cff1b5c' else 'FAILED invalid hash:' + hashlib.md5(html).hexdigest()
    except Exception, e:
            print 'Concurrent Fetch 2' + ': FAILED ' +  str(e)

if __name__ == '__main__':
	test_fetch_concurrent2()


