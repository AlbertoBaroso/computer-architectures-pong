/*
 *   CONSTANT VALUES
 */

#include "../GLCD/GLCD.h" 
 
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

extern uint8_t paddle_threshold;
extern uint16_t paddle_height;
extern uint16_t paddle_width;
extern uint8_t paddle_fps;
extern uint16_t paddle_x;
extern uint16_t paddle_y;

/* BALL */

extern uint16_t ball_x_position;
extern uint16_t ball_y_position;
extern int ball_x_direction;
extern int ball_y_direction;
extern uint16_t ball_height;
extern uint16_t ball_width;
extern uint8_t ball_fps;

/* TEXT */

extern uint16_t you_lose_text_size;
extern uint16_t score_text_size;
extern uint16_t score_record_text_size;
extern uint16_t score_record_y;
extern uint16_t score_x;
extern uint16_t score_y;

/* SOUND */

extern uint16_t SinTable[45];
extern const int freqs[8];
extern uint8_t wall_bounce_sound;
extern uint8_t paddle_bounce_sound;
extern uint8_t sound_lenght;
