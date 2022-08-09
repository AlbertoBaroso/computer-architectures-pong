/*
 *  GAME
 */

#include "LPC17xx.h"

/* GAMEPLAY */

#define PLAYING         1  /* Game status */
#define GAME_ENDED      2
#define PAUSED          3
#define READY_TO_PLAY   4

extern unsigned int game_status;

/* PLAYERS */

#define TOP_PLAYER			1
#define BOTTOM_PLAYER		2

extern uint16_t paddle_top_x;
extern uint16_t paddle_top_y;
extern uint16_t paddle_bottom_x;
extern uint16_t paddle_bottom_y;

/* SCORE */

extern uint32_t top_score;
extern uint32_t bottom_score;

/* FUNCTIONS */

void game_init(void);
void game_start(void);
void game_over(void);
void match_over(int winner);
void match_start(void);
void move_cpu_paddle(void);
