// TODO: PUT NICE HEADER COMMENTS
#include "LPC17xx.h"

void init_GUI(void);
void draw_rectangle( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void draw_rectangle_vertical( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color );
void draw_paddle( uint16_t last_position, uint16_t current_position );

