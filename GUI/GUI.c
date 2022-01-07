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
		draw_rectangle_vertical(0, field_border, field_border, paddle_y, border_color);
		draw_rectangle(0, 0, field_width, field_border, border_color);
		draw_rectangle_vertical(field_width - field_border, field_border, field_width, paddle_y, border_color);
		
		time++;
		
	} else {
		
		int half_field_width = (field_width >> 1);
		int half_field_height = (field_height >> 1);
		
		/* Cancel "You Lose" text */
		draw_rectangle(half_field_width - 4 * 8 * you_lose_text_size, half_field_height - 8 * you_lose_text_size + 8, 
									 half_field_width + 4 * 8 * you_lose_text_size, half_field_height + 8 * you_lose_text_size, background_color);
	}
	
	/* Write instructions to play */
	GUI_Text((field_width >> 1) - 84, (field_height >> 1) - 40, (uint8_t *)"  Press KEY1 to play ", text_color, background_color, 1);
	
	/* Draw score record in top right corner */
	draw_score_record();
	
	/* Draw score in middle-left position */
	draw_score();
}

/*******************************************************************
 ** Function name:      draw_paddle
 ** Descriptions:       Move paddle from last_position to current_position
 ** Input parameters:   uint16_t last_position,
 ** 					uint16_t current_position
 ** Returned value:     None
 *******************************************************************/
void draw_paddle( uint16_t last_position, uint16_t current_position ) {

	if(game_status == PLAYING) {
	
		#ifdef SIMULATOR
			
			if(paddles_overlap(last_position, current_position)) {
				if(current_position > last_position) { /* Move paddle to the right */

					/* Erase paddle from the left */
					draw_rectangle(last_position, paddle_y, current_position, paddle_y + paddle_height, background_color);	
					/* Draw paddle on the right */
					draw_rectangle(last_position + paddle_width + 1, paddle_y, current_position + paddle_width + 1, paddle_y + paddle_height, paddle_color);

				} else if(current_position < last_position) { /* Move paddle to the left */

					/* Erase paddle from the right */
					draw_rectangle(current_position + paddle_width + 1, paddle_y, last_position + paddle_width + 1, paddle_y + paddle_height, background_color);
					/* Draw paddle on the left */
					draw_rectangle(current_position, paddle_y, last_position, paddle_y + paddle_height, paddle_color);
				}
			} else {

			//if(in_field(current_position) && in_field(last_position)) {

					/* Erase whole old paddle */
					draw_rectangle(last_position, paddle_y, last_position + paddle_width + 1, paddle_y + paddle_height, background_color);
					/* Draw new whole paddle */
					draw_rectangle(current_position, paddle_y, current_position + paddle_width + 1, paddle_y + paddle_height, paddle_color);
				
				//}
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
			// END GAME 
			game_over();
			return;
		}

		/* Bounce on lateral borders */
		if ((ball_x_position <= field_border || ball_x_position + ball_width >= field_width - field_border) && ball_y_position <= paddle_y) {
			ball_x_direction = -ball_x_direction;
			play_sound(wall_bounce_sound);
		}

		/* Bounce on top border */
		if (ball_y_position <= field_border) {
			ball_y_direction = -ball_y_direction;
			play_sound(wall_bounce_sound);
		}
		
		/* If ball is at paddle_y and is on top of paddle for at least one pixel => Bounce on paddle */
		if (ball_y_position + ball_height == paddle_y &&
       ((ball_x_position >= paddle_x && ball_x_position <= paddle_x + paddle_width) ||
		   (ball_x_position + ball_width >= paddle_x && ball_x_position + ball_width <= paddle_x + paddle_width))) {
						
      /* Calculate bouncing angle */
      uint16_t bounce_segment = paddle_width / 7;
      if(ball_x_position < paddle_x + bounce_segment) {
        ball_x_direction = - 2;
        ball_y_direction = - 1;
      } else if(ball_x_position < paddle_x + bounce_segment * 2) {
				ball_x_direction = - 2;
				ball_y_direction = - 2;
			} else if(ball_x_position < paddle_x + bounce_segment * 3) {
				ball_x_direction = - 1;
				ball_y_direction = - 2;
			} else if(ball_x_position < paddle_x + bounce_segment * 4) {
				ball_x_direction = 0;
				ball_y_direction = - 2;
			} else if(ball_x_position < paddle_x + bounce_segment * 5) {
				ball_x_direction = 1;
				ball_y_direction = - 2;
			} else if(ball_x_position < paddle_x + bounce_segment * 6) {
				ball_x_direction = 2;
				ball_y_direction = - 2;
			} else {
				ball_x_direction = 2;
				ball_y_direction = - 1;
			}

			/* Emit sound */
			play_sound(paddle_bounce_sound);

			/* Score calculation and drawing */
			old_score = score;
			if(score >= 100)
				score += 10;
			else
				score += 5;
			draw_score();
			if(score > score_record) {
				old_score_record = score_record;
        score_record = score;
        draw_score_record();
      }
		}
			 
		
		
		
		
			 
		///////////////////////////////////////////////////////
    /// IF GAME CRASHES SWAP TO move_ball_inefficient() ///
		///////////////////////////////////////////////////////
		
		//move_ball_inefficient();
		
		move_ball_efficient();
		
		///////////////////////////////////////////////////////
    /// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ ///
		///////////////////////////////////////////////////////
		
		
		
		
		
		
	}
}


