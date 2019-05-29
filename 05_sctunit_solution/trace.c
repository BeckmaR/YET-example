/*
 * trace.c
 *
 *  Created on: 29.05.2019
 *      Author: rbeckmann
 */

#include "trace.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <poll.h>

struct sockaddr_in receiver;
int sockfd, recvlen;

char rcv_buffer[RCV_BUFFER_SIZE];

FILE* tracefile;

bool trace_to_console = false;

void init_tracefile(char* name) {
	tracefile = fopen(name, "w");
}

int init_traceudp(char* ip, uint16_t port) {
	recvlen = sizeof(receiver);
	memset(&receiver, 0, recvlen);

	// Open an outgoing socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		return -1;
	}

	// Set receiver parameters
	receiver.sin_family = AF_INET;
	receiver.sin_port = htons(port);
	if (inet_aton(ip, &receiver.sin_addr) == 0) {
		return -2;
	}

	return 0;
}

int send_message(char* buf, int len) {
	if(sockfd == 0) {
		return -4;
	}
	int bytes_send = sendto(sockfd, buf, len, 0, (struct sockaddr *) &receiver, recvlen);
	if (bytes_send <= 0) {
		fprintf(stderr, "ERRNO %d\n", errno);
		return -3;
	}
	return 0;
}

void print_udp_err(int err) {
	switch (err) {
	case -1:
		fprintf(stderr, "Could not open socket!\n");
		break;
	case -2:
		fprintf(stderr, "Could not parse IP-address!\n");
		break;
	case -3:
		fprintf(stderr, "Could not send message!\n");
		break;
	case -4:
		fprintf(stderr, "Socket not open!\n");
		break;
	default:
		break;
	}
}

char* rcv_message() {
	struct pollfd pfd;
	pfd.fd = sockfd;
	pfd.events = POLLIN;
	poll(&pfd, 1, 0);
	if((pfd.revents & POLLIN) != 0) {
		recvfrom(sockfd, rcv_buffer, RCV_BUFFER_SIZE, 0, 0, 0);
		return rcv_buffer;
	}
	return 0;
}

/*! Function to write YET messages somewhere, e.g. via serial. */
void yet_write(char* buf, int len) {
	print_udp_err(send_message(buf, len));
	if(tracefile != 0) {
		fputs(buf, tracefile);
		fflush(tracefile);
	}
	if(trace_to_console) {
		puts(buf);
	}
}
