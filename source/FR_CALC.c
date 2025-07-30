#include <FR_CALC.h>
#include <infused_calculate.h>
#include "LPC55S16.h"
#include "Bolus_Header.h"
#include "Keypad.h"
#include "TFT_Commands.h"
#include "PWM.h"
#include"FR_CALC.h"
#include "Color.h"
#include "Motor.h"
#include <math.h> //ZOOZ_1
#include "STEPPER.h"
#include "Driver_Timer.h"
#include"FR_CALC.h"
#include "TFT_String.h"
#include "UART.h"

uint32_t RPM;	//for testing only
uint32_t off_time;

//extern void Timer1_Init();

#define FULL_STEP 200
#define STEP_8    8
#define SEC       60.0
#define MINUTE    60.0
#define MHZ_150   150000000.0
#define MHZ_96    96000000.0

extern uint32_t MR_VALUE;
extern uint32_t motor_pulse;
extern uint32_t one_ml_pulse;
 uint32_t G_1ml_pulse;
extern uint32_t G_10ml_value;
extern uint32_t G_10_1ml_pulse;
extern uint32_t G_20_1ml_pulse;
extern uint8_t g_size_vol;
extern volatile uint16_t value_l[8];  //28-03
extern uint8_t normal_run;//ZOOZ_1

extern uint32_t Pulse_ratio(uint32_t pulse);
extern void Timer0_Init( );
extern void Timer1_Init(uint32_t MR_VALUE);//ZOOZ_1
rampvar RAMP;
Runconcept MOT;
syringes syr;

