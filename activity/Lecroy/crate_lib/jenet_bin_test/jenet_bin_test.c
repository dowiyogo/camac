/*
 =======================================================
 JENET Binary Command Test
 =======================================================
 Author      : Mauro Vetuschi
 Company     : ZP Engineering srl
 Description : 
 Date        : 22/02/2007
 Version     : 1.00
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

int main(int argc, char *argv[])
{
	short i, crate_id, retcode, len, pack, large_len, resp_len, sent;
	char cmd1[32], cmd2[32], resp[32];
	char large_ascii_cmd[8192], large_ascii_resp[8192];
	unsigned char bin_packet[8192], bin_resp[8192];
	unsigned char nim_cmd[6] = {STX, BIN_NIM_SETOUTS_CMD, 0x1, 0x0, 0x0, ETX};
	CRATE_INFO cr_info;

	printf("JENET NIM_SetOut Test\n\n");
	
	// NB example assuming that IP address of JENET Board is 192.168.0.103 

	if (argc == 2)
		crate_id = CROPEN(argv[1]);
	else
		crate_id = CROPEN("192.168.0.98");

	if (crate_id < 0) {
		printf("ERROR: %d - Unable to connect with specified ip address!\n", crate_id);
		return 0;
	}

	sprintf(cmd1, "nim_setouts 1 1\r");
	sprintf(cmd2, "nim_setouts 1 0\r");
	
	CRTOUT(crate_id, 3);

	printf("NIM_SetOut ASCII Test\nPress a key...\n");
	getchar();

	for (i = 0; i < 100; i++) {
		if (i & 1)
			retcode = CMDSR(crate_id, cmd2, resp, 32);
		else
			retcode = CMDSR(crate_id, cmd1, resp, 32);

		if (retcode != CRATE_OK)
			printf("ERROR: %d\n", retcode);
	}

	printf("NIM_SetOut BIN Test\nPress a key...\n");
	getchar();

	for (i = 0; i < 100; i++) {
		retcode = NOSOS(crate_id, 1, (char)(1 - (i & 1)));
		if (retcode != CRATE_OK)
			printf("ERROR: %d\n", retcode);
	}

	printf("NIM_SetOut BIN Test (NO RESPONSE)\nPress a key...\n");
	getchar();

	CBINR(crate_id, 0);
	for (i = 0; i < 100; i++) {
		retcode = NOSOS(crate_id, 1, (char)(1 - (i & 1)));
		if (retcode != CRATE_OK)
			printf("ERROR: %d\n", retcode);
	}
	CBINR(crate_id, 1);

	// ***********************************
	// ********** ADVANCED TEST **********
	// ***********************************

	CRGET(crate_id, &cr_info);
	
	len = strlen(cmd1);
	pack = (8192 / len) - 1;
	large_ascii_cmd[0] = '\0';
	large_len = 0;
	for (i = 0; i < pack; i++) {
		if (i & 1)
			strcat(large_ascii_cmd, cmd1);
		else
			strcat(large_ascii_cmd, cmd2);
		large_len += len;
	}
	resp_len = 3 * pack;

	printf("NIM_SetOut ASCII Test (BLOCK: %d item)\nPress a key...\n", pack);
	getchar();

	sent = 0;
	while (sent < large_len) {
		sent += csock_send(cr_info.sock_ascii, (void *)&large_ascii_cmd[sent], large_len - sent);
	}

	sent = 0;
	while (sent < resp_len) {
		sent += csock_recv(cr_info.sock_ascii, (void *)&large_ascii_resp[sent], resp_len - sent);
	}

	len = 6;
	pack = (8192 / len) - 1;
	large_len = 0;
	for (i = 0; i < pack; i++) {
		nim_cmd[3] = 1 - (i & 1);
		memcpy(&bin_packet[large_len], nim_cmd, len);
		large_len += len;
	}
	
	resp_len = 3 * pack;

	printf("NIM_SetOut BIN Test (BLOCK: %d items)\nPress a key...\n", pack);
	getchar();
	
	sent = 0;
	while (sent < large_len) {
		sent += csock_send(cr_info.sock_bin, &bin_packet[sent], large_len - sent);
	}
	
	sent = 0;
	while (sent < resp_len) {
		sent += csock_recv(cr_info.sock_bin, &bin_resp[sent], resp_len - sent);
	}	

	printf("NIM_SetOut BIN Test (BLOCK: %d items - NO RESPONSE)\nPress a key...\n", pack);
	getchar();
	
	for (i = 0; i < pack; i++) {
		bin_packet[4 + (i*len)] = NO_BIN_RESPONSE;
	}

	CBINR(crate_id, 0);
	sent = 0;
	while (sent < large_len) {
		sent += csock_send(cr_info.sock_bin, &bin_packet[sent], large_len - sent);
	}
	CBINR(crate_id, 1);

	CRCLOSE(crate_id);

    printf("Bye bye!\n");
	return 0;
}
