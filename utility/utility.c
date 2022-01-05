
#include "utility.h"

#include "../GUI/GUI.h"
#include "../const/constants.h"
#include "../timer/timer.h"

void game_over(void) {
  int half_field_width = (field_width >> 1) - 4 * 8;  // 8 lenght of "YOU LOSE"
  int half_field_height = (field_height >> 1) - 8;
	disable_timer(0);
	disable_timer(1);
	
	game_status = NOT_PLAYING;

  /* Print game over message */
  GUI_Text(half_field_width, half_field_height, (uint8_t *)"YOU LOSE",
           text_color, background_color);

  /* TODO: Reset game */

  /* Clear paddle */
  draw_rectangle(0, paddle_y, field_width, paddle_y + paddle_height,
                 background_color);
}


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