/*******************************************************************
 ** Function name:     draw_score_record
 ** Descriptions:      Draws the score record on the screen
 ** Input parameters:  None
 ** Returned value:    None
 ********************************************************************/
void draw_score_record(void) {
	int length = snprintf(NULL, 0, "%d", score_record);
	char string[10];
	int text_x = field_width - 10 - length * 8 * score_record_text_size;
	char old_string[10];
	
	if(score == 0)
		sprintf(old_string, "%c", '\0');
	else
		sprintf(old_string, "%d", old_score_record);
	sprintf(string, "%d", score_record);
  GUI_Text_but_only_different_characters(text_x, score_record_y, (uint8_t *)string,
																				 score_color, background_color, score_record_text_size, (uint8_t *)old_string);
}

/*******************************************************************
 ** Function name:     draw_score
 ** Descriptions:      Draws the score on the screen
 ** Input parameters:  None
 ** Returned value:    None
 ********************************************************************/
void draw_score(void) {
	int length = snprintf(NULL, 0, "%d", score);
	char string[10];
	char old_string[10];
	
	if(score == 0)
		sprintf(old_string, "%c", '\0');
	else
		sprintf(old_string, "%d", old_score);
	sprintf(string, "%d", score);
  GUI_Text_but_only_different_characters(score_x, score_y, (uint8_t *)string,
																				 score_color, background_color, score_text_size, (uint8_t *)old_string);
}

/*******************************************************************
 ** Function name:     draw_rectangle
 ** Descriptions:      Draws a rectangle on the screen
 ** Input parameters:  x_start, y_start, x_end, y_end, color
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
 ** Input parameters:  x_start, y_start, x_end, y_end, color
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
 ** Input parameters:  x_start, y_start, x_end, y_end, color, x2, y2, x3, y3
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
 ** Input parameters:  x_start, y_start, x_end, y_end, color, x2, y2, x3, y3, x4, y4, x5, y5
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
 ** Input parameters:  x_start, y_start, x_end, y_end, color, x_start_erase, y_start_erase, x_end_erase, y_end_erase
 ** Returned value:    None
 ********************************************************************/
void draw_rectangle_but_not_in_rectangle(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end , uint16_t color, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3 ) {		 
	int x, y;
	for(y = y_start; y < y_end; y++)
		for(x = x_start; x < x_end; x++)
			if(!((x >= x2 && x <= x3) && (y >= y2 && y <= y3))) 
				LCD_SetPoint(x, y, color);							 						 
}

