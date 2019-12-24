/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Push button and exti setup.
*/

#include "pushb.h"
#include "pin_config.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

/*---------------------------------------------------------------------------*/
/** @brief Push buttons setup

Push buttons and exti setup.
*/

void push_button_exti_setup(void)
{
    /* Enable PORT_PUSHB clock. */
    rcc_periph_clock_enable(PORT_PUSHB_PERIPH);

    /* Enable AFIO clock - EXTI */
    rcc_periph_clock_enable(RCC_AFIO);

    /* Enable EXTI9_5 interrupt. */
    nvic_enable_irq(NVIC_EXTI9_5_IRQ);

    /* Set PORT_PUSHB->PIN_PUSHB_YELLOW to 'input open-drain'. */
    gpio_set_mode(PORT_PUSHB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, PIN_PUSHB_YELLOW);
    /* Configure the EXTI subsystem. */
    exti_select_source(EXTI_PUSHB_YELLOW, PORT_PUSHB);
    exti_set_trigger(EXTI_PUSHB_YELLOW, EXTI_TRIGGER_BOTH);
    exti_enable_request(EXTI_PUSHB_YELLOW);

    /* Set PORT_PUSHB->PIN_PUSHB_BLUE to 'input open-drain'. */
    gpio_set_mode(PORT_PUSHB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, PIN_PUSHB_BLUE);
    /* Configure the EXTI subsystem. */
    exti_select_source(EXTI_PUSHB_BLUE, PORT_PUSHB);
    exti_set_trigger(EXTI_PUSHB_BLUE, EXTI_TRIGGER_BOTH);
    exti_enable_request(EXTI_PUSHB_BLUE);

    /* Set PORT_PUSHB->PIN_PUSHB_RED to 'input open-drain'. */
    gpio_set_mode(PORT_PUSHB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, PIN_PUSHB_RED);
    /* Configure the EXTI subsystem. */
    exti_select_source(EXTI_PUSHB_RED, PORT_PUSHB);
    exti_set_trigger(EXTI_PUSHB_RED, EXTI_TRIGGER_BOTH);
    exti_enable_request(EXTI_PUSHB_RED);

    /* Set PORT_PUSHB->PIN_PUSHB_GREEN to 'input open-drain'. */
    gpio_set_mode(PORT_PUSHB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, PIN_PUSHB_GREEN);
    /* Configure the EXTI subsystem. */
    exti_select_source(EXTI_PUSHB_GREEN, PORT_PUSHB);
    exti_set_trigger(EXTI_PUSHB_GREEN, EXTI_TRIGGER_BOTH);
    exti_enable_request(EXTI_PUSHB_GREEN);
}

