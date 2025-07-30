/*
 * DETECT.c
 *
 *  Created on: 17-Oct-2023
 *      Author: SoftwareRND
 */
#include <FR_CALC.h>
#include <Boot_up.h>
#include "LPC55S16.h"
#include "Color.h"
#include "Driver_SPI.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "TFT_String.h"
#include "clock_config.h"
#include "Keypad.h"
#include "GPIO.h"
#include "Flowrate_Screen.h"
#include "Driver_Timer.h"
#include "fsl_clock.h"
#include "PWM.h"
#include "Main_screen.h"
#include "Occlusion.h"
#include "Main_menu.h"
#include <STEPPER.h>
#include "Driver_ADC.h"
//#include "Bolus.h"
#include "Power_off.h"
#include "Driver_PINT.h"
#include"Driver_I2C.h"
#include "Dosage_mode.h"
#include "Driver_ADC.h"
#include "Driver_Timer.h"
#include "Bolus_Header.h"
#include "SD_Calib.h"
#include "Alarm.h"
#include "SD_Calib.h"

uint8_t G_Loop_alarm=0;

extern uint8_t normal_run;
extern uint8_t brand_select_no;
extern uint8_t ON_Flag;
extern uint8_t g_size_vol;
extern uint16_t press_run_alarm;
extern uint8_t G_Loop_alarm;
extern uint8_t G_syringe_size_flag;
extern uint8_t erase_screen;
extern uint8_t sy_size;
extern uint8_t G_KVO_run_flag;
extern uint8_t sd_time_delay;
//extern uint8_t collar_flag;
extern uint8_t syr_status_chk;

uint16_t G_max_50_ml_val;
uint8_t sy_size=0;

uint8_t s_size=0;
volatile uint8_t G_SD_delay;

uint8_t sd_flag=0;
extern uint16_t temp_adc;
uint8_t G_s_size;
uint16_t G_baseval=0;
uint16_t pull_out_val=0;
extern uint8_t home_flag;

extern uint16_t  TFT_String_align(uint16_t x_start,uint16_t x_end,uint16_t YPOS,uint8_t *popup_message,uint8_t center,uint32_t back_col,uint32_t txt_col);

//uint32_t  G_minval[8]={0,641,892,1152,1457,1943,2332};//old pot adc value

//uint32_t  G_minval[8]={0,605,790,1065,1360,1730,2120};
//uint32_t G_maxval[8]={0,789,1064,1359,1729,2119,0};

//uint32_t  G_minval[8]={0,600,651,991,1241,1741,2201};
//uint32_t G_maxval[8]={0,650,990,1240,1740,2200,2300};

//uint32_t G_minval  [8]={0,585,752,1035,1355,1870,2320};//2100//2250
//uint32_t G_maxval[8]={0,751,1034,1354,1869,2249,2450};

uint32_t G_minval[9]={0,390,650,930,1305,1780,2145,2600,4095};//2100//2250
uint32_t G_maxval[8]={0,649,929,1304,1779,2144,2501};

//uint32_t G_loaded_adc_val[6]={735,1060,1270,1650,2040,2455};
//uint32_t G_loaded_adc_val[9]={0,651,961,1181,1561,1931,2281};
//uint32_t G_loaded_adc_val[9]={0,680,990,1240,1740,2000,2300};

//uint32_t G_loaded_adc_val[9]={0,590,910,1160,1550,2190,5000};//2310

uint32_t G_loaded_adc_val[9]={0,410,720,980,1390,1960,2180};//2310

uint16_t G_diff_min[9]={0,40,150,150,150,150,100};
uint16_t G_diff_max[9]={0,200,410,360,440,720,760};
uint32_t G_default_adc_val[7]={735,1060,1270,1650,2040,2455};
extern uint8_t G_infusion_modes;
//extern unsigned long int keyvalue;
const char* syringe_min_max[2][4]={
										{"  2 ml",	"  5 ml",	"10 ml",	"20 ml"},
										{"30   ml",	"50   ml"}//24-04-2024
									} ;



uint16_t ADC_sdcalib(uint16_t Raw_val, uint16_t Data_diff,uint8_t samples );
uint16_t syringe_noise_filter(void);
void sd_cursor_sel(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4]);
void SD_layout(void);
uint8_t SD_Calibration_option_selection(uint8_t *syringe_size);
void sy_siz(unsigned int Pos);
uint8_t indiv_syr_calib(uint8_t* s_size);
void sd_baseval_check(void);
//unsigned char detect_syringe_size(unsigned int sybrand);
void scroll_bar(uint8_t option_no,uint8_t page_no,uint8_t total_option);                                      //(uint8_t prev_option, uint8_t curr_option, uint8_t page)
uint8_t pullin_check(void);
uint8_t pull_out_check(void);
void calib_all(void);
void calib_all_frame(uint8_t s_size);

void syringe_selection_screen(void);
void frames_syringe_option(uint8_t page,uint8_t starting_count,uint8_t ending_count);
//uint8_t syr_place_screen(uint8_t s_size);
void syringe_adc_val(uint8_t *syringe_size);
void pop_up_screen(void);
void live_adc_calib(void);
void syr_adc_val_min(uint8_t *s_size,uint16_t adc_val);
uint8_t  syr_adc_val_load(uint8_t *s_size,uint16_t adc_val);
void EEPROM_SD_READ(void);
void minval_maxval_display();


