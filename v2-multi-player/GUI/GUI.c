/*******************************************************************
** ########################### File Info ###########################
**
** File name:           GUI.c
** Last modified Date:  2022/01
** Descriptions:        Functions to draw on the GUI
** Correlated files:    GUI.h
**
** #################################################################
*******************************************************************/

#include "LPC17xx.h"
#include "GUI.h"
#include "../GLCD/GLCD.h" 
#include "../const/constants.h"
#include "../game/game.h"
#include "../timer/timer.h"
#include "../utility/utility.h"
#include <stdio.h>

/*******************************************************************
 ** Function name:      init_GUI
 ** Descriptions:       Initial display graphics
 ** Input parameters:   None
 ** Returned value:     None
 *******************************************************************/
void init_GUI() {
	
	static int time = 0;
	
	if(time == 0) {
		
		/* Color background */
		LCD_Clear(background_color);
		
		/* Draw Left-Top-Right red borders */
		draw_rectangle_vertical(0, 0, field_border, field_height, border_color);
		draw_rectangle_vertical(field_width - field_border, 0, field_width, field_height, border_color);
		
		/* Write PONG on screen */
		GUI_Text((field_width >> 1) - 4 * 4 * 2, (field_height >> 2) - 8 * 2, (uint8_t *)"PONG", ball_color, background_color, 2);
		
		time++;
		
	} else {
		
		int half_field_width = (field_width >> 1);
		int half_field_height = (field_height >> 1);
		int text_size = 4 * 8 * you_lose_text_size;
		
		/* Cancel "You Lose" & "You Win" texts */
		draw_rectangle(half_field_width - text_size, half_field_height - 16 * (you_lose_text_size + 2), 
									 half_field_width + text_size, half_field_height - 32, background_color);
		draw_rectangle(half_field_width - text_size, half_field_height + 32, 
									 half_field_width + text_size, half_field_height + 16 * (you_lose_text_size + 2), background_color);
	}
	
	/* Write instructions to play */
	GUI_Text((field_width >> 1) - 4 * 22, (field_height >> 1) - 8, (uint8_t *)"  Press KEY1 to play  ", text_color, background_color, 1);
}

/*******************************************************************
 ** Function name:      draw_paddle
 ** Descriptions:       Move paddle from last_position to current_position
 ** Input parameters:   uint16_t last_position,
 ** 					uint16_t current_position
 ** Returned value:     None
 *******************************************************************/
void draw_paddle( uint16_t last_position, uint16_t current_position, int player ) {

	if(game_status == PLAYING) {
	
		int paddle_y = (player == BOTTOM_PLAYER) ? paddle_bottom_y : paddle_top_y;
		int i;

		#ifdef SIMULATOR
			
			if(paddles_overlap(last_position, current_position)) {
				
				if(current_position > last_position) { /* Move paddle to the right */
				
					int distance = current_position - last_position;

					/* Erase paddle from the left */
					//draw_rectangle(last_position, paddle_y, current_position, paddle_y + paddle_height, background_color);	
					/* Draw paddle on the right */
					//draw_rectangle(last_position + paddle_width + 1, paddle_y, current_position + paddle_width + 1, paddle_y + paddle_height, paddle_color);
					
					
						for(i = 0; i < distance; i++) {
							/* Erase paddle from the left */
							draw_rectangle(last_position + i, paddle_y, last_position + i + 1, paddle_y + paddle_height, background_color);	
							/* Draw paddle on the right */
							draw_rectangle(last_position + paddle_width + 1 + i, paddle_y, last_position + paddle_width + 2 + i, paddle_y + paddle_height, paddle_color);
						}

				} else if(current_position < last_position) { /* Move paddle to the left */
					
						int distance = last_position - current_position;
					
						for(i = 0; i < distance; i++) {
							/* Erase paddle from the right */
							//draw_rectangle(current_position + paddle_width + 1, paddle_y, last_position + paddle_width + 1, paddle_y + paddle_height, background_color);
							/* Draw paddle on the left */
							//draw_rectangle(current_position, paddle_y, last_position, paddle_y + paddle_height, paddle_color);
				
							draw_rectangle(last_position + paddle_width - i, paddle_y, last_position + paddle_width - i + 1, paddle_y + paddle_height, background_color);	
							draw_rectangle(last_position - i, paddle_y, last_position - i + 1, paddle_y + paddle_height, paddle_color);
						}
					
					/* Erase paddle from the right */
					//draw_rectangle(current_position + paddle_width + 1, paddle_y, last_position + paddle_width + 1, paddle_y + paddle_height, background_color);
					/* Draw paddle on the left */
					//draw_rectangle(current_position, paddle_y, last_position, paddle_y + paddle_height, paddle_color);
				}
			} else {

					/* Erase whole old paddle */
					draw_rectangle(last_position, paddle_y, last_position + paddle_width + 1, paddle_y + paddle_height, background_color);
					/* Draw new whole paddle */
					draw_rectangle(current_position, paddle_y, current_position + paddle_width + 1, paddle_y + paddle_height, paddle_color);
				
			}
		
		#else
		
			if(current_position > 0)
				draw_rectangle( 0, paddle_y, current_position, paddle_y + paddle_height, background_color );
			draw_rectangle( current_position, paddle_y, current_position + paddle_width + 1, paddle_y + paddle_height, paddle_color );
			if(current_position + paddle_width + 1 < field_width )
				draw_rectangle( current_position + paddle_width + 1, paddle_y, field_width, paddle_y + paddle_height, background_color );
		
		#endif
	
	}
	
}

