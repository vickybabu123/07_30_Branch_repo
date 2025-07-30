/*
 * Linear_Pot.h
 *
 *  Created on: 08-Jan-2024
 *      Author: Software6
 */

#ifndef LINEAR_POT_H_
#define LINEAR_POT_H_

float syringe_nearempty_calculation(uint16_t flowarte);
void display_funtion();
void motor_stop();
void adc_value_for_induvidual_syringe();
//void syringe_end_check(uint16_t plunger_ADC_val);
void near_empty_check(uint16_t plunger_ADC_val);
float remaining_volume_in_syringe();
void syringe_position_check();
void syringe_end_check(uint16_t plunger_ADC_val,uint8_t syr_size);
void syringe_live_status(uint8_t sy_size,uint16_t rem_vol);
uint16_t LIN_POT_sample(uint16_t Raw_val, uint16_t Data_diff,uint8_t samples );


typedef struct threader
{
	uint16_t pre_current ;
	uint16_t repeat_count;
	uint16_t linear_pot_pulse;


}pot_clamp;

extern pot_clamp CLAM;

#endif /* LINEAR_POT_H_ */
