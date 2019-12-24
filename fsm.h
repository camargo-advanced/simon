/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Definitions and function prototypes.
*/

#ifndef fsm_h
#define fsm_h

#include "FreeRTOS.h"

/* structure to hold event information
received from the game */
typedef struct Event Event;
struct Event {
    uint32_t pushb_port;
    uint16_t pushb_pin;
    uint32_t led_port;
    uint16_t led_pin;    
};

/* structure to hold state information */
typedef struct State State;
struct State {
    uint32_t pushb_port;
    uint16_t pushb_pin;  /* button expected to be pressed on this state */
    uint32_t led_port;
    uint16_t led_pin;    /* led expected to be active on this state */
    uint16_t frequency;  /* sound frequency to be played */
    uint8_t sequence;    /* state sequencial number */
    struct State * next; /* pointer to the next state */
};

/* structure that represents the finite state machine */
typedef struct Fsm Fsm;
struct Fsm {
    uint8_t phase;                 /* current phase */
    struct State * head_state;     /* pointer to linked list of states */
    struct State * current_state;  /* pointer to current state */
};

/* possible outcomes when transitioning states */
typedef enum
{
    Phase_advance,  /* user has advanced to next phase */
    State_advance,  /* user has advanced to next state */
    Fsm_completed,  /* user has completed the fsm sucessfully */
    Fsm_failed      /* user has failed the fsm */
} Transition_status;

/* function prototypes */
Fsm * create_fsm(void);
State * add_state_fsm(Fsm * fsm, uint32_t pushb_port, uint16_t pushb_pin,
    uint32_t led_port, uint16_t led_pin, uint16_t frequency);
Transition_status event_handler_fsm(Fsm * fsm, Event * e);
void destroy_fsm(Fsm * p);

#endif
