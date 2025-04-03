#include "crate_lib.h"
#include "lecroy_2249A.h" 
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>

int events;
short CID, res;
char cmd_res[33];
int counter=1;
double tiempo;

int main(int argc,char *argv[]){

CRATE_INFO crInfo;
cmd_res[32]='\0';
	/*
	else if(argc>2){
		sprintf(filename,"%s",argv[2]);	
		events=atoi(argv[1]);
		wait_time = 0;
	}
	else if(argc>1){
		sprintf(filename,"datos.txt");
		chn=0;
		events=atoi(argv[1]);
		wait_time = 0;
	}
	else {
		sprintf(filename,"datos.txt");
		events=DEFAULT_EVENTS;
		wait_time = 0;
	}
*/
	//if ( (CID=Linit("192.168.1.102"))<0 ){		/*Linit needs CAMAC IP. CID is return from function Linit in Lecroy 2249A driver, output negative is error*/
	if ( (CID=CROPEN("192.168.1.102"))<0 ){		/*Linit needs CAMAC IP. CID is return from function Linit in Lecroy 2249A driver, output negative is error*/
			printf("couldn't initialize device at ip: 192.168.1.102\n");
			return 0;
	}
	
	int i=0,j=0;
	CRATE_OP crate_op;
	crate_op.N=8;
	printf("sub_addr\tvalue\n");
	for(j=0;j<32;j++)

	{
		//printf("\n\ntest in address %d\n",j);
		crate_op.A=j;
		crate_op.F=0;
		crate_op.DATA=0;
		res=CFSA(CID,&crate_op);
		if (res<0)
			{
				printf("Error en el comando numero: %d\n",res);			
			}			
		if (crate_op.X==1)
		{
			printf("%d\t%x\n",j,crate_op.DATA);
		}
	}



	if ( (CRCLOSE(CID))<0 ){
			printf("couldn't close connection\n");
			return 1;	}	
	return 0;
}