/*******************************************************************
 ** Function name:      draw_ball
 ** Descriptions:       Draw ball on the field
 ** Input parameters:   None
 ** Returned value:     None
 *******************************************************************/
void draw_ball(void) {
	
	if(game_status == PLAYING) {

		if (ball_y_position > field_height) {
			match_over(TOP_PLAYER);
		} else if(ball_y_position + ball_height < 0) {
			match_over(BOTTOM_PLAYER);
		} else {

			/* If ball is at paddle_top_y and is right below the paddle for at least one pixel => Bounce on top paddle */
			if (ball_y_position == paddle_top_y + paddle_height &&
			  ((ball_x_position >= paddle_top_x && ball_x_position <= paddle_top_x + paddle_width) ||
			   (ball_x_position + ball_width >= paddle_top_x && ball_x_position + ball_width <= paddle_top_x + paddle_width))) {
							
					/* Calculate bouncing angle */
					uint16_t bounce_segment = paddle_width / 7;
					if(ball_x_position < paddle_top_x + bounce_segment) {
						ball_x_direction = - 3;
						ball_y_direction = 3;
					} else if(ball_x_position < paddle_top_x + bounce_segment * 2) {
						ball_x_direction = - 3;
						ball_y_direction = 3;
					} else if(ball_x_position < paddle_top_x + bounce_segment * 3) {
						ball_x_direction = - 2;
						ball_y_direction = 4;
					} else if(ball_x_position < paddle_top_x + bounce_segment * 4) {
						ball_x_direction = 0;
						ball_y_direction = 4;
					} else if(ball_x_position < paddle_top_x + bounce_segment * 5) {
						ball_x_direction = 2;
						ball_y_direction = 4;
					} else if(ball_x_position < paddle_top_x + bounce_segment * 6) {
						ball_x_direction = 3;
						ball_y_direction = 3;
					} else {
						ball_x_direction = 3;
						ball_y_direction = 3;
					}	
			} else
			
			/* If ball is at paddle_bottom_y and is on top of paddle for at least one pixel => Bounce on bottom paddle */
			if (ball_y_position + ball_height == paddle_bottom_y &&
			  ((ball_x_position >= paddle_bottom_x && ball_x_position <= paddle_bottom_x + paddle_width) ||
			   (ball_x_position + ball_width >= paddle_bottom_x && ball_x_position + ball_width <= paddle_bottom_x + paddle_width))) {
							
					/* Calculate bouncing angle */
					uint16_t bounce_segment = paddle_width / 7;
					if(ball_x_position < paddle_bottom_x + bounce_segment) {
						ball_x_direction = - 3;
						ball_y_direction = - 3;
					} else if(ball_x_position < paddle_bottom_x + bounce_segment * 2) {
						ball_x_direction = - 3;
						ball_y_direction = - 3;
					} else if(ball_x_position < paddle_bottom_x + bounce_segment * 3) {
						ball_x_direction = - 2;
						ball_y_direction = - 4;
					} else if(ball_x_position < paddle_bottom_x + bounce_segment * 4) {
						ball_x_direction = 0;
						ball_y_direction = - 4;
					} else if(ball_x_position < paddle_bottom_x + bounce_segment * 5) {
						ball_x_direction = 2;
						ball_y_direction = - 4;
					} else if(ball_x_position < paddle_bottom_x + bounce_segment * 6) {
						ball_x_direction = 3;
						ball_y_direction = - 3;
					} else {
						ball_x_direction = 3;
						ball_y_direction = - 3;
					}	
			}	
			
			/* Bounce on lateral borders */
			if (ball_x_position <= field_border || ball_x_position + ball_width >= field_width - field_border)
				ball_x_direction = -ball_x_direction;
				
			move_ball();
		
		}
	}
}


/*******************************************************************
 ** Function name:     draw_score
 ** Descriptions:      Draws the score on the screen
 ** Input parameters:  None
 ** Returned value:    None
 ********************************************************************/
