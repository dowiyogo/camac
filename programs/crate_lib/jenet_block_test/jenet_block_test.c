/*
 =======================================================
 Jenet Block Transfer Test
 =======================================================
 Author      : Mauro Vetuschi
 Company     : ZP Engineering srl
 Description : 
 Date        : 22/07/2007
 Version     : 2.00
 Revision    :     
 =======================================================
*/

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

#define GATE_NUM		1

int main(int argc, char *argv[])
{
	short crate_id;
	int i;
	int resp, block_data_size, total_data_size;
	unsigned int blk_transf_buf[8192];
	char cmd[64], response[64];
	BLK_TRANSF_INFO blk_info;
    CRATE_OP op;

	memset(blk_transf_buf, 0, 8192);
  
	printf("\nBlock Transfer Test on C1205 16-channnel QDC\n");
	printf("--------------------------------------------\n\n");
	
/*
	=================================================
	Open connection with a Camac controller
	=================================================
*/
	printf("Initializing communication parameters...\n");

	if (argc >= 2)
		crate_id = CROPEN(argv[1]);
	else
		crate_id = CROPEN("192.168.0.98");

	if (crate_id < 0) {
		printf("ERROR: Unable to init connection with JENET Controller: %d\n", crate_id);
		return 0;
	}
	
/*
	=================================================
	Init C1205 module to output (N = 20)
    =================================================
*/

	printf("Init C1205 module to output\n");

	resp = CCCC(crate_id);
	if (resp != CRATE_OK) {
		printf("ERROR: Negative response from socket server\n");
		return 0;
	}

	// Clear everything
	op.F = 9; 
	op.N = 20;
	op.A = 0;
	op.DATA = 0;
	resp = CFSA(crate_id, &op);
	if (resp != CRATE_OK) {
		printf("ERROR: Negative response from socket server\n");
		return 0;
	}

	// Enable Gate
	op.F = 26;
	op.N = 20;
	op.A = 1;
	op.DATA = 0;
	resp = CFSA(crate_id, &op);
	if (resp != CRATE_OK) {
		printf("ERROR: Negative response from socket server\n");
		return 0;
	}
		
	// Write Control reg - Enable Block mode 
	op.F = 16;
	op.N = 20;
	op.A = 1;
	op.DATA = 0x8080; // Block mode - id=0x80
	resp = CFSA(crate_id, &op);
	if (resp != CRATE_OK) {
		printf("ERROR: Negative response from socket server\n");
		return 0;
	}


	// Perform a n-gate events
	strcpy(cmd,"nim_setoutp 1 1\r");

	for (i = 0; i < GATE_NUM; i++) {
		resp = CMDSR(crate_id, cmd, response, 64);
		if (resp != CRATE_OK) {
			printf("ERROR: Negative response from socket server\n");
			return 0;
		}
	}

/*
	=================================================
	Invoke Block transfer Q-stop mode 
	(read operation on C1205 address 0)
	=================================================
*/

	block_data_size = 16;
	total_data_size = (GATE_NUM * 51);

	printf("Start block transfer read\n");

	blk_info.opcode = OP_BLKFS;
	blk_info.F = 0;
	blk_info.N = 20;
	blk_info.A = 0;
	blk_info.totsize = total_data_size;
	blk_info.blksize = block_data_size;
	blk_info.timeout = 0;
	if (argc >= 3) {
		blk_info.ascii_transf = atoi(argv[2]);
	}
	else {
		blk_info.ascii_transf = 0;
	}

	resp = BLKTRANSF(crate_id, &blk_info, blk_transf_buf);
    if (resp != CRATE_OK) {
		printf("ERROR: Negative response from socket server\n");
		return 0;
	}

	printf("Total data read: %d\n", blk_info.totsize);
	for (i = 0; i < blk_info.totsize; i++) {
		// Show received buffer
		if ((i > 0) && ((i % 10) == 0)) {
			printf("\n");
		}
		printf("%06X ", blk_transf_buf[i]);
	}
	printf("\nEnd of block transfer read\n");

	resp = CCCC(crate_id);
	if (resp != CRATE_OK) {
		printf("ERROR: Negative response from socket server\n");
		return 0;
	}
	
	// Clear everything
	op.F = 9;
	op.N = 20;
	op.A = 0;
	op.DATA = 0;
	resp = CFSA(crate_id, &op);
	if (resp != CRATE_OK) {
		printf("ERROR: Negative response from socket server\n");
		return 0;
	}

	printf("\nBye bye\n");
	return 0;
}

