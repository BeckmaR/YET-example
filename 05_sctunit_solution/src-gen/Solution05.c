
#include "Solution05.h"
#include "../src/sc_types.h"
#include "Solution05Required.h"

/*! \file Implementation of the state machine 'solution05'
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* prototypes of all internal functions */
static sc_boolean check_light_control__choice_0_tr0_tr0(const Solution05* handle);
static void effect_light_control__choice_0_tr0(Solution05* handle);
static void effect_light_control__choice_0_tr1(Solution05* handle);
static void enact_multi_click_idle(Solution05* handle);
static void enact_multi_click_clicking(Solution05* handle);
static void enact_light_control_MotionIdle(Solution05* handle);
static void enact_light_control_MotionIdle__region0_Off(Solution05* handle);
static void enact_light_control_MotionIdle__region0_MotionOn(Solution05* handle);
static void exact_multi_click_clicking(Solution05* handle);
static void exact_light_control_MotionIdle__region0_MotionOn(Solution05* handle);
static void enseq_multi_click_idle_default(Solution05* handle);
static void enseq_multi_click_clicking_default(Solution05* handle);
static void enseq_light_control_MotionIdle_default(Solution05* handle);
static void enseq_light_control_MotionIdle__region0_Off_default(Solution05* handle);
static void enseq_light_control_MotionIdle__region0_MotionOn_default(Solution05* handle);
static void enseq_light_control_On_default(Solution05* handle);
static void enseq_multi_click_default(Solution05* handle);
static void enseq_light_control_default(Solution05* handle);
static void enseq_light_control_MotionIdle__region0_default(Solution05* handle);
static void exseq_multi_click_idle(Solution05* handle);
static void exseq_multi_click_clicking(Solution05* handle);
static void exseq_light_control_MotionIdle(Solution05* handle);
static void exseq_light_control_MotionIdle__region0_Off(Solution05* handle);
static void exseq_light_control_MotionIdle__region0_MotionOn(Solution05* handle);
static void exseq_light_control_On(Solution05* handle);
static void exseq_multi_click(Solution05* handle);
static void exseq_light_control(Solution05* handle);
static void exseq_light_control_MotionIdle__region0(Solution05* handle);
static void react_light_control__choice_0(Solution05* handle);
static void react_multi_click__entry_Default(Solution05* handle);
static void react_light_control__entry_Default(Solution05* handle);
static void react_light_control_MotionIdle__region0__entry_Default(Solution05* handle);
static sc_boolean react(Solution05* handle);
static sc_boolean multi_click_idle_react(Solution05* handle, const sc_boolean try_transition);
static sc_boolean multi_click_clicking_react(Solution05* handle, const sc_boolean try_transition);
static sc_boolean light_control_MotionIdle_react(Solution05* handle, const sc_boolean try_transition);
static sc_boolean light_control_MotionIdle__region0_Off_react(Solution05* handle, const sc_boolean try_transition);
static sc_boolean light_control_MotionIdle__region0_MotionOn_react(Solution05* handle, const sc_boolean try_transition);
static sc_boolean light_control_On_react(Solution05* handle, const sc_boolean try_transition);
static void clearInEvents(Solution05* handle);
static void clearOutEvents(Solution05* handle);


static void solution05Internal_set_button_down_count(Solution05* handle, sc_integer value);

const sc_integer SOLUTION05_SOLUTION05IFACEWIRE_OFF = 0;
const sc_integer SOLUTION05_SOLUTION05IFACEWIRE_DIMMED = 50;
const sc_integer SOLUTION05_SOLUTION05IFACEWIRE_FULL = 100;
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

void yet_send(Solution05* handle, const char* key, const char* value);
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
unsigned int yet_time_event_index(Solution05* handle, sc_eventid evid);

static void yet_convert_sc_integer_tostring(char* buf, sc_integer value);

/* Read functions */

int read_message(char* text, struct yet_message* msg);
int read_key(char* text, char** endptr, struct yet_message * msg);
int read_value(char* text, struct yet_message * msg);
int read_timestamp(char* text, char** endptr, struct yet_message * msg);

