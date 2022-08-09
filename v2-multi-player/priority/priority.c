/********************************************************************
** ########################### File Info ############################
**
** File name:           priority.c
** Last modified Date:  2022/01
** Descriptions:        Unique place to update peripherals priorities
** Correlated files:    priority.h
**
** ##################################################################
********************************************************************/

#include "LPC17xx.h"

/* TIMER */

uint8_t tim0_priority = 8; // Ball movement
uint8_t tim1_priority = 6; // Player paddle movement
uint8_t tim2_priority = 5; // CPU paddle movement
uint8_t tim3_priority = 9;
uint8_t RIT_priority = 1;  // Button debouncing

/* BUTTONS */

uint8_t button0_priority = 4;
uint8_t button1_priority = 3;
uint8_t button2_priority = 2;

/* ADC */

uint8_t ADC_priority = 7;
