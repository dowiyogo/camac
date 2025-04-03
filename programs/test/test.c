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

#endif
#include "crate_lib.h"


int main(int argc,char *argv[]){
	short crate_id, res; // Store crate id for comunication.
	
	if (argc<2){
		printf("Hello, you can specify the ip address as argument\n\"pulse_test <ip_adress>\"\n\
				ip address used: 192.168.0.98");
		crate_id=CROPEN("192.168.0.98"); //Open connection.
	}
	
	else{
		crate_id=CROPEN(argv[1]);
		printf("connection succefull\n");
	}
	
	if (crate_id<0){
		printf("Error: can't find the specific device\n");
		return -1;
	}
	
	char cmd_res[32],cmd[32];
	short add=0,I=5;
	
	printf("insert address to be tested: ");
	scanf("%d",&add);
	putchar('\n');		
	
	res = CCCZ(crate_id);/*Initialize dataway*/
	
	if (res<0){
		printf("Error: CFSA command not executed\n");
		return -1;
	}
	
	res = CCCI(crate_id,1);/* set inhibit for test*/
	
	if (res<0){
		printf("Error: CFSA command not executed\n");
		return -1;
	}

	printf("Inhibit: %d\n",I);
	
		
	//sprintf(cmd,"CFSA 25 6 %d 1",add);
	CRATE_OP op;
	op.F=25;
	op.N=6;
	op.A=add;
	CFSA(crate_id,&op);
	
	printf("1 ok\n");
	//res = CMDSR(crate_id,cmd,cmd_res,32);

	if (res<0){
		printf("Error: CFSA command not executed\n");
		return -1;
	}
	
	//sprintf(cmd,"CFSA 0 6 %d 1",add);

	//res = CMDSR(crate_id,cmd,cmd_res,32);
	
	op.F=0;
	op.N=6;
	op.A=add;
	CFSA(crate_id,&op);
	
	printf("2 ok\n");
	
	if (res<0){
		printf("Error: CFSA command not executed\n");
		return -1;
	}
	
	printf("Performing test on channel %d...\ninsert any key to end the test\n",add);

	printf("responce %d\n",(op.DATA&1023)*256/1024);
	getchar();
	
	res = CRCLOSE(crate_id); 
	if (res<0){
		printf("Error: CRCLOSE command not executed\n");
		return -1;
	}

	
	return 1;
}