void dispatch_yet_message(Solution05* handle, struct yet_message * msg);

const char * solution05_name = "solution05";

void solution05_init(Solution05* handle)
{
	handle->is_running = false;
	handle->skip_yet_send = 0;
	sc_integer i;
	
	for (i = 0; i < SOLUTION05_MAX_ORTHOGONAL_STATES; ++i)
	{
		handle->stateConfVector[i] = Solution05_last_state;
	}
	
	
	handle->stateConfVectorPosition = 0;
	
	clearInEvents(handle);
	clearOutEvents(handle);
	
	/* Default init sequence for statechart solution05 */
	solution05IfaceWire_set_power(handle, 0);
	solution05Internal_set_button_down_count(handle, 0);
}

void solution05_enter(Solution05* handle)
{
	yet_send(handle, "solution05@Enter", "1");
	handle->is_running = true;
	set_current_timestamp(handle, 0);
	/* Default enter sequence for statechart solution05 */
	enseq_multi_click_default(handle);
	enseq_light_control_default(handle);
}

void solution05_runCycle(Solution05* handle)
{
	yet_send(handle, "solution05@RunCycleStart", 0);
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < SOLUTION05_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Solution05_multi_click_idle:
		{
			multi_click_idle_react(handle, bool_true);
			break;
		}
		case Solution05_multi_click_clicking:
		{
			multi_click_clicking_react(handle, bool_true);
			break;
		}
		case Solution05_light_control_MotionIdle__region0_Off:
		{
			light_control_MotionIdle__region0_Off_react(handle, bool_true);
			break;
		}
		case Solution05_light_control_MotionIdle__region0_MotionOn:
		{
			light_control_MotionIdle__region0_MotionOn_react(handle, bool_true);
			break;
		}
		case Solution05_light_control_On:
		{
			light_control_On_react(handle, bool_true);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
	yet_send(handle, "solution05@RunCycleEnd", 0);
}

void solution05_exit(Solution05* handle)
{
	handle->is_running = false;
	/* Default exit sequence for statechart solution05 */
	exseq_multi_click(handle);
	exseq_light_control(handle);
}

sc_boolean solution05_isActive(const Solution05* handle)
{
	sc_boolean result = bool_false;
	sc_integer i;
	
	for(i = 0; i < SOLUTION05_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != Solution05_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean solution05_isFinal(const Solution05* handle)
{
   return bool_false;
}

void solution05_raiseTimeEvent(Solution05* handle, sc_eventid evid)
{
	if ( ((sc_intptr_t)evid) >= ((sc_intptr_t)&(handle->timeEvents))
		&&  ((sc_intptr_t)evid) < ((sc_intptr_t)&(handle->timeEvents)) + (unsigned)sizeof(Solution05TimeEvents))
	{
		*(sc_boolean*)evid = bool_true;
		sc_integer tev_id = yet_time_event_index(handle, evid);
		char buf[25];
		yet_time_event_raise(buf, tev_id);
		yet_send(handle, buf, 0);
	}
}

sc_boolean solution05_isStateActive(const Solution05* handle, Solution05States state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Solution05_multi_click_idle :
			result = (sc_boolean) (handle->stateConfVector[SCVI_SOLUTION05_MULTI_CLICK_IDLE] == Solution05_multi_click_idle
			);
				break;
		case Solution05_multi_click_clicking :
			result = (sc_boolean) (handle->stateConfVector[SCVI_SOLUTION05_MULTI_CLICK_CLICKING] == Solution05_multi_click_clicking
			);
				break;
		case Solution05_light_control_MotionIdle :
			result = (sc_boolean) (handle->stateConfVector[SCVI_SOLUTION05_LIGHT_CONTROL_MOTIONIDLE] >= Solution05_light_control_MotionIdle
				&& handle->stateConfVector[SCVI_SOLUTION05_LIGHT_CONTROL_MOTIONIDLE] <= Solution05_light_control_MotionIdle__region0_MotionOn);
				break;
		case Solution05_light_control_MotionIdle__region0_Off :
			result = (sc_boolean) (handle->stateConfVector[SCVI_SOLUTION05_LIGHT_CONTROL_MOTIONIDLE__REGION0_OFF] == Solution05_light_control_MotionIdle__region0_Off
			);
				break;
		case Solution05_light_control_MotionIdle__region0_MotionOn :
			result = (sc_boolean) (handle->stateConfVector[SCVI_SOLUTION05_LIGHT_CONTROL_MOTIONIDLE__REGION0_MOTIONON] == Solution05_light_control_MotionIdle__region0_MotionOn
			);
				break;
		case Solution05_light_control_On :
			result = (sc_boolean) (handle->stateConfVector[SCVI_SOLUTION05_LIGHT_CONTROL_ON] == Solution05_light_control_On
			);
				break;
			default:
				result = bool_false;
				break;
		}
		return result;
	}

static void clearInEvents(Solution05* handle)
{
	handle->ifaceButton.down_raised = bool_false;
	handle->ifaceSensor.motion_raised = bool_false;
	handle->internal.clicked_raised = bool_false;
	handle->timeEvents.solution05_multi_click_clicking_tev0_raised = bool_false;
	handle->timeEvents.solution05_light_control_MotionIdle__region0_MotionOn_tev0_raised = bool_false;
}

static void clearOutEvents(Solution05* handle)
{
}

void solution05IfaceButton_raise_down(Solution05* handle)
{
	yet_send(handle, "solution05.button.down", 0);
	handle->ifaceButton.down_raised = bool_true;
}




sc_integer solution05IfaceWire_get_power(const Solution05* handle)
{
	return handle->ifaceWire.power;
}
void solution05IfaceWire_set_power(Solution05* handle, sc_integer value)
{
	char valueBuf[21];
	yet_convert_sc_integer_tostring(valueBuf, value);
	yet_send(handle, "solution05.wire.power", valueBuf);
	
	handle->ifaceWire.power = value;
}
sc_integer solution05IfaceWire_get_oFF(const Solution05* handle)
{
	return SOLUTION05_SOLUTION05IFACEWIRE_OFF;
}
sc_integer solution05IfaceWire_get_dIMMED(const Solution05* handle)
{
	return SOLUTION05_SOLUTION05IFACEWIRE_DIMMED;
}
sc_integer solution05IfaceWire_get_fULL(const Solution05* handle)
{
	return SOLUTION05_SOLUTION05IFACEWIRE_FULL;
}
void solution05IfaceSensor_raise_motion(Solution05* handle)
{
	yet_send(handle, "solution05.sensor.motion", 0);
	handle->ifaceSensor.motion_raised = bool_true;
}


void solution05Internal_set_button_down_count(Solution05* handle, sc_integer value)
{
	char valueBuf[21];
	yet_convert_sc_integer_tostring(valueBuf, value);
	yet_send(handle, "solution05.button_down_count", valueBuf);
	
	handle->internal.button_down_count = value;
}

/* implementations of all internal functions */

static sc_boolean check_light_control__choice_0_tr0_tr0(const Solution05* handle)
{
	return (handle->internal.clicked_value) == (1);
}

static void effect_light_control__choice_0_tr0(Solution05* handle)
{
	enseq_light_control_MotionIdle_default(handle);
}

static void effect_light_control__choice_0_tr1(Solution05* handle)
{
	solution05IfaceWire_set_power(handle, ((handle->ifaceWire.power) != (SOLUTION05_SOLUTION05IFACEWIRE_FULL)) ? SOLUTION05_SOLUTION05IFACEWIRE_FULL : SOLUTION05_SOLUTION05IFACEWIRE_DIMMED);
	enseq_light_control_On_default(handle);
}

/* Entry action for state 'idle'. */
static void enact_multi_click_idle(Solution05* handle)
{
	/* Entry action for state 'idle'. */
	solution05Internal_set_button_down_count(handle, 0);
}

/* Entry action for state 'clicking'. */
static void enact_multi_click_clicking(Solution05* handle)
{
	/* Entry action for state 'clicking'. */
	solution05_setTimer(handle, (sc_eventid) &(handle->timeEvents.solution05_multi_click_clicking_tev0_raised) , 400, bool_false);
	yet_send(handle, "solution05@tev.0.Set", 0);
	solution05Internal_set_button_down_count(handle, handle->internal.button_down_count + (1));
}

/* Entry action for state 'MotionIdle'. */
static void enact_light_control_MotionIdle(Solution05* handle)
{
	/* Entry action for state 'MotionIdle'. */
	solution05IfaceWire_set_power(handle, SOLUTION05_SOLUTION05IFACEWIRE_OFF);
}

/* Entry action for state 'Off'. */
static void enact_light_control_MotionIdle__region0_Off(Solution05* handle)
{
	/* Entry action for state 'Off'. */
	solution05IfaceWire_set_power(handle, SOLUTION05_SOLUTION05IFACEWIRE_OFF);
}

/* Entry action for state 'MotionOn'. */
static void enact_light_control_MotionIdle__region0_MotionOn(Solution05* handle)
{
	/* Entry action for state 'MotionOn'. */
	solution05_setTimer(handle, (sc_eventid) &(handle->timeEvents.solution05_light_control_MotionIdle__region0_MotionOn_tev0_raised) , (30 * 1000), bool_false);
	yet_send(handle, "solution05@tev.1.Set", 0);
	solution05IfaceWire_set_power(handle, SOLUTION05_SOLUTION05IFACEWIRE_FULL);
}

/* Exit action for state 'clicking'. */
static void exact_multi_click_clicking(Solution05* handle)
{
	/* Exit action for state 'clicking'. */
	solution05_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.solution05_multi_click_clicking_tev0_raised) );		
	yet_send(handle, "solution05@tev.0.Unset", 0);
}

