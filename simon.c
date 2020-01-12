/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Implementation of simon game.
*/

#include "simon.h"
#include "task.h"
#include "pin_config.h"
#include "adc.h"
#include "sound.h"
#include <libopencm3/stm32/gpio.h>

/* prototypes of private functions */
static void play_sequence_simon(Simon const * const simon);
static void generate_new_game_sequence(Simon * const simon);

/*---------------------------------------------------------------------------*/
/** @brief Simon game constructor

Builds the Simon game object which includes the welcome sequence.

@param[in] level Unsigned int8. @ref level
@returns Simon *: new object instance.
*/

Simon * create_simon(uint8_t level)
{
    /* allocate memory for Simon object */
    Simon * simon = (Simon *) pvPortMalloc(sizeof(Simon));
    
    /* there is no fsm yet */
    simon->fsm = NULL;
    
    /* set the game level */
    simon->level = level;
    
    /* timing control */
    simon->tone_duration = WAIT_420_MS;
    simon->pause_between_tones = WAIT_50_MS;
        
    /* generates welcome sequence */
    simon->led_sequence[0] = PIN_LED_YELLOW;
    simon->led_sequence[1] = PIN_LED_BLUE;
    simon->led_sequence[2] = PIN_LED_GREEN;
    simon->led_sequence[3] = PIN_LED_RED;
    
    /* return pointer to Simon object */
    return simon;
}

/*---------------------------------------------------------------------------*/
/** @brief starts a new game

Starts a new game, creates a new fsm.

@param[in] simon Simon *. @ref simon
*/

void setup_new_game_simon(Simon * const simon)
{
    /* destroy fsm object if it already exists */
    if (simon->fsm) destroy_fsm(simon->fsm);
    
    /* creates finite state machine object */
    simon->fsm = create_fsm();

    /* add state sequence to fsm object */
    generate_new_game_sequence(simon);

    /* play the sequence for current phase */
    play_sequence_simon(simon);
}

/*---------------------------------------------------------------------------*/
/** @brief Create a new game sequence

Create a new game sequence.

@param[in] simon Simon *. @ref simon
*/

static void generate_new_game_sequence(Simon * const simon)
{
    uint16_t random;
        
    for (int8_t i = 0; i < simon->level; i++)
    {
        /* read ADC channels 0 and 1 and sum them to make random number */
        random = read_adc(1) + read_adc(0) + read_adc(1);
     
        /* use only the least 2 significant bits to get a random
        number between 0 and 3 */
        switch (random & 0b00000011)
        {
            case 0:
                add_state_fsm(simon->fsm, PORT_PUSHB, PIN_PUSHB_YELLOW,
                              PORT_LED, PIN_LED_YELLOW, C4_TONE); break;
            case 1:
                add_state_fsm(simon->fsm, PORT_PUSHB, PIN_PUSHB_BLUE,
                              PORT_LED, PIN_LED_BLUE, G3_TONE); break;
            case 2:
                add_state_fsm(simon->fsm, PORT_PUSHB, PIN_PUSHB_GREEN,
                              PORT_LED, PIN_LED_GREEN, G4_TONE); break;
            case 3:
                add_state_fsm(simon->fsm, PORT_PUSHB, PIN_PUSHB_RED,
                              PORT_LED, PIN_LED_RED, E4_TONE); break;
        }
    }
}

/*---------------------------------------------------------------------------*/
/** @brief Play the sequence

Play the sequence for current phase.

@param[in] simon Simon *. @ref simon
*/

static void play_sequence_simon(Simon const * const simon)
{
    /* leds should start turned off */
    for (int8_t i = 0; i < NUM_LEDS_WIRED; i++)
        gpio_set(PORT_LED, simon->led_sequence[i]);

    /* delay a bit before starting the sequence */
    vTaskDelay(pdMS_TO_TICKS(WAIT_800_MS));

    /* play sequence */
    State * state = simon->fsm->head_state;
    for (uint8_t i=0; state && i<simon->fsm->phase; i++)
    {
        /* turn on led, start sound and wait a bit */
        gpio_clear(state->led_port, state->led_pin);
        start_sound_frequency(state->frequency);
        vTaskDelay(pdMS_TO_TICKS(simon->tone_duration));

        /* turn off led, stop sound and wait a bit */
        gpio_set(state->led_port, state->led_pin);
        stop_sound();
        vTaskDelay(pdMS_TO_TICKS(simon->pause_between_tones));

        /* go to next state, if any */
        state = state->next;
    }
}

/*---------------------------------------------------------------------------*/
/** @brief Play welcome sequence

Play welcome sequence.

@param[in] simon Simon *. @ref simon
*/

void play_welcome_sequence_simon(Simon const * const simon)
{
    /* leds should start turned off */
    for (int8_t i = 0; i < NUM_LEDS_WIRED; i++)
        gpio_set(PORT_LED, simon->led_sequence[i]);

    /* play sequence */
    for (int8_t i = 0; i < NUM_LEDS_WIRED; i++)
    {
        gpio_toggle(PORT_LED, simon->led_sequence[i]);
        vTaskDelay(pdMS_TO_TICKS(WAIT_50_MS));
        gpio_toggle(PORT_LED, simon->led_sequence[i]);
    }
}

