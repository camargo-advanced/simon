/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Definitions and function prototypes.
*/

#ifndef simon_h
#define simon_h

#include "FreeRTOS.h"
#include "fsm.h"

/* total number of leds wired in the hardware */
#define NUM_LEDS_WIRED 4

/* Skill levels */
#define LEVEL_1 8
#define LEVEL_2 14
#define LEVEL_3 20
#define LEVEL_4 31

/* time between led sequences */
#define WAIT_1500_MS 1500
#define WAIT_1000_MS 1000
#define WAIT_800_MS 800
#define WAIT_500_MS 500
#define WAIT_420_MS 420
#define WAIT_320_MS 320
#define WAIT_220_MS 220
#define WAIT_250_MS 250
#define WAIT_70_MS  70
#define WAIT_50_MS  50
#define WAIT_25_MS  25
#define WAIT_20_MS  20

/* interrupt bit field structure */
struct Interrupt_Data {
    uint32_t pin_pushb_yellow : 1;
    uint32_t pin_pushb_blue   : 1;
    uint32_t pin_pushb_red    : 1;
    uint32_t pin_pushb_green  : 1;
    uint32_t falling_edge     : 1;
};

/* interrupt class definition */
typedef union Interrupt Interrupt;
union Interrupt {
    uint32_t value;
    struct Interrupt_Data bits;
};
  
/* simon class definition */
typedef struct Simon Simon;
struct Simon
{
    uint16_t led_sequence[NUM_LEDS_WIRED]; /* for welcome sequence */
    uint8_t level;                         /* game level defines the sequence lenght */
    uint16_t tone_duration;                /* time to keep the tone and led on */
    uint16_t pause_between_tones;          /* time between tones-leds */
    Fsm * fsm;                             /* pointer to the finite state machine */
};

/* function prototypes */
Simon * create_simon(uint8_t level);
void destroy_simon(Simon * p);
void play_welcome_sequence_simon(Simon * simon);
void event_handler_simon(Simon * simon, Event * e);
void setup_new_game_simon(Simon * simon);
void interrupt_handler_simon(Simon * simon, Interrupt interrupt);

#endif