void SD_mode(void);


extern char *brand_1[3][4];
extern uint8_t brand;
extern uint8_t G_fr_wrn_flag;
extern uint8_t G_fr_wrn_flag_1;


uint8_t syringe_1;
const char* syringe_settings[2][4]={
										{"  2 ml",	"  5 ml",	"10 ml",	"20 ml"},
										{"30   ml",	"50   ml",	"Calibrate All","Calibration Value"}//24-04-2024
									} ;


const char* syringe[8]={"N/A","2ml","5ml","10ml","20ml","30ml","50ml"};
const uint8_t diff_val[9]={0,40,150,150,150,150,150};

uint8_t s_size;
 uint8_t G_pre_sy_size_1[1];


struct ADC_sdcalib{
	 uint16_t Raw_data[20];
	 uint8_t current_pos;
	 uint16_t Sample_data;
	 uint8_t Err_flag;
	 uint8_t Err_count;
	 uint8_t Fail_count;
	 uint32_t cumm_data;
	 uint8_t count;
}sd_calib;

struct ADC_sampling{
	 uint16_t Raw_data[20];
	 uint8_t  current_pos;
	 uint16_t Sample_data;
	 uint8_t  Err_flag;
	 uint8_t  Err_count;
	 uint8_t  Fail_count;
	 uint32_t cumm_data;
	 uint8_t  count;
	}power_key;
/**************************************************************************
                            Functions Call
***************************************************************************/
uint16_t ADC_sdsample(uint16_t Raw_val, uint16_t Data_diff,uint8_t samples );

/**************************************************************************
                            Functions
***************************************************************************/
void SD_mode(void)
{
	uint8_t exit_flag=1;
	uint8_t status=0;
	uint8_t value=0;

    uint8_t syr_size=1;//=page_no*4+option_no;

	uint8_t (*SD_indiv_syr_fun[4])()={SD_Calibration_option_selection,syr_place_screen,indiv_syr_calib};      //,patient_weight_unit_Screen

	 value=0;
	while(exit_flag)
	{
//		Font_Select(ARIAL_9);
//		TFT_Number(50,60,value,YELLOW,BLACK);


	status=SD_indiv_syr_fun[value](&syr_size);

		if(status==1)
		{
			if(value<2)
			{
			value++;
			}
			else
			{
				value=0;
			}
//			if(value==3)
//			{
//				value=0;
//			}
		}

		else if(status==0)
		{
			if(value)
			{
				value--;
			}

			else
			{
				exit_flag=0;
			}
		}
	}

	exit_flag=0;

}

uint8_t SD_Calibration_option_selection(uint8_t *syringe_size)
{
	uint8_t prev_option = 0;
	//uint8_t screen;
	uint8_t opt_limit=3;
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t total_option=8;
	uint8_t syr_size=*syringe_size;//=page_no*4+option_no;
	//uint8_t state=0;
	uint8_t loop_count=0;
	POW.power_status_flag=1;//27-03

	uint8_t check_flag=0;
	uint8_t s_size_loop=1;

//	syringe_selection_screen();
//	scroll_bar(option_no,page_no,total_option);                                       //(uint8_t prev_option, uint8_t curr_option, uint8_t page)
//	frames_syringe_option(0,0,3);

	while(KEY.value);

	while(s_size_loop)
	{
		Power_off();//27-03


			if(!ON_Flag)
				{
					break;//26-03
				}

				else if(POW.power_status_flag)//27-03

			   {
					POW.power_status_flag=0;//27-03
					syringe_selection_screen();
					scroll_bar(page_no,option_no,total_option);                                       //(uint8_t prev_option, uint8_t curr_option, uint8_t page)
					frames_syringe_option(page_no,option_no,3);
			   }

		switch(KEY.value)
		{

		case DOWN_KEY:


//			if(option_no<3)
//			{

				if((page_no<=1)&&(option_no<3 ))
				{
					option_no++;
				}

//				else
//				{
//					page_no=0;
//					option_no=0;
//				}
		//	}
			else
			{
//				if(page_no==0)
//				{
					page_no++;

					if(page_no==1)
					{
						SD_layout();
						option_no=0;
						opt_limit=3;//2
						syringe_selection_screen();
						frames_syringe_option(page_no,option_no,3);//2
						scroll_bar(option_no,page_no,total_option);
					}

					else if(page_no==2)
					{
						page_no=0;
						SD_layout();
						option_no=0;
						opt_limit=3;//2
						syringe_selection_screen();
						frames_syringe_option(page_no,option_no,3);//2
						scroll_bar(option_no,page_no,total_option);
					}
			}

			if(option_no!=prev_option)
			{
				if(option_no!=0)
				{
					sd_cursor_sel(prev_option,option_no,page_no,syringe_settings);
				//	scroll_bar(option_no,page_no,total_option);                                       //(uint8_t prev_option, uint8_t curr_option, uint8_t page)
				}
				prev_option = option_no;//1
			}
			scroll_bar(page_no,option_no,total_option);                                       //(uint8_t prev_option, uint8_t curr_option, uint8_t page)

		while(KEY.value);
		break;

		case UP_KEY:                                        //UP
			if(option_no)
			{
				option_no--;
			}
			else
			{
				//page_no--;
				if(page_no==1)
				{
					page_no--;
				//	s_size--;
					SD_layout();
					option_no=3;
					opt_limit=3;
					prev_option=3;
					syringe_selection_screen();
					frames_syringe_option(page_no,option_no,3);
				}

				else
				{
					page_no=1;
					option_no=3;
					opt_limit=3;
					prev_option=0;
					syringe_selection_screen();
					frames_syringe_option(page_no,option_no,3);
				}
			}

			if(option_no!=prev_option)
			{
				if(option_no<3)
				{
					sd_cursor_sel(prev_option,option_no,page_no,syringe_settings);
				//	scroll_bar(option_no,page_no,total_option);                                       //(uint8_t prev_option, uint8_t curr_option, uint8_t page)

				}
				prev_option = option_no;
			}
			scroll_bar(page_no,option_no,total_option);                                       //(uint8_t prev_option, uint8_t curr_option, uint8_t page)

		while(KEY.value);
		break;

		case OK_KEY:

		//   state++;
		//   screen=1;
			syr_size=((page_no*4)+(option_no)+1);

		    if((page_no==1)&&(option_no==2))
		    {

		    		calib_all();
					syringe_selection_screen();
					frames_syringe_option(page_no,option_no,opt_limit);
					scroll_bar(page_no,option_no,total_option);                                       //(uint8_t prev_option, uint8_t curr_option, uint8_t page)


		    }
		    else if((page_no==1)&&(option_no==3))
		    {
		    	minval_maxval_display();
		    	syringe_selection_screen();
		    	frames_syringe_option(page_no,option_no,opt_limit);
				scroll_bar(page_no,option_no,total_option);                                       //(uint8_t prev_option, uint8_t curr_option, uint8_t page)


		    }
		    else
		    {

			//	syr_place_screen(syr_size);
			//	check_flag=1;
				s_size_loop=0;

			}

			check_flag=1;

		while(KEY.value);
		break;
		case BACK_KEY:
//			for(int loop_count=0;loop_count<6;loop_count++)
//			  {
////			   EEPROM_datawrite(EEPROM_SD_ADC_LOADED_VAL+(((brand_new_no)*12)+((loop_count)*2)),G_loaded_adc_val[loop_count],2);
//			  }
			check_flag=0;
			s_size_loop=0;
			while(KEY.value);
			break;

		}
		}
	*syringe_size=syr_size;
	return check_flag;
	}


