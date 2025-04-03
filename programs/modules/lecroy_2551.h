#ifndef LECROY_2551
#define LECROY_2551

#include <stdio.h>
#include "crate_lib.h"

//to be used by Lecroy 2551 Scaler
short LclearScaler(short CID, short card);
int LreadScaler(short CID, short card, short add);
int LreaderaseScaler(short CID, short card, short add);

#endif
