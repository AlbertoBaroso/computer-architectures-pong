#include "../GLCD/GLCD.h" 
#include "constants.h"

uint16_t field_width = 240;
uint16_t field_height = 320;
uint16_t field_border = 5;

uint16_t paddle_width = 60;
uint16_t paddle_height = 10;
uint16_t paddle_color = Green;
uint16_t paddle_x = 0;
uint16_t paddle_y = 278;

uint16_t background_color = White;
uint16_t text_color = Black;
uint16_t ball_color = Black;
uint16_t ball_width = 5;
uint16_t ball_height = 5;

uint16_t ball_x_position = 118;
uint16_t ball_y_position = 158;
uint16_t ball_x_direction = 1;
uint16_t ball_y_direction = 1;

uint8_t fps = 30;

unsigned int game_status = NOT_PLAYING;
