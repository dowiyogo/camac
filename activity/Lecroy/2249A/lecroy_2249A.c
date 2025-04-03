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


short Linit(char* ip){			/*function to initialize CAMAC*/
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

short LenCombo(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_enablecombo %d 0\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	//Enable combo
		printf("Error: couldn't enable combo %d\n",combo);
		return -1;
	}
printf("Combo %d Enabled\n",combo);
return 0;
}

short LdisCombo(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_enablecombo %d 1\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	//Disable combo
		printf("Error: couldn't disable combo %d\n",combo);
		return -1;
	}
	printf("Combo %d Disabled\n",combo);
return 0;
}

short Lcack(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_cack %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	//Combo acknowledgement
		printf("Error: couldn't acknowledge combo %d\n",combo);
		return -1;
	}	
printf("Combo %d acknowledgement\n",combo);
return 0;
}

short Lfcack(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_cack %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	//Combo acknowledgement
		printf("Error: couldn't acknowledge combo %d\n",combo);
		return -1;
	}	
return 0;
}

short LFastComboAck(short CID){
char res[33];
res[32]='\0';
	if ( CMDSR(CID,"nim_cack 1\r",res,32)<0 ) {	//Combo 1 acknowledgement
		printf("Error: couldn't acknowledge combo 1\n");
		return -1;
	}	
	if ( CMDSR(CID,"nim_cack 2\r",res,32)<0 ) {	//Combo 2 acknowledgement
		printf("Error: couldn't acknowledge combo 2\n");
		return -1;
	}	
return 0;
}

int Lcreset(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_getcev %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Get Combo Counter Value
	       printf("Error: couldn't get combo %d counter value\n",combo);
			return -1;
	}
int counts=atoi(res);
sprintf(command,"nim_resetcev %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Combo Reset Event Counter
	       printf("Error: couldn't reset combo %d counter\n",combo);
			return -1;
	}
	printf("Combo %d Event Counter Reset\n",combo);
return counts;
}

short Lcfreset(short CID,short combo){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_resetcev %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Combo Reset Event Counter
	       printf("Error: couldn't reset combo %d counter\n",combo);
			return -1;
	}
	printf("Combo %d Event Counter Reset\n",combo);
return 0;
}

int LCevent(short CID,short combo){
char res[33];
res[32]='\0';
char command[32];
sprintf(command,"nim_getcev %d\r",combo);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Get Combo Counter Value
	       printf("Error: couldn't get combo %d counter value\n",combo);
			return -1;
	}
int counts=atoi(res);
return counts;
}

short Lsetcount(short CID,short in,short pol,short on_off,short ext){
// sets input counters, in is input 1 or 3, pol is nim polarity, ext is enable or disable external reset
//on is on_off=1, off is 0
char res[33];
res[32]='\0';
char command[32];
sprintf(command,"nim_setievcnt %d %d %d %d\r",in,on_off,pol,ext);
if (on_off==1){
	if ( CMDSR(CID,command,res,32)<0 ) {	       //settings counter
	       printf("Error: couldn't set counter %d\n",in);
			return -1;
	}
	printf("Counter %d is ON with ",in);
	if (pol==0) printf("NIM Polarity HI to LOW (0)");
		else printf("NIM Polarity LOW to HI (1)");
	if (ext==1) printf(" and external reset enabled\n");
		else printf(" and external reset disabled\n");
	return 0;
	}
else {
	if ( CMDSR(CID,command,res,32)<0 ) {	       //turning off counter
	       printf("Error: couldn't turn off counter %d\n",in);
			return -1;
	}
	printf("Counter %d is OFF\n ",in);
	return 0;	
	}
}

short Lfsetcount(short CID,short in,short pol){
// sets input counters, in is input 1 or 3, pol is nim polarity
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_setievcnt %d 1 %d 0\r",in,pol);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //settings counter
	       printf("Error: couldn't set counter %d\n",in);
	return -1;
	}
return 0;
}

int LINcount(short CID,short in){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_geticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Get IN Counter Value
	       printf("Error: couldn't get in %d counter value\n",in);
			return -1;
	}
int ok,counts;
sscanf(res,"%d %d",&ok,&counts);
return counts;
}

int LINreset(short CID,short in){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_geticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Get IN Counter Value
	       printf("Error: couldn't get in %d counter value\n",in);
			return -1;
	}
