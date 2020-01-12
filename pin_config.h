/**
@brief Simon game

@version 1.0.0

@author Marcelo Camargo <camargo.advanced@gmail.com>

@date 23 December 2019

Pin definitions.
*/

#ifndef pin_config_h
#define pin_config_h

/* Pin configuration - Leds */
#define PORT_LED            GPIOA
#define PORT_LED_PERIPH     RCC_GPIOA
#define PIN_LED_YELLOW      GPIO4  /* GPIOA->GPIO04 = LED YELLOW  */
#define PIN_LED_BLUE        GPIO5  /* GPIOA->GPIO05 = LED BLUE   */
#define PIN_LED_RED         GPIO6  /* GPIOA->GPIO06 = LED RED     */
#define PIN_LED_GREEN       GPIO7  /* GPIOA->GPIO07 = LED GREEN   */

/* Pin configuration - Heart beet led */
#define PORT_LED_HEART_BEET          GPIOC
#define PORT_LED_HEART_BEET_PERIPH   RCC_GPIOC
#define PIN_LED_HEART_BEET           GPIO13  /* GPIOC->GPIO13 = LED HEART BEET */

/* Pin configuration - Push buttons */
#define PORT_PUSHB          GPIOB
#define PORT_PUSHB_PERIPH   RCC_GPIOB
#define PIN_PUSHB_YELLOW    GPIO6  /* GPIOB->GPIO06 = PUSH YELLOW */
#define EXTI_PUSHB_YELLOW   EXTI6  /* isr for yellow push button */
#define PIN_PUSHB_BLUE      GPIO7  /* GPIOB->GPIO07 = PUSH BLUE  */
#define EXTI_PUSHB_BLUE     EXTI7  /* isr for blue push button */
#define PIN_PUSHB_RED       GPIO8  /* GPIOB->GPIO08 = PUSH RED    */
#define EXTI_PUSHB_RED      EXTI8  /* isr for red push button */
#define PIN_PUSHB_GREEN     GPIO9  /* GPIOB->GPIO09 = PUSH GREEN  */
#define EXTI_PUSHB_GREEN    EXTI9  /* isr for green push button */

#endif
