#-*-makefile-*-
binaries = mTest
libPath = /opt/caen/c111c/crate_lib/crate_lib
source = test.c $(libPath)/crate_lib.c
obj = $(source:.c=.o)

all: compile

compile: $(binaries)

INCLUDE = -I$(libPath)

CC  = gcc $(INCLUDE)
LINK = gcc

mTest: $(obj)
	$(CC)  -o $@ $^ -L. -lpthread

crate_lib.o: $(libPath)/crate_lib.c
	$(CC) -c -o $@ $^

clean:
	rm -f *.o core 


