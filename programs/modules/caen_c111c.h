#ifndef CAEN_C111C
#define CAEN_C111C

#include <stdio.h>
#include "crate_lib.h"
#define DEFAULT_EVENTS 10000

void *Lisr(short CID, short irq_type, unsigned int irq_data);
double Lgen(short CID,int width,double period,short polar,short on_off);
double LgenGetFrec(short CID);
short LenCombo(short CID,short combo);
short LdisCombo(short CID,short combo);
short Lcack(short CID,short combo);
int Lcreset(short CID,short combo);
short Lcfreset(short CID,short combo);
int LCevent(short CID,short combo);
short Lsetcount(short CID,short in,short pol,short on_off,short ext);
short Lfsetcount(short CID,short in,short pol);
int LINcount(short CID,short in);
int LINreset(short CID,short in);
short LINfreset(short CID,short in);
int LINRFreset(short CID,short in);
short Lfcack(short CID,short combo);
short LFastComboAck(short CID);
short disableAcknowledge(short CID);
short enableAcknowledge(short CID);
short LINvalue(short CID,short in);
short Linit(char* ip);
short Lclose(short crate_id);

#endif
