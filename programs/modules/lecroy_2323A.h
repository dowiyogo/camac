#ifndef LECROY_2323A
#define LECROY_2323A

#include <stdio.h>
#include "crate_lib.h"

double LreadGate(short CID, short card, short add);
int LsetGateFORMYCASE(short CID, short card, short add);
short LGateStart(short CID, short card, short add);
short LGateStop(short CID, short card, short add);

#endif