int ok,counts;
sscanf(res,"%d %d",&ok,&counts);
sprintf(command,"nim_reseticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //IN Reset Event Counter
	       printf("Error: couldn't reset IN %d counter\n",in);
			return -1;
	}
	printf("IN %d Event Counter Reset\n",in);
return counts;
}

int LINRFreset(short CID,short in){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_geticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //Get IN Counter Value
	       printf("Error: couldn't get in %d counter value\n",in);
			return -1;
	}
int ok,counts;
sscanf(res,"%d %d",&ok,&counts);
sprintf(command,"nim_reseticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //IN Reset Event Counter
	       printf("Error: couldn't reset IN %d counter\n",in);
			return -1;
	}
return counts;
}

short LINfreset(short CID,short in){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_reseticnt %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //IN Reset Event Counter
	       printf("Error: couldn't reset IN %d counter\n",in);
			return -1;
	}
	printf("IN %d Event Counter Reset\n",in);
return 0;
}

short LINvalue(short CID,short in){
char res[33];
res[32]='\0';
char command [32];
sprintf(command,"nim_getins %d\r",in);
	if ( CMDSR(CID,command,res,32)<0 ) {	       //IN Reset Event Counter
	       printf("Error: couldn't get Input %d register value\n",in);
			return -1;
	}
	int ok,value;
	sscanf(res,"%d %d",&ok,&value);
	//printf("Input %d register value is %d\n",in,value);
	return value;
}

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

