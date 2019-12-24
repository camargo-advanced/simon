/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Definitions and function prototypes.
*/

#ifndef sound_h
#define sound_h

#include "FreeRTOS.h"

/* Simon sound notes - Hz */
#define G4_TONE 391 /*.995*/ /* Green */
#define E4_TONE 329/*.628*/ /* Red */
#define C4_TONE 261/*.626*/ /* Yellow */
#define G3_TONE 195/*.998*/ /* Blue */
#define LOSING_TONE 110/*42*/

/* function prototypes */
void sound_setup(void);
void start_sound_frequency(uint16_t frequency);
void stop_sound(void);

#endif