/*---------------------------------------------------------------------------*/
/** @brief Makes an event object

Makes an event object to be processed by the fsm based on interrupt details received from isr.
 
@param[in] simon Simon *. @ref simon
@param[in] interrupt Interrupt type. @ref interrupt
*/

void interrupt_handler_simon(Simon * const simon, Interrupt interrupt)
{
    Event event;

    /* All push buttons are physically wired to the same port */
    event.pushb_port = PORT_PUSHB;

    /* All leds are physically wired to the same port */
    event.led_port = PORT_LED;

    /* complete filling the event object */
    if (interrupt.bits.pin_pushb_yellow)
    {
        event.pushb_pin = PIN_PUSHB_YELLOW;
        event.led_pin = PIN_LED_YELLOW;
        if (interrupt.bits.falling_edge)
        {
            gpio_clear(PORT_LED, PIN_LED_YELLOW);
            start_sound_frequency(C4_TONE);
        }
        else
        {
            gpio_set(PORT_LED, PIN_LED_YELLOW);
            stop_sound();
            event_handler_simon(simon, &event);
        }
    }
    else if (interrupt.bits.pin_pushb_blue)
    {
        event.pushb_pin = PIN_PUSHB_BLUE;
        event.led_pin = PIN_LED_BLUE;
        if (interrupt.bits.falling_edge)
        {
            gpio_clear(PORT_LED, PIN_LED_BLUE);
            start_sound_frequency(G3_TONE);
        }
        else
        {
            gpio_set(PORT_LED, PIN_LED_BLUE);
            stop_sound();
            event_handler_simon(simon, &event);
        }
    } else if (interrupt.bits.pin_pushb_red)
    {
        event.pushb_pin = PIN_PUSHB_RED;
        event.led_pin = PIN_LED_RED;
        if (interrupt.bits.falling_edge)
        {
            gpio_clear(PORT_LED, PIN_LED_RED);
            start_sound_frequency(E4_TONE);
        }
        else {
            gpio_set(PORT_LED, PIN_LED_RED);
            stop_sound();
            event_handler_simon(simon, &event);
        }
    }
    else if (interrupt.bits.pin_pushb_green)
    {
        event.pushb_pin = PIN_PUSHB_GREEN;
        event.led_pin = PIN_LED_GREEN;
        if (interrupt.bits.falling_edge)
        {
            gpio_clear(PORT_LED, PIN_LED_GREEN);
            start_sound_frequency(G4_TONE);
        }
        else
        {
            gpio_set(PORT_LED, PIN_LED_GREEN);
            stop_sound();
            event_handler_simon(simon, &event);
        }
    }
}

/*---------------------------------------------------------------------------*/
/** @brief Handle the user event

Handle the use event by executing the fsm transition and handling the game actions required.
 
@param[in] simon Simon *. @ref simon
@param[in] e Event type. @ref e
*/

void event_handler_simon(Simon * const simon, Event const * const e)
{
    /* execute the fsm transition */
    Transition_status ts = event_handler_fsm(simon->fsm, e);

    switch (ts)
    {
        case Phase_advance:
            /* increases the speed as time goes on */
            if (simon->fsm->phase > 5 && simon->fsm->phase <= 13)
                simon->tone_duration = WAIT_320_MS;
            else if (simon->fsm->phase > 14 && simon->fsm->phase <= 31)
                simon->tone_duration = WAIT_220_MS;

            /* plays the sequence with the additional led */
            play_sequence_simon(simon);
            break;
        case State_advance:
            /* nothing to do here */
            break;
        case Fsm_completed:
            /* display winner sequence */
            vTaskDelay(pdMS_TO_TICKS(WAIT_800_MS));
            gpio_clear(e->led_port, e->led_pin);
            start_sound_frequency(simon->fsm->current_state->frequency);
            vTaskDelay(pdMS_TO_TICKS(WAIT_20_MS));
            gpio_set(e->led_port, e->led_pin);
            stop_sound();
            vTaskDelay(pdMS_TO_TICKS(WAIT_20_MS));
            for (uint8_t i=0; i<9; i++)
            {
                gpio_clear(e->led_port, e->led_pin);
                start_sound_frequency(simon->fsm->current_state->frequency);
                vTaskDelay(pdMS_TO_TICKS(WAIT_70_MS));
                gpio_set(e->led_port, e->led_pin);
                stop_sound();
                vTaskDelay(pdMS_TO_TICKS(WAIT_20_MS));
            }
            setup_new_game_simon(simon);
            break;
        case Fsm_failed:
            /* display game over sequence */
            vTaskDelay(pdMS_TO_TICKS(WAIT_800_MS));
            gpio_clear(e->led_port, e->led_pin);
            start_sound_frequency(LOSING_TONE);
            vTaskDelay(pdMS_TO_TICKS(WAIT_1500_MS));
            gpio_set(e->led_port, e->led_pin);
            stop_sound();
            vTaskDelay(pdMS_TO_TICKS(WAIT_50_MS));
            setup_new_game_simon(simon);
            break;
    }
}

/*---------------------------------------------------------------------------*/
/** @brief Destructor

Destroy the fsm object and the simon object.
 
@param[in] simon Simon *. @ref simon
*/

void destroy_simon(Simon * simon)
{
    destroy_fsm(simon->fsm);
    vPortFree(simon);
}
