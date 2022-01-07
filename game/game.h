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

/* SCORE */

extern uint32_t score;
extern uint32_t score_record;

extern uint32_t old_score;						// Old scores, used for efficient character replacing on screen
extern uint32_t old_score_record;

/* FUNCTIONS */

void game_init(void);
void game_start(void);
void game_over(void);
