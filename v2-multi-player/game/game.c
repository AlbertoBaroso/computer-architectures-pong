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

/* SCORE */

uint32_t top_score = 0;
uint32_t bottom_score = 0;

/* PADDLES POSITIONS */

uint16_t paddle_top_x = 95;
uint16_t paddle_top_y = 26;
uint16_t paddle_bottom_x = 10;
uint16_t paddle_bottom_y = 281;

extern unsigned short measure_index;
extern unsigned short measures_filled;

/* GAME STATUS */

unsigned int game_status = READY_TO_PLAY;

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
	
	int last_position = field_border;
	static uint16_t time = 0;
	
	/* Reset values */
	ball_x_position = initial_ball_x;
	ball_y_position = initial_ball_y;
	ball_x_direction = 3;
	ball_y_direction = 3;
	
  game_status = PLAYING;
	
	ADC_start_conversion();
	
	if(time == 0) {
			time++;
	} else {

				/* Draw bottom paddle */
				last_position = field_border; 	
				if(paddle_bottom_x <= paddle_width + paddle_width + 1)
					 last_position = field_width - paddle_width - field_border - 2;
				draw_paddle(last_position, paddle_bottom_x, BOTTOM_PLAYER);	
	} 
	
	/* Cancel "PONG" text */
	draw_rectangle((field_width >> 1) - 4 * 4 * 2, (field_height >> 2) - 8 * 2, 
								 (field_width >> 1) + 4 * 4 * 2, (field_height >> 2) + 8 * 2, background_color);
	
	/* Cancel "Press KEY 1 to play" text */
	draw_rectangle((field_width >> 1) - 72, (field_height >> 1) - 8, 
								 (field_width >> 1) + 84, (field_height >> 1) + 8, background_color);
	
	/* Draw players scores */
	draw_score(BOTTOM_PLAYER);
	draw_score(TOP_PLAYER);
	
	/* Draw top paddle */
	last_position = field_border; 	
	if(paddle_top_x <= paddle_width + paddle_width + 1)
		 last_position = field_width - paddle_width - field_border - 2;
	draw_paddle(last_position, paddle_top_x, TOP_PLAYER);
	
	/* Draw ball in initial position */
	draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
								 ball_y_position + ball_height, ball_color);
	
	 match_start();
	
}


/*******************************************************************
 ** Function name:       start_match
 ** Descriptions:        Start a new match
 ** Input parameters:    None
 ** Returned value:      None
 *******************************************************************/
void match_start() {
	
	/* Reset values */
	ball_x_position = initial_ball_x;
	ball_y_position = initial_ball_y;
	ball_x_direction = 3;
	ball_y_direction = 3;
	
	/* ENABLE TIMERS */
  enable_timer(0);	// Timer 0: Move ball
  enable_timer(1);	// Timer 1: Move user paddle (start ADC conversion)
  enable_timer(2);	// Timer 2: Move CPU paddle 
}

/*******************************************************************
 ** Function name:       game_over
 ** Descriptions:        End the game
 ** Input parameters:    None
 ** Returned value:      None
 *******************************************************************/
void game_over() {
	
	int half_field_width = (field_width >> 1);
	int bottom_score_length = snprintf(NULL, 0, "%d", bottom_score);
	int top_score_length = snprintf(NULL, 0, "%d", top_score);
	int score_y = (field_height >> 1) - (score_text_size << 3);
	
	disable_timer(0);
	disable_timer(1);
	disable_timer(2);
	
	game_status = GAME_ENDED;

	/* Clear score texts */
	draw_rectangle(score_x, score_y, score_x + 8 * bottom_score_length * score_text_size + 1, score_y + 16 * score_text_size + 1, background_color);
	draw_rectangle(field_width - score_x - (top_score_length * score_text_size * 8) - 5, score_y, field_width - score_x - 5, score_y + 16 * score_text_size + 1, background_color);

	/* Cancel old ball */
	draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height, background_color);
	
	/* Clear paddles */
	draw_rectangle(field_border, paddle_top_y, field_width - field_border, paddle_top_y + paddle_height, background_color);
	draw_rectangle(field_border, paddle_bottom_y, field_width - field_border, paddle_bottom_y + paddle_height, background_color);
	
	/* Print game over messages */
	{
		char * top_player_message = "YOU LOSE";
		char * bottom_player_message = "YOU  WIN";
		if(bottom_score < top_score) {
			top_player_message = "YOU  WIN";
			bottom_player_message = "YOU LOSE";
		}

		GUI_Text_but_reversed(half_field_width - 8 * 4 * you_lose_text_size, (field_height >> 1) - 16 * (you_lose_text_size + 2), (uint8_t *)top_player_message, text_color, background_color, you_lose_text_size, 8);
		GUI_Text(half_field_width - 8 * 4 * you_lose_text_size, (field_height >> 1) + 16 * 2, (uint8_t *)bottom_player_message, text_color, background_color, you_lose_text_size);
		GUI_Text(half_field_width - 4 * 21, (field_height >> 1) - 8, (uint8_t *)"Press KEY0 to restart", text_color, background_color, 1);
	}
	
	bottom_score = 0;
	top_score = 0;
}

/*******************************************************************
 ** Function name:       match_over
 ** Descriptions:        Update score and decide if game is over
 ** Input parameters:    int winner, the player that scored a point
 ** Returned value:      None
 *******************************************************************/
void match_over(int winner)
{
	
	/* DISABLE TIMERS */
	
  disable_timer(0);
  disable_timer(1);
  disable_timer(2);

	/* UPDATE SCORE */

	if(winner == TOP_PLAYER)
		top_score += 1;
	else 
		bottom_score += 1;
	draw_score(winner);

	/* Start new match or end game */

	if(top_score == score_limit || bottom_score == score_limit) 
		game_over();
	else 
		match_start();

}


/*******************************************************************
 ** Function name:       move_cpu_paddle
 ** Descriptions:        Move the cpu (top) paddle to follow the ball
 ** Input parameters:    None
 ** Returned value:      None
 *******************************************************************/
void move_cpu_paddle(void) {

   if(game_status == PLAYING) {

			int paddle_middle = paddle_top_x + (paddle_width >> 1);
			int last_position = paddle_top_x;
			int direction = 0;
				
			// Move paddle
			if(ball_x_direction < 0 && ball_x_position < paddle_middle + 10) 
				direction = -paddle_cpu_speed;	// Move towards its right
			else if (ball_x_direction > 0 && ball_x_position + ball_width > paddle_middle - 10)
				direction = paddle_cpu_speed;		// Move towards its left
				
			paddle_top_x += direction;
			
			// Check paddle is not outside the field
			if(paddle_top_x < field_border || paddle_top_x > field_width)
				paddle_top_x = field_border;
			else if(paddle_top_x + paddle_width >= field_width - field_border)
				paddle_top_x = field_width - field_border - paddle_width - 1;		
				
			if(last_position != paddle_top_x)
				draw_paddle(last_position, paddle_top_x, TOP_PLAYER);

  }
}
