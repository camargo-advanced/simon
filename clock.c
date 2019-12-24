/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Microprocessor clock setup.
*/

#include "clock.h"
#include <libopencm3/stm32/rcc.h>

/*---------------------------------------------------------------------------*/
/** @brief Clock setup.

Clock setup: Set STM32 to 72 MHz.
*/

void clock_setup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
}