/* Exit action for state 'MotionOn'. */
static void exact_light_control_MotionIdle__region0_MotionOn(Solution05* handle)
{
	/* Exit action for state 'MotionOn'. */
	solution05_unsetTimer(handle, (sc_eventid) &(handle->timeEvents.solution05_light_control_MotionIdle__region0_MotionOn_tev0_raised) );		
	yet_send(handle, "solution05@tev.1.Unset", 0);
}

/* 'default' enter sequence for state idle */
static void enseq_multi_click_idle_default(Solution05* handle)
{
	/* 'default' enter sequence for state idle */
	enact_multi_click_idle(handle);
	handle->stateConfVector[0] = Solution05_multi_click_idle;
	handle->stateConfVectorPosition = 0;
	yet_send(handle, "solution05@StateEntered", "multi_click.idle");
}

/* 'default' enter sequence for state clicking */
static void enseq_multi_click_clicking_default(Solution05* handle)
{
	/* 'default' enter sequence for state clicking */
	enact_multi_click_clicking(handle);
	handle->stateConfVector[0] = Solution05_multi_click_clicking;
	handle->stateConfVectorPosition = 0;
	yet_send(handle, "solution05@StateEntered", "multi_click.clicking");
}

/* 'default' enter sequence for state MotionIdle */
static void enseq_light_control_MotionIdle_default(Solution05* handle)
{
	/* 'default' enter sequence for state MotionIdle */
	enact_light_control_MotionIdle(handle);
	enseq_light_control_MotionIdle__region0_default(handle);
}

