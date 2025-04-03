#-*-makefile-*-
binaries = jenet_cfsa_test_lx jenet_irq_test_lx jenet_bin_test_lx jenet_block_test_lx

all: compile

compile: $(binaries)

INCLUDE = -I ./crate_lib

CC  = gcc $(INCLUDE)
LINK = gcc

CFSA_SRC = crate_lib/crate_lib.c jenet_cfsa_test/jenet_cfsa_test.c
CFSA_OBJ = $(CFSA_SRC:.c=.o) 

IRQ_SRC = crate_lib/crate_lib.c jenet_irq_test/jenet_irq_test.c 
IRQ_OBJ = $(IRQ_SRC:.c=.o) 

BIN_SRC = crate_lib/crate_lib.c jenet_bin_test/jenet_bin_test.c 
BIN_OBJ = $(BIN_SRC:.c=.o)

BLK_SRC = crate_lib/crate_lib.c jenet_block_test/jenet_block_test.c 
BLK_OBJ = $(BIN_SRC:.c=.o)

jenet_cfsa_test_lx:  $(CFSA_OBJ) $(addsuffix .o, $(common))  
	$(LINK)  -o $@ $^ -L. -lpthread 

jenet_irq_test_lx: $(IRQ_OBJ) $(addsuffix .o, $(common)) 
	$(LINK) -o $@ $^ -L. -lpthread 

jenet_bin_test_lx: $(BIN_OBJ) $(addsuffix .o, $(common)) 
	$(LINK) -o $@ $^ -L. -lpthread 

jenet_block_test_lx: $(BLK_OBJ) $(addsuffix .o, $(common)) 
	$(LINK) -o $@ $^ -L. -lpthread 

clean:
	rm -f core crate_lib/*.o jenet_cfsa_test/*.o jenet_irq_test/*.o jenet_bin_test/*.o jenet_block_test/*.o $(binaries) $(addsuffix .gdb, $(binaries))

