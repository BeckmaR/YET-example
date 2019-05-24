
#include "Tictoc.h"
#include "../src/sc_types.h"
#include "TictocRequired.h"

/*! \file Implementation of the state machine 'tictoc'
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* prototypes of all internal functions */
static void enact_main_region_TicToc__region0_Tic(Tictoc* handle);
static void enact_main_region_TicToc__region0_Toc(Tictoc* handle);
static void exact_main_region_TicToc__region0_Tic(Tictoc* handle);
static void exact_main_region_TicToc__region0_Toc(Tictoc* handle);
static void enseq_main_region_Start_default(Tictoc* handle);
static void enseq_main_region_TicToc_default(Tictoc* handle);
static void enseq_main_region_TicToc__region0_Tic_default(Tictoc* handle);
static void enseq_main_region_TicToc__region0_Toc_default(Tictoc* handle);
static void enseq_main_region_default(Tictoc* handle);
static void enseq_main_region_TicToc__region0_default(Tictoc* handle);
static void exseq_main_region_Start(Tictoc* handle);
static void exseq_main_region_TicToc__region0_Tic(Tictoc* handle);
static void exseq_main_region_TicToc__region0_Toc(Tictoc* handle);
static void exseq_main_region(Tictoc* handle);
static void exseq_main_region_TicToc__region0(Tictoc* handle);
static void react_main_region__entry_Default(Tictoc* handle);
static void react_main_region_TicToc__region0__entry_Default(Tictoc* handle);
static sc_boolean react(Tictoc* handle);
static sc_boolean main_region_Start_react(Tictoc* handle, const sc_boolean try_transition);
static sc_boolean main_region_TicToc_react(Tictoc* handle, const sc_boolean try_transition);
static sc_boolean main_region_TicToc__region0_Tic_react(Tictoc* handle, const sc_boolean try_transition);
static sc_boolean main_region_TicToc__region0_Toc_react(Tictoc* handle, const sc_boolean try_transition);
static void clearInEvents(Tictoc* handle);
static void clearOutEvents(Tictoc* handle);


#define YET_MESSAGE_INIT_BEGIN '%'
#define YET_MESSAGE_UPDATE_BEGIN '#'
#define YET_MESSAGE_SEPARATOR ','
#define YET_ERR_INVALID_MSG 1
#define YET_ERR_INVALID_BEGIN 2
#define YET_ERR_INVALID_TIMESTAMP 4
#define YET_ERR_INVALID_KEY 8
#define YET_ERR_INVALID_VALUE 16
#define YET_ERR_MISSING_VALUE 32
#define YET_ERR_NULLPTR 64

enum yet_message_type {
	INIT,
	UPDATE
};

struct yet_message {
	enum yet_message_type type;
	unsigned long timestamp;
	char* key;
	char* value;
};

/* Write functions */

void yet_send(Tictoc* handle, const char* key, const char* value);
void create_message(char* buf, struct yet_message * msg);
void create_init_message(char* buf, char* key, char* value);
void create_update_message(char* buf, unsigned long timestamp, char* key, char* value);

unsigned int get_msg_len(struct yet_message* msg);
unsigned int get_msg_len_init(char* key, char* value);
unsigned int get_msg_len_update(unsigned long timestamp, char* key, char* value);
unsigned int get_char_count_ulong(unsigned long data);

void yet_time_event_raise(char* buf, int index);
void yet_time_event_set(char* buf, int index);
void yet_time_event_unset(char* buf, int index);
unsigned int yet_time_event_index(Tictoc* handle, sc_eventid evid);

static void yet_convert_sc_integer_tostring(char* buf, sc_integer value);

/* Read functions */

int read_message(char* text, struct yet_message* msg);
int read_key(char* text, char** endptr, struct yet_message * msg);
int read_value(char* text, struct yet_message * msg);
int read_timestamp(char* text, char** endptr, struct yet_message * msg);

void dispatch_yet_message(Tictoc* handle, struct yet_message * msg);

const char * tictoc_name = "tictoc";

