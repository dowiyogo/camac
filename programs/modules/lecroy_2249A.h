#ifndef LECROY_2249A
#define LECROY_2249A

#include <stdio.h>
#include "crate_lib.h"
#define QDCn1	20 //esta en el slot 10
#define QDCn2	22 //esta en el slot 11
#define address 1 // donde leo la direccion

extern unsigned int datos[24];

short Lread(short CID, short card, short add);
float Lreadf(short CID, short card, short add);
short LenableLAM(short CID, short card);
short LdisableLAM(short CID, short card);
short Lreadall(short CID);
short LreadBLOCK(short CID);

#endif


