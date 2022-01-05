#include "LPC17xx.h"
#include "GUI.h"
#include "../GLCD/GLCD.h" 
#include "../const/constants.h"
#include "../game/game.h"
#include "../timer/timer.h"
#include "../utility/utility.h"
#include <stdio.h>

/*
	Initialize display graphics:
	- Draw red borders
*/
void init_GUI() {
	
	/* Color background */
  LCD_Clear(background_color);
	
	/* Draw Left-Top-Right red borders */
	draw_rectangle_vertical(0, field_border, field_border, paddle_y, Red);
	draw_rectangle(0, 0, field_width, field_border, Red);
	draw_rectangle_vertical(field_width - field_border, field_border, field_width, paddle_y, Red);
	
	/* Draw score record in top right corner */
	draw_score_record();
	
	/* Draw score in middle-left position */
	draw_score();
}

/////// TODO: DELETE IF NOT USING EFFICIENT PADDLE SOLUTION
int paddles_overlap(uint16_t current_position, uint16_t last_position) {
	return (current_position > last_position && current_position < last_position + paddle_width) ||
	   (last_position > current_position && last_position < current_position + paddle_width);
}
/////// TODO: DELETE IF NOT USING EFFICIENT PADDLE SOLUTION
int in_field(uint16_t position) {
	return position >= field_border && position < field_width - field_border;
}
///////////////////////////////////////////////////////////





/*
	Move paddle to current_position
*/
void draw_paddle( uint16_t last_position, uint16_t current_position ) {

	if(game_status == PLAYING) {
	
		//////// INEFFICIENT BUT WORKS: ///////////
		/*
		if(current_position > 0)
			draw_rectangle( 0, paddle_y, current_position, paddle_y + paddle_height, background_color );
		draw_rectangle( current_position, paddle_y, current_position + paddle_width + 1, paddle_y + paddle_height, paddle_color );
		if(current_position + paddle_width + 1 < field_width )
			draw_rectangle( current_position + paddle_width + 1, paddle_y, field_width, paddle_y + paddle_height, background_color );
		*/
		///////////////////////////////////////////
		
		
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
	
	}
	
}