void tictoc_init(Tictoc* handle)
{
	handle->is_running = false;
	handle->skip_yet_send = 0;
	sc_integer i;
	
	for (i = 0; i < TICTOC_MAX_ORTHOGONAL_STATES; ++i)
	{
		handle->stateConfVector[i] = Tictoc_last_state;
	}
	
	
	handle->stateConfVectorPosition = 0;
	
	clearInEvents(handle);
	clearOutEvents(handle);
	
	/* Default init sequence for statechart tictoc */
	tictocIface_set_count(handle, 0);
}

void tictoc_enter(Tictoc* handle)
{
	yet_send(handle, "tictoc@Enter", "1");
	handle->is_running = true;
	set_current_timestamp(handle, 0);
	/* Default enter sequence for statechart tictoc */
	enseq_main_region_default(handle);
}

void tictoc_runCycle(Tictoc* handle)
{
	yet_send(handle, "tictoc@RunCycleStart", 0);
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < TICTOC_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Tictoc_main_region_Start:
		{
			main_region_Start_react(handle, bool_true);
			break;
		}
		case Tictoc_main_region_TicToc__region0_Tic:
		{
			main_region_TicToc__region0_Tic_react(handle, bool_true);
			break;
		}
		case Tictoc_main_region_TicToc__region0_Toc:
		{
			main_region_TicToc__region0_Toc_react(handle, bool_true);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
	yet_send(handle, "tictoc@RunCycleEnd", 0);
}

void tictoc_exit(Tictoc* handle)
{
	handle->is_running = false;
	/* Default exit sequence for statechart tictoc */
	exseq_main_region(handle);
}

sc_boolean tictoc_isActive(const Tictoc* handle)
{
	sc_boolean result = bool_false;
	sc_integer i;
	
	for(i = 0; i < TICTOC_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != Tictoc_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean tictoc_isFinal(const Tictoc* handle)
{
   return bool_false;
}

void tictoc_raiseTimeEvent(Tictoc* handle, sc_eventid evid)
{
	if ( ((sc_intptr_t)evid) >= ((sc_intptr_t)&(handle->timeEvents))
		&&  ((sc_intptr_t)evid) < ((sc_intptr_t)&(handle->timeEvents)) + (unsigned)sizeof(TictocTimeEvents))
	{
		*(sc_boolean*)evid = bool_true;
		
		sc_integer tev_id = yet_time_event_index(handle, evid);
		char buf[21];
		yet_time_event_raise(buf, tev_id);
		yet_send(handle, buf, 0);
		
		tictoc_runCycle(handle);
	}
}

sc_boolean tictoc_isStateActive(const Tictoc* handle, TictocStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Tictoc_main_region_Start :
			result = (sc_boolean) (handle->stateConfVector[SCVI_TICTOC_MAIN_REGION_START] == Tictoc_main_region_Start
			);
				break;
		case Tictoc_main_region_TicToc :
			result = (sc_boolean) (handle->stateConfVector[SCVI_TICTOC_MAIN_REGION_TICTOC] >= Tictoc_main_region_TicToc
				&& handle->stateConfVector[SCVI_TICTOC_MAIN_REGION_TICTOC] <= Tictoc_main_region_TicToc__region0_Toc);
				break;
		case Tictoc_main_region_TicToc__region0_Tic :
			result = (sc_boolean) (handle->stateConfVector[SCVI_TICTOC_MAIN_REGION_TICTOC__REGION0_TIC] == Tictoc_main_region_TicToc__region0_Tic
			);
				break;
		case Tictoc_main_region_TicToc__region0_Toc :
			result = (sc_boolean) (handle->stateConfVector[SCVI_TICTOC_MAIN_REGION_TICTOC__REGION0_TOC] == Tictoc_main_region_TicToc__region0_Toc
			);
				break;
			default:
				result = bool_false;
				break;
		}
		return result;
	}

static void clearInEvents(Tictoc* handle)
{
	handle->iface.start_raised = bool_false;
	handle->timeEvents.tictoc_main_region_TicToc__region0_Tic_tev0_raised = bool_false;
	handle->timeEvents.tictoc_main_region_TicToc__region0_Toc_tev0_raised = bool_false;
}

static void clearOutEvents(Tictoc* handle)
{
	handle->iface.tic_raised = bool_false;
	handle->iface.toc_raised = bool_false;
}

void tictocIface_raise_start(Tictoc* handle)
{
	yet_send(handle, "tictoc.start", 0);
	handle->iface.start_raised = bool_true;
	
	tictoc_runCycle(handle);
}

sc_boolean tictocIface_israised_tic(const Tictoc* handle)
{
	return handle->iface.tic_raised;
}
sc_boolean tictocIface_israised_toc(const Tictoc* handle)
{
	return handle->iface.toc_raised;
}

sc_integer tictocIface_get_count(const Tictoc* handle)
{
	return handle->iface.count;
}
void tictocIface_set_count(Tictoc* handle, sc_integer value)
{
	char valueBuf[21];
	yet_convert_sc_integer_tostring(valueBuf, value);
	yet_send(handle, "tictoc.count", valueBuf);
	
	handle->iface.count = value;
}

/* implementations of all internal functions */

/* Entry action for state 'Tic'. */
static void enact_main_region_TicToc__region0_Tic(Tictoc* handle)
{
	/* Entry action for state 'Tic'. */
	tictoc_setTimer(handle, (sc_eventid) &(handle->timeEvents.tictoc_main_region_TicToc__region0_Tic_tev0_raised) , (1 * 1000), bool_false);
	yet_send(handle, "tictoc@tev.0.Set", 0);
}

/* Entry action for state 'Toc'. */
static void enact_main_region_TicToc__region0_Toc(Tictoc* handle)
{
	/* Entry action for state 'Toc'. */
	tictoc_setTimer(handle, (sc_eventid) &(handle->timeEvents.tictoc_main_region_TicToc__region0_Toc_tev0_raised) , (1 * 1000), bool_false);
	yet_send(handle, "tictoc@tev.1.Set", 0);
}

/* Exit action for state 'Tic'. */
static void exact_main_region_TicToc__region0_Tic(Tictoc* handle)
{
	/* Exit action for state 'Tic'. */
	tictoc_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.tictoc_main_region_TicToc__region0_Tic_tev0_raised) );		
	yet_send(handle, "tictoc@tev.0.Unset", 0);
	tictocIface_set_count(handle, (handle->iface.count + 1));
}