uint16_t ADC_sdsample(uint16_t Raw_val, uint16_t Data_diff,uint8_t samples )
{
	sd_calib.cumm_data=0;
	if(!sd_calib.current_pos)
	{
		for(sd_calib.count=0;sd_calib.count<samples;sd_calib.count++)
		{
			sd_calib.Raw_data[sd_calib.count]=Raw_val;
		}
		sd_calib.current_pos=samples-1;
		sd_calib.Err_flag=0;
		sd_calib.Fail_count=0;
		sd_calib.Sample_data=Raw_val;
	}

	else if((Raw_val>=Data_diff && Raw_val>(sd_calib.Raw_data[sd_calib.current_pos]-Data_diff) &&

	Raw_val<(sd_calib.Raw_data[sd_calib.current_pos]+Data_diff) )
				||	( Raw_val<=Data_diff && Raw_val>0 && Raw_val<(sd_calib.Raw_data[sd_calib.current_pos]+Data_diff) ))

	{

	for(sd_calib.count=0;sd_calib.count<samples-1;sd_calib.count++)
	{
		sd_calib.Raw_data[sd_calib.count]=sd_calib.Raw_data[sd_calib.count+1];
	}

	sd_calib.Raw_data[sd_calib.current_pos]=Raw_val;


	for(sd_calib.count=0;sd_calib.count<samples;sd_calib.count++)
	{
		sd_calib.cumm_data+=sd_calib.Raw_data[sd_calib.count];
	}

	sd_calib.Sample_data=sd_calib.cumm_data/samples;

	}
	else
	{
		sd_calib.current_pos	=	0;
	}
	return sd_calib.Sample_data;

}



/**********************************************
FUNCTION 		:void pullin_out_check(void)
DESCRIPTION     :To get the baseval of the syringe
INPUT 			:void
OUTPUT 			:baseval value
**********************************************/

