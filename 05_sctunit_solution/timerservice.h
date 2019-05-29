/*
 * timerservice.h
 *
 *  Created on: 29.05.2019
 *      Author: rbeckmann
 */

#ifndef TIMERSERVICE_H_
#define TIMERSERVICE_H_

#include "src-gen/Solution05.h"
#include "src-gen/Solution05Required.h"

#define NUM_TIMERS SOLUTION05_MAX_PARALLEL_TIME_EVENTS

struct timer_task {
	Solution05 * handle;
	sc_eventid evid;
	sc_integer period;
	unsigned long end_time;
	sc_boolean is_periodic;
};

void proceed(Solution05 * handle);

unsigned long get_ms();

#endif /* TIMERSERVICE_H_ */
