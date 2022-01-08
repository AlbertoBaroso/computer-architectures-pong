/****************************************Copyright*(c)*****************************************************
**
**                                 http://www.powermcu.com
**
**-------------- File Info -------------------------------------------------------------------------------
** File name:               sample.c
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
** Descriptions:            The single-player PONG game
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
#include "utility/utility.h"
#include "button_EXINT/button.h"
#include "game/game.h"

#ifdef SIMULATOR
		extern uint8_t ScaleFlag;
#endif

int main(void) {

    uint32_t rit_time;
    uint32_t timer0_time;
    uint32_t timer1_time;

		/* K = Freq[1/s] * Time[s] */
		/* TIMING CALCULATION */
	
     rit_time = 0x4C4B40;
    timer0_time = 0x98968;
		timer1_time = 0x4C4B40;
    #ifdef SIMULATOR
        timer0_time = 2500000 / ball_fps; 
				timer1_time = 2500000 / paddle_fps;
    #endif
	
		/* PERIPHERALS INITIALIZATION */
	
		SystemInit();
		init_DAC();
    ADC_init();
		ADC_start_conversion();						// Start conversion (avoids first 0 result)
		LCD_Initialization();
		play_sound(wall_bounce_sound);		// Initial sound
		BUTTON_init();	
    init_RIT(rit_time);
    init_timer(0, timer0_time, 0);   	// Timer 0: Ball movement
    init_timer(1, timer1_time, 0);    // Timer 1: Paddle movement

		enable_RIT();   									// RIT: Debouncing buttons

		game_init();											// Initialize GUI

		/* Power-Down Mode */

		LPC_SC->PCON |= 0x1; 
    LPC_SC->PCON &= ~(0x2);

    while (1) {
        __ASM("wfi");
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
