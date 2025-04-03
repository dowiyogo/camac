#ifndef LECROY_2249A
#define LECROY_2249A

#include <stdio.h>
#include "crate_lib.h"
#define QDCn1	20 //esta en el slot 10
#define QDCn2	22 //esta en el slot 11
#define address 1 // donde leo la direccion
#define DEFAULT_EVENTS 10000

unsigned int datos[24];

typedef struct values{
int freq;
int reg;
int mult;
} camac_clock;

camac_clock frequencies[19];
// frequencies : 20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000,1000000,2000000,5000000,10000000,20000000

//to be used by Lecroy 2551 Scaler
short LclearScaler(short CID, short card);
int LreadScaler(short CID, short card, short add);
int LreaderaseScaler(short CID, short card, short add);
double LSetClock(short CID, short card, int frequency);

short LINvalue(short CID,short in);
short Lreadall(short CID);
short Linit(char* ip);
short Lclose(short crate_id);
short Lread(short CID, short card, short add);
float Lreadf(short CID, short card, short add);
short LenableLAM(short CID, short card);
short LdisableLAM(short CID, short card);
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

double  LreadGate(short CID, short card, short add);
int LsetGateFORMYCASE(short CID, short card, short add);
short LGateStart(short CID, short card, short add);
short LGateStop(short CID, short card, short add);

short LSetNimOutPulse(short CID, short card,int word);
short LEnableNimOutPulse(short CID, short card);
short LWordNimOut(short CID, short card, int word);
void FastLWordNimOut(short CID, short card, int word);
short LFastComboAck(short CID);

short disableAcknowledge(short CID);
short enableAcknowledge(short CID);

short LreadBLOCK(short CID);

#endif