void draw_ball(void) {
	
	if(game_status == PLAYING) {
	
		int x_movement;
		int y_movement;

		if (ball_y_position + ball_height >= field_height) {
			// END GAME 
			game_over();
			return;
		}

		/* Bounce on lateral borders */
		if (ball_x_position <= field_border ||
				ball_x_position + ball_width >= field_width - field_border) {
			ball_x_direction = -ball_x_direction;
			play_sound(wall_bounce_sound);
		}

		/* Bounce on top border */
		if (ball_y_position <= field_border) {
			ball_y_direction = -ball_y_direction;
			play_sound(wall_bounce_sound);
		}
		
		/* If ball is at paddle_y and is on top of paddle for at least one pixel => Bounce on paddle */
		if (ball_y_position + ball_height >= paddle_y &&
				((ball_x_position >= paddle_x &&
					ball_x_position <= paddle_x + paddle_width) ||
				 (ball_x_position + ball_width >= paddle_x &&
					ball_x_position + ball_width <= paddle_x + paddle_width))) {
						
					/* Calculate bouncing angle */
					uint16_t bounce_segment = paddle_width / 7;
					if(ball_x_position < paddle_x + bounce_segment) {
						ball_x_direction = -2;
						ball_y_direction = -1;
					} else if(ball_x_position < paddle_x + bounce_segment * 2) {
						ball_x_direction = -1;
						ball_y_direction = -1;
					} else if(ball_x_position < paddle_x + bounce_segment * 3) {
						ball_x_direction = -1;
						ball_y_direction = -2;
					} else if(ball_x_position < paddle_x + bounce_segment * 4) {
						ball_x_direction = 0;
						ball_y_direction = -2;
					} else if(ball_x_position < paddle_x + bounce_segment * 5) {
						ball_x_direction = 1; 
						ball_y_direction = -2;
					} else if(ball_x_position < paddle_x + bounce_segment * 6) {
						ball_x_direction = 1; 
						ball_y_direction = -1;
					} else {
						ball_x_direction = 2;
						ball_y_direction = -1;
					}
					
					/* Emit sound */
					play_sound(paddle_bounce_sound);
					
					/* Score calculation and drawing */
					if(score >= 100) {
						score += 10;
					} else {
						score += 5;
					}
					draw_score();
					
					if(score > score_record) {
							score_record = score;
							draw_score_record();
					}
		}
					
		{
			int score_length = snprintf(NULL, 0, "%d", score);
			int score_record_length = snprintf(NULL, 0, "%d", score_record);
			int score_record_x = field_width - 10 - score_record_length * 8 * score_record_text_size;
			
			/* Check if ball overlaps with score */
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
						
			} else if (((ball_x_position >= score_record_x && ball_x_position <= score_record_x + score_length * score_record_text_size * 8) ||
				  (ball_x_position + ball_width >= score_record_x && ball_x_position + ball_width <= score_record_x + score_record_length * score_record_text_size * 8)) && 
				 ((ball_y_position >= score_record_y && ball_y_position <= score_record_y + score_record_text_size * 16) || 
					(ball_y_position + ball_height >= score_record_y && ball_y_position + ball_height <= score_record_y + score_record_text_size * 16))) {
				/* Check if ball overlaps with score record */
			
				int length = snprintf(NULL, 0, "%d", score_record);
				char string[10];
				int score_record_x = field_width - 10 - length * 8 * score_record_text_size;
				
				sprintf(string, "%d", score_record);
				GUI_Text_but_only_in_rectangle(score_record_x, score_record_y, (uint8_t *)string,
																			 score_color, background_color, score_record_text_size, 
																			 ball_x_position, ball_y_position, ball_x_position + ball_width, ball_y_position + ball_height);

			} else {
				
				/* Cancel old ball */
				draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
											 ball_y_position + ball_height, background_color);
			}
		}
		
		/* Move ball */
		ball_x_position += ball_x_direction;
		ball_y_position += ball_y_direction;
		x_movement = ball_x_direction;
		y_movement = ball_y_direction;

		/* Avoid ball to go out of field */
		if (ball_x_position < field_border) {
			x_movement = -(x_movement + (field_border - ball_x_position));
			ball_x_position = field_border;
		} else if (ball_x_position + ball_width > field_width - field_border) {
			x_movement = x_movement - ((ball_x_position + ball_width) - (field_width - field_border));
			ball_x_position = field_width - field_border - ball_width;
		}
		if (ball_y_position < field_border) {
			y_movement = -(y_movement + (ball_y_position - field_border));
			ball_y_position = field_border;
		} else if (ball_y_position + ball_height >= paddle_y) {
			y_movement = y_movement - (ball_y_position + ball_height - paddle_y);
			ball_y_position = paddle_y - ball_height;
		}
		
		/*
		{
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
		
			if(x_movement > 0) {
					x_start_erase = ball_x_position - x_movement;
					x_end_erase = x_start_erase + ball_width;
					x_start_draw = ball_x_position;
					x_end_draw = ball_x_position + ball_width;
					x_start_erase_2 = x_start_erase;
					x_end_erase_2 = ball_x_position;
					x_start_draw_2 = x_end_erase;
					x_end_draw_2 = x_end_draw;
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
							y_start_erase_2 = ball_y_position - y_movement;
							y_end_erase_2 = ball_y_position + ball_height - y_movement;
						y_start_draw_2 = ball_y_position;
							y_end_draw_2 = ball_y_position - y_movement;
					}
			} else if (x_movement < 0) {
					x_start_erase = ball_x_position;
					x_end_erase = ball_x_position + ball_width;
					x_start_draw = ball_x_position + x_movement;
					x_end_draw = ball_x_position + ball_width + x_movement;
					x_start_erase_2 = ball_x_position + ball_width + x_movement;
					x_end_erase_2 = ball_x_position + ball_width;
					x_start_draw_2 = ball_x_position - x_movement;
					x_end_draw_2 = ball_x_position;
					if(y_movement > 0) {
								y_start_erase = ball_y_position;
								y_end_erase = ball_y_position + y_movement;
								y_start_draw = ball_y_position + ball_height;
								y_end_draw = ball_y_position + ball_height + y_movement;
								y_start_erase_2 = ball_y_position + y_movement;
								y_end_erase_2 = ball_y_position + ball_height;
								y_start_draw_2 = ball_y_position + y_movement;
								y_end_draw_2 = ball_y_position + ball_height - y_movement;
						} else if (y_movement < 0) {
								y_start_erase = ball_y_position + ball_height + y_movement;
								y_end_erase = ball_y_position + ball_height;
								y_start_draw = ball_y_position + y_movement;
								y_end_draw = ball_y_position;
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
							y_start_erase = ball_y_position + ball_height + y_movement;
							y_end_erase = ball_y_position + ball_height;
							y_start_draw = ball_y_position + y_movement;
							y_end_draw = ball_y_position;
							y_start_erase_2 = ball_y_position;
							y_end_erase_2 = y_start_erase;
							y_start_draw_2 = ball_y_position;
							y_end_draw_2 = y_start_erase;
					}
			}
			
			// Erase ball //
			draw_rectangle(x_start_erase, y_start_erase, x_end_erase, y_end_erase, background_color);
			draw_rectangle(x_start_erase_2, y_start_erase_2, x_end_erase_2, y_end_erase_2, background_color);
			
			// Draw ball //
			draw_rectangle(x_start_draw, y_start_draw, x_end_draw, y_end_draw, ball_color);
			draw_rectangle(x_start_draw_2, y_start_draw_2, x_end_draw_2, y_end_draw_2, ball_color);

		}
		*/
		
		/* Draw new ball */
		draw_rectangle(ball_x_position, ball_y_position, ball_x_position + ball_width,
									 ball_y_position + ball_height, ball_color);
	}
}

void draw_score_record(void) {
	int length = snprintf(NULL, 0, "%d", score_record);
	char string[10];
	int text_x = field_width - 10 - length * 8 * score_record_text_size;
	
	sprintf(string, "%d", score_record);
  GUI_Text(text_x, score_record_y, (uint8_t *)string,
           score_color, background_color, score_record_text_size);
}

void draw_score(void) {
	int length = snprintf(NULL, 0, "%d", score);
	char string[10];
	
	sprintf(string, "%d", score);
  GUI_Text(score_x, score_y, (uint8_t *)string,
           score_color, background_color, score_text_size);
}


/*
	Draw rectangular shape on LCD screen
*/
void draw_rectangle( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color ) {
	int x, y;
	for(x = x0; x < x1; x++)
		for(y = y0; y < y1; y++)
			LCD_SetPoint(x, y, color);
}

void draw_rectangle_vertical( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color ) {
	int x, y;
	for(y = y0; y < y1; y++)
		for(x = x0; x < x1; x++)
			LCD_SetPoint(x, y, color);
}

void draw_rectangle_but_only_in_rectangle( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3 ) {
	int x, y;
	for(y = y0; y < y1; y++)
		for(x = x0; x < x1; x++)
			if((x >= x2 && x <= x3) && (y >= y2 && y <= y3)) 
				LCD_SetPoint(x, y, color);
}