uint8_t pullin_check(void)
{
	uint8_t baseval_err=0;
	uint8_t check_flag=0;

	uint8_t check=1;
	uint16_t adc_val=0;
	uint8_t s_size_loop=0;
	//static
	//uint8_t dis_flag=0;
	temp_adc=0;
	POW.power_status_flag=1;//27-03

	while(KEY.value);

	while(check)
	{

		Power_off();//27-03


		if(!ON_Flag)
			{
				break;//26-03
			}

			else if(POW.power_status_flag)//27-03

		   {
				POW.power_status_flag=0;//27-03
				pull_in_frame();
				pullin_popup();
				while(KEY.value);

		   }


if(!G_SD_delay)
{
		adc_val=ADC_read(ADC_CH_SD_POT);


		adc_val =(ADC_sdsample(adc_val,1000,10));
		adc_val=(adc_val/10)*10;
		G_SD_delay=20;
}

if(temp_adc!= adc_val)

{
	Font_Select(ARIAL_BOLD_14);
	TFT_Number(180,57,temp_adc,LIGHT_GRAY,LIGHT_GRAY);
	TFT_Number(180,57,adc_val,LIGHT_GRAY,MAIN_FRAME_COL);
	temp_adc=adc_val;

}

		switch(KEY.value)
		{

			case OK_KEY:
			{
				if((adc_val<=BASEVAL_MAX))
				{
				//      baseval=ADC.Result;

					pullin_popup();

					baseval_err=0;
				//	dis_flag=1;
					check_flag=1;
					check=0;
					G_baseval=adc_val;
				//	p_calib();
					EEPROM_datawrite(EEPROM_SD_BASE_VAL,G_baseval,2);
				}
				//	p_calib();

				else  if((adc_val>BASEVAL_MAX))
				{

					baseval_err=1;
				//	dis_flag=0;
					calibrate_popup();
					delay_secs(2);
					pullin_popup();
				}

				while(KEY.value);
				break;
			case BACK_KEY:
				check=0;
				check_flag=0;
						//s_size_loop=0;
				while(KEY.value);
				break;
			}
		}

	}
	return check_flag;
}
uint8_t pull_out_check()
{

	uint8_t full_out=0,full_out_err=0;
	uint8_t check_flag=0;
//	uint8_t dis_flag1=0;
	uint16_t adc_val=0;
//	pull_out_frame();
//	pull_out_popup();
	uint8_t check=1;
	POW.power_status_flag=1;//27-03

	Font_Select(ARIAL_14);
	while(check)
	{
		if(!G_SD_delay)
		{
				adc_val=ADC_read(ADC_CH_SD_POT);


				adc_val =(ADC_sdsample(adc_val,1000,10));
				adc_val=(adc_val/10)*10;
				G_SD_delay=20;
		}

		if(temp_adc!= adc_val)
		{
			Font_Select(ARIAL_BOLD_14);
			TFT_Number(180,57,temp_adc,LIGHT_GRAY,LIGHT_GRAY);
			TFT_Number(180,57,adc_val,LIGHT_GRAY,MAIN_FRAME_COL);
			temp_adc= adc_val;

		}
		Power_off();//27-03


					if(!ON_Flag)
						{
							break;//26-03
						}

						else if(POW.power_status_flag)//27-03

					   {
							POW.power_status_flag=0;//27-03
							pull_out_frame();
							pull_out_popup();
					   }

		switch(KEY.value)
		{

		case OK_KEY:
		{
			if((adc_val<DISPOVAN_SD_FOUT_MIN_DEFAULT_))
			{

				full_out_err=1;
			//	dis_flag1=0;
				calibrate_popup();
				delay_secs(2);
				pull_out_popup();



			}

			else  if((adc_val>=DISPOVAN_SD_FOUT_MIN_DEFAULT_))

			{

				pull_out_popup();
				full_out_err=0;

				pull_out_val=adc_val;
				check=0;
				check_flag=1;

				EEPROM_datawrite(EEPROM_SD_PULL_OUT,pull_out_val,2);
			}
			while(KEY.value);
			break;
		case BACK_KEY:
			check=0;
			check_flag=0;
					//s_size_loop=0;
			while(KEY.value);
			break;
		}
		}
	}
	return check_flag;
}
uint8_t indiv_syr_calib(uint8_t* s_size)
{

	uint16_t adc_val=0;
	uint8_t syr_size=0;

	uint8_t sts_check_flag;
	uint8_t check_flag=0;
	uint8_t ssize_loop=1;
//	syringe_brand=1;
	syr_size=*s_size;


	POW.power_status_flag=1;//27-03


//     while(KEY.value)
//     {
//
//     }
	while(ssize_loop)
	{

		    if(!ON_Flag)
			{
				break;//26-03
			}

		else if(POW.power_status_flag)//27-03
		{
			POW.power_status_flag=0;//27-03
			syringe_adc_val(&syr_size);
		}
		Font_Select(BEBAS_60);
		adc_val=ADC_read(8);
		adc_val =ADC_sdsample(adc_val,1000,10);
		adc_val=(adc_val/10)*10;

		Font_Select(BEBAS_60);
		TFT_Number(110,80,adc_val,MAIN_FRAME_COL,WHITE);

		if(temp_adc!= adc_val)
		{


			TFT_Number(110,80,temp_adc,MAIN_FRAME_COL,MAIN_FRAME_COL);
			TFT_Number(110,80,adc_val,MAIN_FRAME_COL,WHITE);
			delay_ms(100);
			temp_adc= adc_val;

		}
		Power_off();//27-03

		switch(KEY.value)
		{
		 while(KEY.value)
		 {

		 }
			case OK_KEY:
			{
				sts_check_flag=syr_adc_val_load(&syr_size,adc_val);
				if(sts_check_flag==1)
				{
					syr_adc_val_min(&syr_size,adc_val);

					ssize_loop=0;

					check_flag=1;
					pop_up_screen();
					delay_secs(2);
			//		TFT_Number(85,38,brand_select_no,MAIN_FRAME_COL,WHITE);

					EEPROM_datawrite(EEPROM_BRAND1_2ML_SYR_ADC+(brand_select_no*12)+((syr_size-1)*2), G_loaded_adc_val[syr_size], 2);

					EEPROM_datawrite(EEPROM_BRAND1_2ML_SYR_MIN_ADC+(brand_select_no*12)+((syr_size-1)*2),G_minval[syr_size], 2);//12-04
				}
				else
				{
					 Font_Select(ARIAL_BOLD_14);
					 TFT_ST7789V__rectangle_curve(35,65,98,255,0,WHITE);//70//80//120

					 TFT_String(80,90,(uint8_t*)"Calibration fail",WHITE,BLACK);
					 delay_secs(2);
//				//	 Font_Select(ARIAL_14);

					 TFT_ST7789V__rectangle_curve(35,65,98,255,0,MAIN_FRAME_COL);//70//80//120
					 TFT_String(80,90,(uint8_t*)"Calibration fail",MAIN_FRAME_COL,MAIN_FRAME_COL);


					// syr_adc_val_min(&syr_size,adc_val);
				}

			 while(KEY.value);
			 break;
			}
			case BACK_KEY:
			{
				if(sts_check_flag==0)
				{
				ssize_loop=0;
				check_flag=0;
				}
				while(KEY.value);
			    break;

	}


	}
	//	ssize_loop=0;
}

//	 pop_up_screen();

			         	return check_flag;

}


