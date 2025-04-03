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
#include "lecroy_8501.h"

short Linit_clock(char* ip){			/*function to initialize CAMAC*/
	short crate_id;			
//	char res[33];
//	res[32]='\0';
	int data_block=16;	//default is 16
	
	frequencies[0].freq=20;
	frequencies[0].reg=0;
	frequencies[0].mult=0;
	frequencies[1].freq=50;
	frequencies[1].reg=1;
	frequencies[1].mult=0;
	frequencies[2].freq=100;
	frequencies[2].reg=2;
	frequencies[2].mult=0;
	frequencies[3].freq=200;
	frequencies[3].reg=3;
	frequencies[3].mult=0;
	frequencies[4].freq=500;
	frequencies[4].reg=4;
	frequencies[4].mult=0;
	frequencies[5].freq=1000;
	frequencies[5].reg=5;
	frequencies[5].mult=0;
	frequencies[6].freq=2000;
	frequencies[6].reg=6;
	frequencies[6].mult=0;
	frequencies[7].freq=5000;
	frequencies[7].reg=7;
	frequencies[7].mult=0;
	frequencies[8].freq=10000;
	frequencies[8].reg=8;
	frequencies[8].mult=0;
	frequencies[9].freq=20000;
	frequencies[9].reg=9;
	frequencies[9].mult=0;
	frequencies[10].freq=50000;
	frequencies[10].reg=10;
	frequencies[10].mult=0;
	frequencies[11].freq=100000;
	frequencies[11].reg=11;
	frequencies[11].mult=0;	
	frequencies[12].freq=200000;
	frequencies[12].reg=12;
	frequencies[12].mult=0;
	frequencies[13].freq=500000;
	frequencies[13].reg=13;
	frequencies[13].mult=0;
	frequencies[14].freq=1000000;
	frequencies[14].reg=14;
	frequencies[14].mult=0;
	frequencies[15].freq=2000000;
	frequencies[15].reg=15;
	frequencies[15].mult=0;
	frequencies[16].freq=5000000;
	frequencies[16].reg=13;
	frequencies[16].mult=1;
	frequencies[17].freq=10000000;
	frequencies[17].reg=14;
	frequencies[17].mult=1;
	frequencies[18].freq=20000000;
	frequencies[18].reg=15;
	frequencies[18].mult=1;

	if ( ( crate_id=CROPEN(ip) ) < 0){		/*CROPEN is Library Function to open connection to CAMAC, negative output is error*/
		printf("Error: couldn't find the specific device at ip: %s\n",ip);
		return -1;
	}
	
	if ( CCCZ(crate_id)<0 ) {	/*Initialize dataway, negative output is error*/
		printf("Error: couldn't initialize DATA WAY\n");
		return -1;
	}

	if ( CCCC(crate_id)<0 ) {	/*Generate Crate Clear, negative output is error*/
		printf("Error: couldn't send clear signal\n");
		return -1;
       	}

	printf("DATA WAY initialized - CAMAC Clear Executed\n");
	
	// --- IT IS NOT NECESARY TO CHANGE DATA BLOCK SIZE
	if ( BLKBUFFS(crate_id,data_block)<0 ){
		printf("Error: Couldn't set data block size to %d words\n",data_block);}
	else {
		printf("Data block size set to %d words\n",data_block);}
	
	//-----------------This Lines are used to work with LAM only
	//LdisCombo(crate_id,1);//Disable combo 1
	//LdisCombo(crate_id,2);//Disable combo 2
	//---------------- This Lines MUST be enabled to use combo
	LenCombo(crate_id,1);//Enable combo 1
	LdisCombo(crate_id,2);//Disable combo 2
	Lcfreset(crate_id,1);//Combo 1 Reset Event Counter
	Lcack(crate_id,1);//Combo 1 acknowledgement
	return crate_id;		/* if successfull, CAMAC assigned ID*/
}

double LSetClock(short CID, short card, int frequency){
// This clock only allows the following frequencies: 20,50,100,200,500,1000,2000,5000,10000,20000,50000,100000,200000,500000,1000000,2000000,5000000,10000000,20000000
	short res;
	CRATE_OP op;
	op.F=16;
	op.N=card;	
	int i;
	for (i=0;i<19;i++)
	{	if(frequencies[i].freq==frequency)
			{
			op.A=2;
			op.DATA=1;
			res=CSSA(CID,&op);	
				if (res<0){			
					printf("Error: Couldn't reset CLOCK frequency multiplier\n");
					return -1;
				}
			op.A=2;
			op.DATA=frequencies[i].mult;
			res=CSSA(CID,&op);	
				if (res<0){			
					printf("Error: Couldn't set CLOCK frequency multiplier\n");
					return -1;
				}
			op.A=3;
			op.DATA=frequencies[i].reg;			
			res=CSSA(CID,&op);	
				if (res<0){			
					printf("Error: Couldn't set CLOCK frequency register\n");
					return -1;
				}
			printf("CAMAC Clock Frequency set to: %d\n",frequency);
			return (1./(double)frequency);
			}
	}
	printf("ERROR: Frequency %d not allowed\n",frequency);
	return -1;
}