extern uint32_t DAC__datawrite(uint16_t addrs1,float voltage, uint16_t bytes_val);
extern void infuseD_ml(uint32_t pulse_to_one_ml);
extern void Timer_disable();
uint32_t Ramp_pre_FR=0;
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void MR_Value_calculate(uint32_t floWrate)
{
	  uint32_t MR_VALUE=0;
	  uint32_t pulse_per_ml=0;
	  float time_per_pulse=0;
	  float temp_pre_rpm=0.0;
	  float FR_pulse=0.0;
	  float _1sec_pulse=0.0;
	  float pulse=0.0;

	  Ramp_pre_FR=floWrate;
	 temp_pre_rpm=RAMP.RPM_control;
	 uint8_t syr_vol[7]={0,2,5,10,20,30,50};
//   float syr_Len[7]={0.0,33.0,38.0,51.0,63.0,68.0,75.0};

	 syr.size=syr_vol[6];
//	 TFT_Number(200,20,syr.size,WHITE,RED);
     //syr.length=(float)value_l[g_size_vol]/10;
	 syr.length=750.0;      //(float)value_l[g_size_vol]/10;
//	 TFT_Number(230,20,syr.length,WHITE,RED);
	 pulse_per_ml=(((((float)syr.length/syr.size)/10.0)*5.0)*FULL_STEP*STEP_8);

	 FR_pulse=(float)pulse_per_ml*((float)floWrate/10.0);
//	 TFT_Number(230,60,floWrate,WHITE,RED);
	 _1sec_pulse=FR_pulse/3600.0;

	 pulse=(1.0/_1sec_pulse);//*1000.0;

	 time_per_pulse=pulse;

//	 Font_Select(ARIAL_14);
//
//	 TFT_Number(0,20,pulse_per_ml,WHITE,RED);
//	 TFT_writenumber_float_com(0,60,FR_pulse,WHITE,RED);
//	 TFT_writenumber_float_com(0,90,_1sec_pulse,WHITE,BLACK);
//	 TFT_writenumber_float_com(0,120,pulse,YELLOW,BLACK);
//	 TFT_writenumber_float_com(0,150,time_per_pulse,WHITE,BLACK);

	 MR_VALUE=(uint32_t)(((time_per_pulse*MHZ_96)/2.0));

	 infuseD_ml(pulse_per_ml); // to infused ml


//	Font_Select(ARIAL_14);
//	TFT_Number(60,120,MR_VALUE,YELLOW,RED);

 	 RAMP.temp_MRVALUE = MR_VALUE;
	 RAMP.RPM_control= (float)MR_Value_TO_RPM_calculate(MR_VALUE);// to rpm
	 MFB.rpm_decode =RAMP.RPM_control;
//	 Font_Select(ARIAL_14);
//	 TFT_Number(200,50,RAMP.RPM_control,YELLOW,BLACK);
//	 Font_Select(ARIAL_14);
//	 TFT_Number(0,180,MR_VALUE,RED,BLACK);
	 MFB.cuts_time_diff=(60*1000*1.6)/(3*MFB.rpm_decode);// cuts per ms

	 if(MFB.rpm_decode<2)
	 {
	   MFB.cuts_time_diff=(MFB.cuts_time_diff*3*190)/360;
	 }

	 if(RAMP.RPM_control > 400.0)
	 {
		RAMP.RPM_control=400;          					 //The rpm limit is 400
		RAMP.temp_MRVALUE=(_1_RPM/400); 					 // mr value for 400 rpm
	 }

	RAMP.down_flag=0; 				  					     // NOT TO RAMP DOWN WHILE STARTING THE MOTOR
    if( (RAMP.RPM_control <= 50.0) || (RAMP.on_off == 0))    //50rpm and ramp on off flag
    {
    	RPM_to_DAC(RAMP.RPM_control);						//vref value calculation using pwm
//    	DRV8428_ENABLE();
    	stepper_Forward();
    	Timer1_Init(MR_VALUE); 								// to handler
    	RAMP.up_flag=0;
    }

    else
    {
    	RAMP.up_flag=1;
    	if(!normal_run)
    	{
          RAMP.inc_dec=0;
    	}
    	else
    	{
          RAMP.inc_dec=temp_pre_rpm/50;
    	}
     }
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

float MR_Value_TO_RPM_calculate(unsigned int mr_value)
{
	float mr_value1,rpm=0;
	mr_value1 = mr_value * 2.0; // 281250 * 2 = 562500 ;
	mr_value1 = mr_value1 /MHZ_96; // 0.00375
	mr_value1 = 1/mr_value1; // 1/0.00375 = 266.66666666666;
	mr_value1 = mr_value1 * 60 ; //  266.66666666666 * 60 = 16000
	rpm = (float)(mr_value1/(FULL_STEP*STEP_8));
//	mr_value1 = ((mr_value * 2.0)/MHZ_96); // 281250 * 2 = 562500 ;
//	mr_value1=((1/mr_value1)*60);
//	rpm = (float)(mr_value1/(FULL_STEP*STEP_8));
//	 Font_Select(ARIAL_14);
//
//	 TFT_Number(0,20,mr_value,WHITE,RED);
//	 TFT_writenumber_float_com(0,60,mr_value1,WHITE,RED);
//	 TFT_writenumber_float_com(0,90,rpm,WHITE,BLACK);
//	 TFT_writenumber_float_com(0,120,pulse,YELLOW,BLACK);
//	 TFT_writenumber_float_com(0,150,time_per_pulse,WHITE,BLACK);
	return rpm;
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void RAMP_UP(uint32_t millsec,uint16_t rpm,float RPM_control)
{
	if(RAMP.up_flag)
	{
		if(!RAMP.timer_interval)
		{
			RAMP.timer_interval=millsec;
			RAMP.inc_dec++;
			RPM_to_DAC(RAMP.RPM_control);

			if((RAMP.inc_dec*rpm) < (RPM_control))
			{
//				DRV8428_ENABLE();
				stepper_Forward();
				Timer1_Init(   ((_1_RPM)/(RAMP.inc_dec*rpm))  );
			}
			else
			{
				DRV8428_ENABLE();
//				RPM_to_DAC(RAMP.RPM_control);
				Timer1_Init(RAMP.temp_MRVALUE);
				RAMP.up_flag=0;
			//	RAMP.inc=0; /*CHECK*/
			}
		}
	}
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void RAMP_DOWN(uint32_t millsec,uint16_t rpm)
{
	if(RAMP.down_flag)
	{
		if(!RAMP.timer_interval)
		{
			RAMP.timer_interval=millsec;

			if((RAMP.inc_dec*rpm) >= (rpm)) ///  125
			{
				RPM_to_DAC((float)(RAMP.inc_dec*rpm));
				Timer1_Init(   ((_1_RPM)/(RAMP.inc_dec*rpm))  );
			}
			else
			{
				Timer_disable();
				RAMP.down_flag=0;
			}

				if(RAMP.inc_dec)
				{
					RAMP.inc_dec--;
				}
		}
	}
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
/**/

void clear_data()
{
	MOT.flowrate=0;
	INFUSED.sec=0;
	INFUSED.ml=0;
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void RPM_to_DAC(float v_rpm )
{

//	float voltage=0.0;
	uart_frame_func("$",0,"#");
	if(v_rpm<=50.0)
	{
		uart_frame_func("$",310,"#");
//		voltage=0.9;			    //(0.1-130ml/h)
	}
	else if(v_rpm<=100.0)   		//(131-262 ml/h)
	{
		uart_frame_func("$",310,"#");
//		voltage=0.20;
	}
	else if( v_rpm<=200.0)  		//ok(263 -  523ml/h)
	{
		uart_frame_func("$",310,"#");
//		voltage=0.21;
	}
	else if(v_rpm<=300.0)	   		//(524-783ml/h)
	{
		uart_frame_func("$",310,"#");
//		voltage=0.30;
	}
	else if( v_rpm<=400.0)			//(784-1043 ml/h)
	{
		uart_frame_func("$",310,"#");
//		voltage=0.35;
	}
	else if( v_rpm<=500.0)   		//1200  maximun-461 rpm  //(1044-1200ml/h)
	{
		uart_frame_func("$",310,"#");
//		voltage=0.3;
	}
	else
	{
		uart_frame_func("$",310,"#");
//		voltage=0.50;
	}
//	DAC__datawrite(0x00,voltage,2);
	delay_ms(100);
}