void move_ball_inefficient() {
	
			int x_movement;
			int y_movement;
			int score_length = snprintf(NULL, 0, "%d", score);
			int score_record_length = snprintf(NULL, 0, "%d", score_record);
			int score_record_x = field_width - 10 - score_record_length * 8 * score_record_text_size;
			
			// Cancel old ball //
			if(ball_y_position + ball_height > paddle_y) {
				draw_rectangle_but_not_in_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height, 
																						background_color, paddle_x, paddle_y, paddle_x + paddle_width, paddle_y + paddle_height);
			} else {
				draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height, background_color);
			}
			
			// Check if ball overlaps with score //
			if(((ball_x_position >= score_x && ball_x_position <= score_x + score_length * score_text_size * 8) ||
				  (ball_x_position + ball_width >= score_x && ball_x_position + ball_width <= score_x + score_length * score_text_size * 8)) && 
				 ((ball_y_position >= score_y && ball_y_position <= score_y + score_text_size * 16) || 
					(ball_y_position + ball_height >= score_y && ball_y_position + ball_height <= score_y + score_text_size * 16))) {
					
					int length = snprintf(NULL, 0, "%d", score);
					char string[10];
	
					sprintf(string, "%d", score);
					GUI_Text_but_only_in_rectangle(score_x, score_y, (uint8_t *)string,
																			   score_color, background_color, score_text_size,
																			   ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height);
						
			} else if (((ball_x_position >= score_record_x && ball_x_position <= field_width - 10) ||
				  (ball_x_position + ball_width >= score_record_x && ball_x_position + ball_width <= score_record_x + score_record_length * score_record_text_size * 8)) && 
				 ((ball_y_position >= score_record_y && ball_y_position <= score_record_y + score_record_text_size * 16) || 
					(ball_y_position + ball_height >= score_record_y && ball_y_position + ball_height <= score_record_y + score_record_text_size * 16))) {
				// Check if ball overlaps with score record //

				char string[10];
				sprintf(string, "%d", score_record);
				GUI_Text_but_only_in_rectangle(score_record_x, score_record_y, (uint8_t *)string,
																			 score_color, background_color, score_record_text_size, 
																			 ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height);

			}
		
		/* Move ball */
		ball_x_position += ball_x_direction;
		ball_y_position += ball_y_direction;
		x_movement = ball_x_direction;
		y_movement = ball_y_direction;

		/* Avoid ball to go out of field */
		if (ball_x_position < field_border && ball_y_position <= paddle_y) {
			x_movement = x_movement + (field_border - ball_x_position);
			ball_x_position = field_border;
		} else if (ball_x_position + ball_width > field_width - field_border && ball_y_position <= paddle_y) {
			x_movement = x_movement - ((ball_x_position + ball_width) - (field_width - field_border));
			ball_x_position = field_width - field_border - ball_width;
		}
		if (ball_y_position < field_border) {
			y_movement = y_movement + (field_border - ball_y_position);
			ball_y_position = field_border;
		}
	

    /* Draw new ball */

      
		if(ball_y_position + ball_height > paddle_y) {
			draw_rectangle_but_not_in_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
										 ball_y_position + ball_height, ball_color, paddle_x, paddle_y, paddle_x + paddle_width, paddle_y + paddle_height);
		} else {
			draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
										 ball_y_position + ball_height, ball_color);
		}

}

