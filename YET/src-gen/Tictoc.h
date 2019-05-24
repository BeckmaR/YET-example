
#ifndef TICTOC_H_
#define TICTOC_H_

#include "../src/sc_types.h"

#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'tictoc'.
*/

#ifndef TICTOC_EVENTQUEUE_BUFFERSIZE
#define TICTOC_EVENTQUEUE_BUFFERSIZE 20
#endif
#ifndef SC_INVALID_EVENT_VALUE
#define SC_INVALID_EVENT_VALUE 0
#endif
/*! Define number of states in the state enum */

#define TICTOC_STATE_COUNT 4

/*! Define dimension of the state configuration vector for orthogonal states. */
#define TICTOC_MAX_ORTHOGONAL_STATES 1

/*! Define maximum number of time events that can be active at once */
#define TICTOC_MAX_PARALLEL_TIME_EVENTS 1

/*! Define indices of states in the StateConfVector */
#define SCVI_TICTOC_MAIN_REGION_START 0
#define SCVI_TICTOC_MAIN_REGION_TICTOC 0
#define SCVI_TICTOC_MAIN_REGION_TICTOC__REGION0_TIC 0
#define SCVI_TICTOC_MAIN_REGION_TICTOC__REGION0_TOC 0


/*! Enumeration of all states */ 
typedef enum
{
	Tictoc_last_state,
	Tictoc_main_region_Start,
	Tictoc_main_region_TicToc,
	Tictoc_main_region_TicToc__region0_Tic,
	Tictoc_main_region_TicToc__region0_Toc
} TictocStates;

/*! Type definition of the data structure for the TictocIface interface scope. */
typedef struct
{
	sc_boolean start_raised;
	sc_boolean tic_raised;
	sc_boolean toc_raised;
	sc_integer count;
} TictocIface;



/*! Type definition of the data structure for the TictocTimeEvents interface scope. */
typedef struct
{
	sc_boolean tictoc_main_region_TicToc__region0_Tic_tev0_raised;
	sc_boolean tictoc_main_region_TicToc__region0_Toc_tev0_raised;
} TictocTimeEvents;




/*! 
 * Type definition of the data structure for the Tictoc state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	sc_boolean is_running;
	sc_integer skip_yet_send;
	TictocStates stateConfVector[TICTOC_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	TictocIface iface;
	TictocTimeEvents timeEvents;
} Tictoc;



/*! Initializes the Tictoc state machine data structures. Must be called before first usage.*/
extern void tictoc_init(Tictoc* handle);

/*! Activates the state machine */
extern void tictoc_enter(Tictoc* handle);

/*! Deactivates the state machine */
extern void tictoc_exit(Tictoc* handle);

/*! Performs a 'run to completion' step. */
extern void tictoc_runCycle(Tictoc* handle);

/*! Raises a time event. */
extern void tictoc_raiseTimeEvent(Tictoc* handle, sc_eventid evid);

/*! Raises the in event 'start' that is defined in the default interface scope. */ 
extern void tictocIface_raise_start(Tictoc* handle);

/*! Checks if the out event 'tic' that is defined in the default interface scope has been raised. */ 
extern sc_boolean tictocIface_israised_tic(const Tictoc* handle);

/*! Checks if the out event 'toc' that is defined in the default interface scope has been raised. */ 
extern sc_boolean tictocIface_israised_toc(const Tictoc* handle);

/*! Gets the value of the variable 'count' that is defined in the default interface scope. */ 
extern sc_integer tictocIface_get_count(const Tictoc* handle);
/*! Sets the value of the variable 'count' that is defined in the default interface scope. */ 
extern void tictocIface_set_count(Tictoc* handle, sc_integer value);

/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean tictoc_isActive(const Tictoc* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean tictoc_isFinal(const Tictoc* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean tictoc_isStateActive(const Tictoc* handle, TictocStates state);

extern void tictoc_accept_message(Tictoc* handle, char* text);

#ifdef __cplusplus
}
#endif 

#endif /* TICTOC_H_ */
