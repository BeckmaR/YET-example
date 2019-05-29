
#ifndef SOLUTION05_H_
#define SOLUTION05_H_

#include "../src/sc_types.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'solution05'.
*/

/*! Define number of states in the state enum */

#define SOLUTION05_STATE_COUNT 6

/*! Define dimension of the state configuration vector for orthogonal states. */
#define SOLUTION05_MAX_ORTHOGONAL_STATES 2

/*! Define maximum number of time events that can be active at once */
#define SOLUTION05_MAX_PARALLEL_TIME_EVENTS 2

/*! Define indices of states in the StateConfVector */
#define SCVI_SOLUTION05_MULTI_CLICK_IDLE 0
#define SCVI_SOLUTION05_MULTI_CLICK_CLICKING 0
#define SCVI_SOLUTION05_LIGHT_CONTROL_MOTIONIDLE 1
#define SCVI_SOLUTION05_LIGHT_CONTROL_MOTIONIDLE__REGION0_OFF 1
#define SCVI_SOLUTION05_LIGHT_CONTROL_MOTIONIDLE__REGION0_MOTIONON 1
#define SCVI_SOLUTION05_LIGHT_CONTROL_ON 1

/*! Enumeration of all states */ 
typedef enum
{
	Solution05_last_state,
	Solution05_multi_click_idle,
	Solution05_multi_click_clicking,
	Solution05_light_control_MotionIdle,
	Solution05_light_control_MotionIdle__region0_Off,
	Solution05_light_control_MotionIdle__region0_MotionOn,
	Solution05_light_control_On
} Solution05States;

/*! Type definition of the data structure for the Solution05IfaceButton interface scope. */
typedef struct
{
	sc_boolean down_raised;
} Solution05IfaceButton;



/*! Type definition of the data structure for the Solution05IfaceWire interface scope. */
typedef struct
{
	sc_integer power;
} Solution05IfaceWire;


/* Declaration of constants for scope Solution05IfaceWire. */
extern const sc_integer SOLUTION05_SOLUTION05IFACEWIRE_OFF;
extern const sc_integer SOLUTION05_SOLUTION05IFACEWIRE_DIMMED;
extern const sc_integer SOLUTION05_SOLUTION05IFACEWIRE_FULL;


/*! Type definition of the data structure for the Solution05IfaceSensor interface scope. */
typedef struct
{
	sc_boolean motion_raised;
} Solution05IfaceSensor;



/*! Type definition of the data structure for the Solution05Internal interface scope. */
typedef struct
{
	sc_integer button_down_count;
	sc_boolean clicked_raised;
	sc_integer clicked_value;
} Solution05Internal;



/*! Type definition of the data structure for the Solution05TimeEvents interface scope. */
typedef struct
{
	sc_boolean solution05_multi_click_clicking_tev0_raised;
	sc_boolean solution05_light_control_MotionIdle__region0_MotionOn_tev0_raised;
} Solution05TimeEvents;




/*! 
 * Type definition of the data structure for the Solution05 state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	sc_boolean is_running;
	sc_integer skip_yet_send;
	Solution05States stateConfVector[SOLUTION05_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	Solution05IfaceButton ifaceButton;
	Solution05IfaceWire ifaceWire;
	Solution05IfaceSensor ifaceSensor;
	Solution05Internal internal;
	Solution05TimeEvents timeEvents;
} Solution05;



/*! Initializes the Solution05 state machine data structures. Must be called before first usage.*/
extern void solution05_init(Solution05* handle);

/*! Activates the state machine */
extern void solution05_enter(Solution05* handle);

/*! Deactivates the state machine */
extern void solution05_exit(Solution05* handle);

/*! Performs a 'run to completion' step. */
extern void solution05_runCycle(Solution05* handle);

/*! Raises a time event. */
extern void solution05_raiseTimeEvent(Solution05* handle, sc_eventid evid);

/*! Raises the in event 'down' that is defined in the interface scope 'button'. */ 
extern void solution05IfaceButton_raise_down(Solution05* handle);

/*! Gets the value of the variable 'power' that is defined in the interface scope 'wire'. */ 
extern sc_integer solution05IfaceWire_get_power(const Solution05* handle);
/*! Sets the value of the variable 'power' that is defined in the interface scope 'wire'. */ 
extern void solution05IfaceWire_set_power(Solution05* handle, sc_integer value);
/*! Gets the value of the variable 'OFF' that is defined in the interface scope 'wire'. */ 
extern sc_integer solution05IfaceWire_get_oFF(const Solution05* handle);
/*! Gets the value of the variable 'DIMMED' that is defined in the interface scope 'wire'. */ 
extern sc_integer solution05IfaceWire_get_dIMMED(const Solution05* handle);
/*! Gets the value of the variable 'FULL' that is defined in the interface scope 'wire'. */ 
extern sc_integer solution05IfaceWire_get_fULL(const Solution05* handle);
/*! Raises the in event 'motion' that is defined in the interface scope 'sensor'. */ 
extern void solution05IfaceSensor_raise_motion(Solution05* handle);


/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean solution05_isActive(const Solution05* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean solution05_isFinal(const Solution05* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean solution05_isStateActive(const Solution05* handle, Solution05States state);

extern void solution05_accept_message(Solution05* handle, char* text);

#ifdef __cplusplus
}
#endif 

#endif /* SOLUTION05_H_ */
