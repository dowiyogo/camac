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
#include "lecroy_2551.h"


int LreaderaseScaler(short CID, short card, short add){		/*This function is specifically designed for this CAMAC module*/
	short res;
	int dato;
	CRATE_OP op;			/*CRATE_OP is a STRUCTURE, so it assigns op the type of STRUCTURE*/
	op.F=2;				
	op.N=card;			
	op.A=add;			/*Address of Register. Correspondes to ANALOG Input NUMBER in MODULE*/
	res=CFSA(CID,&op);			/*execute a CAMAC command with 24bit, uses Crate ID and fills and/or uses structure. In this case function is F(0)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't read module count\n");
		return -1;
	}
	dato=op.DATA;			/*gets data response from F(0) CAMAC command, this is register data. It is saved inside op STRUCTURE*/
	op.A=11;			/* A(11) clears and Module */
	op.F=2;				/*Clears MODULE and LAM (look at me)*/
	res=CFSA(CID,&op);			/*Executes CAMAC command F(2)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't clear module and LAM\n");
		return -1;
	}
	return dato;			/*output of program function Lread is dato with is DATA from register*/
}

int LreadScaler(short CID, short card, short add){		/*This function is specifically designed for this CAMAC module*/
	
	short res;
	int dato;
	CRATE_OP op;			/*CRATE_OP is a STRUCTURE, so it assigns op the type of STRUCTURE*/
	op.F=0;				
	op.N=card;			
	op.A=add;			/*Address of Register. Correspondes to ANALOG Input NUMBER in MODULE*/
	res=CFSA(CID,&op);			/*execute a CAMAC command with 24bit, uses Crate ID and fills and/or uses structure. In this case function is F(0)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't read module count\n");
		return -1;
	}
	dato=op.DATA;			/*gets data response from F(0) CAMAC command, this is register data. It is saved inside op STRUCTURE*/
	return dato;			/*output of program function Lread is dato with is DATA from register*/
}


short LclearScaler(short CID, short card){
	short res;
	CRATE_OP op;			/*CRATE_OP is a STRUCTURE, so it assigns op the type of STRUCTURE*/
	op.F=9;			
	op.A=11;				
	op.N=card;			
	res=CFSA(CID,&op);			/*execute a CAMAC command with 24bit, uses Crate ID and fills and/or uses structure. In this case function is F(0)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't Clear Scaler\n");
		return -1;
	}
	printf("Scaler Cleared\n");
	return 0;
}

