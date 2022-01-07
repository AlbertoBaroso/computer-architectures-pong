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

unsigned short AD_current;
unsigned short AD_last = 0xFF;

unsigned int measures_amount = 3;
unsigned short measure_index = 0;
unsigned short measures_filled = 0;
uint16_t measures[3];

void ADC_IRQHandler(void) {
	
	// current_position GENERATED IS IN RANGE [0, 209]
	AD_current = ((LPC_ADC->ADGDR >> 4) & 0xFFF); // Read Conversion Result
	
	#ifdef SIMULATOR
		
		if (AD_current != AD_last) {
				
			unsigned int max_value = field_width - paddle_width - 1;
			static int current_position = -1;

			current_position = AD_current * max_value / 0xFFF;
			
			measures[measure_index++] = current_position;
			if(measure_index == measures_amount)
				measure_index = 0;
		
			if(measures_filled >= measures_amount - 1) {
				
				uint16_t max_diff_measure;
				int max_diff = -1;
				int avg_similar_measures = 0;
				int i, j;
				int divisor = ((measures_amount - 1) >> 1);
				int skipped = 0;
				static int time = 0;
				
				// Find most different measure from all other measures //
				for(i=0; i<measures_amount; i++) {
					int difference = 0;
					for(j=0; j<measures_amount; j++) {
						if(i != j) {
							if(measures[i] > measures[j]) {
								difference += measures[i] - measures[j];
							} else {
								difference += measures[j] - measures[i];
							}
						}
					}
					if(max_diff == -1 || difference > max_diff) {
						max_diff = difference;
						max_diff_measure = measures[i];
					}
				}
				
				// Calculate average measures among similar measures //
				for(i=0; i<measures_amount; i++)
					if(measures[i] == max_diff_measure && skipped == 0)
						skipped = 1;
					else 
						avg_similar_measures += measures[i];
				avg_similar_measures = avg_similar_measures >> divisor;
				
				// Draw paddle in new position //
				if(time == 0) {
					
					int last_position = 0; 
					paddle_x = avg_similar_measures;
					time++;
					if(paddle_x <= paddle_width + 1)
						 last_position = field_width - paddle_width - 2;
					draw_paddle(last_position, paddle_x);
					
				} else {
					
					if(positive_value(avg_similar_measures - paddle_x) >= paddle_threshold) {
						draw_paddle(paddle_x, avg_similar_measures);
						paddle_x = avg_similar_measures;
					}
				}
				
					
			} else {
				measures_filled++;
			}
		}
		AD_last = AD_current;
		
	#else 
  
			// current_position GENERATED IS IN RANGE [0, 209]

			if (AD_current != AD_last) {
		
				int max_value = field_width - paddle_width - 1;
				uint16_t last_position = AD_last * max_value / 0xFFF;
				uint16_t current_position = AD_current * max_value / 0xFFF;
				draw_paddle(last_position, current_position);
				AD_last = AD_current;
			}
		
	#endif
}