/* 'default' enter sequence for state Off */
static void enseq_light_control_MotionIdle__region0_Off_default(Solution05* handle)
{
	/* 'default' enter sequence for state Off */
	enact_light_control_MotionIdle__region0_Off(handle);
	handle->stateConfVector[1] = Solution05_light_control_MotionIdle__region0_Off;
	handle->stateConfVectorPosition = 1;
	yet_send(handle, "solution05@StateEntered", "light_control.MotionIdle._region0.Off");
}

/* 'default' enter sequence for state MotionOn */
static void enseq_light_control_MotionIdle__region0_MotionOn_default(Solution05* handle)
{
	/* 'default' enter sequence for state MotionOn */
	enact_light_control_MotionIdle__region0_MotionOn(handle);
	handle->stateConfVector[1] = Solution05_light_control_MotionIdle__region0_MotionOn;
	handle->stateConfVectorPosition = 1;
	yet_send(handle, "solution05@StateEntered", "light_control.MotionIdle._region0.MotionOn");
}

/* 'default' enter sequence for state On */
static void enseq_light_control_On_default(Solution05* handle)
{
	/* 'default' enter sequence for state On */
	handle->stateConfVector[1] = Solution05_light_control_On;
	handle->stateConfVectorPosition = 1;
	yet_send(handle, "solution05@StateEntered", "light_control.On");
}

