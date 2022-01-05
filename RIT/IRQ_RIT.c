/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage RIT interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include "RIT.h"
#include "lpc17xx.h"
#include "../button_EXINT/button.h"

extern int button0_pressed;
extern int button1_pressed;
extern int button2_pressed;

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:	None
**
******************************************************************************/

void RIT_IRQHandler (int button)
{			
	static int down = 0;	
	//int value;
	down++;
	
	if((button0_pressed == 1 && (LPC_GPIO2->FIOPIN & (1<<10)) == 0) || 
		 (button1_pressed == 1 && (LPC_GPIO2->FIOPIN & (1<<11)) == 0) || 
	   (button2_pressed == 1 && (LPC_GPIO2->FIOPIN & (1<<12)) == 0)) {		/* button pressed */
		reset_RIT();
		switch(down) {
			case 1:
				if(button0_pressed == 1 && (LPC_GPIO2->FIOPIN & (1<<10)) == 0)
					button0_action();
				if(button1_pressed == 1 && (LPC_GPIO2->FIOPIN & (1<<11)) == 0)
					button1_action();
				if(button2_pressed == 1 && (LPC_GPIO2->FIOPIN & (1<<12)) == 0)
					button2_action();
			break;
			default:
				break;
		}
	} else {					/* button released */
		down = 0;			
		disable_RIT();
		reset_RIT();
		
		if(button0_pressed) {
				button0_pressed = 0;
				NVIC_EnableIRQ(EINT0_IRQn);								/* disable Button interrupts					*/
				LPC_PINCON->PINSEL4    |= (1 << 20);     	/* External interrupt 0 pin selection */
		}
		if(button1_pressed) {	
				button1_pressed = 0;		
				NVIC_EnableIRQ(EINT1_IRQn);							 	/* disable Button interrupts					*/
				LPC_PINCON->PINSEL4    |= (1 << 22);     	/* External interrupt 1 pin selection */	
		}
		if(button2_pressed) {
				button2_pressed = 0;		
				NVIC_EnableIRQ(EINT2_IRQn);								/* disable Button interrupts					*/
				LPC_PINCON->PINSEL4    |= (1 << 24);     	/* External interrupt 2 pin selection */	
		}	
	}
		
  LPC_RIT->RICTRL |= 0x1;															/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
