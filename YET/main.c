/*
 * main.c
 *
 *  Created on: 23.05.2019
 *      Author: rbeckmann
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "src-gen/Tictoc.h"
#include "src-gen/TictocRequired.h"
#include "timerservice.h"
#include "udp.h"

#define SCT_PORT 4444
#define SCT_IP "127.0.0.1"
#define RCV_BUFFER_SIZE 256

Tictoc tictoc;

FILE* tracefile;

int main(int argc, char **argv) {
	tracefile = fopen("trace.yet", "w");

	uint16_t port;
	char rcv_buffer[RCV_BUFFER_SIZE];
	int rcv_byte_len;

	char* ip;
	if(argc > 2) {
		port = atoi(argv[2]);
	} else {
		port = SCT_PORT;
	}
	if(argc > 1) {
		ip = argv[1];
	} else {
		ip = SCT_IP;
	}

	printf("Sending to %s:%d\n", ip, port);

	int err;
	if((err = init_udp_parameters(ip, port)) != 0) {
		print_udp_err(err);
		return err;
	}

	tictoc_init(&tictoc);
	tictoc_enter(&tictoc);

	struct timespec sleep;
	sleep.tv_sec = 0;
	sleep.tv_nsec = 1000;

	while(true) {
		rcv_byte_len = rcv_message(rcv_buffer, RCV_BUFFER_SIZE);
		if(rcv_byte_len > 0) {
			tictoc_accept_message(&tictoc, rcv_buffer);
		}
		proceed(&tictoc);
		nanosleep(&sleep, 0);
	}

}

/*! Function to write YET messages somewhere, e.g. via serial. */
void yet_write(char* buf, int len) {
	print_udp_err(send_message(buf, len));
	fputs(buf, tracefile);
	fflush(tracefile);
	puts(buf);
}

