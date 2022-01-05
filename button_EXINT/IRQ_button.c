#include "button.h"
#include "lpc17xx.h"
#include "../RIT/RIT.h"

uint8_t old_value = 0;
uint8_t new_value = 1;
volatile int button0_pressed = 0;
volatile int button1_pressed = 0;
volatile int button2_pressed = 0;

void button0_action (void) {	    // RESET COUNT

}

void button1_action (void) {			// INCREMENT COUNT  

}

void button2_action (void) {			// DECREMENT COUNT  	  

}

void EINT0_IRQHandler (void)
{
		button0_pressed = 1;
		enable_RIT();
		NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
		LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}

void EINT1_IRQHandler (void)
{
		button1_pressed = 1;
		enable_RIT();
		NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
		LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)
{
		button2_pressed = 1;
		enable_RIT();
		NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
		LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */
}