void draw_score(int player) {
	int score = (player == BOTTOM_PLAYER) ? bottom_score : top_score;
	int length = snprintf(NULL, 0, "%d", score);
	int actual_score_x = (player == BOTTOM_PLAYER) ? score_x : field_width - (field_border << 1) - (length * score_text_size * 8) - 5;
	int score_y = (field_height >> 1) - (score_text_size << 3);
	char string[10];

	sprintf(string, "%d", score);
	if(player == BOTTOM_PLAYER)
      GUI_Text(actual_score_x, score_y, (uint8_t *)string, score_color, background_color, score_text_size);
	else
      GUI_Text_but_reversed(actual_score_x, score_y, (uint8_t *)string, score_color, background_color, score_text_size, length);
}

/*******************************************************************
 ** Function name:     draw_rectangle
 ** Descriptions:      Draws a rectangle on the screen
 ** Input parameters:  x_start, y_start: coordinates (x, y) of upper left corner of the rectangle
 **                    x_end, y_end: coordinates (x, y) of lower right corner of the rectangle
 **                    color: color of the rectangle
 ** Returned value:    None
 ********************************************************************/
void draw_rectangle( uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end , uint16_t color ) {
	int x, y;
	for(x = x_start; x < x_end; x++)
		for(y = y_start; y < y_end; y++)
			LCD_SetPoint(x, y, color);
}

/*******************************************************************
 ** Function name:     draw_rectangle_vertical
 ** Descriptions:      Draws a rectangle on the screen from top to bottom
 ** Input parameters:  x_start, y_start: coordinates (x, y) of upper left corner of the rectangle
 **                    x_end, y_end: coordinates (x, y) of lower right corner of the rectangle
 **                    color: color of the rectangle
 ** Returned value:    None
 ********************************************************************/
void draw_rectangle_vertical( uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end , uint16_t color ) {
	int x, y;
	for(y = y_start; y < y_end; y++)
		for(x = x_start; x < x_end; x++)
			LCD_SetPoint(x, y, color);
}

/*******************************************************************
 ** Function name:     draw_rectangle_but_only_in_rectangle
 ** Descriptions:      Draws a rectangle on the screen but only in a specified rectangular area
 ** Input parameters:  x_start, y_start: coordinates (x, y) of upper left corner of the rectangle to draw
 **                    x_end, y_end: coordinates (x, y) of lower right corner of the rectangle to draw
 **                    color: color of the rectangle
 **                    x2, y2: coordinates (x, y) of upper left corner of the rectangle in which to draw
 **                    x3, y3: coordinates (x, y) of lower right corner of the rectangle in which to draw
 ** Returned value:    None
 ********************************************************************/
void draw_rectangle_but_only_in_rectangle( uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end , uint16_t color, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3 ) {
    int x, y;
    for(y = y_start; y < y_end; y++)
        for(x = x_start; x < x_end; x++)
            if((x >= x2 && x <= x3) && (y >= y2 && y <= y3))
                LCD_SetPoint(x, y, color);
}

/*******************************************************************
 ** Function name:     draw_rectangle_but_only_in_rectangles
 ** Descriptions:      Draws a rectangle on the screen but only in 2 specified rectangular areas
 ** Input parameters:  x_start, y_start: coordinates (x, y) of upper left corner of the rectangle to draw
 **                    x_end, y_end: coordinates (x, y) of lower right corner of the rectangle to draw
 **                    color: color of the rectangle
 **                    x2, y2: coordinates (x, y) of upper left corner of the first rectangle in which to draw
 **                    x3, y3: coordinates (x, y) of lower right corner of the first rectangle in which to draw
 **                    x4, y4: coordinates (x, y) of upper left corner of the second rectangle in which to draw
 **                    x5, y5: coordinates (x, y) of lower right corner of the second rectangle in which to draw
 ** Returned value:    None
 ********************************************************************/
void draw_rectangle_but_only_in_rectangles( uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end , uint16_t color, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t x4, uint16_t y4, uint16_t x5, uint16_t y5 ) {
    int x, y;
    for(y = y_start; y < y_end; y++)
        for(x = x_start; x < x_end; x++)
            if(((x >= x2 && x <= x3) && (y >= y2 && y <= y3)) || ((x >= x4 && x <= x5) && (y >= y4 && y <= y5)))
                LCD_SetPoint(x, y, color);
}

