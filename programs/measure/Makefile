#-*-makefile-*-
binaries = spectra
libPath = /opt/caen/c111c/crate_lib/crate_lib
libPath_lecroy = /home/utfsm/rene/programs/Lecroy/2249A
source = measure.c $(libPath)/crate_lib.c $(libPath_lecroy)/lecroy_2249A.c
obj = $(source:.c=.o)

all: compile

compile: $(binaries)

INCLUDE = -I$(libPath) -I$(libPath_lecroy)

CC  = gcc $(INCLUDE)
LINK = gcc

spectra: $(obj)
	$(CC)  -o $@ $^ -L. -lpthread


crate_lib.o: $(libPath)/crate_lib.c
	$(CC) -c -o $@ $^

lecroy_2249A.o: $(libPath_lecroy)/lecroy_2249A.c
	$(CC) -c -o $@ $^
	
clean:
	rm -f *.o core 


