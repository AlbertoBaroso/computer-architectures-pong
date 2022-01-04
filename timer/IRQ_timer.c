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
#include <stdio.h>
#include <string.h>

#include "../GLCD/GLCD.h"
#include "../GUI/GUI.h"
#include "../const/constants.h"
#include "../utility/utility.h"
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

  /* Cancel old ball */  // TODO: CANCEL ONLY non-overlapping ball
  draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
                 ball_y_position + ball_height, background_color);

  if (ball_y_position + ball_height >= field_height) {
    // GAME OVER
    disable_timer(0);
    game_over();
    return;
  }

  /* Bounce on lateral borders */
  if (ball_x_position <= field_border ||
      ball_x_position + ball_width >= field_width - field_border)
    ball_x_direction = -ball_x_direction;

  /* Bounce on top border */
  if (ball_y_position <= field_border) ball_y_direction = -ball_y_direction;

  /* If ball is at paddle_y and is on top of paddle for at least one pixel =>
   * Bounce on paddle */
  if (ball_y_position + ball_height == paddle_y &&
      ((ball_x_position >= paddle_x &&
        ball_x_position <= paddle_x + paddle_width) ||
       (ball_x_position + ball_width >= paddle_x &&
        ball_x_position + ball_width <= paddle_x + paddle_width))) {
    ball_y_direction = -ball_y_direction;
  }

  /* Move ball */

  ball_x_position += ball_x_direction;
  ball_y_position += ball_y_direction;

  /* Avoid ball to go out of field */

  if (ball_x_position < field_border)
    ball_x_position = field_border;
  else if (ball_x_position + ball_width >= field_width - field_border)
    ball_x_position = field_width - field_border - ball_width;
  if (ball_y_position <= field_border) ball_y_position = field_border;

  /* Draw new ball */

  draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
                 ball_y_position + ball_height, ball_color);

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
  LPC_TIM1->IR = 1; /* clear interrupt flag */
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
  LPC_TIM2->IR = 1; /* clear interrupt flag */
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
