/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Using a floating ADC input as a source of entropy to get a random number.
*/

#include "adc.h"
#include "task.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/adc.h>

/*---------------------------------------------------------------------------*/
/** @brief Start the sound on a specified frequency

Initializes the ADC 1 hardware. THis function should be called just once, usually at the
beggining of the appliucation by the main() funciton.
*/

void adc_setup(void)
{
    /* ADC peripheral clock needs to be enabled */
    rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_ADC1EN);
    
    /* peripheral should be powered off during initialization */
    adc_power_off(ADC1);
    
    /* reset peripheral further */
    rcc_peripheral_reset(&RCC_APB2RSTR, RCC_APB2RSTR_ADC1RST);
    rcc_peripheral_clear_reset(&RCC_APB2RSTR, RCC_APB2RSTR_ADC1RST);
    
    /* set prescaler to 12 MHz */
    rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV6);
    
    /* set ADC1 and 2 to oerate independently */
    adc_set_dual_mode(ADC_CR1_DUALMOD_IND);
    /* disable scan option, which means to operate in 1 channel only */
    adc_disable_scan_mode(ADC1);
    /* store the result right-justified */
    adc_set_right_aligned(ADC1);
    /* stop when single conversion is completed */
    adc_set_single_conversion_mode(ADC1);
    
    /* power on ADC and wait it to get calibrated */
    adc_power_on(ADC1);
    adc_reset_calibration(ADC1);
    adc_calibrate_async(ADC1);
    while ( adc_is_calibrating(ADC1) );
}

/*---------------------------------------------------------------------------*/
/** @brief Read ADC channel

Read ADC channel specified as parameter to get a random number.

@param[in] channel Unsigned int8. @ref channel
@returns Unsigned int16: Value readed from ADC channel.
*/

uint16_t read_adc(uint8_t channel)
{
    /* set sample time to 21 us */
    adc_set_sample_time(ADC1, channel, ADC_SMPR_SMP_239DOT5CYC);
    
    /* read just 1 channel, the one passed as an argument */
    adc_set_regular_sequence(ADC1, 1, &channel);

    /* start the convertion and wait until it gets completed */
    adc_start_conversion_direct(ADC1);
    while ( !adc_eoc(ADC1) )
        taskYIELD();
    
    /* return the value */
    return adc_read_regular(ADC1);
}