/* Exit action for state 'Toc'. */
static void exact_main_region_TicToc__region0_Toc(Tictoc* handle)
{
	/* Exit action for state 'Toc'. */
	tictoc_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.tictoc_main_region_TicToc__region0_Toc_tev0_raised) );		
	yet_send(handle, "tictoc@tev.1.Unset", 0);
}

/* 'default' enter sequence for state Start */
static void enseq_main_region_Start_default(Tictoc* handle)
{
	/* 'default' enter sequence for state Start */
	handle->stateConfVector[0] = Tictoc_main_region_Start;
	handle->stateConfVectorPosition = 0;
	yet_send(handle, "tictoc@StateEntered", "main_region.Start");
}

/* 'default' enter sequence for state TicToc */
static void enseq_main_region_TicToc_default(Tictoc* handle)
{
	/* 'default' enter sequence for state TicToc */
	enseq_main_region_TicToc__region0_default(handle);
}

/* 'default' enter sequence for state Tic */
static void enseq_main_region_TicToc__region0_Tic_default(Tictoc* handle)
{
	/* 'default' enter sequence for state Tic */
	enact_main_region_TicToc__region0_Tic(handle);
	handle->stateConfVector[0] = Tictoc_main_region_TicToc__region0_Tic;
	handle->stateConfVectorPosition = 0;
	yet_send(handle, "tictoc@StateEntered", "main_region.TicToc._region0.Tic");
}

