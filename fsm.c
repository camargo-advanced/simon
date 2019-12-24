/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Finite state machine implementation - FSM.
*/

#include "fsm.h"
#include "sound.h"

/* prototypes of private functions */
static void destroy_state_fsm(State * state);

/*---------------------------------------------------------------------------*/
/** @brief Fsm constructor.

Fsm constructor - allocates memory for fsm object.
*/

Fsm * create_fsm(void)
{
    /* Fsm initialization */
    Fsm * fsm = (Fsm *) pvPortMalloc(sizeof(Fsm));

    /* starts on phase 1 */
    fsm->phase = 1;
    
    /* state sequence will be added later */
    fsm->head_state = NULL;
    fsm->current_state = NULL;
    
    /* return pointer to Simon structure variable */
    return fsm;
}

/*---------------------------------------------------------------------------*/
/** @brief Add a state object to the liked list

Add a state object to the liked list.

@param[in] fsm Fsm *. @ref fsm
@param[in] pushb_port Unsigned int32. @ref pushb_port
@param[in] pushb_pin Unsigned int16. @ref pushb_pin
@param[in] led_port Unsigned int16. @ref led_port
@param[in] led_pin Unsigned int16. @ref led_pin
@param[in] frequency Unsigned int16. @ref frequency
@returns State *: pointer to fsm object.
*/

State * add_state_fsm(Fsm * fsm, uint32_t pushb_port, uint16_t pushb_pin,
                      uint32_t led_port, uint16_t led_pin, uint16_t frequency)
{
    State * p = NULL;

    if (fsm->head_state == NULL)
    {
        /* this is the frist state object */
        p = (State *) pvPortMalloc(sizeof(State));
        fsm->head_state = p;
        fsm->current_state = p;
        fsm->head_state->sequence = 1;
    }
    else
    {
        /* navigates to the last node and add a new state object */
        State * n = fsm->head_state;
        for (;n;) { p = n; n = n->next; }
        p->next = (State *) pvPortMalloc(sizeof(State));
        p->next->sequence = p->sequence + 1;
        p = p->next;
    }
    p->next = NULL;

    /* update push button and led states */
    p->pushb_port = pushb_port;
    p->pushb_pin = pushb_pin;
    p->led_port = led_port;
    p->led_pin = led_pin;
    p->frequency = frequency;

    return p;
}

/*---------------------------------------------------------------------------*/
/** @brief Handler to game events

Handle all game events, in this case push buttons events.

@param[in] fsm Fsm *. @ref fsm
@param[in] e Event *. @ref e
@returns Transition_status: Transition_status enumeration.
*/

Transition_status event_handler_fsm(Fsm * fsm, Event * e)
{
    if (fsm->current_state != NULL &&
        fsm->current_state->pushb_port == e->pushb_port &&
        fsm->current_state->pushb_pin == e->pushb_pin)
    {
        /* check if we have a winner */
        if (fsm->current_state->next == NULL)
        {
            return Fsm_completed;
        }

        /* phase advance */
        if (fsm->current_state->sequence == fsm->phase)
        {
            fsm->phase++;
            fsm->current_state = fsm->head_state;
            return Phase_advance;
        }

        /* state advance */
        fsm->current_state = fsm->current_state->next;
        return State_advance;
    }

    /* did not make it... */
    return Fsm_failed;
}

/*---------------------------------------------------------------------------*/
/** @brief Frees up memory of a state object

Frees up memory of a state object.

@param[in] state State *. @ref state
*/

static void destroy_state_fsm(State * state)
{
    vPortFree(state);
}

/*---------------------------------------------------------------------------*/
/** @brief Frees up memory of a fsm linked list

Frees up memory of a fsm linked list.

@param[in] fsm Fsm *. @ref fsm
*/

void destroy_fsm(Fsm * fsm)
{
    State * p = fsm->head_state;
    for (State * pf;p;)
    {
        pf = p;
        p = p->next;
        destroy_state_fsm(pf);
    }
    vPortFree(fsm);
}
