#macro
CC=gcc

FILES = client.c open_listenfd.c rio.c getfiletype.c serve.c parse_uri.c doit.c read.c clienterror.c m.c

OUT_EXE = Server


#targets the name of the target, the dependencies of the target, and finally the action associated with the target:

build: $(FILES)
    $(CC) -o $(OUT_EXE) $(FILES)

clean:
    -rm -f *.o core

rebuild: clean build