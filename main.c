/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Main entry point of the application.
*/

#include "FreeRTOS.h"
#include "simon.h"
#include "task.h"
#include "pushb.h"
#include "pin_config.h"
#include "led.h"
#include "clock.h"
#include "sound.h"
#include "adc.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

/* used for stack overflow error */
uint32_t volatile * const pClockEnablelReg = (uint32_t *) 0x40021018;
uint32_t volatile * const pPorCConfiglHighReg = (uint32_t *) 0x40011004;
uint32_t volatile * const pPortCOutputReg = (uint32_t *) 0x4001100c;

/* prototypes of private functions */
static void task_exti9_5_isr(void *args __attribute((unused)));
static void task_game_welcome(void *args __attribute((unused)));
static void task_heart_beat(void *args __attribute((unused)));
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName);

/* Handlers to FreeRTOS tasks */
TaskHandle_t xHandle_task_exti9_5_isr;
TaskHandle_t xHandle_task_welcome;

/* Global variable for the simon game - private to this file */
static Simon * simon = NULL;

/*---------------------------------------------------------------------------*/
/** @brief Isr for handling push buttons

Isr for handling push buttons.
*/

void exti9_5_isr(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    Interrupt interrupt;

    interrupt.value = 0;

    /* check the interrup flag to identify the button pressed and then
    fill in an interrupt object with the interrupt details, as for
    instance if there is a falling or rising edge */
    if (exti_get_flag_status(EXTI_PUSHB_YELLOW)) /* yellow push button has been pressed */
    {
        interrupt.bits.pin_pushb_yellow = 1;
        interrupt.bits.falling_edge =
            ((gpio_port_read(PORT_PUSHB) & PIN_PUSHB_YELLOW) ^ PIN_PUSHB_YELLOW) ? 1 : 0;
    }
    else if (exti_get_flag_status(EXTI_PUSHB_BLUE)) /* blue push button has been pressed */
    {
        interrupt.bits.pin_pushb_blue = 1;
        interrupt.bits.falling_edge =
            ((gpio_port_read(PORT_PUSHB) & PIN_PUSHB_BLUE) ^ PIN_PUSHB_BLUE) ? 1 : 0;
    }
    else if (exti_get_flag_status(EXTI_PUSHB_RED)) /* red push button has been pressed */
    {
        interrupt.bits.pin_pushb_red = 1;
        interrupt.bits.falling_edge =
            ((gpio_port_read(PORT_PUSHB) & PIN_PUSHB_RED) ^ PIN_PUSHB_RED) ? 1 : 0;
    }
    else if (exti_get_flag_status(EXTI_PUSHB_GREEN)) /* green push button has been pressed */
    {
        interrupt.bits.pin_pushb_green = 1;
        interrupt.bits.falling_edge =
            ((gpio_port_read(PORT_PUSHB) & PIN_PUSHB_GREEN) ^ PIN_PUSHB_GREEN) ? 1 : 0;
    }

    /* clear all interrupts */
    exti_reset_request(EXTI_PUSHB_YELLOW | EXTI_PUSHB_BLUE |
                       EXTI_PUSHB_RED | EXTI_PUSHB_GREEN);

    /* Send a notification directly to the task to which interrupt
    processing is being deferred. */
    xTaskNotifyFromISR(xHandle_task_exti9_5_isr, interrupt.value,
        eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*---------------------------------------------------------------------------*/
/** @brief Task that receives notifications from isr

Task that receives notifications from isr and pushes it to simon game object for handling.
*/

static void task_exti9_5_isr(void *args __attribute__((unused)))
{
    uint8_t first_event_sent = pdTRUE;
    Interrupt interrupt;  /* stores interrupt details */

    for (;;)
    {
        /* ignores old and irrelevant interrupt event
        by removing the existing notification, if any */
        xTaskNotifyWait(0, 0, &interrupt.value, pdMS_TO_TICKS(0));

        /* wait and captures new interrupt event */
        xTaskNotifyWait(0, 0, &interrupt.value, portMAX_DELAY);
        
        if (first_event_sent)
        {
            /* first time the user press a button, the welcome sequence
            will be suspended, the game will be setup and a new led
            sequence displayed to the user */
            first_event_sent = pdFALSE;
            vTaskSuspend(xHandle_task_welcome);
            setup_new_game_simon(simon);
        }
        else
            interrupt_handler_simon(simon, interrupt);
    }
}

/*---------------------------------------------------------------------------*/
/** @brief Task to displays the welcome led sequence

Task to displays the welcome led sequence.
*/

static void task_game_welcome(void *args __attribute__((unused)))
{
    for (;;) play_welcome_sequence_simon(simon);
}

/*---------------------------------------------------------------------------*/
/** @brief Task to toggles the heart beat led

Task to toggles the heart beat led.
*/

static void task_heart_beat(void *args __attribute((unused)))
{
    for (;;)
    {
        gpio_toggle(PORT_LED_HEART_BEET, PIN_LED_HEART_BEET);
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

/*---------------------------------------------------------------------------*/
/** @brief Application entry point.

Main application entry point.
*/

int main(void)
{
    /* hardware configuration and initialization */
	clock_setup();
    led_gpio_setup();
	push_button_exti_setup();
    sound_setup();
    adc_setup();

    /* new simon game object */
    simon = create_simon(LEVEL_1);

    /* creates all tasks */
	xTaskCreate(task_game_welcome, "GAME_STARTUP",
        100, NULL, configMAX_PRIORITIES-1, &xHandle_task_welcome);
	xTaskCreate(task_exti9_5_isr, "PUSHB_ISR",
        100, NULL, configMAX_PRIORITIES-1, &xHandle_task_exti9_5_isr);
    xTaskCreate(task_heart_beat, "HEART_BEAT_LED",
        100, NULL, configMAX_PRIORITIES-1, NULL);

    /* start the scheduler */
	vTaskStartScheduler();

    /* should never get here */
	while (1)
		__asm("nop");

	return 0;
}

/*---------------------------------------------------------------------------*/
/** @brief Stack overflow

This funtion is called if freeRTOS find a stack overflow. We then stop the heart beet to signal something
went wrong to the application programmer.
*/

void vApplicationStackOverflowHook(xTaskHandle *pxTask __attribute((unused)),
    signed portCHAR *pcTaskName __attribute((unused)))
{
    /* enable clock for GPIOC */
    *pClockEnablelReg |= (1 << 4);

    /* configure GPIOC - clear 20th, 21st, 22nd and 23rd bits */
    *pPorCConfiglHighReg &= ~(15 << 20);
    /* set 21st bit */
    *pPorCConfiglHighReg |= (1 << 21);

    while(1)
    {
        /* turn pin 13 off */
        *pPortCOutputReg |= (1 << 13);

        for (int i=0; i<100000; i++) __asm("nop");

        /* turn pin 13 on */
        *pPortCOutputReg &= ~(1 << 13);

        for (int i=0; i<100000; i++) __asm("nop");
    }
    
//    /* stops heart beat on indicating a failure */
//    gpio_clear(PORT_LED, PIN_LED_HEART_BEET);
//
//    while (1) /* Loop forever here */
//        __asm("nop");
}
