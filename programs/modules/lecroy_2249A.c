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
#include "lecroy_2249A.h"

unsigned int datos[24];

short LenableLAM(short CID, short card){
	short res;
	CRATE_OP op;			/*CRATE_OP is a STRUCTURE, so it assigns op the type of STRUCTURE*/
	op.F=26;			/*Function F(26) of 2249A to Enable LAM*/
	op.A=1;				/*Needs any subaddress*/
	op.N=card;			/*Number of 2249A SLOT*/
	res=CSSA(CID,&op);			/*execute a CAMAC command with 24bit, uses Crate ID and fills and/or uses structure. In this case function is F(0)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't enable LAM\n");
		return -1;
	}
	printf("LAM initialized on card %d\n",card);
	return 0;
}

short LdisableLAM(short CID, short card){
	short res;
	CRATE_OP op;			/*CRATE_OP is a STRUCTURE, so it assigns op the type of STRUCTURE*/
	op.F=24;			/*Function F(24) of 2249A to Disable LAM*/
	op.A=1;				/*Needs any subaddress*/
	op.N=card;			/*Number of 2249A SLOT*/
	res=CSSA(CID,&op);			/*execute a CAMAC command with 16bit, uses Crate ID and fills and/or uses structure. In this case function is F(0)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't disable LAM\n");
		return -1;
	}
	printf("LAM disabled on card %d\n",card);
	return 0;
}


short Lread(short CID, short card, short add){		/*This function is specifically designed for this CAMAC module*/
	
	short res;
	short dato;
	CRATE_OP op;			/*CRATE_OP is a STRUCTURE, so it assigns op the type of STRUCTURE*/
	op.F=2;				/*Function F(0) of 2249A to Read Registers erasing it and LAM in A(11)*/
	op.N=card;			/*Number of 2249A SLOT*/
	op.A=add;			/*Address of Register. Correspondes to ANALOG Input NUMBER in MODULE*/
	res=CSSA(CID,&op);			/*execute a CAMAC command with 24bit, uses Crate ID and fills and/or uses structure. In this case function is F(0)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't read module\n");
		return -1;
	}
	dato=op.DATA;			/*gets data response from F(0) CAMAC command, this is register data. It is saved inside op STRUCTURE*/
//	op.F=9;				/*Clears MODULE and LAM (look at me)*/
//	CSSA(CID,&op);			/*Executes CAMAC command F(9)*/
//	if (res<0){			/*Error responses are negative*/
//		printf("Error: CSSA command not executed\n");
//		return -1;
//	}
	op.A=11;			/* A(11) clears and Module */
	op.F=2;				/*Clears MODULE and LAM (look at me)*/
	res=CSSA(CID,&op);			/*Executes CAMAC command F(2)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't clear module and LAM\n");
		return -1;
	}
		
	return dato;			/*output of program function Lread is dato with is DATA from register*/
}

float Lreadf(short CID, short card, short add){		/*it is exactly the same function than before, BUT it shows data without overflow bit*/
	short res;
	int dato;
	CRATE_OP op;
	op.F=2;				/*Function F(2) of 2249A to Read Registers WITHOUT erasing it*/
	op.N=card;
	op.A=add;
	res=CSSA(CID,&op);
	if (res<0){
		printf("Error: Couldn't read module\n");
		return -1;
	}
	dato=op.DATA;
//	op.F=9;				/*Executes CAMAC command F(9) - clear module and LAM*/
//	CSSA(CID,&op);
//	if (res<0){
//		printf("Error: CFSA command not executed\n");
//		return -1;
//	}
	op.A=11;			/* A(11) clears and Module */
	op.F=2;				/*Clears MODULE and LAM (look at me)*/
	CFSA(CID,&op);			/*Executes CAMAC command F(2)*/
	if (res<0){			/*Error responses are negative*/
		printf("Error: Couldn't clear module and LAM\n");
		return -1;
	}
	return dato&(1023)*256/1024;		/*deletes the overflow bit to see the data correctly*/
	
}

short Lreadall(short CID){		
	short res;
	short i;
	CRATE_OP op;
	op.F=2;				//Function F(2) of 2249A to Read Registers erasing them
	op.N=QDCn1;
		
	for (i=0; i<12;i++)
	{		//para todos los canales
		op.A=i;
		res=CSSA(CID,&op);
		if (res<0){
			printf("Error: Couldn't read module\n");
			return -1;
		}
		datos[i]=op.DATA;
	}
	op.N=QDCn2;
	for (i=0; i<12;i++)
	{		//para todos los canales
		op.A=i;
		res=CSSA(CID,&op);
		if (res<0){
			printf("Error: Couldn't read module\n");
			return -1;
		}
		datos[i+12]=op.DATA;
	}
	return 0;		//deletes the overflow bit to see the data correctly
}

short LreadBLOCK(short CID){		
//	unsigned int blk_transf_buf[12];	
	BLK_TRANSF_INFO blk_info;	
	short res;
	int block_data_size=24;
	int total_data_size=block_data_size;
	blk_info.opcode=OP_BLKSA;
	blk_info.F=2;				
	blk_info.N=QDCn1;
	blk_info.A=0;	
	blk_info.timeout=0;
	blk_info.blksize=block_data_size;
	blk_info.totsize=total_data_size;
//	res=BLKTRANSF(CID,&blk_info,blk_transf_buf);
	res=BLKTRANSF(CID,&blk_info,datos);
		if (res<0){
			printf("Error: Couldn't read module\n");
			return -1;
		}
	return 0;		
}


