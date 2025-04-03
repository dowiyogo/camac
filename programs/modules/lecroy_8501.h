#ifndef LECROY_8501
#define LECROY_8501

#include <stdio.h>
#include "crate_lib.h"

typedef struct values{
int freq;
int reg;
int mult;
} camac_clock;

camac_clock frequencies[19];
// frequencies : 20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000,1000000,2000000,5000000,10000000,20000000

short Linit_clock(char* ip);
double LSetClock(short CID, short card, int frequency);

#endif
