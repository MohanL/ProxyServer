#macro
CC=gcc

FILES = client.o open_listenfd.o rio.o getfiletype.o serve.o parse_uri.o doit.o read.o clienterror.o m.o

OUT_EXE = Server

#targets the name of the target, the dependencies of the target, and finally the action associated with the target:

build: $(FILES)
    $(CC) -o $(OUT_EXE) $(FILES)

client.o: client.c 
open_listenfd.o: open_listenfd.c
rio.o: rio.c rio.h
getfiletype.o: getfiletype.c getfiletype.h
serve.o: getfiletype.c getfiletype.h rio.c rio.h serve.c
parse_uri.o: parse_uri.c
doit.o: doit.h doit.c rio.h rio.c parse_uri.h parse_uri.c read.h read.c serve.h serve.c clienterror.h clienterror.c
read.o: rio.c rio.h read.h
clienterror.o: rio.h rio.c clienterror.h
m.o: open_listenfd.h open_listenfd.c rio.h rio.c doit.h doit.c

clean:
    -rm -f *.o core Server

rebuild:
    clean build

test:
    ~cs252/labs15/proxy-test.py cycle1.csug.rochester.edu 8080