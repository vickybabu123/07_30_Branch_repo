#include "LPC55S16.h"
//#include "Power_off.h"


#ifndef FR_CALC_H_
#define FR_CALC_H_

#define RAMP_RPM 50
#define _MILLISEC 10               //10

#define  STEP           8
#define MFB_CUT_FACTOR  50
#define FLOWRATE      500
//#define TEMPCUTS        106.3
#define CLK_FRQ       150000000
#define _1_RPM 		  2812500

typedef struct ramping
{
	uint8_t on_off;
	uint16_t timer_interval;
	uint32_t temp_MRVALUE;
	 float RPM_control;
	volatile uint8_t up_flag;
	uint16_t inc_dec;
	uint8_t down_flag;
	//uint8_t dec;
}rampvar;

extern rampvar RAMP;

typedef struct
 {
	//float ppm;
	float pps;
	//float temp_ppm;
	//float RPM_flt;
	//uint16_t actual_rpm_decode;
	uint32_t pulse_per_pt_one_ml;
	uint32_t pulse_per_pt_one_ml_cnt;
	uint32_t calc_PPS;
	 uint32_t flowrate;
	 uint32_t volume;
	 uint32_t time;
	 uint32_t intervel;
	 uint32_t ramp_up;
	 uint32_t ramp_down;
	uint32_t plateau_rate;

 }Runconcept;

 typedef struct
 {
 	uint8_t setclr;
 	volatile uint8_t on;
 	volatile uint8_t micro_sec;
 }pulse_control ;


 typedef struct flowrate
  {
 	float length;    //Give 75   for 75  mm
 	uint8_t size;    //Give 50   for 50  ml
 	uint16_t  flowrate;
  }syringes;

extern syringes syr;
extern pulse_control PULSE;
extern Runconcept MOT;
void MR_Value_calculate(uint32_t floWrate);
float MR_Value_TO_RPM_calculate(unsigned int mr_value);
void RAMP_UP(uint32_t millsec,uint16_t rpm,float RPM_control);
void RAMP_DOWN(uint32_t millsec,uint16_t rpm);
void clear_data();
void RPM_to_DAC(float v_rpm );

#endif /* FR_CALC_H_ */
