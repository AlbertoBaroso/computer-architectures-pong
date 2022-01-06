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

int positive_value(int value) {
		if (value < 0)
			value = -value;
		return value;
}

void play_sound(int sound) {
	reset_timer(2);
	init_timer(2, freqs[sound], 0);
	enable_timer(2);
}

void init_DAC() {
		LPC_PINCON->PINSEL1 |= (1<<21);
	  LPC_PINCON->PINSEL1 &= ~(1<<20);
	  LPC_GPIO0->FIODIR |= (1<<26);
}

int paddles_overlap(uint16_t current_position, uint16_t last_position) {
	return (current_position > last_position && current_position < last_position + paddle_width) ||
	   (last_position > current_position && last_position < current_position + paddle_width);
}

int in_field(uint16_t position) {
	return position >= field_border && position < field_width - field_border;
}
