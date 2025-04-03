#include "crate_lib.h"
#include "lecroy_2249A.h" 
 
int main(int argc,char *argv[]){
FILE *file1, *file2;	
short CID, res;
char cmd_res[32];

	
	if (argc>1){		/*There are not arguments for input in this executable file*/
		printf("You can't insert parameters\n"); 
		return 0;
	}

	if( !(file1=fopen("datosoff.txt","wr")) ){	/*If file can't be opened for writting, fopen returns NULL, that's why if is true if fopen is NULL*/
			printf("couldn't open file: datosoff.txt\n");
			return 0;
	}
	if( !(file2=fopen("datoson.txt","wr")) ){	/*If file can't be opened for writting, fopen returns NULL, that's why if is true if fopen is NULL*/
			printf("couldn't open file: datoson.txt\n");
			return 0;
	}

		
	if ( (CID=Linit("192.168.1.101"))<0 ){		/*Linit needs CAMAC IP. CID is return from function Linit in Lecroy 2249A driver, output negative is error*/
			printf("couldn't initialize device at ip: 192.168.1.101\n");
			return 0;
	}
	printf("CID: %d\n",CID);		/*if Linit is successful, it returns CAMAC assigned ID*/
	
	
	int i;
	int data;
		
	if ( (CMDSR(CID,"nim_setouts 4 0\r",cmd_res,32))<0 ){		/*CMDSR sends ascii command to CAMAC, needs CID, command, response(cmd_res), size in bits*/
		printf("couldn't set nim pulse\n");			/*nim_setouts is special function to enable CAMAC controller OUTPUTS*/
		return 0;						/*nim_setouts <out> <val>, out is output number, val is NIM output type*/
	}								/*val NIM 0 is direct polarization, NIM 1 is reverse polarization*/

	for (i=0;i<5000;i++){						/*5000 pulses for measurement will be produced*/
		
										/*Now we will make a VETO OFF for NIM Discriminator (lecroy 821)*/
		if ( (CMDSR(CID,"nim_setpulse 110 1 0 \r",cmd_res,32))<0 ){	/*nim_setpulse sets pulse, first number is period in multiples of 93.1 uS*/
			printf("couldn't set nim pulse\n");			/*period from 186.2 to 95325.2 uS*/
			return 0;						/*second number is width, multiples of 88 nS, up to 616 nS*/	
		}								/*third number is NIM polarization*/
		system("sleep 0.01");	/*system call for 1ms pause*/
				
		if ( (CMDSR(CID,"nim_pulseoff\r",cmd_res,32))<0 ){		/*turn off nim pulse*/
			printf("couldn't set nim pulse\n");
			return 0;
		}
		data=Lread(CID,6,11);	/*Lecroy 2249A function to read data, needs slot number for card and Analog Input address, returns data of register*/
		fprintf(file1,"%d\n",data); /*writes data into "datosoff" file (this means VETO off)*/
			
			
	}
								/*This lines will make a VETO OFF for NIM Discriminator*/ 
	if ( (CMDSR(CID,"nim_setouts 4 1\r",cmd_res,32))<0 ){	/*Same CMDSR to output 4 of Crate controller, but now pulse is in reverse polarization*/
		printf("couldn't set nim pulse\n");		
		return 0;
	}		
	
	for (i=0;i<5000;i++){						/*5000 pulses for measurement will be produced*/
									
										
		if ( (CMDSR(CID,"nim_setpulse 110 1 0 \r",cmd_res,32))<0 ){	/*nim_setpulse is set*/
			printf("couldn't set nim pulse\n");
			return 0;
		}		
		
		system("sleep 0.01");
			
		if ( (CMDSR(CID,"nim_pulseoff\r",cmd_res,32))<0 ){		/*turn off pulse*/
			printf("couldn't set nim pulse\n");
			return 0;
		}
		data=Lread(CID,6,11);
		fprintf(file2,"%d\n",data);		/*writes data into "datoson" file (this means VETO on)*/
			
			
	}
	
	
	/*the remaining lines close the files*/		
	
	if( fclose(file1)){	/*fclose returns EOF if it fails. In C every value BUT 0 are TRUE. Closing datosoff*/
		printf("couldn't close file: datos.txt\n");
		return 0;
	}
	
	if( fclose(file2)){	/*closing datoson*/
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
