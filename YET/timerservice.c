/*
 * timerservice.c
 *
 *  Created on: 23.05.2019
 *      Author: rbeckmann
 */

#include "timerservice.h"
#include <sys/time.h>
#include <string.h>
#include <stdio.h>

unsigned long timestamp_offset;
struct timer_task timers[NUM_TIMERS];

unsigned long get_ms() {
	struct timeval tv;
	unsigned long ms;
	gettimeofday(&tv, 0);
	ms = tv.tv_sec * 1000 + (tv.tv_usec / 1000);
	return ms;
}

void proceed(Tictoc * handle) {
	unsigned long time = get_current_timestamp(handle);
	int i;
	for(i = 0; i<NUM_TIMERS; i++) {
		if(timers[i].handle == handle && timers[i].evid != 0 && time >= timers[i].end_time) {
			sc_eventid evid = timers[i].evid;

			if(timers[i].is_periodic) {
				timers[i].end_time += timers[i].period;
			}

			tictoc_raiseTimeEvent(handle, evid);
		}
	}
}

/*! Function to retrieve the current timestamp, ideally in milliseconds. */
unsigned long get_current_timestamp(Tictoc* handle) {
	unsigned long ms = get_ms();
	unsigned long timestamp =  ms - timestamp_offset;
	return timestamp;
}
/*! Function to set the current timestamp - will be called on enter. */
void set_current_timestamp(Tictoc* handle, unsigned long timestamp) {
	unsigned long ms = get_ms();
	timestamp_offset = ms - timestamp;
}

void tictoc_setTimer(Tictoc* handle, const sc_eventid evid, const sc_integer time_ms, const sc_boolean periodic) {
	unsigned long time = get_current_timestamp(handle);
	for(int i = 0; i<NUM_TIMERS; i++) {
		if(timers[i].handle == 0 && timers[i].evid == 0) {
			timers[i].handle = handle;
			timers[i].evid = evid;
			timers[i].end_time = time + time_ms;
			if(periodic) {
				timers[i].period = time_ms;
				timers[i].is_periodic = true;
			}

			return;
		}
	}
	fprintf(stderr, "Could not set timer!\n");
}

/*! This function has to unset timers for the time events that are required by the state machine. */
/*!
	This function will be called for each time event that is relevant for a state when a state will be left.
	\param evid An unique identifier of the event.
*/
extern void tictoc_unsetTimer(Tictoc* handle, const sc_eventid evid) {
	for(int i = 0; i<NUM_TIMERS; i++) {
		if(timers[i].handle == handle && timers[i].evid == evid) {
			memset(timers + i, 0, sizeof(struct timer_task));
			return;
		}
	}
}

