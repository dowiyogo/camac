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
#include "OR_1320.h"

short LEnableNimOutPulse(short CID, short card){
	short res;
	CRATE_OP op;			
	op.F=26;			
	op.A=0;				
	op.N=card;			
	res=CSSA(CID,&op);		
	if (res<0){			
		printf("Error: Couldn't enable Nim Out Reg Pulse Mode\n");
		return -1;
	}
	printf("Nim Out Reg Pulse Mode Enabled in card %d\n",card);
	return 0;
}

short LSetNimOutPulse(short CID, short card,int word){
	short res;
	CRATE_OP op;			
	op.F=18;			
	op.A=0;				
	op.N=card;
	op.DATA=word;			
	res=CSSA(CID,&op);		
	if (res<0){			
		printf("Error: Couldn't Set Nim Out Reg\n");
		return -1;
	}
	printf("Nim Out Reg Set in card %d\n",card);
	return 0;
}

short LWordNimOut(short CID, short card, int word){
	short res;
	CRATE_OP op;			
	op.F=16;			
	op.A=0;				
	op.N=card;
	op.DATA=word;			
	res=CSSA(CID,&op);		
	if (res<0){			
		printf("Error: Couldn't send Word to Register\n");
		return -1;
	}
	printf("Nim Out Word sent to Slot %d\n",card);
	return 0;
}

void FastLWordNimOut(short CID, short card, int word){
	short res;
	CRATE_OP op;			
	op.F=16;			
	op.A=0;				
	op.N=card;
	op.DATA=word;			
	res=CSSA(CID,&op);		
}
