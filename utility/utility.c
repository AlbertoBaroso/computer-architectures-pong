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
