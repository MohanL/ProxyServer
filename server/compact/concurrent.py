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

def test_fetch_single(n):
    # concurrent connections
    for x in range(0, n):
        try:
            # creating a client socket
            print 'connection'
            clientsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                # connecting to the proxy server and leaving the connection open
            clientsock.connect((iphost, port))
            #clientsock[x].send('G')
        except Exception, e:
            print 'Concurrent Fetch ' + str(n) + ': FAILED ' +  str(e)
            return None
# this verifies that all the connections are connected .
# the problem is that it could not set up >=2 connections with the server.
# This test verifies that your proxy server can handle concurrent (= 2) connections successfully.
#try:
#html = urllib2.urlopen("http://cs.rochester.edu/u/hedayati/csc252/lorem.html").read()
#        print ('Single Fetch ' + ': ') + 'PASSED' if hashlib.md5(html).hexdigest() == '5ac3495fa2ffab9e97d519ce8cff1b5c' else 'FAILED invalid hash:' + hashlib.md5(html).hexdigest()
        #except Exception, e:
#       print 'Single Fetch '+ ': FAILED ' +  str(e)
def test_fetch_text():
    # simple fetch - content
    # [http://cs.rochester.edu/u/hedayati/csc252/lorem.html] is fetched through your proxy server,
    # and we check to see if the word 'pellentesque' is in the content or not
    try:
        html = urllib2.urlopen("http://cs.rochester.edu/u/hedayati/csc252/lorem.html").read()
        print 'Simple Fetch (Text): ' + 'PASSED' if 'pellentesque' in html else 'FAILED'
    except Exception, e:
        print 'Simple Fetch (Text): FAILED ' +  str(e)

if __name__ == '__main__':
#test_fetch_single(1)
    test_fetch_single(0);

