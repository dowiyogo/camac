#-*-makefile-*-
binaries = pGen
libPath = /opt/caen/c111c/crate_lib/crate_lib
all: compile

compile: $(binaries)

INCLUDE = -I$(libPath)

CC  = gcc $(INCLUDE)
LINK = gcc

pGen: first.o crate_lib.o
	$(CC)  -o $@ $^ -L. -lpthread

crate_lib.o: $(libPath)/crate_lib.c
	$(CC) -c -o $@ $^

clean:
	rm -f *.o core 


