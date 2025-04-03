#-*-makefile-*-
binaries = spectra
#libPath = /opt/caen/c111c/crate_lib/crate_lib
libPath = /home/xplus/camac/programs/crate_lib/crate_lib
libPath_modules = /home/xplus/camac/programs/modules
source = espectro.c $(libPath)/crate_lib.c $(libPath_modules)/lecroy_2249A.c $(libPath_modules)/caen_c111c.c $(libPath_modules)/OR_1320.c
obj = $(source:.c=.o)

all: compile

compile: $(binaries)

INCLUDE = -I$(libPath) -I$(libPath_modules)

CC  = gcc $(INCLUDE)
LINK = gcc

# $^ es lo de despues de los ":" y $@ es el objetivo, antes de ":"
spectra: $(obj) 
	$(CC)  -o $@ $^ -L. -lpthread -lm

crate_lib.o: $(libPath)/crate_lib.c
	$(CC) -c -o $@ $^

lecroy_2249A.o: $(libPath_modules)/lecroy_2249A.c
	$(CC) -c -o $@ $^
	
caen_c111c.o: $(libPath_modules)/caen_c111c.c
	$(CC) -c -o $@ $^

OR_1320.o: $(libPath_modules)/OR_1320.c
	$(CC) -c -o $@ $^

clean:
	rm -f *.o core 

rootprogram: rootSpectrum.o
	g++ rootSpectrum.o -o rootprogram `root-config --libs`

rootSpectrum.o: rootSpectrum.C
	g++ -c -g -Wall rootSpectrum.C `root-config --cflags` -o rootSpectrum.o

rootdraw: SpectrumDraw.o
	g++ SpectrumDraw.o -o rootdraw `root-config --libs`

SpectrumDraw.o: SpectrumDraw.C
	g++ -c -g -Wall SpectrumDraw.C `root-config --cflags` -o SpectrumDraw.o

visualspectrum: visualspectrum.o
	g++ visualspectrum.o $(libPath)/crate_lib.o $(libPath_modules)/lecroy_2249A.o -o livespectrum -L. -lpthread -lm `root-config --libs` 

visualspectrum.o: visualspectrum.C
	g++ -c -g -Wall visualspectrum.C $(libPath)/crate_lib.c $(libPath_modules)/lecroy_2249A.c `root-config --cflags` -o visualspectrum.o crate_lib.o lecroy_2249A.o

