#macro
CC=gcc

FILES = open_listenfd.o rio.o doit.o main.o interclient.o clienterror.o


OUT_EXE = ProxyServer

#targets the name of the target, the dependencies of the target, and finally the action associated with the target:

rebuild::clean build

open_listenfd.o: open_listenfd.c
rio.o: rio.c rio.h
interclient.o : interclient.c interclient.h
clienterror.o : clienterror.c clienterror.h rio.c rio.h
doit.o: doit.h doit.c rio.h rio.c interclient.c interclient.h clienterror.c clienterror.h
main.o: open_listenfd.h open_listenfd.c rio.h rio.c doit.h doit.c

build: $(FILES);$(CC) -pthread -o $(OUT_EXE) $(FILES)
clean:;-rm -f *.o core ProxyServer
test:;~cs252/labs15/proxy-test.py cycle1.csug.rochester.edu 8080