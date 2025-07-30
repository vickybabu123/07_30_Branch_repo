/*
 * infused_calculate.h
 *
 *  Created on: 25-Sep-2023
 *      Author: Software4
 */
#include<stdint.h>

#ifndef INFUSED_CALCULATE_H_
#define INFUSED_CALCULATE_H_

typedef struct infused_ml
{
	uint16_t step_count;
	uint32_t stepsper_point_1ml;
	uint16_t ml;
	uint16_t mill_sec;
	uint16_t sec;

}infuse;

extern infuse INFUSED;

typedef struct tim  //ZOOZ_1
{
	 uint8_t hour;
	 uint8_t minute;
	 uint8_t seconds;
} clk;

extern clk time;

void infused_data(uint8_t screen,uint8_t r_mode);		//28-03
void bottom_para_frame(uint8_t mode);
void near_to_target();
clk sec_to_time_format(uint32_t sec);
#endif /* INFUSED_CALCULATE_H_ */