/* 'default' enter sequence for region multi click */
static void enseq_multi_click_default(Solution05* handle)
{
	/* 'default' enter sequence for region multi click */
	react_multi_click__entry_Default(handle);
}

/* 'default' enter sequence for region light control */
static void enseq_light_control_default(Solution05* handle)
{
	/* 'default' enter sequence for region light control */
	react_light_control__entry_Default(handle);
}

/* 'default' enter sequence for region null */
static void enseq_light_control_MotionIdle__region0_default(Solution05* handle)
{
	/* 'default' enter sequence for region null */
	react_light_control_MotionIdle__region0__entry_Default(handle);
}

/* Default exit sequence for state idle */
static void exseq_multi_click_idle(Solution05* handle)
{
	/* Default exit sequence for state idle */
	handle->stateConfVector[0] = Solution05_last_state;
	handle->stateConfVectorPosition = 0;
	yet_send(handle, "solution05@StateExited", "multi_click.idle");
}

/* Default exit sequence for state clicking */
static void exseq_multi_click_clicking(Solution05* handle)
{
	/* Default exit sequence for state clicking */
	handle->stateConfVector[0] = Solution05_last_state;
	handle->stateConfVectorPosition = 0;
	yet_send(handle, "solution05@StateExited", "multi_click.clicking");
	exact_multi_click_clicking(handle);
}

/* Default exit sequence for state MotionIdle */
static void exseq_light_control_MotionIdle(Solution05* handle)
{
	/* Default exit sequence for state MotionIdle */
	exseq_light_control_MotionIdle__region0(handle);
}

/* Default exit sequence for state Off */
static void exseq_light_control_MotionIdle__region0_Off(Solution05* handle)
{
	/* Default exit sequence for state Off */
	handle->stateConfVector[1] = Solution05_last_state;
	handle->stateConfVectorPosition = 1;
	yet_send(handle, "solution05@StateExited", "light_control.MotionIdle._region0.Off");
}

/* Default exit sequence for state MotionOn */
static void exseq_light_control_MotionIdle__region0_MotionOn(Solution05* handle)
{
	/* Default exit sequence for state MotionOn */
	handle->stateConfVector[1] = Solution05_last_state;
	handle->stateConfVectorPosition = 1;
	yet_send(handle, "solution05@StateExited", "light_control.MotionIdle._region0.MotionOn");
	exact_light_control_MotionIdle__region0_MotionOn(handle);
}

/* Default exit sequence for state On */
static void exseq_light_control_On(Solution05* handle)
{
	/* Default exit sequence for state On */
	handle->stateConfVector[1] = Solution05_last_state;
	handle->stateConfVectorPosition = 1;
	yet_send(handle, "solution05@StateExited", "light_control.On");
}