void calib_all(void)
{
	uint8_t s_size=1;// =*syr_size;
	uint8_t loop_count;
 //   syringe_brand=1;
    uint16_t adc_val=0;
//	calib_all_frame(s_size);
	uint8_t s_size_loop=1;
	uint8_t sts_check_flag;

	POW.power_status_flag=1;//27-03

	while(KEY.value);
	while(s_size_loop)
	{
		if(!G_SD_delay)
		{
				adc_val=ADC_read(8);
				Font_Select(BEBAS_60);
				adc_val =(ADC_sdsample(adc_val,1000,10));
				adc_val=(adc_val/10)*10;
				G_SD_delay=20;
		}

			if(temp_adc!= adc_val)
			{

			    Font_Select(BEBAS_60);
				TFT_Number(85+20,38+40-5,temp_adc,MAIN_FRAME_COL,MAIN_FRAME_COL);
				TFT_Number(85+20,38+40-5,adc_val,MAIN_FRAME_COL,WHITE);
				temp_adc=adc_val;

			}
			Power_off();//27-03


						if(!ON_Flag)
							{
								break;//26-03
							}

							else if(POW.power_status_flag)//27-03

						   {
								POW.power_status_flag=0;//27-03
								calib_all_frame(s_size);
						   }

						Font_Select(ARIAL_12);

			TFT_String(100-13,180-10,(uint8_t*)syringe[s_size],MAIN_FRAME_COL,WHITE);//24-04-2024

		switch(KEY.value)
		{
			case OK_KEY:


				while(KEY.value);

				sts_check_flag=syr_adc_val_load(&s_size,adc_val);

						if(sts_check_flag==1)
						{
							syr_adc_val_min(&s_size,adc_val);
							s_size++;

						if(s_size>6)//12-04
						{
							s_size_loop=0;
							 pop_up_screen();
							 delay_secs(2);
						}

//							for(loop_count=1;loop_count<=6;loop_count++)
							{

								EEPROM_datawrite(EEPROM_BRAND1_2ML_SYR_ADC+(brand_select_no*12)+((loop_count-1)*2), G_loaded_adc_val[loop_count],2);

								EEPROM_datawrite(EEPROM_BRAND1_2ML_SYR_MIN_ADC+(brand_select_no*12)+((loop_count-1)*2),G_minval[loop_count],2);

							}

						}

						else
						{
							 Font_Select(ARIAL_BOLD_14);

							 TFT_ST7789V__rectangle_curve(20,55,105,280,0,WHITE);//70//80//120
							 TFT_String(80,96,(uint8_t*)"calibration fail",WHITE,BLACK);
							 delay_secs(2);
						//	 Font_Select(ARIAL_14);

								TFT_ST7789V__rectangle_curve(20,55,105,280,0,MAIN_FRAME_COL);//70//80//120
								TFT_String(80,96,(uint8_t*)"calibration fail",MAIN_FRAME_COL,MAIN_FRAME_COL);
						}
						while(KEY.value);
						break;

			case BACK_KEY:
				if(sts_check_flag==0)
				{
					s_size_loop=0;
				}
				while(KEY.value);
				break;
//




		}
	}
}


uint8_t  syr_adc_val_load(uint8_t *s_size,uint16_t adc_val)
{

	uint8_t syr_range=0;
	uint8_t syringe_flag=2;

	syr_range=*s_size;
//	Font_Select(ARIAL_14);
////    TFT_Number(0,70,syr_range,YELLOW,RED);
////
//    Font_Select(ARIAL_14);
////	TFT_Number(50,200,G_baseval,YELLOW,RED);
//
//	Font_Select(ARIAL_14);
////	TFT_Number(100,200,G_diff_min[syr_range],YELLOW,RED);
//
////	Font_Select(ARIAL_14);
////	TFT_Number(150,200,adc_val,WHITE,RED);
//
//	delay_secs(2);

		if((((adc_val>=(G_loaded_adc_val[syr_range-1])+G_diff_min[syr_range]) &&
							((adc_val<=(G_loaded_adc_val[syr_range-1])+G_diff_max[syr_range]))) &&(syr_range!=1))
			||
			(((adc_val>=(G_baseval)+G_diff_min[syr_range])&&
									((adc_val<=(G_baseval)+G_diff_max[syr_range])) )&&(syr_range==1)))
			{


				G_loaded_adc_val[syr_range]=adc_val;
				syringe_flag=1;

			}
			else
			{
			//	G_loaded_adc_val[syr_range]=adc_val;
				syringe_flag=0;


			}
		*s_size=syr_range;
		return syringe_flag;

}

