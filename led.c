/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Led gpio setup.
*/

#include "led.h"
#include "pin_config.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

/*---------------------------------------------------------------------------*/
/** @brief Led gpio setup

Led gpio setup for game leds and heart beet led.
*/

void led_gpio_setup(void)
{
    /* Enable GPIOA clock. */
    rcc_periph_clock_enable(PORT_LED_PERIPH);

    /* Set PORT_LED->PIN_LED_YELLOW to 'output push-pull'. */
    gpio_set_mode(PORT_LED, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, PIN_LED_YELLOW);

    /* Set PORT_LED->PIN_LED_BLUE to 'output push-pull'. */
    gpio_set_mode(PORT_LED, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, PIN_LED_BLUE);

    /* Set PORT_LED->PIN_LED_RED to 'output push-pull'. */
    gpio_set_mode(PORT_LED, GPIO_MODE_OUTPUT_2_MHZ,
              GPIO_CNF_OUTPUT_PUSHPULL, PIN_LED_RED);

    /* Set PORT_LED->PIN_LED_GREEN to 'output push-pull'. */
    gpio_set_mode(PORT_LED, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, PIN_LED_GREEN);
    
    /* Set PORT_LED_HEART_BEET->PIN_LED_HEART_BEET to 'output push-pull'. */
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_set_mode(PORT_LED_HEART_BEET, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, PIN_LED_HEART_BEET);
}
