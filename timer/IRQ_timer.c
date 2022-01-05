/*********************************************************************************************************
**--------------File
*Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include "../const/constants.h"
#include "../utility/utility.h"
#include "../GLCD/GLCD.h"
#include "../GUI/GUI.h"
#include "../adc/adc.h"
#include "lpc17xx.h"
#include "timer.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler(void) {
  // TODO: CHECK GAME MODE IS ON

  draw_ball();

  LPC_TIM0->IR = 1; /* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler(void) {

  /* ADC management */
  ADC_start_conversion();

	LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler(void) {
  static int ticks = 0;
	static int repetition = 0;

	/* DAC management */	
	LPC_DAC->DACR = SinTable[ticks] << 6;
	ticks++;
	
	if(ticks == 45) {
		repetition++;
		ticks = 0; 
		if(repetition == sound_lenght) {
			disable_timer(2);
			repetition = 0;
		}
	}

	LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
 ** Function name:		Timer3_IRQHandler
 **
 ** Descriptions:		Timer/Counter 3 interrupt handler
 **
 ** parameters:			None
 ** Returned value:		None
 **
 ******************************************************************************/
void TIMER3_IRQHandler (void)
{
	uint32_t interruptRegister = LPC_TIM3->IR;			/* get interrupt flag 				*/

	// if(interruptRegister & (1<<0))					/* if interrupt flag 0 (MR0) is set */
	// {
	// 	LPC_TIM3->IR |= (1<<0);							/* clear interrupt flag 			*/
	// }

	//if(interruptRegister & (1<<1))						/* if interrupt flag 1 (MR1) is set */
	//{
	//	LPC_TIM3->IR |= (1<<1);							/* clear interrupt flag 			*/
	//}
	return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
