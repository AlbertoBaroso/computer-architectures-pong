/*
 *   CONSTANT VALUES
 */

#include "../GLCD/GLCD.h" 
 
/* COLORS */

extern uint16_t background_color;
extern uint16_t paddle_color;
extern uint16_t text_color;
extern uint16_t ball_color;

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

/* GAMEPLAY */

#define PLAYING 1
#define NOT_PLAYING 2
#define PAUSED 3

extern unsigned int game_status;

/* SOUND */

extern uint16_t SinTable[45];
extern const int freqs[8];
extern uint8_t wall_bounce_sound;
extern uint8_t paddle_bounce_sound;
extern uint8_t sound_lenght;
