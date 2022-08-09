/*
 *   CONSTANT VALUES
 */

#include "../GLCD/GLCD.h" 

/* GAME */

extern uint8_t score_limit;
 
/* COLORS */

extern uint16_t background_color;
extern uint16_t paddle_color;
extern uint16_t text_color;
extern uint16_t ball_color;
extern uint16_t score_color;
extern uint16_t border_color;

/* FIELD */

extern uint16_t field_width;
extern uint16_t field_height;
extern uint16_t field_border;

/* PADDLE */

extern uint16_t paddle_user_refresh;
extern uint16_t paddle_cpu_refresh;
extern uint8_t paddle_threshold;
extern uint8_t paddle_cpu_speed;	
extern uint16_t paddle_height;
extern uint16_t paddle_width;

/* BALL */

extern int ball_x_position;
extern int ball_y_position;
extern uint16_t ball_refresh;
extern int ball_x_direction;
extern int ball_y_direction;
extern uint16_t ball_height;
extern uint16_t ball_width;
extern uint16_t initial_ball_x;
extern uint16_t initial_ball_y;

/* TEXT */

extern uint16_t you_lose_text_size;
extern uint16_t score_text_size;
extern uint16_t score_record_text_size;
extern uint16_t score_record_y;
extern uint16_t score_x;
