#include <stdio.h>
#include "LPC55S16.h"
#include "color.h"
#include "shapes.h"
#include "Driver_SPI.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include"Driver_Timer.h"
#include "Keypad.h"
#include "Dosage_mode.h"
#include "modes.h"
#include "Bolus_Header.h"
#include"Driver_ADC.h"
#include"Drug_List.h"
#include"FR_CALC.h"
#include"Alarm.h"
#include"Boot_up.h"
#include"UART.h"
#include <STEPPER.h>
#include"Brand.h"
#include"Main_screen.h"
#include"infused_calculate.h"
#include"clock_config.h"
#include"Driver_HX711.h"
#include"Power_off.h"
#include "RTC.h"
#include "gcov_support.h"
//#include "gcov-embedded.h"

//#include "libgcov"
//BOLUS BOL;
uint8_t temporary_variable=2;
uint8_t frequent_variable=5;
uint8_t prev_data_flag=0;
uint8_t ON_Flag=0;
uint8_t timer_flag=0;
uint8_t G_bootup_flag=0;
uint16_t power_key_values=0;
uint32_t adc=0;
uint8_t load_flag=0;
extern uint8_t brightness;
extern uint8_t total_brand;
extern uint16_t total_drugs;
extern  char Brand_Names[6][4][15];
extern char Drug_Names[150][4][20];
extern uint8_t keylock;
extern uint8_t key_pressed;
extern volatile uint8_t normal_run;
extern volatile uint8_t waitingtime_1;
extern uint8_t total_fav_Brand;//17.09.24

extern uint8_t d_inf;//17.09.24

extern uint8_t Brand_favourite_func(void);//17.09.24

extern uint8_t G_drug_list;
extern uint8_t G_infusion_mode_opt;
extern uint8_t G_infusion_modes;
extern uint8_t G_mode_sw_flag;
extern uint8_t prev_mode_val;
extern uint8_t G_mode_switch_flag;
extern uint8_t home_flag;
extern uint8_t G_Long_press_back;
extern uint16_t m_count1;
extern void  EX_RTC_date_time_update();
extern void infuse_clr();
extern uint8_t infusion_mode_screen(uint8_t id);
void loading_Animation(uint16_t x_axis,uint16_t y_axis,uint16_t Length,uint16_t Height);
extern uint8_t G_Menu_Brand_flag;//17.09.24
void mp3_tx(uint32_t command, uint32_t para_1, uint32_t para_2);
uint16_t rem_adc=0;
uint16_t ST_DATA=0;

