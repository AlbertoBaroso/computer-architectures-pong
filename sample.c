/****************************************Copyright*(c)*****************************************************
**
**                                 http://www.powermcu.com
**
**--------------File
*Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi & Alberto Baroso
** Modified date:           2022
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching &
*excercising
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "GLCD/GLCD.h"
#include "GUI/GUI.h"
#include "LPC17xx.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "adc/adc.h"
#include "const/constants.h"

#ifdef SIMULATOR
extern uint8_t
    ScaleFlag;  // <- ScaleFlag needs to visible in order for the emulator to
                // find the symbol (can be placed also inside system_LPC17xx.h
                // but since it is RO, it needs more work)
#endif

int main(void) {

	uint32_t rit_time = 0x004C4B40;
	uint32_t timer0_time = 0x00098968;
	// TODO: CALCULATE TIMES BASED ON fps "CONSTANT"
	#ifdef SIMULATOR
		rit_time = 0x001C4B40;
		timer0_time = 0x08968;
	#endif
	
	game_status = NOT_PLAYING;
	
	SystemInit(); /* System Initialization (i.e., PLL)  */

  LCD_Initialization();
  init_GUI();
  ADC_init();           /* ADC Initialization 					*/
  init_RIT(rit_time); 
	init_timer(0, timer0_time, 0);
	
  enable_RIT();         /* RIT enabled         						*/
	enable_timer(0);	// Timer 0: Move ball
	

  // init_timer(0, 0x1312D0 ); 						/* 50ms
  // * 25MHz = 1.25*10^6 = 0x1312D0 */ init_timer(0, 0x6108 );
  // /* 1ms * 25MHz = 25*10^3 = 0x6108 */ init_timer(0, 0x4E2 );
  // /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
  //init_timer(0, 0xC8); /* 8us * 25MHz = 200 ~= 0xC8 */
  //enable_timer(0);

  LPC_SC->PCON |= 0x1; /* power-down	mode
                        */
  LPC_SC->PCON &= ~(0x2);

  while (1) {
    __ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
