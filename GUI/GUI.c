#include "LPC17xx.h"
#include "GUI.h"
#include "../GLCD/GLCD.h" 
#include "../const/constants.h"

// TODO: PUT NICE HEADER COMMENTS

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