extern uint8_t AKAS_LOGO(uint16_t xpos, uint16_t ypos,
				uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
extern uint32_t G_kvo;
extern uint8_t G_standby;
//extern DateTime getRTCDateTime();
//extern rtc_datetime_t date;	//22-03
//extern DateTime rtcDateTime;

extern void __gcov_flush(void);

int main(void)
{

	char *lnf_sw_txt[4]={"Switch to FR Mode","Switch to VTBI Mode","Switch to Dosage Mode","Switch to Trapezia mode"};

		//	uint8_t xpos=0;
//			uint8_t exit_flag=1;
			uint32_t option_selection_no=0;
			uint16_t loop_count=0;
			uint16_t loop_count_1=0;
			uint16_t demo=0;

//			uint8_t flag=220;
			MOT.flowrate=00;
			MOT.time=0;
			MOT.volume=0;
			uint16_t flag=0;



		//    uint32_t Target_val_disp[3]={100,200,300};
			//uint32_t Target_val_disp[4]={MOT.flowrate,MOT.volume,flag,MOT.time};


			char* custom_data2="(+).Add drug";
			char* add_data3="(+).Add Brand";
			/* Init board hardware. */
			BOL.flowrate=1000;
			BOL.bolus_limit=1000;
			dose.drug_con=10;
			dose.drug_con_ml=10;
			dose.pat_weight=6000;
			dose.pat_height=10000;
			brightness=1;
			MOT.flowrate=0;
		//	 {
		//		 Font_Select(ARIAL_14);
		//		 TFT_Number(0,140,KEY.value,YELLOW,RED);
		//	 }

		/* Init board hardware. */

    BOARD_InitBootPins();
	BOARD_BootClockPLL150M();

//  BOARD_InitBootClocks();
//  BOARD_InitBootPeripherals();

	Boot_up();
	UART7_init(9600,0);
	Rectangle(0,0,320,240,NAVY);
	__gcov_flush();
	Rectangle(0,0,320,240,NAVY);
//return 0;
//}
//	while(1);
////while(1)
////{
////	getRTCDateTime();
//////	demo=RTC_dataread(0x00, 1);
//////	loop_count=RTC_dataread(0x01, 1);
//////	Font_Select(ARIAL_14);
//////	 TFT_Number(100,70,rtcDateTime.second,YELLOW,RED);
//////	Printf("%d\n",loop_count);
////
////}
	while(temporary_variable)		//21-03-2024
	{

				 if(GPIO->PIN[1] & (1<<8))
				 {
					 ON_Flag=1;
				 }

				 else
				 {
					 ON_Flag=0;
				 }
				 temporary_variable--;

	}

				if(total_brand<=19)		//22-03
				 {
					 for(loop_count=0;loop_count<15;loop_count++)//22-03
					 {
						Brand_Names[total_brand/4][total_brand%4][loop_count]=add_data3[loop_count];
					 }
				 }

				 for(loop_count=0;loop_count<15;loop_count++)//22-03
				 {

					Drug_Names[total_drugs/4][total_drugs%4][loop_count]=custom_data2[loop_count];

				 }

			while(1)
			{

			 if(ON_Flag==1)
			 {
				 if(G_mode_sw_flag!=1)
				 {
					 self_test_process();
				 }
				 option_selection_no=0;


			while(ON_Flag)//home_flag<3)
			 {

				 switch(option_selection_no)
				 {
					 case 0:
					if(!prev_data_flag)
					{

						if(d_inf)
						{
							   G_Menu_Brand_flag=1;
							option_selection_no=Brand_List(0);
						}
						else if(total_fav_Brand==1)
						{
						   option_selection_no=1;
						}
						else
						{
						   G_Menu_Brand_flag=0;
						   option_selection_no=Brand_favourite_func();//verify the functionality
						}

						//option_selection_no=Brand_List(0);
					}
					else
					{
						option_selection_no=3;
					}
//					TFT_Number(80,40,G_infusion_mode_opt,RED,YELLOW);
//					 TFT_Number(100,40,G_infusion_modes,RED,YELLOW);
//					 delay_secs(1);
						// Font_Select(ARIAL_14);
						// G_drug_list=1;
						 while(KEY.value);
						 break;

					 case 1:

						 if((G_drug_list) && (!prev_data_flag))
						 {
							 option_selection_no=Drug_List();	//Dosage_screen();
//							 Font_Select(ARIAL_14);
//							 TFT_String(10,140,"100",RED,YELLOW);
//							 TFT_Number(10,40,(uint8_t*)option_selection_no,RED,YELLOW);
						 }
						 else
						 {
							 option_selection_no++;
						 }
						 while(KEY.value);
						 break;

					 case 2:
						 if((G_infusion_mode_opt==4)&&  (G_infusion_modes>3)&&(!prev_data_flag))//	  &&  G_infusion_modes>2 ))//||(1))

						 {

							 option_selection_no=infusion_mode_screen(0);

//							 TFT_Number(40,40,(uint8_t*)option_selection_no,RED,YELLOW);
//							 if(G_Long_press_back==1)
//							 {
//
//								 option_selection_no=3;
//								 G_mode_sw_flag=0;
//								 G_Long_press_back=0;
//
//							 }

							 if(option_selection_no!=1)
							 {
								 option_selection_no=3;
							 }

							 else
							 {
								 if(G_drug_list)
								 {
									 option_selection_no = 1;
								 }

								 else
								 {
									 option_selection_no = 0;
								 }
							 }

						 }

						 else
						 {


							 option_selection_no=3;

						 }
							 while(KEY.value);
						 break;

					 case 3:

						 if(G_mode_sw_flag!=1)
						 {
							 option_selection_no=Home_screen_functionality(G_infusion_modes);	//26-03-2024
						 }

						 else
						 {
							 option_selection_no=1;
						 }

					 	     if(G_mode_switch_flag)
					 	 	 {
					 		    infuse_clr();                           //After switching to one mode to clear the infusion data
								Rectangle(0,0,320,240,MAIN_FRAME_COL);
								Font_Select(ARIAL_BOLD_14);

								Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Switching Mode");
								Frame_Footer_new(NEW_FTR_COL,WHITE,(uint8_t *)"Please wait...");
								CENTER_SCR(MAIN_FRAME_COL);  //center green
								Font_Select(ARIAL_BOLD_14);
//								xpos=Letters_Alignments(0,320, (uint8_t*)lnf_sw_txt[G_infusion_modes],0);
//								TFT_String(xpos,150,(uint8_t *)lnf_sw_txt[G_infusion_modes],MAIN_FRAME_COL,WHITE);

								TFT_String_align(0,320,150,(uint8_t *)lnf_sw_txt[G_infusion_modes],0,MAIN_FRAME_COL,WHITE);

								for(loop_count=0;loop_count<=140;loop_count++)
								{
									Rectangle(20+(loop_count*2),90,0,30,WHITE);
									delay_us(7);
								}

								option_selection_no=2;

								G_mode_sw_flag=0;
								ON_Flag=1;

					 	 	 }

						 	 else if(option_selection_no==2)
						 	 {
						 		G_mode_switch_flag = 1;
						 	 }
						 	 else if(option_selection_no==3)
						 	 {
						 		option_selection_no=2;
						 	 }
					 	     break;
			 }
		 	 frequent_variable = 30;	//21-03-2024


					 }
		 }


		else	//21-03-2024
		 {

			 while(frequent_variable)	//21-03-2024
			 {
				 power_key_values=ADC_read(ADC_CH_POWER_KEY);
				 frequent_variable--;
			 }
			 power_key_values=ADC_read(ADC_CH_POWER_KEY);
		 	 if(power_key_values>700)   //FOR DEBUG TEST
			 {
		 		SUPPLY_TRIGGER_ON;
				ON_Flag=1;
				power_key_values=0;
			 }
		 }

		 }


    return 0 ;
}

void mp3_tx(uint32_t command, uint32_t para_1, uint32_t para_2)
{
	//memset(g_rxBuffer,'\0',100);

	txdata_transfer(USART7, 0x7E);      //start bit

	txdata_transfer(USART7, 0xFF);      //version
	txdata_transfer(USART7, 0x06);      //no. of bytes length
	txdata_transfer(USART7, command);      //command

	txdata_transfer(USART7, 0x00);      //feedback

	txdata_transfer(USART7, para_1);      //parameter 1
	txdata_transfer(USART7, para_2);      //parameter 2

	txdata_transfer(USART7, 0xEF);      //end bit


}
//
