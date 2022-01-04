
#include "utility.h"

#include "../GUI/GUI.h"
#include "../const/constants.h"

void game_over(void) {
  int half_field_width = field_width / 2 - 4 * 9;  // 9 lenght of "GAME OVER"
  int half_field_height = field_height / 2 - 8;

  /* Print game over message */
  GUI_Text(half_field_width, half_field_height, (uint8_t *)"GAME OVER",
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
