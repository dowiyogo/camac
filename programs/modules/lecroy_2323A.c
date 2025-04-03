#ifdef WIN32

#include <stdio.h>

#else
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
#include <netdb.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <math.h>

#endif
#include "crate_lib.h"
#include "lecroy_2323A.h"

double LreadGate(short CID, short card, short add){
	short res;
	int word;
	double gate;
	CRATE_OP op;			
	op.F=1;			
	op.A=add;		
	op.N=card;		
	res=CFSA(CID,&op);	
	if (res<0){		
		printf("Error: Couldn't read Gate %d\n",add);
		return -1;
	}
	word=op.DATA;
	int M = word & 1023;
	int C = word & 7168;
	int L = word & 8192;
	int D = word & 49152;
	C=C>>10;
	L=L>>13;
	D=D>>14;
  	if(L==1) {printf("Gate %d time is defined by START-STOP Latch\n",add);
		gate=-1;}
	else {
		gate=((double)M)*pow(10,(double)C);
		printf("Gate %d time is: %10.2f [nS]\n",gate,add);
		}
	switch(D){
		case 0:
			printf("Delay pulse width is 10 [nS]\n");
			break;
		case 1:
			printf("Delay pulse width is 30 [nS]\n");
			break;
		case 2:
			printf("Delay pulse width is 100 [nS]\n");
			break;
		default:
			printf("Delay pulse width is 300 [nS]\n");
			break;
		}
	return gate;
}

int LsetGateFORMYCASE(short CID, short card, short add){
	short res;
	int word;
	double gate;
	CRATE_OP op;			
	op.F=17;			
	op.A=add;		
	op.N=card;		
	int M = 100;
	int C = 0;
  	int L = 0;
  	int D = 2;
	C=C<<10;
	L=L<<13;
	D=D<<14;
	word=M|C|L|D;
	op.DATA=word;
	res=CFSA(CID,&op);	
	if (res<0){		
		printf("Error: Couldn't set Gate\n");
		return -1;
	}
	return 0;
}

short LGateStart(short CID, short card, short add){
	short res;
	CRATE_OP op;			
	op.F=26;			
	op.A=add;				
	op.N=card;			
	res=CFSA(CID,&op);		
	if (res<0){			
		printf("Error: Couldn't start gate\n");
		return -1;
	}
	printf("Gate %d started\n",add);
	return 0;
}

short LGateStop(short CID, short card, short add){
	short res;
	CRATE_OP op;			
	op.F=9;			
	op.A=add;				
	op.N=card;			
	res=CFSA(CID,&op);		
	if (res<0){			
		printf("Error: Couldn't stop gate\n");
		return -1;
	}
	printf("Gate %d stopped\n",add);
	return 0;
}

