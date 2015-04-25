CC=gcc


build::clean builp built builq
builp:;gcc -o eserver server.c
built:;gcc -o eclient client.c
builq:;gcc -o mserver multiserver.c
clean:;-rm -f *.o core eserver eclient mserver