/* Default exit sequence for region multi click */
static void exseq_multi_click(Solution05* handle)
{
	/* Default exit sequence for region multi click */
	/* Handle exit of all possible states (of solution05.multi_click) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Solution05_multi_click_idle :
		{
			exseq_multi_click_idle(handle);
			break;
		}
		case Solution05_multi_click_clicking :
		{
			exseq_multi_click_clicking(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region light control */
static void exseq_light_control(Solution05* handle)
{
	/* Default exit sequence for region light control */
	/* Handle exit of all possible states (of solution05.light_control) at position 1... */
	switch(handle->stateConfVector[ 1 ])
	{
		case Solution05_light_control_MotionIdle__region0_Off :
		{
			exseq_light_control_MotionIdle__region0_Off(handle);
			break;
		}
		case Solution05_light_control_MotionIdle__region0_MotionOn :
		{
			exseq_light_control_MotionIdle__region0_MotionOn(handle);
			break;
		}
		case Solution05_light_control_On :
		{
			exseq_light_control_On(handle);
			break;
		}
		default: break;
	}
}

/* Default exit sequence for region null */
static void exseq_light_control_MotionIdle__region0(Solution05* handle)
{
	/* Default exit sequence for region null */
	/* Handle exit of all possible states (of solution05.light_control.MotionIdle._region0) at position 1... */
	switch(handle->stateConfVector[ 1 ])
	{
		case Solution05_light_control_MotionIdle__region0_Off :
		{
			exseq_light_control_MotionIdle__region0_Off(handle);
			break;
		}
		case Solution05_light_control_MotionIdle__region0_MotionOn :
		{
			exseq_light_control_MotionIdle__region0_MotionOn(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state null. */
static void react_light_control__choice_0(Solution05* handle)
{
	/* The reactions of state null. */
	if (check_light_control__choice_0_tr0_tr0(handle) == bool_true)
	{ 
		effect_light_control__choice_0_tr0(handle);
	}  else
		{
			effect_light_control__choice_0_tr1(handle);
		}
}

/* Default react sequence for initial entry  */
static void react_multi_click__entry_Default(Solution05* handle)
{
	/* Default react sequence for initial entry  */
	enseq_multi_click_idle_default(handle);
}

/* Default react sequence for initial entry  */
static void react_light_control__entry_Default(Solution05* handle)
{
	/* Default react sequence for initial entry  */
	enseq_light_control_MotionIdle_default(handle);
}

/* Default react sequence for initial entry  */
static void react_light_control_MotionIdle__region0__entry_Default(Solution05* handle)
{
	/* Default react sequence for initial entry  */
	enseq_light_control_MotionIdle__region0_Off_default(handle);
}

static sc_boolean react(Solution05* handle) {
	/* State machine reactions. */
	return bool_false;
}

static sc_boolean multi_click_idle_react(Solution05* handle, const sc_boolean try_transition) {
	/* The reactions of state idle. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if ((react(handle)) == (bool_false))
		{ 
			if (handle->ifaceButton.down_raised == bool_true)
			{ 
				exseq_multi_click_idle(handle);
				enseq_multi_click_clicking_default(handle);
			}  else
				{
					did_transition = bool_false;
				}
		} 
	} 
	return did_transition;
}

static sc_boolean multi_click_clicking_react(Solution05* handle, const sc_boolean try_transition) {
	/* The reactions of state clicking. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if ((react(handle)) == (bool_false))
		{ 
			if (handle->ifaceButton.down_raised == bool_true)
			{ 
				exseq_multi_click_clicking(handle);
				enseq_multi_click_clicking_default(handle);
			}  else
				{
					if (handle->timeEvents.solution05_multi_click_clicking_tev0_raised == bool_true)
					{ 
						exseq_multi_click_clicking(handle);
						handle->internal.clicked_value = handle->internal.button_down_count;
						handle->internal.clicked_raised = bool_true;
						enseq_multi_click_idle_default(handle);
					}  else
						{
							did_transition = bool_false;
						}
				}
		} 
	} 
	return did_transition;
}

static sc_boolean light_control_MotionIdle_react(Solution05* handle, const sc_boolean try_transition) {
	/* The reactions of state MotionIdle. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if (handle->internal.clicked_raised == bool_true)
		{ 
			exseq_light_control_MotionIdle(handle);
			solution05IfaceWire_set_power(handle, SOLUTION05_SOLUTION05IFACEWIRE_DIMMED);
			enseq_light_control_On_default(handle);
		}  else
			{
				did_transition = bool_false;
			}
	} 
	return did_transition;
}

static sc_boolean light_control_MotionIdle__region0_Off_react(Solution05* handle, const sc_boolean try_transition) {
	/* The reactions of state Off. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if ((light_control_MotionIdle_react(handle, try_transition)) == (bool_false))
		{ 
			if (handle->ifaceSensor.motion_raised == bool_true)
			{ 
				exseq_light_control_MotionIdle__region0_Off(handle);
				enseq_light_control_MotionIdle__region0_MotionOn_default(handle);
			}  else
				{
					did_transition = bool_false;
				}
		} 
	} 
	return did_transition;
}

static sc_boolean light_control_MotionIdle__region0_MotionOn_react(Solution05* handle, const sc_boolean try_transition) {
	/* The reactions of state MotionOn. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if ((light_control_MotionIdle_react(handle, try_transition)) == (bool_false))
		{ 
			if (handle->timeEvents.solution05_light_control_MotionIdle__region0_MotionOn_tev0_raised == bool_true)
			{ 
				exseq_light_control_MotionIdle__region0_MotionOn(handle);
				enseq_light_control_MotionIdle__region0_Off_default(handle);
			}  else
				{
					if (handle->ifaceSensor.motion_raised == bool_true)
					{ 
						exseq_light_control_MotionIdle__region0_MotionOn(handle);
						enseq_light_control_MotionIdle__region0_MotionOn_default(handle);
					}  else
						{
							did_transition = bool_false;
						}
				}
		} 
	} 
	return did_transition;
}

static sc_boolean light_control_On_react(Solution05* handle, const sc_boolean try_transition) {
	/* The reactions of state On. */
	sc_boolean did_transition = try_transition;
	if (try_transition == bool_true)
	{ 
		if (handle->internal.clicked_raised == bool_true)
		{ 
			exseq_light_control_On(handle);
			react_light_control__choice_0(handle);
		}  else
			{
				did_transition = bool_false;
			}
	} 
	return did_transition;
}


void yet_send(Solution05* handle, const char* key, const char* value)
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
	sprintf(buf, "solution05@tev.%d.Expire", index);
}

void yet_time_event_set(char* buf, int index)
{
	sprintf(buf, "solution05@tev.%d.Set", index);
}

void yet_time_event_unset(char* buf, int index)
{
	sprintf(buf, "solution05@tev.%d.Unset", index);
}

unsigned int yet_time_event_index(Solution05* handle, sc_eventid evid)
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

void dispatch_yet_message(Solution05* handle, struct yet_message * msg)
{
	// Decide if prefix matches statechart name
	if(strncmp(msg->key, solution05_name, 10) != 0) {
		return;
	}
	// Skip statechart name + '.'
	char* scope = msg->key + 11;
	char* member;
	if(strncmp(scope, "button.", 7) == 0) {
		member = scope + 7;
		if(strcmp(member, "down") == 0) {
			// Skip one message; this should not be re-raised on host
			handle->skip_yet_send++;
			solution05IfaceButton_raise_down(handle);
		}
		return;
	}
	if(strncmp(scope, "wire.", 5) == 0) {
		member = scope + 5;
		if(strcmp(member, "power") == 0) {
			sc_integer converted = atoi(msg->value);
			solution05IfaceWire_set_power(handle, converted);
		}
		return;
	}
	if(strncmp(scope, "sensor.", 7) == 0) {
		member = scope + 7;
		if(strcmp(member, "motion") == 0) {
			// Skip one message; this should not be re-raised on host
			handle->skip_yet_send++;
			solution05IfaceSensor_raise_motion(handle);
		}
		return;
	}
}

void solution05_accept_message(Solution05* handle, char* text)
{
	struct yet_message msg;
	read_message(text, &msg);
	dispatch_yet_message(handle, &msg);
}
