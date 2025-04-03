/*
 =======================================================
 Camac Block Transfer Test
 =======================================================
 Author      : Mauro Vetuschi
 Company     : ZP Engineering srl
 Description : 
 Date        : 15/09/2004
 Version     : 1.00
 Revision    :     
 =======================================================
*/

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

#include "crate_lib.h"

#define BLOCK_SIZE 10

int main(int argc, char *argv[])
{
	short crate_id;
	int i, j;
	int resp, block_data_size, total_data_size;
	unsigned int blk_transf_buf[32800];
	char cmd[64], response[64];
	BLK_TRANSF_INFO blk_info;
    CRATE_OP op;

	printf("\nBlock Transfer Test on C219\n");
	printf("---------------------------\n\n");
	
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
	
	resp = CCCC(crate_id);
	if (resp != CRATE_OK) {
		printf("ERROR: Negative response from socket server\n");
		return 0;
	}

//	=================================================
//	Init C219 module to output
//  =================================================

	printf("\n-------------------------------------------\nInit C219 module to output\n");

    op.F = 17; 
	op.N = 12;
	for (i = 0; i < 16; i++) {
		op.A = i;
		op.DATA = 6;
		resp = CSSA(crate_id, &op);
		if (resp != 0) {
			printf("ERROR: Negative response from socket server\n");
			return 0;
		}
	}

//	=================================================
//	Invoke Block transfer Q-stop mode 
//	(write operation F=16 on C219 Module address 0)
//	=================================================

	block_data_size = BLOCK_SIZE;
	total_data_size = 32766; 

	// Prepare send test pattern
	for (i = 0; i < total_data_size; i++) {
		if (i & 1)
			blk_transf_buf[i] = 0x5555;
		else
			blk_transf_buf[i] = 0xAAAA;
	}

	blk_info.opcode = OP_BLKSS;
	blk_info.F = 16;
	blk_info.N = 12;
	blk_info.A = 0;
	blk_info.totsize = total_data_size;
	blk_info.blksize = block_data_size;
	blk_info.timeout = 0;
	blk_info.ascii_transf = 1;

	printf("\nBegin of block transfer write\n");
	for (j = 0; j < 1; j++) {
		printf("\tStart block transfer write: %d\n", j + 1);
		
		resp = BLKTRANSF(crate_id, &blk_info, blk_transf_buf);
		if (resp != 0) {
			printf("ERROR: Negative response from socket server\n");
			return 0;
		}

		printf("\tTotal data written: %d\n", blk_info.totsize);
	}
	printf("End of block transfer write\n");

//	=================================================
//	Init C219 module to input
//  =================================================

	printf("\n-------------------------------------------\nInit C219 module to input\n");

	op.F = 17; 
	op.N = 12;
	for (i = 0; i < 16; i++) {
		op.A = i;
		op.DATA = 7;
		resp = CSSA(crate_id, &op);
		if (resp != 0) {
			printf("ERROR: Negative response from socket server\n");
			return 0;
		}
	}


//	=================================================
//	Invoke Block transfer Q-stop mode 
//	(read operation F=0 on C219 Module address 0)
//	=================================================

	block_data_size = BLOCK_SIZE;
	total_data_size = 32766; 

	// Prepare send test pattern
	for (i = 0; i < total_data_size; i++) {
		blk_transf_buf[i] = 0x0;
	}

	blk_info.opcode = OP_BLKSS;
	blk_info.F = 0;
	blk_info.N = 12;
	blk_info.A = 0;
	blk_info.totsize = total_data_size;
	blk_info.blksize = block_data_size;
	blk_info.timeout = 0;
	blk_info.ascii_transf = 0;

	printf("\nBegin of block transfer read\n");
	for (j = 0; j < 1024;j++) {
		printf("\tStart block transfer read %d\n", j + 1);
	
		resp = BLKTRANSF(crate_id, &blk_info, blk_transf_buf);
		if (resp != 0) {
			printf("ERROR: Negative response from socket server\n");
			return 0;
		}

		printf("\tTotal data read: %d\n", blk_info.totsize);
		/*
		for (i = 0; i < blk_info.totsize; i++) {
			// Show received buffer
			if ((i > 0) && ((i % 10) == 0)) {
				printf("\n");
			}
			printf("%06X ", blk_transf_buf[i]);
		}
		*/
	}
	printf("End of block transfer read\n");

	printf("\n-------------------------------------------\nBye bye\n");
/*
	items_per_row = 16;
	total_items = (items_per_row * 4);

	printf("Start block transfer address scan\n");

	blk_info.opcode = OP_BLKSA;
	blk_info.F = 1;
	blk_info.N = 17;
	blk_info.A = 0;
	blk_info.totsize = total_items;
	blk_info.blksize = items_per_row;
	blk_info.timeout = 3;

	resp = BLKTRANSF(crate_id, &blk_info, blk_transf_buf);
    if (resp != 0) {
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
*/
	return 0;
}