void move_ball_efficient() {
	
		int x_movement;
		int y_movement;

		// Horizontal rectangles
		int x_start_erase;
		int y_start_erase;
		int x_end_erase;
		int y_end_erase;
		int x_start_draw;
		int y_start_draw;
		int x_end_draw;
		int y_end_draw;
		
		// Vertical rectangles
		int x_start_erase_2;
		int y_start_erase_2;
		int x_end_erase_2;
		int y_end_erase_2;
		int x_start_draw_2;
		int y_start_draw_2;
		int x_end_draw_2;
		int y_end_draw_2;

    int old_ball_x_position = ball_x_position - ball_x_direction;
    int old_ball_y_position = ball_y_position - ball_y_direction;

		/* Move ball */
		ball_x_position += ball_x_direction;
		ball_y_position += ball_y_direction;
		x_movement = ball_x_direction;
		y_movement = ball_y_direction;

		/* Avoid ball to go out of field */
		if (ball_x_position < field_border && ball_y_position <= paddle_y) {
			x_movement = x_movement + (field_border - ball_x_position);
			ball_x_position = field_border;
		} else if (ball_x_position + ball_width > field_width - field_border && ball_y_position <= paddle_y) {
			x_movement = x_movement - ((ball_x_position + ball_width) - (field_width - field_border));
			ball_x_position = field_width - field_border - ball_width;
		}
		if (ball_y_position < field_border) {
			y_movement = y_movement + (field_border - ball_y_position);
			ball_y_position = field_border;
		}
		
		if(x_movement > 0) {

      x_start_erase = ball_x_position - x_movement;
			x_end_erase = x_start_erase + ball_width;
			x_start_draw = ball_x_position;
			x_end_draw = ball_x_position + ball_width;
			x_start_erase_2 = x_start_erase;
			x_end_erase_2 = ball_x_position;
			x_start_draw_2 = x_end_erase;
			x_end_draw_2 = x_end_draw;

			if (y_movement < 0) {
				y_start_erase = ball_y_position + ball_height;
				y_end_erase = ball_y_position + ball_height - y_movement;
				y_start_draw = ball_y_position;
				y_end_draw = ball_y_position - y_movement;
				y_start_erase_2 = ball_y_position - y_movement;
				y_end_erase_2 = ball_y_position + ball_height;
				y_start_draw_2 = ball_y_position - y_movement;
				y_end_draw_2 = ball_y_position + ball_height;
			} else if(y_movement > 0) {
				y_start_erase = ball_y_position - y_movement;
				y_end_erase = ball_y_position;
				y_start_draw = ball_y_position + ball_height - y_movement;
				y_end_draw = ball_y_position + ball_height;
				y_start_erase_2 = ball_y_position;
				y_end_erase_2 = y_start_draw;
				y_start_draw_2 = ball_y_position;
				y_end_draw_2 = y_start_draw;
			}  

		} else if (x_movement < 0) {

				x_start_erase = ball_x_position - x_movement;
				x_end_erase = ball_x_position - x_movement + ball_width;
				x_start_draw = ball_x_position;
				x_end_draw = ball_x_position + ball_width;
				x_start_erase_2 = ball_x_position + ball_width;
				x_end_erase_2 = ball_x_position + ball_width - x_movement;
				x_start_draw_2 = ball_x_position;
				x_end_draw_2 = ball_x_position - x_movement;
				
				if(y_movement > 0) {
					y_start_erase = ball_y_position - y_movement;
					y_end_erase = ball_y_position;
					y_start_draw = ball_y_position - y_movement + ball_height;
					y_end_draw = ball_y_position + ball_height;
					y_start_erase_2 = ball_y_position;
					y_end_erase_2 = ball_y_position - y_movement + ball_height;
					y_start_draw_2 = ball_y_position;
					y_end_draw_2 = ball_y_position - y_movement + ball_height;
				} else if (y_movement < 0) {
					y_start_erase = ball_y_position + ball_height;
					y_end_erase = ball_y_position + ball_height - y_movement;
					y_start_draw = ball_y_position;
					y_end_draw = ball_y_position - y_movement;
					y_start_erase_2 = ball_y_position;
					y_end_erase_2 = y_start_erase;
					y_start_draw_2 = ball_y_position;
					y_end_draw_2 = y_start_erase;
				}

		} else {

			x_start_erase = ball_x_position;
			x_end_erase = ball_x_position + ball_width;
			x_start_draw = ball_x_position;
			x_end_draw = ball_x_position + ball_width;
			x_start_erase_2 = 0;
			x_end_erase_2 = 0;
			x_start_draw_2 = 0;
      x_end_draw_2 = 0;

			if(y_movement > 0) {
				y_start_erase = ball_y_position - y_movement;
				y_end_erase = ball_y_position;
				y_start_draw = ball_y_position + ball_height - y_movement;
				y_end_draw = ball_y_position + ball_height;
				y_start_erase_2 = ball_y_position;
				y_end_erase_2 = y_start_draw;
				y_start_draw_2 = ball_y_position;
				y_end_draw_2 = y_start_draw;
			} else if (y_movement < 0) {
				y_start_erase = ball_y_position + ball_height;
				y_end_erase = ball_y_position + ball_height - y_movement;
				y_start_draw = ball_y_position;
				y_end_draw = ball_y_position - y_movement;
				y_start_erase_2 = ball_y_position;
				y_end_erase_2 = y_start_erase;
				y_start_draw_2 = ball_y_position;
				y_end_draw_2 = y_start_erase;
			}
		}

    // Draw new ball //
    if(ball_y_position + ball_height > paddle_y) {
        draw_rectangle_but_not_in_rectangle(x_start_erase, y_start_erase, x_end_erase, y_end_erase, background_color, paddle_x, paddle_y, paddle_x + paddle_width, paddle_y + paddle_height);
        draw_rectangle_but_not_in_rectangle(x_start_draw, y_start_draw, x_end_draw, y_end_draw, ball_color, paddle_x, paddle_y, paddle_x + paddle_width, paddle_y + paddle_height);
        draw_rectangle_but_not_in_rectangle(x_start_erase_2, y_start_erase_2, x_end_erase_2, y_end_erase_2, background_color, paddle_x, paddle_y, paddle_x + paddle_width, paddle_y + paddle_height);
        draw_rectangle_but_not_in_rectangle(x_start_draw_2, y_start_draw_2, x_end_draw_2, y_end_draw_2, ball_color, paddle_x, paddle_y, paddle_x + paddle_width, paddle_y + paddle_height);
    } else {

        int score_length = snprintf(NULL, 0, "%d", score);
        int score_record_length = snprintf(NULL, 0, "%d", score_record);
        int score_record_x = field_width - 10 - score_record_length * 8 * score_record_text_size;

        // Cancel old ball //
        draw_rectangle(x_start_erase, y_start_erase, x_end_erase, y_end_erase, background_color);
        draw_rectangle(x_start_erase_2, y_start_erase_2, x_end_erase_2, y_end_erase_2, background_color);

        // Check if ball overlaps with score //
        if(((old_ball_x_position >= score_x && old_ball_x_position <= score_x + score_length * score_text_size * 8) ||
            (old_ball_x_position + ball_width >= score_x && old_ball_x_position + ball_width <= score_x + score_length * score_text_size * 8)) &&
           ((old_ball_y_position >= score_y && old_ball_y_position <= score_y + score_text_size * 16) ||
            (old_ball_y_position + ball_height >= score_y && old_ball_y_position + ball_height <= score_y + score_text_size * 16))) {

						// Rewrite overlapping score section //
            char string[10];
            sprintf(string, "%d", score);
            GUI_Text_but_only_in_rectangles(score_x, score_y, (uint8_t *)string,
                                           score_color, background_color, score_text_size,
                                           x_start_erase, y_start_erase, x_end_erase, y_end_erase,
                                           x_start_erase_2, y_start_erase_2, x_end_erase_2, y_end_erase_2);

             // Check if ball overlaps with score record //
         } else if (((old_ball_x_position >= score_record_x && old_ball_x_position <= field_width - 10) ||
                     (old_ball_x_position + ball_width >= score_record_x && old_ball_x_position + ball_width <= score_record_x + score_record_length * score_record_text_size * 8)) &&
                    ((old_ball_y_position >= score_record_y && old_ball_y_position <= score_record_y + score_record_text_size * 16) ||
                     (old_ball_y_position + ball_height >= score_record_y && old_ball_y_position + ball_height <= score_record_y + score_record_text_size * 16))) {

						// Rewrite overlapping score record section //
            char string[10];
            sprintf(string, "%d", score_record);
            GUI_Text_but_only_in_rectangles(score_record_x, score_record_y, (uint8_t *)string,
                                            score_color, background_color, score_record_text_size,
                                            x_start_erase, y_start_erase, x_end_erase, y_end_erase,
                                            x_start_erase_2, y_start_erase_2, x_end_erase_2, y_end_erase_2);
         }

         // Draw ball //
         draw_rectangle(x_start_draw, y_start_draw, x_end_draw, y_end_draw, ball_color);
         draw_rectangle(x_start_draw_2, y_start_draw_2, x_end_draw_2, y_end_draw_2, ball_color);

    }
}
