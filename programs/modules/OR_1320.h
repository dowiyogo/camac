#ifndef OR_1320
#define OR_1320

#include <stdio.h>
#include "crate_lib.h"

short LSetNimOutPulse(short CID, short card,int word);
short LEnableNimOutPulse(short CID, short card);
short LWordNimOut(short CID, short card, int word);
void FastLWordNimOut(short CID, short card, int word);

#endif
