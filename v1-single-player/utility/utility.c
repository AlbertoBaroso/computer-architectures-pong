/*******************************************************************
** ########################### File Info ###########################
**
** File name:           utility.c
** Last modified Date:  2022/01
** Descriptions:        Utility functions
** Correlated files:    utility.h
**
** #################################################################
*******************************************************************/

#include "utility.h"
#include "../GUI/GUI.h"
#include "../const/constants.h"
#include "../timer/timer.h"
#include "../game/game.h"

/*******************************************************************
 ** Function name:       positive_value
 ** Descriptions:        Transforms value into a positive number
 ** Input parameters:    int value
 ** Returned value:      int, positive value
 *******************************************************************/
int positive_value(int value) {
		if (value < 0)
			value = -value;
		return value;
}

/*******************************************************************
 ** Function name:       play_sound
 ** Descriptions:        Play sound
 ** Input parameters:    int sound_type, the frequency to be played
 ** Returned value:      None
 *******************************************************************/
void play_sound(int sound) {
	uint32_t timer_freq = freqs[sound];
	reset_timer(2);
	
	#ifdef SIMULATOR
		timer_freq = freqs[sound] / SCALE_FACTOR_APPROXIMATION;
	#endif
	
	init_timer(2, timer_freq, 0);
	enable_timer(2);
}

/*******************************************************************
 ** Function name:      init_DAC
 ** Descriptions:       Initialize DAC
 ** Input parameters:   None
 ** Returned value:     None
 *******************************************************************/
void init_DAC() {
		LPC_PINCON->PINSEL1 |= (1<<21);
	  LPC_PINCON->PINSEL1 &= ~(1<<20);
	  LPC_GPIO0->FIODIR |= (1<<26);
}

/*******************************************************************
 ** Function name:      paddles_overlap
 ** Descriptions:       Check if paddles overlap
 ** parameters:         uint16_t current_position, actual paddle position
 **  					uint16_t last_position, old paddle position
 ** Returned value:     int, 1 if paddles overlap, 0 otherwise
 *******************************************************************/
int paddles_overlap(uint16_t current_position, uint16_t last_position) {
	return (current_position > last_position && current_position < last_position + paddle_width) ||
	   (last_position > current_position && last_position < current_position + paddle_width);
}

/******************************************************************************
** Function name:		in_field
** Descriptions:		Timer/Counter 1 interrupt handler
** parameters:			uint16_t position
** Returned value:		int, 1 if position is in field, 0 if not
******************************************************************************/
int in_field(uint16_t position) {
	return position >= field_border && position < field_width - field_border;
}
