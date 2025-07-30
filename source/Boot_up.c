#include "LPC55S16.h"
#include "Color.h"
#include "Driver_SPI.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "TFT_String.h"
#include "clock_config.h"
#include "Boot_up.h"
#include"Driver_I2C.h"
#include"Driver_Timer.h"
#include "HEADER.h"
#include <Consumed_hrs.h>
#include "Occlusion.h"
#include"Driver_ADC.h"
#include "GPIO.h"
#include "PWM.h"
#include  "sound.h"
#include <STEPPER.h>
#include "Keypad.h"
#include "Driver_PINT.h"
#include "Factory.h"
#include "Bolus_Header.h"
#include "Dosage_mode.h"//29-03
#include "FR_CALC.h"
//#include "DAC_driver.h"
//#include "battery.h"
#include <infused_calculate.h>
#include <Main_screen.h>

extern uint16_t reset_year;

//extern factorys FACT;
//Occlusion Occl;
extern uint8_t G_infusion_mode_opt;
extern uint8_t G_occl_unit;
extern uint8_t G_occl_level;
extern uint8_t G_kvo;
extern uint32_t G_KVO_fr;
extern uint16_t G_bol_fr;
extern uint8_t G_bolus_mode;
extern uint8_t G_bright_level;
extern uint8_t G_sound_level;
extern uint16_t G_date_mnth;
extern uint16_t G_year;
extern uint16_t G_time;
extern uint16_t G_time_format;
extern uint32_t G_10ml_value;
extern uint32_t G_20ml_value;
extern uint32_t G_10_1ml_pulse;
extern uint32_t G_20_1ml_pulse;
extern uint16_t total_drugs;
extern uint8_t total_brand;
extern char Drug_Names[150][4][20];//22-03
extern char Brand_Names[6][4][15];
extern uint16_t G_store_val;
extern uint16_t manu_dt_yr;
extern uint32_t serialno;
extern uint8_t G_qc_track;
extern uint8_t G_infusion_modes;
extern uint8_t G_drug_list;
extern uint8_t G_lock_time;
extern uint16_t total_events;
extern uint16_t fav_drug_list[11];//29-03
extern uint8_t total_fav;//29-03
extern uint8_t total_recent;//29-03
extern uint32_t G_loaded_adc_val[9];
extern uint32_t  G_minval[9];
extern uint8_t brand_new_no;//22-03
extern uint16_t sl_pullout_ADC;
extern uint16_t sl_pullin_ADC;
extern uint8_t brand_select_no;
extern uint8_t bright_count;
extern uint8_t G_last_data;
extern uint8_t G_screen_saver;
extern uint8_t G_qc_track;
extern uint8_t home_flag;
//
//extern void Reset_loading(uint8_t loading_level);

//extern void RTC_INIT_time (void);//22-03
void EEPROM_default_write(void);
void EEPROM_value_read(void);
void fav_eeprom_read(void);//29-03
extern uint8_t  previous_data(uint8_t mode);
extern void EEPROM_SD_READ(void);
extern void SL_EEPROM_read();
extern void Pressure_EEPROM_read();
extern void recent_eeprom_read(void);//29-03
extern void Pressure_EEPROM_read();



extern volatile uint16_t value_l[8];
extern volatile uint16_t value_c[8];
extern uint16_t pull_out_val;
extern uint16_t G_baseval;
extern uint8_t ON_Flag;
extern uint8_t prev_data_flag;
extern uint16_t maxi_lmt[7];
//extern uint32_t  G_minval[8];
extern uint32_t G_maxval[8];
//extern void Complete_reset_frame(void);
//extern uint16_t xpos;
extern char local_buffer[10];

