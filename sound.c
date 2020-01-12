/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Using PWM 1 to generate 5 frequencies for the 5 tones required by simon game.
 
 The frequencies are defined in the include file sound.h, as folows:
 -> G4_TONE 391 for  Green
 -> E4_TONE 329 for Red
 -> C4_TONE 261 for Yellow
 -> G3_TONE 195 for Blue
 -> LOSING_TONE 110 in case of a user error

Duty cycle is set as 50%, which means the output is a square signal.
*/

#include "sound.h"
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

/*---------------------------------------------------------------------------*/
/** @brief Start the sound on a specified frequency

Start timer 2 on a specific frequency, setting the duty cycle in microseconds
to 50%, generating a perfect square signal.

@param[in] frequency Unsigned int16. @ref frequency
*/

void start_sound_frequency(uint16_t frequency)
{
    /* set the frequency */
    timer_set_period(TIM2, (36000000*2/72/frequency) - 1);

    /* duty cicle in microseconds */
    timer_set_oc_value(TIM2, TIM_OC2, (1000000/frequency/2));

    /* enable sound */
    timer_enable_counter(TIM2);
    timer_enable_oc_output(TIM2, TIM_OC2);

}

/*---------------------------------------------------------------------------*/
/** @brief Stop the sound

Stop the sound.
*/

void stop_sound(void)
{
    timer_disable_counter(TIM2);
    timer_disable_oc_output(TIM2, TIM_OC2);
}

/*---------------------------------------------------------------------------*/
/** @brief Sound hardware setup.

This function initializes the hardware required for PWM1.
This function must be called just once at the beggining of the application, usually by main().
*/

void sound_setup(void)
{
    /* Enable TIM2 clock */
    rcc_periph_clock_enable(RCC_TIM2);
    
    /* Enable alternate function IO - AFIO
    for using TIM2.CH2 on PA1 */
    rcc_periph_clock_enable(RCC_AFIO);

    /* Need GPIOA clock for PA1 */
    rcc_periph_clock_enable(RCC_GPIOA);

    /* AFIO call - use default mapping where CH2 of TIM2 comes out of PA1 */
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_ON,
        AFIO_MAPR_TIM2_REMAP_NO_REMAP); /* TIM2.CH2=GPIOA1 */

    /* ALTFN macro to connect GPIO pin to the timer for PA1 */
    gpio_set_mode(GPIOA,GPIO_MODE_OUTPUT_50_MHZ, /* High speed */
        GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO1);  /* GPIOA1=TIM2.CH2 */

    /* Reset TIM2 peripheral to defaults */
    timer_disable_counter(TIM2);
    rcc_periph_reset_pulse(RST_TIM2);
    
    /* Timer mode: No divider, Alignment edge, Direction up */
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT,
                   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    
    /* each count of the counter takes 1 us */
    timer_set_prescaler(TIM2, 72-1);
    
    /* ARR register is buffered for better performance */
    timer_enable_preload(TIM2);
    /* keep timer running rather than to stop after one pulse */
    timer_continuous_mode(TIM2);

    /* configure channel OC2 to operate in PWM1 mode */
    timer_disable_oc_output(TIM2, TIM_OC2);
    timer_set_oc_mode(TIM2, TIM_OC2, TIM_OCM_PWM1);
    timer_enable_oc_output(TIM2, TIM_OC2);
}
