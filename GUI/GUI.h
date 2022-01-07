/*
 *   GUI
 */

#include "LPC17xx.h"

void init_GUI(void);
void draw_rectangle( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void draw_rectangle_vertical( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void draw_rectangle_but_only_in_rectangle( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3 );
void draw_rectangle_but_only_in_rectangles( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t x4, uint16_t y4, uint16_t x5, uint16_t y5 );
void draw_rectangle_but_not_in_rectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3 );
void draw_paddle( uint16_t last_position, uint16_t current_position );
void draw_ball(void);
void draw_score_record(void);
void draw_score(void);
void move_ball_inefficient(void);
void move_ball_efficient(void);
