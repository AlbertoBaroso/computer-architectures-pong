#include "LPC17xx.h"

int positive_value(int);
void play_sound(int);
void init_DAC(void);
int paddles_overlap(uint16_t current_position, uint16_t last_position);
int in_field(uint16_t position);
