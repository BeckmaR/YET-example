/*
 * timerservice.h
 *
 *  Created on: 23.05.2019
 *      Author: rbeckmann
 */

#ifndef TIMERSERVICE_H_
#define TIMERSERVICE_H_

#include "src/sc_types.h"
#include "src-gen/Tictoc.h"
#include "src-gen/TictocRequired.h"

#define NUM_TIMERS TICTOC_MAX_PARALLEL_TIME_EVENTS

struct timer_task {
	Tictoc * handle;
	sc_eventid evid;
	sc_integer period;
	unsigned long end_time;
	sc_boolean is_periodic;
};

void proceed(Tictoc * handle);

unsigned long get_ms();


#endif /* TIMERSERVICE_H_ */
