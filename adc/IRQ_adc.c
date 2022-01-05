/*********************************************************************************************************
**--------------File
*Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include "../GUI/GUI.h"
#include "../const/constants.h"
#include "../utility/utility.h"
#include "adc.h"
#include "lpc17xx.h"

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/
/*
unsigned short AD_current;
unsigned short AD_last = 0xFF;

unsigned int measures_amount = 3;
uint16_t measures[3];
unsigned short measure_index = 0; // TODO: RESET ON GAMEOVER
unsigned short measures_filled = 0; // TODO: RESET ON GAMEOVER

void ADC_IRQHandler(void) {
	
	// current_position GENERATED IS IN RANGE [0, 209]
	AD_current = ((LPC_ADC->ADGDR >> 4) & 0xFFF); // Read Conversion Result
	
	if (AD_current != AD_last) {
			
		unsigned int max_value = field_width - paddle_width - 1;
		static int current_position = -1;

		current_position = AD_current * max_value / 0xFFF;
		
		measures[measure_index++] = current_position;
		if(measure_index == measures_amount)
			measure_index = 0;
	
		if(measures_filled > 2) {
			
			unsigned int avg_measure;
			uint16_t max_diff_measure = measures[0];
			uint16_t max_diff;
			int avg_similar_measures = 0;
			int i;
			int min_measure = -1;
			int max_measure = -1;
			
			for(i=0; i<measures_amount; i++) {
				if(min_measure == -1 || min_measure > measures[i])
					min_measure = measures[i]; // Update new min measure in array
				if(max_measure == -1 || max_measure < measures[i])
					max_measure = measures[i];  // Update new max measure in array
			}
			
			avg_measure = (max_measure + min_measure) >> 1;
			max_diff = positive_value(max_diff_measure - avg_measure);
			
			// Find most different measure to exclude it
			for(i=1; i<measures_amount; i++) {
				int new_diff = positive_value(measures[i] - avg_measure);
				if(new_diff > max_diff) {
					max_diff_measure = measures[i];
					max_diff = new_diff;
				} 
			}
			
			// Calculate average measures among similar measures //
			for(i=0; i<measures_amount; i++)
				if(measures[i] != max_diff_measure)
					avg_similar_measures += measures[i];
			avg_similar_measures = avg_similar_measures >> 1;
			
			// Draw paddle in new position //
			if(paddle_x != avg_similar_measures) {
				draw_paddle(paddle_x, avg_similar_measures);
				paddle_x = avg_similar_measures;
			}
				
		} else {
			measures_filled++;
		}
	}
	AD_last = AD_current;
}
*/


unsigned short AD_current;
unsigned short AD_last = 0xFF;

uint16_t measures[3];
unsigned short measure_index = 0;

void ADC_IRQHandler(void) {
    int max_value =
            field_width - paddle_width - 1; // Display width - paddle width

    // current_position GENERATED IS IN RANGE [0, 209]

    AD_current = ((LPC_ADC->ADGDR >> 4) & 0xFFF); // Read Conversion Result

    measures[measure_index] = AD_current;
    measure_index = (measure_index + 1) % 3;

    if (AD_current != AD_last) {
        uint16_t last_position = AD_last * max_value / 0xFFF;
        paddle_x = AD_current * max_value / 0xFFF;
        draw_paddle(last_position, paddle_x);
        AD_last = AD_current;
    }

}

