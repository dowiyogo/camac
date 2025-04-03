#include "crate_lib.h"
#include "lecroy_2249A.h" 

int main(int argc,char *argv[]){
FILE *file1;	
short CID, res;
char cmd_res[32];

	
	if (argc>1){		/*There are not arguments for input in this executable file*/
		printf("You can't insert parameters\n"); 
		return 0;
	}

	if( !(file1=fopen("datos.txt","wr")) ){	/*If file can't be opened for writting, fopen returns NULL, that's why if is true if fopen is NULL*/
			printf("couldn't open file: datos.txt\n");
			return 0;
	}
	
	if ( (CID=Linit("192.168.1.101"))<0 ){		/*Linit needs CAMAC IP. CID is return from function Linit in Lecroy 2249A driver, output negative is error*/
			printf("couldn't initialize device at ip: 192.168.1.101\n");
			return 0;
	}
	printf("CID: %d\n",CID);		/*if Linit is successful, it returns CAMAC assigned ID*/
	
	
	int i;
	int data;
	
	for (i=0;i<100;i++){
	res=CRIRQ(CID, data=Lisr);
	fprintf(file1,"%d\n",data);		/*writes data into "datoson" file (this means VETO on)*/
	system("sleep 0.1");
	}	
	
	/*the remaining lines close the files*/	

	if( fclose(file1)){	/*fclose returns EOF if it fails. In C every value BUT 0 are TRUE. Closing datosoff*/
		printf("couldn't close file: datos.txt\n");
		return 0;
	}
	
	/*this lines will close connection with CAMAC, it needs CID*/
	if ( (Lclose(CID))<0 ){
			printf("couldn't close connection\n");
			return 0;
	}	
		
		
	


	return 1;
} 
