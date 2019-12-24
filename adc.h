/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Definitions and function prototypes.
*/

#ifndef adc_h
#define adc_h

#include "FreeRTOS.h"

/* function prototypes */
void adc_setup(void);
uint16_t read_adc(uint8_t channel);

#endif 