void syr_adc_val_min(uint8_t *s_size,uint16_t adc_val)
{

	uint8_t syr_range=*s_size;

	//	for(loop_count=0;loop_count<=syr_range;loop_count++)
//	if(syr_range)
//		{
//			G_minval[syr_range]=((G_loaded_adc_val[syr_range] + G_loaded_adc_val[syr_range-1])/2);
//		}
//	else if(!syr_range)
//	{
//		G_minval[syr_range]=((G_loaded_adc_val[syr_range] + G_baseval)/2);
//
//	}
	 if(syr_range==1)
			{
				G_minval[syr_range]=((G_loaded_adc_val[syr_range] + G_baseval)/2);
			}

	 else if((syr_range>1)&&(syr_range<7))
			{
				G_minval[syr_range]=((G_loaded_adc_val[syr_range] + G_loaded_adc_val[syr_range-1])/2);
			}

	//	Font_Select(ARIAL_12);
	// TFT_Number(100,60,G_minval[6] ,MAIN_FRAME_COL,RED);
	// delay_secs(2);

	*s_size=syr_range;
}
void syringe_disp_status(uint8_t screen)
{
	uint8_t xpos=0;
	uint8_t prev_value=0;
	static uint8_t G_pre_sy_size[1];
//	static char syr_pre[1] ; //={"N/A","2ml","5ml","10ml","20ml","30ml","50ml"};
//	char* syr_size[8]={"N/A","2ml","5ml","10ml","20ml","30ml","50ml"};

	sy_size=detect_syringe_size(1);


	if(!sy_size)
	{
//		press_run_alarm=10;
	}

//	if((sy_size!=G_pre_sy_size[0] && syr_status_chk) || (screen || (!sy_size)))
//	{
//		if(((normal_run)||(BOL.run_mode))) //&& ((sy_size!=G_pre_sy_size[0])|| (!sy_size)))
//		{
//		  G_syringe_size_flag=1;
//		  G_KVO_run_flag=0;
//		   Alarm(SYR_DIS);
//		  G_Loop_alarm=1;
//		}
	if((sy_size!=G_pre_sy_size[0]) || screen) //|| (!sy_size))
		{
			if(((normal_run)||(BOL.run_mode))&&((sy_size!=G_pre_sy_size[0])|| (!sy_size)))
			{
			  G_syringe_size_flag=1;
			  G_KVO_run_flag=0;
			  Alarm(SYR_DIS);
			  POW.power_status_flag=1;
			  G_Loop_alarm=1;

			}
			syringe_size_display();
//	Font_Select(ARIAL_12);
//	TFT_String(20,120+5-2,(uint8_t *)syr_size[pre_sy_size],LIGHT_TURQUI,LIGHT_TURQUI);
		//if( (sy_size!=G_pre_sy_size[0]) || (screen) )
		{



//			xpos=Letters_Alignments(10,70,(uint8_t *)syr_size[sy_size],0);

			G_pre_sy_size[0]=sy_size;
			G_pre_sy_size_1[0]=sy_size;
			g_size_vol=sy_size;
		}
	}
	else
	{
		syr_status_chk=1;
	}
}
void syringe_size_display(void)
{
	char* syr_size[8]={"N/A","2ml","5ml","10ml","20ml","30ml","50ml"};

	Font_Select(ARIAL_BOLD_14);
	if(G_infusion_modes!=2)
	{


	if(home_flag)
	{
		Rectangle(11,15,30+20,20,LIGHT_TURQUI);
		TFT_String_align(55,15,15,(uint8_t *)syr_size[sy_size],0,LIGHT_TURQUI,DARK_TEAL_1);
	}
	else
	{
		//TFT_String_align(55,15,15,(uint8_t *)syr_size[sy_size],0,LIGHT_TURQUI,DARK_TEAL_1);
	}

	}

}

