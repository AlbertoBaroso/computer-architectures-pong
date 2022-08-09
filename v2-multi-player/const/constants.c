/*******************************************************************
** ########################### File Info ###########################
**
** File name:           constants.c
** Last modified Date:  2022/01
** Descriptions:        constant values common to all files
** Correlated files:    constants.h
**
** #################################################################
*******************************************************************/

#include "../GLCD/GLCD.h"
#include "constants.h"

/* GAME */

uint8_t score_limit = 5; 								// Score to reach to end a match

/* COLORS */

uint16_t paddle_color = Green;
uint16_t background_color = White;
uint16_t text_color = Black;
uint16_t ball_color = Magenta;
uint16_t score_color = Blue;
uint16_t border_color = Red;

/* FIELD */

uint16_t field_width = 240;
uint16_t field_height = 320;
uint16_t field_border = 5;

/* PADDLE */

uint16_t paddle_width = 60;
uint16_t paddle_height = 10;
uint16_t paddle_user_refresh = 50;	 		// Paddle refresh rate in a second (time = 1 /paddle_user_refresh)
uint16_t paddle_cpu_refresh = 250;	 		// Paddle CPU refresh rate in a second (time = 1 /paddle_cpu_refresh)
uint8_t paddle_threshold = 10; 					// Don't move paddle if next position is within this distance
uint8_t paddle_cpu_speed = 4;						// Pixels moved at every timer interrupt

/* BALL */

uint16_t ball_width = 5;
uint16_t ball_height = 5;
int ball_x_position = 204;							// Initial position and direction to bounce middle-height with 45 degree angle
int ball_y_position = 129;
int ball_x_direction = 2;
int ball_y_direction = 2;
uint16_t ball_refresh = 500;	 					// Ball refresh rate in a second (time = 1 /ball_refresh)
uint16_t initial_ball_x = 185;
uint16_t initial_ball_y = 129;

/* TEXT */

uint16_t you_lose_text_size = 2;
uint16_t score_text_size = 4;
uint16_t score_record_text_size = 2;
uint16_t score_x = 10;
uint16_t score_record_y = 10;
