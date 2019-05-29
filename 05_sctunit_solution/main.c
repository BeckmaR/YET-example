/*
 * main.c
 *
 *  Created on: 23.05.2019
 *      Author: rbeckmann
 */

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <poll.h>
#include <stdlib.h>
#include "src-gen/Solution05.h"
#include "src-gen/Solution05Required.h"
#include "timerservice.h"
#include "trace.h"

Solution05 handle;

char read_stdin();

int main(int argc, char **argv) {
	uint16_t port;
	int power = 0;
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

	printf("Sending to %s:%d\n\n", ip, port);
	printf("Usage:\n\tb - Press button\n\tm - Activate motion sensor\n\tt - Toggle tracing to stdout\n\n");

	int err;
	if((err = init_traceudp(ip, port)) != 0) {
		print_udp_err(err);
		return err;
	}
	init_tracefile("trace.yet");

	solution05_init(&handle);
	solution05_enter(&handle);

	struct timespec sleep;
	sleep.tv_sec = 0;
	sleep.tv_nsec = 10000000; // 10ms

	while(true) {
		char* msg = rcv_message();
		if(msg != 0) {
			solution05_accept_message(&handle, msg);
		}
		char input = read_stdin();
		switch(input) {
		case 'b':
			solution05IfaceButton_raise_down(&handle);
			break;
		case 'm':
			solution05IfaceSensor_raise_motion(&handle);
			break;
		case 't':
			trace_to_console = !trace_to_console;
			break;
		}
		proceed(&handle);
		solution05_runCycle(&handle);
		if(power != solution05IfaceWire_get_power(&handle)) {
			power = solution05IfaceWire_get_power(&handle);
			printf("POWER %d\n", power);
		}
		nanosleep(&sleep, 0);
	}

}

char read_stdin() {
	struct pollfd pfd;
	pfd.fd = 0;
	pfd.events = POLLIN;
	poll(&pfd, 1, 1);
	if((pfd.revents & POLLIN) != 0) {
		char c = getc(stdin);
		return c;
	}
	return 0;
}