uint8_t detect_syringe_size(unsigned int sybrand)
{
//	static uint16_t adc_val=0;
	uint16_t sd_value=0;

	 uint8_t sd_val[8]={0,1,2,3,4,5,6,0};  //REW_2
	 uint16_t return_sd_val[8]={0,1,2,3,4,5,6,0};
	 uint8_t loop_count=0;
	static uint8_t sd_rise = 0, prev_sdrise = 0;   //REW_2
	static uint8_t sd_check=0;
	static uint8_t sd_check_count=0;

//	if(!sd_time_delay)
//	{
//		sd_time_delay=1;
		sd_value=ADC_read(ADC_CH_SD_POT);
//		Font_Select(ARIAL_BOLD_14);
//		TFT_Number(100,100,adc_val,BLACK,RED);

		sd_value = ADC_sdsample(sd_value,1000,10);//REW_2
//		Font_Select(ARIAL_BOLD_14);
//		TFT_Number(180,55,sd_value,BLACK,RED);
//		TFT_Number(180,55,sd_value,BLACK,RED);
//		TFT_Number(180,55,sd_value,BLACK,RED);

		for(loop_count=0;loop_count<=7;loop_count++)
	    {
	    	if((sd_value>G_minval[loop_count]) && (sd_value<=G_minval[loop_count+1]))
	    	{
	    		sd_rise=sd_val[loop_count];
	    	}
	    }

//		Font_Select(ARIAL_BOLD_14);
//		TFT_Number(120,20,G_minval[loop_count],BLACK,RED);

	    if(prev_sdrise != sd_rise)
		{
			if(sd_check_count<200)
			{
				sd_check_count++;
			}

			else
			{
				sd_check_count=0;
				prev_sdrise = sd_rise;
			}
		}

		for(int lp_count=0;lp_count<=7;lp_count++)
		{
			if(sd_rise==sd_val[lp_count])
			{
				return return_sd_val[lp_count];
//				return 6;
			}
		}
}
//	static uint16_t adc_val=0;
//	volatile uint8_t sd_val[8]={0,1,2,3,4,5,6,0};  //REW_2
//	volatile uint16_t return_sd_val[8]={0,1,2,3,4,5,6,0};
//	uint8_t loop_count=0;
//	static uint8_t sd_rise = 0, prev_sdrise = 0;   //REW_2
//	static uint8_t sd_check=0;
//	static uint8_t sd_check_count=0;
//
//	if(!sd_time_delay)
//	{
//		sd_time_delay=1;
//		adc_val=ADC_read(ADC_CH_SD_POT);
//		adc_val = ADC_sdsample(adc_val,1000,5);//REW_2
//	}
//
////		for(loop_count=1;loop_count<=7;loop_count++)
////	    {
////	    	if((adc_val>G_minval[loop_count]) && (adc_val<=G_minval[loop_count+1]))
////	    	{
////	    		sd_rise=sd_val[loop_count];
////	    	}
////	    }
////
////	    if(prev_sdrise != sd_rise)
////		{
////			if(sd_check_count<200)
////			{
////				sd_check_count++;
////			}
////
////			else
////			{
////				sd_check_count=0;
////				prev_sdrise = sd_rise;
////			}
////		}
////
////		for(int lp_count=0;lp_count<=7;lp_count++)
////		{
////			if(sd_rise==sd_val[lp_count])
////			{
////				return return_sd_val[lp_count];
////			}
////		}
////	Font_Select(ARIAL_14);
////    TFT_Number(0,70,adc_val,YELLOW,RED);
//
//	for(loop_count=0;loop_count<=7;loop_count++)
//    {
//    	if((adc_val>=G_minval[loop_count]) && (adc_val<G_minval[loop_count+1]))
//    	{
//    		sd_rise=sd_val[6];
//    		break;
//    	}
//    }
//	if(prev_sdrise != sd_rise)
//	{
//		if(sd_check<2)
//		{
//			sd_check++;
//		}
//
//		else
//		{
//			sd_check=0;
//			prev_sdrise = sd_rise;
//		}
//	}
//		return sd_rise;
//}

//
//uint8_t detect_syringe_size(unsigned int sybrand)
//{
////	static uint16_t adc_val=0;
//	uint16_t sd_value=0;
//
//	 uint8_t sd_val[8]={0,1,2,3,4,5,6,0};  //REW_2
//	 uint16_t return_sd_val[8]={0,1,2,3,4,5,6,0};
//	 uint8_t loop_count=0;
//	static uint8_t sd_rise = 0, prev_sdrise = 0;   //REW_2
//	static uint8_t sd_check=0;
//	static uint8_t sd_check_count=0;
//
////	if(!sd_time_delay)
////	{
////		sd_time_delay=1;
//		sd_value=ADC_read(ADC_CH_SD_POT);
////		Font_Select(ARIAL_BOLD_14);
////		TFT_Number(100,100,adc_val,BLACK,RED);
//
//		sd_value = ADC_sdsample(sd_value,1000,10);//REW_2
////		Font_Select(ARIAL_BOLD_14);
////		TFT_Number(180,55,sd_value,BLACK,RED);
////		TFT_Number(180,55,sd_value,BLACK,RED);
////		TFT_Number(180,55,sd_value,BLACK,RED);
//
//		for(loop_count=1;loop_count<7;loop_count++)
//	    {
//	    	if((sd_value>G_minval[loop_count]) && (sd_value<=G_minval[loop_count+1]))
//	    	{
//	    		sd_rise=sd_val[loop_count];
//	    	}
//	    }
//
////		Font_Select(ARIAL_BOLD_14);
////		TFT_Number(120,20,G_minval[loop_count],BLACK,RED);
//
//	    if(prev_sdrise != sd_rise)
//		{
//			if(sd_check_count<200)
//			{
//				sd_check_count++;
//			}
//
//			else
//			{
//				sd_check_count=0;
//				prev_sdrise = sd_rise;
//			}
//		}
//
//		for(int lp_count=0;lp_count<=7;lp_count++)
//		{
//			if(sd_rise==sd_val[lp_count])
//			{
//				return return_sd_val[lp_count];
////				return 4;
//			}
//		}
//}

