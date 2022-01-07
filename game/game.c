/*******************************************************************
** ########################### File Info ###########################
**
** File name:           game.c
** Last modified Date:  2022/01
** Descriptions:        Functions to manage the gameplay
** Correlated files:    game.h
**
** #################################################################
*******************************************************************/

#include "LPC17xx.h"
#include "game.h"
#include "../GLCD/GLCD.h"
#include "../GUI/GUI.h"
#include "../timer/timer.h"
#include "../const/constants.h"
#include "../ADC/ADC.h"
#include <stdio.h>

uint32_t score = 100;
uint32_t score_record = 100;

uint32_t old_score = 0;						// Old scores, used for efficient character replacing on screen
uint32_t old_score_record = 100;

unsigned int game_status = READY_TO_PLAY;
extern unsigned short measure_index;
extern unsigned short measures_filled;

/*******************************************************************
 ** Function name:       game_init
 ** Descriptions:        Initialize the game
 ** Input parameters:    None
 ** Returned value:      None
 *******************************************************************/
void game_init() {
	
  init_GUI();
	
}

/*******************************************************************
 ** Function name:       game_start
 ** Descriptions:        Start the game
 ** Input parameters:    None
 ** Returned value:      None
 *******************************************************************/
void game_start() {
	
	static uint16_t time = 0;
	
	/* Reset values */
	ball_x_position = 204;
	ball_y_position = 129;
	ball_x_direction = 2;
	ball_y_direction = 2;
  game_status = PLAYING;
	
	ADC_start_conversion();
	
	if(time == 0) {
		/* Cancel "Press KEY 1 to Start" text */
		draw_rectangle((field_width >> 1) - 80, (field_height >> 1) - 40, 
									 (field_width >> 1) + 80, (field_height >> 1) - 24, background_color);
			time++;
	} else {
				int last_position = 0; 
				if(paddle_x <= paddle_width + 1)
					 last_position = field_width - paddle_width - 2;
				draw_paddle(last_position, paddle_x);	
	}
	
	/* Draw ball in initial position */
	draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
								 ball_y_position + ball_height, ball_color);
	
	/* ENABLE TIMERS */
	
  enable_timer(0);	// Timer 0: Move ball
  enable_timer(1);	// Timer 1: Move paddle (start ADC conversion)
	
}

/*******************************************************************
 ** Function name:       game_over
 ** Descriptions:        End the game
 ** Input parameters:    None
 ** Returned value:      None
 *******************************************************************/
void game_over() {
	
	int half_field_width = (field_width >> 1) - 8 * 8;  // 8 lenght of "YOU LOSE"
  int half_field_height = (field_height >> 1) - 8;
	int score_length = snprintf(NULL, 0, "%d", score);
	int score_record_length = snprintf(NULL, 0, "%d", score_record);
	
	disable_timer(0);
	disable_timer(1);
	
	game_status = GAME_ENDED;

	/* Clear score texts */
  draw_rectangle(field_width - 10 - 8 * score_record_length * score_record_text_size, 10, field_width - 10 + 1, 10 + 16 * score_record_text_size + 1, background_color);
  draw_rectangle(10, 150, 10 + 8 * score_length * score_text_size + 1, 150 + 16 * score_text_size + 1, background_color);
	
	/* Cancel old ball */
	draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
								 ball_y_position + ball_height, background_color);
	
  /* Clear paddle */
  draw_rectangle(0, paddle_y, field_width, paddle_y + paddle_height,
                 background_color);
	
  /* Print game over message */
  GUI_Text(half_field_width, half_field_height, (uint8_t *)"YOU LOSE",
           text_color, background_color, you_lose_text_size);

	score = 0;
}