/*******************************************************************
 ** Function name:     draw_rectangle_but_not_in_rectangle
 ** Descriptions:      Draws a rectangle on the screen but not in another specified rectangular area
 ** Input parameters:  x_start, y_start: coordinates (x, y) of upper left corner of the rectangle to draw
 **                    x_end, y_end: coordinates (x, y) of lower right corner of the rectangle to draw
 **                    color: color of the rectangle
 **                    x2, y2: coordinates (x, y) of upper left corner of the rectangle in which not to draw
 **                    x3, y3: coordinates (x, y) of lower right corner of the rectangle in which not to draw
 ** Returned value:    None
 ********************************************************************/
void draw_rectangle_but_not_in_rectangle(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end , uint16_t color, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3 ) {		 
	int x, y;
	for(y = y_start; y < y_end; y++)
		for(x = x_start; x < x_end; x++)
			if(!((x >= x2 && x <= x3) && (y >= y2 && y <= y3))) 
				LCD_SetPoint(x, y, color);							 						 
}

/*******************************************************************
 ** Function name:     move_ball_inefficient
 ** Descriptions:      completely erases old ball and rewrites new ball
 ** Input parameters:  None
 ** Returned value:    None
 ********************************************************************/
void move_ball() {
	
			int x_movement;
			int bottom_score_length = snprintf(NULL, 0, "%d", bottom_score);
			int top_score_length = snprintf(NULL, 0, "%d", top_score);
			int top_score_x = field_width - (field_border << 1) - top_score_length * 8 * score_text_size - 5;
			int score_y = (field_height >> 1) - (score_text_size << 3);
			
			// Cancel old ball //
			if(ball_y_position + ball_height > paddle_bottom_y) {
				draw_rectangle_but_not_in_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height, 
																						background_color, paddle_bottom_x, paddle_bottom_y, paddle_bottom_x + paddle_width, paddle_bottom_y + paddle_height);
			} else if(ball_y_position <= paddle_top_y + paddle_height) {
				draw_rectangle_but_not_in_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height, 
																						background_color, paddle_top_x, paddle_top_y, paddle_top_x + paddle_width, paddle_top_y + paddle_height);
			} else {
				draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height, background_color);
			}
			
			// Check if ball overlaps with bottom player score //
			if(((ball_x_position >= score_x && ball_x_position <= score_x + bottom_score_length * score_text_size * 8) ||
				  (ball_x_position + ball_width >= score_x && ball_x_position + ball_width <= score_x + bottom_score_length * score_text_size * 8)) && 
				 ((ball_y_position >= score_y && ball_y_position <= score_y + score_text_size * 16) || 
					(ball_y_position + ball_height >= score_y && ball_y_position + ball_height <= score_y + score_text_size * 16))) {
					
					char string[10];
					sprintf(string, "%d", bottom_score);
					GUI_Text_but_only_in_rectangle(score_x, score_y, (uint8_t *)string,
																			   score_color, background_color, score_text_size,
																			   ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height);
			
			// Check if ball overlaps with top player score //
      } else if(((ball_x_position >= top_score_x && ball_x_position <= field_width - 10) ||
								 (ball_x_position + ball_width >= top_score_x && ball_x_position + ball_width <= field_width - 10)) &&
								((ball_y_position >= score_y && ball_y_position <= score_y + score_text_size * 16) ||
								 (ball_y_position + ball_height >= score_y && ball_y_position + ball_height <= score_y + score_text_size * 16))) {

						char string[10];
						sprintf(string, "%d", top_score);
						GUI_Text_but_only_in_rectangle_reversed(top_score_x, score_y, (uint8_t *)string,
																					 score_color, background_color, score_text_size,
																					 ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height, 1);

			}
		
		/* Move ball */
		ball_x_position += ball_x_direction;
		ball_y_position += ball_y_direction;
		x_movement = ball_x_direction;

		/* Avoid ball to go out of field */
		if (ball_x_position < field_border && ball_y_position <= paddle_bottom_y) {
			x_movement = x_movement + (field_border - ball_x_position);
			ball_x_position = field_border;
		} else if (ball_x_position + ball_width > field_width - field_border && ball_y_position <= paddle_bottom_y) {
			x_movement = x_movement - ((ball_x_position + ball_width) - (field_width - field_border));
			ball_x_position = field_width - field_border - ball_width;
		}

    /* Draw new ball */
		if(ball_y_position + ball_height > paddle_bottom_y) {
			draw_rectangle_but_not_in_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
										 ball_y_position + ball_height, ball_color, paddle_bottom_x, paddle_bottom_y, paddle_bottom_x + paddle_width, paddle_bottom_y + paddle_height);
		} else if(ball_y_position <= paddle_top_y + paddle_height) {
				draw_rectangle_but_not_in_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height, 
																						ball_color, paddle_top_x, paddle_top_y, paddle_top_x + paddle_width, paddle_top_y + paddle_height);
		} else {
			draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
										 ball_y_position + ball_height, ball_color);
		}

}