void progress_bar(uint16_t strat_x,uint16_t end_x,uint16_t y_pos,uint8_t space,uint8_t height,uint32_t color);
void shutting_down(void);
void loading_Animation(uint16_t x_axis,uint16_t y_axis,uint16_t Length,uint16_t Height);
char* array_seg[11]={"Verifying Firmware Authenticity ","Verifying Self-test Thresholds","Logging System Startup Events ","Initializing Communication Interfaces ",
		             "Fetching EEPROM Configuration ","Boot Display Module Diagnosing","Evaluating Battery Health Status ","Diagnosing Motor DRV Module ",
					 "Performing Sensor Calibration Check","Transition To Operational Mode "
};
extern uint8_t load_flag;
extern uint8_t G_standby;
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Boot_up()
{
	     GPIO_Init();
	 	 batt_pin_init();
	//   buzzer_pwm();
	     PINT_EXT_Init1();
	     LPADC_Init(ADC_CH_PLUNGER_POT);
	     LPADC_Init(ADC_CH_SD_POT);
		 LPADC_Init(ADC_CH_POWER_KEY);
		 LPADC_Init( ADC_CH_BATTERY);
//	     RTC_INIT_time();//22-03
		 DRV8428_Init();
		// stepper_Forward();
	     Keypad_Init();
	     Timer0_Init();
	 	 Timer2_Init();
//	 	 Timer3_Init();
		 SPI5_Init();
		 TFT_ST7789V_init();
	// 	 DRV8428_DISABLE();
		 i2c_Init(0);
//		 UART7_init(9600,0);
	 	 PWM_Init();

//		 DAC__datawrite(0x00,0.0,2);

//		 Rectangle(0,0,320,240,MAIN_FRAME_COL);
//		 self_test_process();
//  	 EEPROM_value_read();

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void progress_bar( uint16_t start_x,uint16_t end_x,uint16_t y_pos,uint8_t space,uint8_t height,uint32_t color)
{
	uint16_t loop_count=0;
	uint16_t loop_count1 = 0;
	Rectangle(20,y_pos-5,280,0,color);
	Rectangle(20,y_pos+height+5,280,0,color);

	Rectangle(20,y_pos-5,0,height+5+5,color);
	Rectangle(300,y_pos-5,0,height+5+5,color);

	for(loop_count=start_x;loop_count<=end_x;loop_count++)
	{
		Rectangle(start_x+(loop_count1*space),y_pos,0,height,color);
//		delay_ms(900);
		delay_us(10);
//		delay_secs(1);
		loop_count1++;
	}
	delay_secs(1);
//	delay_ms(900);
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void shutting_down(void)
{
	uint16_t XPOS=0;

	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Power off");

	Frame_Footer_new(NEW_FTR_COL,HDR_TXT_COL,(uint8_t *)"Shutting Down");

//	XPOS=Letters_Alignments(0,319,(uint8_t *)"Please wait",0);
//	TFT_String(XPOS,170,(uint8_t *)"Please wait",MAIN_FRAME_COL,BLACK);
	TFT_String_align(0,319,170,(uint8_t *)"Please wait",0,MAIN_FRAME_COL,WHITE);

	CENTER_SCR(MAIN_FRAME_COL); //center green
	progress_bar(25,160,120,2,30,RED);


}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void self_test_process()
{
//	uint16_t loop_count=0;
	uint16_t XPOS=0;
	uint8_t key_fail_count=0;
	uint8_t ver_check_flag=0;

	Rectangle(0,0,320,240,MAIN_FRAME_COL);

//		G_bright_level=EEPROM_dataread(EEPROM_G_bright_level,1);

		if(G_bright_level>4)
		{
			G_bright_level=4;
		}

		//AKAS_LOGO(83,55,0,MAIN_FRAME_COL,BLACK);
		akas_logo();
		delay_secs(2);
		Rectangle(0,0,320,240,WHITE);
		loading_Animation(130,120,10,4);
//		Font_Select(ARIAL_BOLD_14);

//		Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Self Testing");
//		Frame_Footer_new(NEW_FTR_COL, HDR_TXT_COL,(uint8_t *)"Please wait...");
//
//		CENTER_SCR(MAIN_FRAME_COL); //center green
//
//		Font_Select(ARIAL_BOLD_14);
//		XPOS=Letters_Alignments(0,319,(uint8_t *)"Processing...",0);
//		TFT_String(XPOS,170,(uint8_t *)"Processing...",MAIN_FRAME_COL,BLACK);
//		TFT_String_align(0,319,170,(uint8_t *)"Processing...",0,MAIN_FRAME_COL,BLACK);
//		progress_bar(25,60,90,2,30,BAR_COL);
//		delay_ms(500);
//
//		//Rectangle(0,170,320,20,MAIN_FRAME_COL);
////		XPOS=Letters_Alignments(0,319,(uint8_t *)"EEPROM Check",0);
////		TFT_String(XPOS,170,(uint8_t *)"EEPROM Check",MAIN_FRAME_COL,BLACK);
//		TFT_String_align(0,319,170,(uint8_t *)"EEPROM check",0,MAIN_FRAME_COL,BLACK);
//		progress_bar(60+25,140,90,2,30,BAR_COL);
//		delay_ms(500);

		if(VERSION == EEPROM_dataread(EEPROM_SOFTWARE_VERSION,2))
			{

				EEPROM_value_read();
				G_infusion_modes=EEPROM_dataread(EEPROM_LAST_IFMODE,1); //read the last infuision mode from EEPROM
//
				if(G_infusion_modes>4)
				{
					G_infusion_modes=4;
					G_infusion_mode_opt=4;
				}
//
				ver_check_flag=1;
//
			}

			else
			{
//				Rectangle(XPOS,170,200,20,MAIN_FRAME_COL);    how to match
				Font_Select(ARIAL_9);
////				XPOS=Letters_Alignments(0,319,(uint8_t *)"Updating default value",0);
////				TFT_String(XPOS,170,(uint8_t *)"Updating default value",MAIN_FRAME_COL,BLACK);
				TFT_String_align(0,295,210,(uint8_t *)"Updating default value",0,WHITE,INDIGO);
				EEPROM_default_write();
				EEPROM_datawrite(EEPROM_SOFTWARE_VERSION,VERSION,2);

				if(VERSION == EEPROM_dataread(EEPROM_SOFTWARE_VERSION,2))
				{

				}

				else
				{
//					ON_Flag=0;
//					pop_up_info_frame(1, ORANGE);
//					shutting_down();
//					CLRSCR(BLACK);
				}
			}

			if(ON_Flag)
			{
				//Font_Select(ARIAL_BOLD_14);
				//Rectangle(0,170,320,20,MAIN_FRAME_COL);
//				XPOS=Letters_Alignments(0,319,(uint8_t *)"Key Test",0);
//				TFT_String(XPOS,170,(uint8_t *)"Key Test",MAIN_FRAME_COL,BLACK);
				//TFT_String_align(0,319,170,(uint8_t *)"Key Test",0,BLACK,GREEN);
				//progress_bar(165,230,90,2,30,BAR_COL);
				//delay_ms(500);

				while(KEY.value)
				{
					key_fail_count++;

					if(key_fail_count>10)
					{
						pop_up_info_frame(4, ORANGE);
						shutting_down();
						ON_Flag=0;
						CLRSCR(BLACK);
						break;

					}
				}
			}

			if(ON_Flag &&  ver_check_flag)
			{
				ver_check_flag=0;

					if(G_last_data)
					{
						prev_data_flag = previous_data(0);
					}

					if(!prev_data_flag)
					{
						G_infusion_modes = G_infusion_mode_opt;
					}

					else
					{
						//home_flag=0;
					}

			}

}


/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void EEPROM_default_write(void)
{
	uint8_t loop_count1=0,loop_count=0;

	G_occl_unit=0;
	EEPROM_datawrite(EEPROM_G_occl_unit ,G_occl_unit,1);

	G_occl_level=1;
	EEPROM_datawrite(EEPROM_G_occl_level ,G_occl_level,1);

	BOL.mode=1;
	EEPROM_datawrite(EEPROM_G_bolus_mode ,BOL.mode,1);

	BOL.flowrate=1000;
	EEPROM_datawrite(EEPROM_G_bol_fr ,BOL.flowrate,2);
//	for(loop_count=0;loop_count<6;loop_count++)
	{
		//EEPROM_datawrite(EEPROM_G_bol_limit+(loop_count*2) ,BOL.limit[loop_count],2);
	}

//	Font_Select(ARIAL_14);
//	TFT_Number(0,140,109,YELLOW,RED);
	G_kvo=0;
	EEPROM_datawrite(EEPROM_G_kvo ,G_kvo,1);
	G_KVO_fr=1;
	EEPROM_datawrite(EEPROM_G_KVO_fr,G_KVO_fr,2);
	G_bright_level=4;
	EEPROM_datawrite(EEPROM_G_bright_level,G_bright_level,1);
	G_sound_level=4;
	EEPROM_datawrite(EEPROM_G_sound_level,G_sound_level,1);
	G_last_data=0;
	EEPROM_datawrite(EEPROM_LAST_DATA,G_last_data,1);
	G_screen_saver=1;
	EEPROM_datawrite(EEPROM_SCREEN_SAVER,G_screen_saver,1);
	G_date_mnth=0;
	EEPROM_datawrite(EEPROM_G_date_mnth,G_date_mnth,2);
	G_year=0;
	EEPROM_datawrite(EEPROM_G_year,G_year,2);
	G_time=0;
	EEPROM_datawrite(EEPROM_G_time,G_time,2);
	G_time_format=0;
	EEPROM_datawrite(EEPROM_G_time_format,G_time_format,1);

	G_standby=0;
	EEPROM_datawrite(EEPROM_STANDBY,G_standby,1);
	//-----------------------For Drugs---------------------------//

	total_drugs =500;	//22-03
	EEPROM_datawrite(EEPROM_TOTAL_DRUG,total_drugs,2);
	//----------------------------------------------------------------
	total_brand=8;
	EEPROM_datawrite(EEPROM_TOTAL_BRAND,total_brand,1);//22-03
	brand_select_no=0;
	EEPROM_datawrite(EEPROM_BRAND_NO,brand_select_no,1);
	G_store_val=0;
	EEPROM_datawrite(EEPROM_LOAD_CELL_VAL,G_store_val,2);
	Occl.Zero_PSI=0;
	EEPROM_datawrite(EEPROM_Occl_strain_0PSI_value,Occl.Zero_PSI,2);
	FACT.factory[0]=0;
	EEPROM_datawrite(EEPROM_MFG_MON_YEAR,FACT.factory[0],2);
	FACT.factory[3]=0;
	EEPROM_datawrite(EEPROM_SERIAL_NO,FACT.factory[3],3);
	//FACT.factory[5]=0;
	//EEPROM_datawrite(EEPROM_MFG_MON_YEAR,FACT.factory[5],2);
	G_qc_track=0;
	EEPROM_datawrite(EEPROM_QC_TRACK,G_qc_track,1);

	G_lock_time=0;
	EEPROM_datawrite(EEPROM_G_lock_time,G_lock_time,1);

	G_infusion_modes=4;
	G_infusion_mode_opt=4;
	prev_data_flag = 0;

	EEPROM_datawrite(EEPROM_G_infusion_modes,G_infusion_modes,1);
	total_fav=0;
	EEPROM_datawrite(EEPROM_TOTAL_FAV,total_fav,1);//29-03
	total_recent=0;
	EEPROM_datawrite(EEPROM_TOTAL_RECENT,total_recent,1);
	for(loop_count=0;loop_count<10;loop_count++)
	{
		EEPROM_datawrite(EEPROM_FAV_DRUG_LIST+(loop_count*2),0,2);
		EEPROM_datawrite(EEPROM_RECENT_DRUG_LIST+(loop_count*2),0,2);//29-03
	}
	total_events=0;
	EEPROM_datawrite(EEPROM_TOTAL_EVENTS,total_events,2);//29-03
	G_drug_list=0;
	EEPROM_datawrite(EEPROM_G_drug_list,G_drug_list,1);
//*******************************USED_HRS******************************//

		EEPROM_usedhrs_write(1);//ALL USED_HRS RESET TO ZERO(PASSING VALUE 1)

//*******************************SL_CALIBRATION******************************//

   sl_pullin_ADC=2901;
   EEPROM_datawrite(EEPROM_SL_PULL_IN,sl_pullin_ADC,2);
   sl_pullout_ADC=62001;
   EEPROM_datawrite(EEPROM_SL_PULL_OUT,sl_pullout_ADC,2);
   pull_out_val=3050;
   EEPROM_datawrite(EEPROM_SD_PULL_OUT,pull_out_val,2);
//  G_baseval=570;
   G_baseval=370;
   EEPROM_datawrite(EEPROM_SD_BASE_VAL,G_baseval,2);//12-04


	for(uint8_t loop=1;loop<7;loop++)
	{
		EEPROM_datawrite(EEPROM_SL_LEN_VAL+(brand_select_no*12)+((loop-1)*2),value_l[loop], 2);//12-04
	}
	for(uint8_t loop=1;loop<7;loop++)
	{
		EEPROM_datawrite(EEPROM_SL_COL_VAL+(brand_select_no*12)+((loop-1)*2),value_c[loop], 2);//12-04
	}

	 for(loop_count1=0;loop_count1<20;loop_count1++)
	 {
		    for(loop_count=1;loop_count<=6;loop_count++)
		      {
		   	    EEPROM_datawrite(EEPROM_BRAND1_2ML_SYR_ADC+(loop_count1*12)+((loop_count-1)*2),G_loaded_adc_val[loop_count],2);//12-04
		      }
		   }
		   for(loop_count1=0;loop_count1<20;loop_count1++)
		    {
		       	    for(loop_count=1;loop_count<7;loop_count++)
		       	      {
		              	EEPROM_datawrite(EEPROM_BRAND1_2ML_SYR_ADC+(loop_count1*12)+((loop_count-1)*2),G_minval[loop_count],2);
		       	      }
		     }
		   for(loop_count1=0;loop_count1<20;loop_count1++)
		  	{
		  		       	    for(loop_count=1;loop_count<7;loop_count++)//12-04
		  		       	      {
		  		       	    		EEPROM_datawrite(EEPROM_Occl_strain_20PSI_value+(loop_count1*12)+((loop_count-1)*2), Occl.strain_20PSI_value[loop_count], 2);
		  		       	      }
		     }


}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void EEPROM_value_read(void)
{
	uint16_t loop_count1=0,loop_count=0;
	Font_Select(ARIAL_14);

	G_occl_unit=EEPROM_dataread(EEPROM_G_occl_unit ,1);
		if(G_occl_unit>3)
		{
			G_occl_unit=0;
		}
		G_occl_level=EEPROM_dataread(EEPROM_G_occl_level ,1);
		if(G_occl_level>9)
		{
			G_occl_level=0;
		}
		Occl.PSI_level=(G_occl_level+1)*2;
		Occl.dynamic_ratio=54/Occl.PSI_level;

		BOL.mode=EEPROM_dataread(EEPROM_G_bolus_mode ,1);
		if(BOL.mode>2)
		{
			BOL.mode=0;
		}
		BOL.flowrate=EEPROM_dataread(EEPROM_G_bol_fr ,2);
		if(BOL.flowrate >10000)
		{
			BOL.flowrate =10000;
		}

		for(loop_count=0;loop_count<6;loop_count++)
			{
			BOL.limit[loop_count]=EEPROM_dataread(EEPROM_G_bol_limit+(loop_count*2) ,2);
			if(BOL.limit[loop_count]>maxi_lmt[loop_count] || BOL.limit[loop_count]==0 )
			{
				BOL.limit[loop_count]=maxi_lmt[loop_count];
			}
			}

		G_kvo=EEPROM_dataread(EEPROM_G_kvo,1);

		if(G_kvo>1)
		{
			G_kvo=0;
		}
		G_KVO_fr=EEPROM_dataread(EEPROM_G_KVO_fr,2);
		if(G_KVO_fr>50)//22-03
		{
			G_KVO_fr=1;
		}
		G_bright_level=EEPROM_dataread(EEPROM_G_bright_level,1);
		if(G_bright_level>5)
		{
			G_bright_level=0;
		}
		G_sound_level=EEPROM_dataread(EEPROM_G_sound_level,1);
		if(G_sound_level>5)
		{
			G_sound_level=0;
		}

		G_last_data=EEPROM_dataread(EEPROM_LAST_DATA,1);
		if(G_last_data<1)
		{
			G_last_data=0;
		}

		EEPROM_dataread(EEPROM_STANDBY,1);
		if(G_standby<1)
		{
			G_standby=0;
		}
		G_screen_saver=EEPROM_dataread(EEPROM_SCREEN_SAVER,1);
		if(G_screen_saver<1)
		{
			G_screen_saver=0;
		}

		G_date_mnth=EEPROM_dataread(EEPROM_G_date_mnth,2);

		if(G_date_mnth>3112)
		{
		G_date_mnth=0;
		}
		G_year=EEPROM_dataread(EEPROM_G_year,2);
		if(G_year>9999)
		{
		G_year=2023;
	   }
		G_time=EEPROM_dataread(EEPROM_G_time,2);
		if(G_time>2359)
		{
		G_time=0;
		}
		G_time_format=EEPROM_dataread(EEPROM_G_time_format,1);

		if(G_time_format>1)
		{

		G_time_format=0;
		}
		reset_year=EEPROM_dataread(EEPROM_RESET_YEAR,2);
		if(reset_year>9999)
		{
			reset_year=0;
		}


		//-----------------------For Drugs---------------------------//

	total_drugs =EEPROM_dataread(EEPROM_TOTAL_DRUG,2);
	if(total_drugs >600)
	{
		total_drugs =500;
		EEPROM_datawrite(EEPROM_TOTAL_DRUG,total_drugs,2);
	}
	fav_eeprom_read();
	recent_eeprom_read();//29-03

		//----------------------------------------------------------


	G_lock_time=EEPROM_dataread(EEPROM_G_lock_time,1);

	G_infusion_mode_opt=EEPROM_dataread(EEPROM_G_infusion_modes,1);
	if(G_infusion_mode_opt>4)
	{
		G_infusion_mode_opt=0;
	}

	if(G_infusion_modes)
	{
		G_infusion_modes=G_infusion_mode_opt;
	}

	G_drug_list=EEPROM_dataread(EEPROM_G_drug_list,1);
   if(G_drug_list>1)
   {
	   G_drug_list=1;
   }

	total_brand=EEPROM_dataread(EEPROM_TOTAL_BRAND,1);
	if(total_brand>20)//22-03
	{
		total_brand=9;
		EEPROM_datawrite(EEPROM_TOTAL_BRAND,total_brand,1);
	}
	brand_select_no=EEPROM_dataread(EEPROM_BRAND_NO,1);
	if(brand_select_no>20)
	{
		brand_select_no=0;
	}
	total_events=EEPROM_dataread(EEPROM_TOTAL_EVENTS,2);

	if(total_events>1000)//29-03
	{
		total_events=0;
	}

	//-----------------------For Drugs---------------------------//

     if(total_drugs>500)			//22-03
     {
    	 for(loop_count1=500;loop_count1<total_drugs;loop_count1++)
    	 {
    		 for(loop_count=0;loop_count<15;loop_count++)
    		 {
    			 Drug_Names[loop_count1/4][loop_count1%4][loop_count]=EEPROM_dataread(EEPROM_DRUG_NAMES+((loop_count1-500)*15)+loop_count,1);
    		 }
    	 }
     }

     //	-----------------------------------------------------------------------

   //  EEPROM_usedhrs_read();//ALL USED_HRS READ
    //	------------------------------SL_CALIBRATION----------------------------------------
     	 sl_pullin_ADC=EEPROM_dataread(EEPROM_SL_PULL_IN,2);
     	 if((sl_pullin_ADC > 6000)||(sl_pullin_ADC < 2500))
          {
        	    sl_pullin_ADC=2900;//default value will be updated later
       	      EEPROM_datawrite(EEPROM_SL_PULL_IN,sl_pullin_ADC,2);

          }

     	 sl_pullout_ADC =EEPROM_dataread(EEPROM_SL_PULL_OUT,2);
     	 if((sl_pullout_ADC > 63000)||(sl_pullout_ADC < 58000))
          {
     		 	sl_pullout_ADC=61000;//default value will be updated later
       	    EEPROM_datawrite(EEPROM_SL_PULL_OUT,sl_pullout_ADC,2);
          }

     //	-----------------------------------------------------------------------




   if(total_brand>=8)//22-03
   {
	for(loop_count1=8;loop_count1<=total_brand;loop_count1++)//22-03
	 {
	  for(loop_count=0;loop_count<15;loop_count++)
		{
		  Brand_Names[loop_count1/4][loop_count1%4][loop_count]=EEPROM_dataread(EEPROM_BRAND_NAMES+((loop_count1-8)*15)+loop_count,1);
		}

	}
   }

//************************************SD_CALIBRATION***********************************//


   EEPROM_SD_READ( );
   SL_EEPROM_read();


   Pressure_EEPROM_read();

			for(loop_count=1;loop_count<=6;loop_count++)
			{
				Occl.strain_20PSI_value[loop_count]= EEPROM_dataread(EEPROM_Occl_strain_20PSI_value+(brand_select_no*12)+((loop_count-1)*2), 2);
			}



		 G_store_val=EEPROM_dataread(EEPROM_LOAD_CELL_VAL,2);

		 if(G_store_val>8388)
			{
			   G_store_val=0;

			}
         Occl.Zero_PSI=EEPROM_dataread(EEPROM_Occl_strain_0PSI_value,2);

			if( Occl.Zero_PSI>8388)//12-04
			{

				Occl.Zero_PSI=0;
			}

			FACT.factory[0]=EEPROM_dataread(EEPROM_MFG_MON_YEAR,2);

			if(FACT.factory[0]>1234)
			{
				FACT.factory[0]=0;
			}

			FACT.factory[3]=EEPROM_dataread(EEPROM_SERIAL_NO,3);

			if(FACT.factory[3]>99999)
			{
				FACT.factory[3]=0;
			}

			G_qc_track=EEPROM_dataread(EEPROM_QC_TRACK,1);
			if(G_qc_track>26)
			{
				G_qc_track=0;
			}

	}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Complete_Reset()
{
	uint8_t loop_count=0;

	     Complete_reset_frame();
		 Reset_loading(1);

		 /****************************LEVEL1*******************************/

		 /*****************************************************************/
		 Reset_loading(2);
		 /****************************LEVEL2*******************************/

		 /*****************************************************************/
		 Reset_loading(3);
		 /****************************LEVEL3*******************************/

		 /*****************************************************************/
		 Reset_loading(4);
		 /****************************LEVEL4*******************************/

		 /*****************************************************************/
		 Reset_loading(5);
		 /****************************LEVEL5******************************/

		 /*****************************************************************/
		 Reset_loading(6);
		 /****************************LEVEL6*******************************/

		 /*****************************************************************/

//	Occl.Zero_PSI=0;

	for(loop_count=1;loop_count<=6;loop_count++)
	{
		Occl.strain_20PSI_value[loop_count]=0;
	}

	//dynamic_pressure(0,0);
//	TFT_String(xpos,DYN_VAL_POS,0,MAIN_FRAME_COL,YELLOW);

//******************SD_Calibration*************************************//

	for(loop_count=1;loop_count<=6;loop_count++)
	{
		G_loaded_adc_val[loop_count]=0;
	}

	for(loop_count=1;loop_count<=6;loop_count++)
    {
		G_minval[loop_count]=0;
	}
	for(loop_count=1;loop_count<=6;loop_count++)
	{
		G_maxval[loop_count]=0;
	}

	   G_baseval=370;
	   pull_out_val=3050;

//******************SL_Calibration*****************************************//

	   for(uint8_t loop=1;loop<7;loop++)
	   {
		   value_l[loop]=0;
	   }

	   for(uint8_t loop=1;loop<7;loop++)
	   {
		   value_c[loop]=0;
	   }
	   sl_pullin_ADC=2901;
	   sl_pullout_ADC=62001;

//***************************FR and Elapsed Data***************************//
//
//	   MOT.flowrate=00;
//	   dose.flow_rate=00;
//	   MOT.time=00;
//	   MOT.volume=00;
//	   INFUSED.ml=0;
//***************************Menu_Settings*********************************//

		BOL.mode=1;                             //Bolus
		BOL.flowrate=1000;

		G_occl_unit=0;							//Occlusion
		G_occl_level=1;

		G_kvo=0;								//KVO
		G_KVO_fr=1;

		brand_select_no=0;						//Syringe_Brand

		G_infusion_modes=0;                     //Modes


		G_drug_list=1; 							//Drug Archive
		total_fav=0;
		total_recent=0;

		total_events=0;							//Event Log

//		G_date_mnth=0;                          //Date and Month
//		G_year=0;								//Year
//		G_time=0;								//Time
//		G_time_format=0;						//Format

//****************************System_Setting****************************//

		G_bright_level=5;						//Brightness Level
		G_sound_level=5;						//Sound Level
		G_lock_time=0;							//Auto Lock
		G_infusion_mode_opt=4;					//Infusion Modes
		G_drug_list=0;							//Drug List

//****************************Device_Setting****************************//

		UTILIZED.device_hrs_cnt=0;				//Device ON
		UTILIZED.battery_hrs_cnt=0;				//Battery ON
		UTILIZED.running_hrs_cnt=0;				//Battery Run
		UTILIZED.factory_used_hrs=0;			//Factory Test

//**********************************************************************//

		 Reset_loading(7);
}

void akas_logo(void)
{
	uint16_t loop_count=0;
	Rectangle(0,0,320,240,BACKCOLOR9);
	for ( loop_count = 0; loop_count <255; loop_count+=6)
			{
			   AKAS_LOGO(83,55,0,BACKCOLOR9,(uint32_t)((45+(uint16_t)(loop_count*0.82))<<16)+((179+(uint16_t)(loop_count*0.29))<<8)+((86+(uint16_t)(loop_count*0.66))<<0));
			// AKAS_LOGO(83,55,0,GREEN,colorArray[0]+i);
		      delay_ms(5);
			}
}

void loading_Animation(uint16_t x_axis,uint16_t y_axis,uint16_t Length,uint16_t Height)

{
	uint8_t value=0;
    uint8_t array[2]={40,15};
    uint8_t arr_element=0;
    uint8_t char_cnt=0;
    uint8_t YPOS=0;
    uint8_t event_triggered = 1;  // Add a flag to make sure the event happens only once
    uint8_t event_count = 0;

   // uint32_t colors[6] = { GREEN22 ,TEST_COLOR1, TEST_COLOR5 ,TEST_COLOR1,GREEN22  ,BACKCOLOR9};
    uint32_t colors[6] = { TEST_COLOR5 ,TEST_COLOR4, TEST_COLOR1 ,GREEN22, Lionheart ,WHITE};//KEYPAD COLOR
  //uint32_t colors[6] = { TEST_GREEN1,TEST_GREEN2, TEST_GREEN3 ,TEST_GREEN4, TEST_GREEN5,WHITE};// LIGHT GREEN TO DARK GREEN
 // uint32_t colors[6] = { TEST_GREEN8,TEST_GREEN1, TEST_GREEN7 ,TEST_GREEN6, TEST_GREEN5,WHITE};//PALE GREEN TO DARK GREEN

    //uint8_t x_positions[6] = {x_axis, x_axis+(14*1), x_axis+(14*2), x_axis+(14*3), x_axis+(14*4), x_axis+(14*5)};
    int num_rectangles = 6;
    for (int i = 0; i < num_rectangles; i++)
    {
    	Rectangle( x_axis+(14*i), y_axis, Length, Height, colors[i]);
    }

    while(event_triggered)
    {
    //	event_triggered++;
//    	 if(event_triggered>=2)
//    	    {
//    	    event_triggered = 0;
//    	    }
    	  for(int loop=0;loop<=array[arr_element];loop++)
    	    {
			if(!load_flag)
			{
			if(value!=0)
			{
				Rectangle(x_axis+(14*(value-1)),(y_axis+Height+1)+(array[arr_element]-loop),10,1,WHITE);//bottom  //RED
				Rectangle(x_axis+(14*(value-1)),(y_axis-1)-(array[arr_element]-loop),10,1,WHITE);//top
				delay_ms(4);
			}

				Rectangle(x_axis+(14*value),(y_axis+Height+1)+loop,Length,1,colors[value]);//159//bottom
				Rectangle(x_axis+(14*value),(y_axis-1)-loop,Length,1,colors[value]);//89//top
				delay_ms(4);


    		 if(loop==array[arr_element])
			//	if (loop == array[arr_element] && !event_triggered)
    		  {
    		     load_flag=1;
    		    // event_triggered = 1;
              if(!char_cnt)
			  {
            	  Font_Select(ARIAL_9);
            	  YPOS=Letters_Alignments(0,295,(uint8_t*)array_seg[8],0);
            	  TFT_String(YPOS,210,(uint8_t*)array_seg[8],WHITE,WHITE);
			  }

			  else
			  {
				  YPOS=Letters_Alignments(0,295,(uint8_t*)array_seg[char_cnt-1],0);
				  TFT_String(YPOS,210,(uint8_t*)array_seg[char_cnt-1],WHITE,WHITE);
			  }

//    		      delay_ms(8);
				  YPOS=Letters_Alignments(0,295,(uint8_t*)array_seg[char_cnt],0);
				  TFT_String(YPOS,210,(uint8_t*) array_seg[char_cnt],WHITE,INDIGO);
				if(char_cnt<11)
				{
					char_cnt++;

				}
				else
				{
					char_cnt=0;
				}
    		  }
    	      }
    	     else
    	    {

				Rectangle(x_axis+(14*value),(y_axis+Height+1)+(array[arr_element]-loop),Length,1,WHITE);//bottom  //RED
				Rectangle(x_axis+(14*value),(y_axis-1)-(array[arr_element]-loop),Length,1,WHITE);//top
				delay_ms(4);

				Rectangle(x_axis+(14*(value+1)),(y_axis+Height+1)+loop,Length,1,colors[value+1]);//159//bottom
				Rectangle(x_axis+(14*(value+1)),(y_axis-1)-loop,Length,1,colors[value+1]);//89//top
				delay_ms(4);

		        if(loop>=array[arr_element])
			   {
			      load_flag=0;
			      value=value+2;
			      if(!char_cnt)
			      {
					  YPOS=Letters_Alignments(0,295,(uint8_t*)array_seg[8],0);
					  TFT_String(YPOS,210,(uint8_t*)array_seg[8],WHITE,WHITE);
			      }
			      else
			      {
					  YPOS=Letters_Alignments(0,295,(uint8_t*)array_seg[char_cnt-1],0);
					  TFT_String(YPOS,210,(uint8_t*)array_seg[char_cnt-1],WHITE,WHITE);
			      }
//					  delay_ms(8);
					  YPOS=Letters_Alignments(0,295,(uint8_t*)array_seg[char_cnt],0);
					  TFT_String(YPOS,210,(uint8_t*)array_seg[char_cnt],WHITE,INDIGO);
			      if(char_cnt<11)
			      {
			    	  char_cnt++;
			      }
			      else
			      {
			    	  char_cnt=0;

			      }
			   }
    	            if(value==6)
    	            {
    	            	value=0;
    	            	event_count++;
    	            	 if (event_count > 1)
    	            	 {
    	            	   event_triggered = 0;

    	                 }
//    	            	if (arr_element == 7)
//    	            	{
//    	            	event_triggered = 0;
//    	               }
//    	            	else
//    	            	{
//    	            		event_triggered = 1;
//
//    	    	          }
    	    }
    	    }




//    if(event_triggered>=2)
//    {
//    event_triggered = 0;
//    }
    }
}
}
