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
#include "utility/utility.h"
#include "button_EXINT/button.h"
#include "game/game.h"

#ifdef SIMULATOR
extern uint8_t
    ScaleFlag;  // <- ScaleFlag needs to visible in order for the emulator to
                // find the symbol (can be placed also inside system_LPC17xx.h
                // but since it is RO, it needs more work)
#endif

int main(void) {

    uint32_t rit_time;
    uint32_t timer0_time;
    uint32_t timer1_time;

		/* K = Freq[1/s] * Time[s] */
	
    rit_time = 0x4C4B40;
    timer0_time = 0x98968;
		timer1_time = 0x4C4B40;
    // TODO: CALCULATE TIMES BASED ON fps "CONSTANT"
    #ifdef SIMULATOR
        //rit_time =  100000000 / paddle_fps;
        timer0_time = 2500000 / ball_fps; 
				timer1_time = 2500000 / paddle_fps; // 6 FPS: FE502A
    #endif

    //game_status = NOT_PLAYING;
	
		SystemInit(); /* System Initialization (i.e., PLL)  */
		BUTTON_init();
    ADC_init();           /* ADC Initialization 					*/
    init_RIT(rit_time);
    init_timer(0, timer0_time, 0);
    init_timer(1, timer1_time, 0);
		init_DAC();

		enable_RIT();   // RIT: Debouncing buttons


		game_init();	// TODO: REMOVE
		game_start();	// TODO: REMOVE
		

		LPC_SC->PCON |= 0x1; /* power-down mode */
    LPC_SC->PCON &= ~(0x2);

    while (1) {
        __ASM("wfi");
    }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
