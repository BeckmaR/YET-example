/*
 * trace.h
 *
 *  Created on: 29.05.2019
 *      Author: rbeckmann
 */

#ifndef TRACE_H_
#define TRACE_H_

#include <stdint.h>
#include "src-gen/Solution05Required.h"

#define SCT_PORT 4444
#define SCT_IP "127.0.0.1"
#define RCV_BUFFER_SIZE 256

extern char rcv_buffer[];

extern bool trace_to_console;

int init_traceudp(char* ip, uint16_t port);
void init_tracefile(char* name);
void print_udp_err(int errno);
int send_message(char* buf, int len);
char* rcv_message();

#endif /* TRACE_H_ */
