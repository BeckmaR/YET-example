/*
 * udp.h
 *
 *  Created on: 24.05.2019
 *      Author: rbeckmann
 */

#ifndef UDP_H_
#define UDP_H_

#include <stdint.h>

int init_udp_parameters(char* ip, uint16_t port);
void print_udp_err(int errno);
int send_message(char* buf, int len);
int rcv_message(char* buf, int len);

#endif /* UDP_H_ */
