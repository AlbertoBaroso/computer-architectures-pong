#include "button.h"
#include "lpc17xx.h"
#include "../RIT/RIT.h"
#include "../game/game.h"
#include "../timer/timer.h"
#include "../GUI/GUI.h"
#include "../const/constants.h"

uint8_t old_value = 0;
uint8_t new_value = 1;
volatile int button0_pressed = 0;
volatile int button1_pressed = 0;
volatile int button2_pressed = 0;

void button0_action (void) {	    // 

}

void button1_action (void) {			//  

}

void button2_action (void) {			// PAUSE/RESUME GAME
	
	int text_x = (field_width >> 1) - 11 * 4;
	int text_y = (field_height >> 1) - 4;
	
	if(game_status == PLAYING) {
		
		/* Pause game */
		disable_timer(0); // Stop ball movement
		disable_timer(1); // Stop paddle movement
		GUI_Text(text_x, text_y, (uint8_t *)"GAME PAUSED",
           text_color, background_color, 1);
		game_status = PAUSED;
		
	} else if(game_status == PAUSED) {
		
		/* Resume playing */
		enable_timer(0);	// Resume ball movement
		enable_timer(1);	// Resume paddle movement
		draw_rectangle(text_x, text_y + 1, text_x + 11 * 8, text_y + 12, background_color);
		game_status = PLAYING;
		
	}
}

void EINT0_IRQHandler (void)
{
		button0_pressed = 1;
		enable_RIT();
		NVIC_DisableIRQ(EINT0_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 20);     /* GPIO pin selection */
		LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}

void EINT1_IRQHandler (void)
{
		button1_pressed = 1;
		enable_RIT();
		NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
		LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)
{
		button2_pressed = 1;
		enable_RIT();
		NVIC_DisableIRQ(EINT2_IRQn);		/* disable Button interrupts			 */
		LPC_PINCON->PINSEL4    &= ~(1 << 24);     /* GPIO pin selection */
		LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */
}


