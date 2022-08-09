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
#include "../game/game.h"
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

  if(game_status == PLAYING)
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
  if(game_status == PLAYING)
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

	move_cpu_paddle();
    LPC_TIM2->IR = 1;			          			/* Clear interrupt flag */
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
	uint32_t interruptRegister = LPC_TIM3->IR;	/* get interrupt flag 				*/

	// if(interruptRegister & (1<<0))						/* if interrupt flag 0 (MR0) is set */
	// {
	// 	LPC_TIM3->IR |= (1<<0);									/* clear interrupt flag 			*/
	// }

	//if(interruptRegister & (1<<1))						/* if interrupt flag 1 (MR1) is set */
	//{
	//	LPC_TIM3->IR |= (1<<1);									/* clear interrupt flag 			*/
	//}
	return;
}


/******************************************************************************
**                            End Of File
******************************************************************************/