/* 'default' enter sequence for state Toc */
static void enseq_main_region_TicToc__region0_Toc_default(Tictoc* handle)
{
	/* 'default' enter sequence for state Toc */
	enact_main_region_TicToc__region0_Toc(handle);
	handle->stateConfVector[0] = Tictoc_main_region_TicToc__region0_Toc;
	handle->stateConfVectorPosition = 0;
	yet_send(handle, "tictoc@StateEntered", "main_region.TicToc._region0.Toc");
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(Tictoc* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* 'default' enter sequence for region  */
static void enseq_main_region_TicToc__region0_default(Tictoc* handle)
{
	/* 'default' enter sequence for region  */
	react_main_region_TicToc__region0__entry_Default(handle);
}

/* Default exit sequence for state Start */
static void exseq_main_region_Start(Tictoc* handle)
{
	/* Default exit sequence for state Start */
	handle->stateConfVector[0] = Tictoc_last_state;
	handle->stateConfVectorPosition = 0;
	yet_send(handle, "tictoc@StateExited", "main_region.Start");
}

/* Default exit sequence for state Tic */
static void exseq_main_region_TicToc__region0_Tic(Tictoc* handle)
{
	/* Default exit sequence for state Tic */
	handle->stateConfVector[0] = Tictoc_last_state;
	handle->stateConfVectorPosition = 0;
	yet_send(handle, "tictoc@StateExited", "main_region.TicToc._region0.Tic");
	exact_main_region_TicToc__region0_Tic(handle);
}

/* Default exit sequence for state Toc */
static void exseq_main_region_TicToc__region0_Toc(Tictoc* handle)
{
	/* Default exit sequence for state Toc */
	handle->stateConfVector[0] = Tictoc_last_state;
	handle->stateConfVectorPosition = 0;
	yet_send(handle, "tictoc@StateExited", "main_region.TicToc._region0.Toc");
	exact_main_region_TicToc__region0_Toc(handle);
}

/* Default exit sequence for region main region */
static void exseq_main_region(Tictoc* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of tictoc.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Tictoc_main_region_Start :
		{
			exseq_main_region_Start(handle);
			break;
		}
		case Tictoc_main_region_TicToc__region0_Tic :
		{
			exseq_main_region_TicToc__region0_Tic(handle);
			break;
		}
		case Tictoc_main_region_TicToc__region0_Toc :
		{
			exseq_main_region_TicToc__region0_Toc(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region  */
static void exseq_main_region_TicToc__region0(Tictoc* handle)
{
	/* Default exit sequence for region  */
	/* Handle exit of all possible states (of tictoc.main_region.TicToc._region0) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Tictoc_main_region_TicToc__region0_Tic :
		{
			exseq_main_region_TicToc__region0_Tic(handle);
			break;
		}
		case Tictoc_main_region_TicToc__region0_Toc :
		{
			exseq_main_region_TicToc__region0_Toc(handle);
			break;
		}
		default: break;
	}
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(Tictoc* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_Start_default(handle);
}

/* Default react sequence for initial entry  */
static void react_main_region_TicToc__region0__entry_Default(Tictoc* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_TicToc__region0_Tic_default(handle);
}

static sc_boolean react(Tictoc* handle) {
	/* State machine reactions. */
	return bool_false;
}

static sc_boolean main_region_Start_react(Tictoc* handle, const sc_boolean try_transition) {
	/* The reactions of state Start. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if (handle->iface.start_raised == bool_true)
		{ 
			exseq_main_region_Start(handle);
			enseq_main_region_TicToc_default(handle);
			react(handle);
		}  else
			{
				did_transition = bool_false;
			}
	} 
	if ((did_transition) == (bool_false))
	{ 
		did_transition = react(handle);
	} 
	return did_transition;
}

static sc_boolean main_region_TicToc_react(Tictoc* handle, const sc_boolean try_transition) {
	/* The reactions of state TicToc. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		did_transition = bool_false;
	} 
	if ((did_transition) == (bool_false))
	{ 
		did_transition = react(handle);
	} 
	return did_transition;
}

static sc_boolean main_region_TicToc__region0_Tic_react(Tictoc* handle, const sc_boolean try_transition) {
	/* The reactions of state Tic. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if (handle->timeEvents.tictoc_main_region_TicToc__region0_Tic_tev0_raised == bool_true)
		{ 
			exseq_main_region_TicToc__region0_Tic(handle);
			enseq_main_region_TicToc__region0_Toc_default(handle);
			main_region_TicToc_react(handle, bool_false);
		}  else
			{
				did_transition = bool_false;
			}
	} 
	if ((did_transition) == (bool_false))
	{ 
		did_transition = main_region_TicToc_react(handle, try_transition);
	} 
	return did_transition;
}

static sc_boolean main_region_TicToc__region0_Toc_react(Tictoc* handle, const sc_boolean try_transition) {
	/* The reactions of state Toc. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if (handle->timeEvents.tictoc_main_region_TicToc__region0_Toc_tev0_raised == bool_true)
		{ 
			exseq_main_region_TicToc__region0_Toc(handle);
			enseq_main_region_TicToc__region0_Tic_default(handle);
			main_region_TicToc_react(handle, bool_false);
		}  else
			{
				did_transition = bool_false;
			}
	} 
	if ((did_transition) == (bool_false))
	{ 
		did_transition = main_region_TicToc_react(handle, try_transition);
	} 
	return did_transition;
}


void yet_send(Tictoc* handle, const char* key, const char* value)
{
	if(handle->skip_yet_send > 0) {
		handle->skip_yet_send--;
		return;
	}
	int len = 0;
	struct yet_message msg;
	
	if(handle->is_running) {
		msg.type = UPDATE;
		msg.timestamp = get_current_timestamp(handle);
	} else {
		msg.type = INIT;
	}
	
	msg.key = key;
	msg.value = value;
	len = get_msg_len(&msg);
	char buf[len];
	create_message(buf, &msg);
	yet_write(buf, len);
}

void create_init_message(char* buf, char* key, char* value)
{
	sprintf(buf, "%c%s,%s\n", YET_MESSAGE_INIT_BEGIN, key, value);
}

void create_update_message(char* buf, unsigned long timestamp, char* key, char* value)
{
	if(value != NULL) {
		sprintf(buf, "%c%lu,%s,%s\n", YET_MESSAGE_UPDATE_BEGIN, timestamp, key, value);
	} else {
		sprintf(buf, "%c%lu,%s\n", YET_MESSAGE_UPDATE_BEGIN, timestamp, key);
	}
}

void create_message(char* buf, struct yet_message * msg)
{
	if(msg->type == INIT) {
		create_init_message(buf, msg->key, msg->value);
	} else if(msg->type == UPDATE) {
		create_update_message(buf, msg->timestamp, msg->key, msg->value);
	}
}

unsigned int get_msg_len_init(char* key, char* value)
{
	unsigned int count = 4; // Start char '%', a comma, a newline character and a null terminator
	count += strlen(key);
	count += strlen(value);
	return count;
}

unsigned int get_msg_len_update(unsigned long timestamp, char* key, char* value)
{
	unsigned int count = 4; // Start char '#', a comma, a newline character and a null terminator
	count += get_char_count_ulong(timestamp);
	count += strlen(key);
	if (value != NULL) {
		count += strlen(value) + 1; //For needed additional comma separator
	}
	return count;
}

unsigned int get_msg_len(struct yet_message* msg)
{
	if(msg->type == INIT) {
		return get_msg_len_init(msg->key, msg->value);
	} else if(msg->type == UPDATE) {
		return get_msg_len_update(msg->timestamp, msg->key, msg->value);
	}
	return 0;
}

unsigned int get_char_count_ulong(unsigned long data)
{
	unsigned int c = snprintf(NULL, 0, "%lu", data);
	return c;
}

void yet_time_event_raise(char* buf, int index)
{
	sprintf(buf, "tictoc@tev.%d.Expire", index);
}

void yet_time_event_set(char* buf, int index)
{
	sprintf(buf, "tictoc@tev.%d.Set", index);
}

void yet_time_event_unset(char* buf, int index)
{
	sprintf(buf, "tictoc@tev.%d.Unset", index);
}

unsigned int yet_time_event_index(Tictoc* handle, sc_eventid evid)
{
	sc_integer tev_id = ((sc_intptr_t)evid - (sc_intptr_t)&(handle->timeEvents)) / sizeof(sc_boolean);
	return tev_id;
}

void yet_convert_sc_integer_tostring(char* buf, sc_integer value)
{
	sprintf(buf, "%d", value);
}


int read_message(char* text, struct yet_message* msg)
{
	char* endptr = NULL;
	char begin;
	int e;

	if(text == NULL || msg == NULL) {
		return YET_ERR_NULLPTR;
	}

	begin = *text;
	if(begin != YET_MESSAGE_INIT_BEGIN && begin != YET_MESSAGE_UPDATE_BEGIN) {
		return (YET_ERR_INVALID_MSG | YET_ERR_INVALID_BEGIN);
	}

	text++;
	if(begin == YET_MESSAGE_UPDATE_BEGIN) {
		msg->type = UPDATE;
		e = read_timestamp(text, &endptr, msg);
		if(e != 0) {
			return (YET_ERR_INVALID_MSG | e);
		}
		text = endptr + 1;
	} else {
		msg->type = INIT;
	}

	e = read_key(text, &endptr, msg);
	if(e != 0) {
		return (YET_ERR_INVALID_MSG | e);
	}

	text = endptr;
	if(begin == YET_MESSAGE_INIT_BEGIN && *text == '\0') {
		return (YET_ERR_INVALID_MSG | YET_ERR_MISSING_VALUE);
	}

	if(*text == YET_MESSAGE_SEPARATOR) {
		*text = '\0';
		e = read_value(text + 1, msg);
	} else {
		msg->value = NULL;
	}
	if(*text == '\n') {
		*text = '\0';
	}
	if(e != 0) {
		return (YET_ERR_INVALID_MSG | e);
	}
	return 0;
}

int read_key(char* text, char** endptr, struct yet_message * msg)
{
	if(text == 0 || *text == '\0') {
		return YET_ERR_INVALID_KEY;
	}
	int i = 0;
	msg->key = text;
	while(text[i] != YET_MESSAGE_SEPARATOR && text[i] != '\0' && text[i] != '\n') {
		i++;
	}
	if(i != 0) {
		*endptr = text + i;
		return 0;
	}
	return YET_ERR_INVALID_KEY;
}

int read_value(char* text, struct yet_message * msg)
{
	if(text == 0 || *text == '\0' || (*text == '\n' && *(text+1) == '\0')) {
		return YET_ERR_INVALID_VALUE;
	}
	int i = 0;
	msg->value = text;
	while(text[i] != '\0') {
		i++;
	}
	if(text[i-1] == '\n') {
		// Remove message delimiting newline character
		text[i-1] = '\0';
	}
	if(i != 0) {
		return 0;
	}
	return YET_ERR_INVALID_VALUE;
}

int read_timestamp(char* text, char** endptr, struct yet_message * msg)
{
	msg->timestamp = strtoul(text, endptr, 10);
	/*
	 * strtoul saves the address of the first character that is not a digit
	 * in endptr. If endptr == text, something went wrong / no digits were found.
	 */
	if(*endptr != text) {
		return 0;
	}

	return YET_ERR_INVALID_TIMESTAMP;
}

void dispatch_yet_message(Tictoc* handle, struct yet_message * msg)
{
	// Decide if prefix matches statechart name
	if(strncmp(msg->key, tictoc_name, 6) != 0) {
		return;
	}
	// Skip statechart name + '.'
	char* scope = msg->key + 7;
	char* member;
	member = scope;
	if(strcmp(member, "start") == 0) {
		// Skip one message; this should not be re-raised on host
		handle->skip_yet_send++;
		tictocIface_raise_start(handle);
	}
	if(strcmp(member, "count") == 0) {
		sc_integer converted = atoi(msg->value);
		tictocIface_set_count(handle, converted);
	}
}

void tictoc_accept_message(Tictoc* handle, char* text)
{
	struct yet_message msg;
	read_message(text, &msg);
	dispatch_yet_message(handle, &msg);
}
