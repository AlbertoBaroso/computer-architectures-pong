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

/* COLORS */

uint16_t paddle_color = Green;
uint16_t background_color = White;
uint16_t text_color = Black;
uint16_t ball_color = Black;
uint16_t score_color = Blue;
uint16_t border_color = Red;

/* FIELD */

uint16_t field_width = 240;
uint16_t field_height = 320;
uint16_t field_border = 5;

/* PADDLE */

uint16_t paddle_width = 60;
uint16_t paddle_height = 10;
uint16_t paddle_x = 0;
uint16_t paddle_y = 278;
uint8_t paddle_fps = 1;
uint8_t paddle_threshold = 10; // Don't move paddle if next position is within this distance

/* BALL */

uint16_t ball_width = 5;
uint16_t ball_height = 5;
uint16_t ball_x_position = 205;
uint16_t ball_y_position = 130;
int ball_x_direction = 1;
int ball_y_direction = 1;
uint8_t ball_fps = 30;

/* TEXT */

uint16_t you_lose_text_size = 2;
uint16_t score_text_size = 4;
uint16_t score_record_text_size = 2;
uint16_t score_x = 10;
uint16_t score_y = 150;
uint16_t score_record_y = 10;

/* SOUND */

uint16_t SinTable[45] = 
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

uint8_t wall_bounce_sound = 5;
uint8_t paddle_bounce_sound = 7;
uint8_t sound_lenght = 1;

//const int freqs[8] = {4240, 3779, 3367, 3175, 2834, 2525, 2249, 2120};
/* 
131Hz		k=4240 C3
147Hz		k=3779
165Hz		k=3367
175Hz		k=3175
196Hz		k=2834		
220Hz		k=2525
247Hz		k=2249
262Hz		k=2120 C4
*/

const int freqs[8] = {2120, 1890, 1684, 1592, 1417, 1263, 1125, 1062};
/*
262Hz	k=2120		c4
294Hz	k=1890		
330Hz	k=1684		
349Hz	k=1592		
392Hz	k=1417		
440Hz	k=1263		
494Hz	k=1125		
523Hz	k=1062		c5

*/
