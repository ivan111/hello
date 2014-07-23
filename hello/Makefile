CC  =  gcc
CFLAGS  =  -Wall  -Wno-unused-function  -std=gnu99  -Iinclude


all: lex.yy.c  y.tab.c



lex.yy.c: scanner.l
	flex  scanner.l



y.tab.c: parser.y
	bison  -y  -d  parser.y
	mv y.tab.h ../include/



clean:
	rm  -f  lex.yy.c  lex.yy.o  y.tab.c  ../include/y.tab.h  y.tab.o