void max_val_display()
{
	G_maxval[1]=G_minval[2]-1;
	G_maxval[2]=G_minval[3]-1;
	G_maxval[3]=G_minval[4]-1;
	G_maxval[4]=G_minval[5]-1;
	G_maxval[5]=G_minval[6]-1;
	G_maxval[6]=G_max_50_ml_val-1;

}
uint16_t ADC_power_key_sampling(uint16_t Raw_val, uint16_t Data_diff,uint8_t samples)
{
	power_key.cumm_data=0;

		if(!power_key.current_pos)
		{
			for(power_key.count=0;power_key.count<samples;power_key.count++)
			{
				power_key.Raw_data[power_key.count]=Raw_val;
			}
			power_key.current_pos=samples-1;
			power_key.Err_flag=0;
			power_key.Fail_count=0;
			power_key.Sample_data=Raw_val;
		}

		else if((Raw_val>=Data_diff && Raw_val>(power_key.Raw_data[power_key.current_pos]-Data_diff) &&

		Raw_val<(power_key.Raw_data[power_key.current_pos]+Data_diff) )
					||	( Raw_val<=Data_diff && Raw_val>0 && Raw_val<(power_key.Raw_data[power_key.current_pos]+Data_diff) ))//Linerity checking with differences 100

		{

		for(power_key.count=0;power_key.count<samples-1;power_key.count++)
		{
			power_key.Raw_data[power_key.count]=power_key.Raw_data[power_key.count+1];//FIFO METHOD TO LOAD THE VALUES IN THE SAMPLING
		}

		power_key.Raw_data[power_key.current_pos]=Raw_val;     //ZEROTH POSITION TO LOAD THE SAMPLING VALUES


		for(power_key.count=0;power_key.count<samples;power_key.count++)
		{
			power_key.cumm_data+=power_key.Raw_data[power_key.count];  //AVERAGING THE 10 SAMPLE DATA
		}

		power_key.Sample_data=power_key.cumm_data/samples;    //DIVIDED BY 10 TO GET THE AVERAGE

		}
		else
		{
			power_key.current_pos	=	0;
		}
		return power_key.Sample_data;



}


void SD_Calibration(void)
{
	uint8_t exit_flag=1;
	uint8_t status=0;
    uint8_t value=0;

	uint8_t (*SD_Calibration_entry[4])()={pullin_check,pull_out_check,SD_mode};

	 value=0;
	while(exit_flag)
	{
//		Font_Select(ARIAL_9);
//		TFT_Number(50,60,value,YELLOW,BLACK);


		status=SD_Calibration_entry[value]();



		if(status==1)
		{
			if(value<2)
			{
			value++;
			}
			else
			{
				value=0;
			}
//			if(value==3)
//			{
//				value=0;
//			}
		}

		else if(status==0)
		{
			if(value==0)
			{
				if(!ON_Flag)
				{
					exit_flag=0;
				}
			}

			if(value==1)
			{
				value--;
			}
		   if(value==2)
			{
				exit_flag=0;
			}
		}
	}

	exit_flag=0;
}
void minval_maxval_display()
{

	//    uint8_t prev_option = 0;
//		uint8_t screen;
		uint8_t page_no=0;
		uint8_t option_no=0;
	//	uint8_t loop_count=0;
		POW.power_status_flag=1;//27-03

		uint8_t s_size_loop=1;
		Font_Select(ARIAL_9);

//		Rectangle(0,0,320,240,MAIN_FRAME_COL);

		while(KEY.value);

		while(s_size_loop)
		{
			Power_off();//27-03


				if(!ON_Flag)
					{
						break;//26-03
					}

					else if(POW.power_status_flag)//27-03

				   {
						POW.power_status_flag=0;//27-03
						min_max_val_header_footer();
						minval_adc_option_screen(0,0,3);
				   }

			switch(KEY.value)
			{

			case DOWN_KEY:


					if(page_no==0)
					{
						page_no++;
						option_no=0;
						min_max_val_header_footer();
						minval_adc_option_screen(page_no,option_no,1);//2

					}

			while(KEY.value);
			break;

			case UP_KEY:                                        //UP

					if(page_no==1)
					{
						page_no--;
						option_no=3;
						min_max_val_header_footer();
						minval_adc_option_screen(page_no,option_no,3);
					}

			while(KEY.value);
			break;


			case BACK_KEY:

			//	check_flag=0;
				s_size_loop=0;
				while(KEY.value);
				break;

			}
			}
		}

void EEPROM_SD_READ(void)
{
	uint8_t loop_count=0;
	for(loop_count=1;loop_count<=6;loop_count++)
		{
			G_loaded_adc_val[loop_count]=EEPROM_dataread(EEPROM_BRAND1_2ML_SYR_ADC+(((brand_select_no)*12)+((loop_count-1)*2)),2);
		}


	for(loop_count=1;loop_count<=6;loop_count++)
		  {
				G_minval[loop_count]=EEPROM_dataread(EEPROM_BRAND1_2ML_SYR_MIN_ADC+(brand_select_no*12)+((loop_count-1)*2),2);
		  }
		pull_out_val=EEPROM_dataread(EEPROM_SD_PULL_OUT,2);
		if(pull_out_val>3050)
		{
			pull_out_val=3050;
		}

			G_baseval=EEPROM_dataread(EEPROM_SD_BASE_VAL,2);
			if(G_baseval>370)
			{
				G_baseval=370;//12-04
			}
}
