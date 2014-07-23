CC  =  gcc
CFLAGS  =  -g  -Wall  -Wno-unused-function  -std=gnu99  -Iinclude

SRC_DIRS = objects hello
INC_DIRS = include include/objects

C_SRCS  := $(foreach sdir,$(SRC_DIRS),$(wildcard $(sdir)/*.c))
C_OBJS  := $(C_SRCS:.c=.o)

INCS    := $(foreach idir,$(INC_DIRS),$(wildcard $(idir)/*.h))



all: hello


.PHONY: hello

hello: flex_bison  $(C_OBJS)  $(INCS)
	$(CC) $(CFLAGS)  -o bin/$@  $(C_OBJS)



flex_bison: hello/scanner.l  hello/parser.y
	$(MAKE) -C hello



clean:
	$(MAKE) clean -C hello
	rm  -f  bin/hello  $(C_OBJS)
