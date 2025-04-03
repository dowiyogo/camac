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
	short crate_id; // Store crate id for comunication.
	
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
	int T=1,w=1,p=0;
	
	printf("insert integer, to be multipled by 93.1us, Period:  ");
	scanf("%d",&T);
	printf("Period:= %.1f[us]\n",93.1*T);
	
	printf("insert integer, to be multipled by 88ns, width:  ");
	scanf("%d",&w);
	printf("width:= %d[ns]\n",88*w);
	
	printf("insert integer, 0 -> direct polarity, 1 -> reverse: ");
	scanf("%d",&p);
	printf("polarity:= %d\n",p);

	sprintf(cmd,"nim_setpulse %d %d %d\r",T,w,p);
	short res = CMDSR(crate_id,cmd,cmd_res,32); /* Turn on the pulse generator in 
																Nim output 0.
																comand description:
																nim_setpulse <period> <width> <polarity>
																<period>	integer multiple of 93.1us, limit [186.2us, 95325.2us]
																<width>		integer multiple of 88ns, limit [88us,616ns]		
																<polarity>	0 direct, 1 reverse.*/
	
	if (res<0){
		printf("Error: CMDSR command not executed\n");
		return -1;
	}
	else{
	short res = CMDSR(crate_id,"nim_setouts 4 0\r",cmd_res,32);
	printf("T: %.1f,w: %d, p: %.1f \n",T,w,p);
	printf("Pulse generator turned on in Nim_0 output,\n\
	 period: %.1f[us]\twidth: %d[ns]\n\
	 press any key to stop\n",T*93.1,w*88);
	
	
	scanf("%d",&p);
	res = CMDSR(crate_id,"nim_pulseoff\r",cmd_res,32); /* Turn off pulse generator.*/
	if (res<0){
		printf("Error: CMDSR command not executed\n");
		return -1;
	}
	res = CRCLOSE(crate_id); 
	if (res<0){
		printf("Error: CRCLOSE command not executed\n");
		return -1;
	}
	}
	
	return 1;
}