short Lclose(short crate_id){
	
	if ( (CRCLOSE(crate_id) ) < 0){		/*Close connection to CAMAC*/
		printf("Error: can't close the specific device\n");
		return -1;
	}
	
	return 1;
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
	op.F=2;				/*Function F(2) of 2249A to Read Registers erasing them*/
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
	return 0;		/*deletes the overflow bit to see the data correctly*/
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

double Lgen(short CID,int width,double period,short polar,short on_off){
/*This function SETS special output of CAMAC Controller, OUTPUT 1 is Pulse Generator*/
/*If on_off=1 generator will be ON, if on_off=0, generator will be off*/
/*POLAR 0 means Reverse Polarity (HI to LOW), POLAR 1 means Direct Polarity (LOW to HI)*/
/*width MUST be set in multiples of 88 nS up to 616 ns*/
char res [33];
res[32]='\0';
int mult_width;
int mult_per;
if (on_off==1){
	mult_width = width/88;
	int mod_width = width%88;
		if (width>88 && width<616 && mod_width != 0){
			printf("Warning! you should set pulse time of width in multiples of 88 nS\n");
			printf("Pulse Width Setting: %d[nS]\tUser Seting:%d[nS]\tError is:%d[nS]\n",mult_width*88,width,mod_width);}
		else if (width>=88 && width<=616 && mod_width == 0){
			printf("Pulse Width Setting: %d[nS]\n",width);}
		else if (width>616){mult_width=616/88;
			printf("Warning! maximum value for width is 616 nS\nPulse Width Setting: 616[nS]\tUser Seting:%d[nS]\tError is:%d[nS]\n",width,width-616);}
		else {mult_width=1;
			printf("Warning! minimum value for width is 88 nS\nPulse Width Setting: 88[nS]\tUser Seting:%d[nS]\tError is:%d[nS]\n",width,88-width);}
/*Period MUST be set in steps of 93.1uS, from 186.2uS up to 95325.2*/
	mult_per = (int)floor(period/93.1);
	double mod_per = fmod(period,93.1);
		if (period>186.2 && period<95325.2 && mod_per != 0.0){
			printf("Warning! you should set pulse period in multiples of 93.1 uS but more than 186.2 uS\n");
			printf("Pulse Period Setting: %5.2f[uS]\tUser Seting:%5.2f[uS]\tError is:%5.2f[uS]\n",((double)mult_per)*93.1,period,mod_per);}
		else if (period>=186.2 && period<=95325.2 && mod_per == 0.0){
			printf("Pulse Period Setting: %5.2f[uS]\n",period);}
		else if (period<186.2){mult_per=2;
			printf("Warning! Minimum Period Value is 186.2[uS]\n");
			printf("Pulse Period Setting: 186.2[uS]\tUser Seting:%5.2f[uS]\tError is:%5.2f[uS]\n",period,186.2-period);}
		else {mult_per=(int)(95325.2/93.1);
			printf("Warning! Maximum Period Value is 95325.2[uS]\n");
			printf("Pulse Period Setting: 95325.2[uS]\tUser Seting:%5.2f[uS]\tError is:%5.2f[uS]\n",period,period-95325.2);}
	if (polar==0) printf("NIM Polarity is HI to LOW (0)\n");
	else printf("NIM Polarity is LOW to HI (1)\n");
	char command[32];
/*nim_setpulse sets pulse, first number is period second number is width third number is NIM polarization*/
	sprintf(command,"nim_setpulse %d %d %d\r",mult_per-1,mult_width,polar);
		if ( (CMDSR(CID,command,res,32))<0 ){	
			printf("couldn't set nim pulse\n");				
			return -1;						
		}								
	return (((double)mult_per)*93.1);
	}
else	{printf("Pulse generator will be turn off, previous settings in generator are:\n");
	if ( (CMDSR(CID,"nim_getpulse\r",res,32))<0 ){		/*getting previous settings*/
			printf("couldn't get previous settings\n");
			return -1;
		}
	int ok,pol;
	sscanf(res,"%d %d %d %d",&ok,&mult_per,&mult_width,&pol);
	if (ok==0 && mult_per==0 && mult_width==0 && pol==0)
		{printf("Generator was already off\n");
		return 0;
		}
	else printf("Pulse Period: %5.2f[uS]\tPulse Width: %d[nS]\tPolarity:%d\n",((double)mult_per+1.)*93.1,mult_width*88,pol);
	if ( (CMDSR(CID,"nim_pulseoff\r",res,32))<0 ){		/*turn off pulse*/
			printf("couldn't turn off pulse\n");
			return -1;
		}
	return 0;
	}
}

double LgenGetFrec(short CID){
char res [33];
res[32]='\0';
int mult_width;
int mult_per;
	if ( (CMDSR(CID,"nim_getpulse\r",res,32))<0 ){		/*getting previous settings*/
			printf("couldn't get previous settings\n");
			return -1;
		}
	int ok,pol;
	sscanf(res,"%d %d %d %d",&ok,&mult_per,&mult_width,&pol);
	if (ok==0 && mult_per==0 && mult_width==0 && pol==0)
		{printf("Generator was OFF\n");
		return 0;
		}
	else printf("Frecuency: %6.3f[Hz]\tPulse Width: %d[nS]\tWork percent: %5.3f%\tPolarity:%d\n",1/(((double)mult_per+1.)*93.1*0.000001),mult_width*88,mult_width*88/(((double)mult_per)*93.1),pol);
	return (1./((double)mult_per+1.)*93.1);
}

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

short disableAcknowledge(short CID){
	if ( CBINR(CID,0xA0)<0){
		printf("Error: couldn't disable acknowledge\n");
		return -1;
	}
printf("WARNING! Binary command acknowledge disabled, communication speed improved\n");
return 0;
}

short enableAcknowledge(short CID){
	if ( CBINR(CID,0)<0){
		printf("Error: couldn't enable acknowledge\n");
		return -1;
	}
printf("Binary command acknowledge enabled\n");
return 0;
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

/// THIS LISR WORKS WITH LAM
/*
void *Lisr(short CID, short irq_type, unsigned int irq_data){
	switch (irq_type){
		case LAM_INT:
			{
			if (irq_data==QDCx){
			Lreadall(CID);
			}	
			LACK(CID);
			break;
			}
	}
	pthread_exit(irq_data);
}
*/
/*
/// THIS LISR WORKS WITH COMBO
void *Lisr(short CID, short irq_type, unsigned int irq_data){
	switch (irq_type){
		case COMBO_INT:
			{
			nanosleep(60000,NULL);
				Lreadall(CID);
//				LreadBLOCK(CID);
			break;
			}
	}
	pthread_exit(irq_data);
}
*/
void *Lisr(short CID, short irq_type, unsigned int irq_data){
//	short dato;
	char res[33];
	res[32]='\0';
	switch (irq_type){	
		case COMBO_INT:
			CMDSR(CID,"nim_cack 1\r",res,32);
			break;
	}
//	pthread_exit(dato);
	pthread_exit(&irq_data);
}
