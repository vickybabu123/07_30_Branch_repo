#include "LPC55S16.h"
#include "Color.h"
#include "Alarm.h"
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
#include "Boot_up.h"
#include "battery.h"
#include <infused_calculate.h>
#include <Main_screen.h>
#include <power_off.h>
#include <Drug_List.h>

uint8_t G_mode_switch_flag=0;
uint8_t G_mode_sw_flag=0;
uint8_t previous_data(uint8_t mode);
extern void Frame_layout3(uint8_t start_pos, uint8_t end_pos, uint8_t *header_txt, uint8_t *footer_txt, uint8_t *nav_text, bool use_alternate_separation);

extern char disp_para[15];  //ZOOZ_1
extern char dose_rate_unit[3][13];
extern uint8_t dose_option_no;
extern uint8_t G_drug_unit_no;
extern  char* drug_unit[4][4];
extern uint8_t auto_lock_time;
extern uint8_t G_infusion_modes;
extern uint8_t ON_Flag;	//26-03-2024
extern uint8_t home_flag;
extern uint8_t G_bootup_flag;
extern uint8_t M_nav_flag;
extern uint8_t G_last_infusion;
void read_last_data(uint8_t mode);

extern void layout();

char* mode_list1[1][4]={{"Flowrate","VTBI","Dosage","Trapezia"}};
uint8_t infusion_mode_screen(uint8_t id)
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t exit_flag=1;
	uint8_t edit_flag=0;
	uint8_t check_flag=0;
	uint8_t end_opt_no=3;
	//uint8_t total_option=5; // 22-03
	//auto_lock_time=AUTO_EXIT_TIME;
	//uint8_t time_interval=30;//common_bug //ZOOZ_1

	POW.power_status_flag=1;
	//auto_lock_time=30;//common_bug //ZOOZ_1
		while(KEY.value);
		while(exit_flag)
			{

			Power_off();

			if(!ON_Flag)
				{
					break;//26-03
				}

				else if(POW.power_status_flag)
			   {
					POW.power_status_flag=0;//26-03
					infuse_mode_frame();
					infuse_mode_opt_frame(page_no,option_no,end_opt_no);
					//scroll_bar(page_no,option_no,total_option);  //23-03
					while(KEY.value);//28-03//ZOOZ_1

			   }
//				if(KEY.value)//common_bug//ZOOZ_1
//				{
//					//	while(KEY.value);
//						auto_lock_time=AUTO_EXIT_TIME;
//				}
//				else if(!auto_lock_time)
//				{
//					break;
//				}//common_bug

				switch(KEY.value)
				{
				case DOWN_KEY :
//					beep_sound();
				if(!edit_flag)
				{
					if( (page_no==0 && option_no<3)  )
					{
						option_no++;
					}
					else
					{
						page_no++;
						if(page_no==0 &&option_no<3 )
						{
							option_no=0;
							prev_option=0;
							end_opt_no=0;
						}
						else
						{
							option_no=0;
							page_no=0;
							prev_option=0;
							end_opt_no=3;
						}

						layout();
						infuse_mode_opt_frame(page_no,option_no,end_opt_no);



					}

					if(option_no!=prev_option)
					{

						infuse_mode_curser(prev_option,option_no,page_no);

						prev_option = option_no;
					}
					//scroll_bar(page_no,option_no,5);//23-03

				}
					while(KEY.value);
					break;

					case UP_KEY:
//						beep_sound();
					if(!edit_flag)
					{
						if(  (page_no==0 && option_no>0)  )
						{
							option_no--;
						}
						else
						{
							page_no++;
							if(page_no==0 &&option_no==3 )
							{
								option_no=0;
								prev_option=0;
								end_opt_no=0;
							}

//							if(page_no==1)
//							{
//								option_no=0;
//								//page_no=1;
//								end_opt_no=0;
//								//prev_option=1;
//							}
							else
							{
								prev_option=3;
								option_no=3;
								page_no=0;
								end_opt_no=3;

							}
							layout();
							infuse_mode_opt_frame(page_no,option_no,end_opt_no);

						}

						if(option_no!=prev_option)
						{

						infuse_mode_curser(prev_option,option_no,page_no);

							prev_option = option_no;
						}
						//scroll_bar(page_no,option_no,5);//23-03

					}

					while(KEY.value);
					break;

					case OK_KEY:
//						beep_sound();
						home_flag=3;

						//check_flag = Drug_fun(page_no,option_no);
						if((G_infusion_modes!=option_no) ||(!id))
						{
							G_infusion_modes=option_no;
//							if((G_bootup_flag)&&(check_flag))
							if(id)
							{
								ON_Flag=0;
								check_flag=0;
								G_mode_switch_flag=1;
								G_mode_sw_flag=0;
								//exit_flag=0;
							}
							exit_flag=0;

//							M_nav_flag=option_no;
//							POW.power_status_flag=1;
					//	}
						}
						else
						{
							option_no=0;
							caution_info("Already Selected");
							delay_secs(2);
							POW.power_status_flag=1;
							exit_flag=0;

						}
						//

					//	exit_flag=0;


						while(KEY.value);
						break;

					case BACK_KEY:
//						if(!G_bootup_flag)
					//	{
//							beep_sound();
							check_flag=1;
							exit_flag=0;
//							Alarm(BACK_KEY_FAIL);
							while(KEY.value);
							break;

			}

		}
		infuse_clr();
		return check_flag;

}

void infuse_mode_frame(void)
{
	Frame_layout3(1,3,(uint8_t*)"Infusion modes",(uint8_t*)"OKAY",(uint8_t*)"to confirm",1);
}

void infuse_mode_opt_frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{


    uint8_t loop_count=0;
    uint32_t back_colour = 0;
   	uint32_t text_colour = 0;

	for(loop_count=0;loop_count<=end_opt;loop_count++)
	{
		if(loop_count == start_opt)
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;

			Rectangle(9,48+(38*(start_opt)),300,31,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
		}

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,(uint8_t*)mode_list1[page][loop_count],back_colour,text_colour);

		separation_lines2(1,3);
	}

}



void infuse_mode_curser(uint8_t prev_option, uint8_t curr_option, uint8_t page)
{

	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;

	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour = MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			opt_no		= prev_option;//0
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			opt_no= curr_option;
		}


		Rectangle(9,48+(38*(opt_no)),300,31,back_colour);
		//			Font_Select(ARIAL_14);
			Font_Select(ARIAL_BOLD_14);
			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
			(uint8_t*)mode_list1[page][opt_no],back_colour,text_colour);

			separation_lines2(1,3);

	}
}

void  read_last_data(uint8_t mode)
{

	MOT.flowrate= EEPROM_dataread(EEPROM_PD_FRFLOWRATE,2);//EEPROM_PD_FRFLOWRATE
	INFUSED.ml= EEPROM_dataread(EEPROM_PD_FRINFUSED_ml,2);

	if(mode<2)
	{

		INFUSED.sec= EEPROM_dataread(EEPROM_PD_FRINFUSED_SEC,2);
		if(mode==1)
		{
			MOT.volume= EEPROM_dataread(EEPROM_PD_FRVOLUME,4);
		}
	}

	else if(mode==2)
	{

		dose.drug_select_no=EEPROM_dataread(EEPROM_PD_DRDRUG_SELECT,2);

		dose.conc_value =	EEPROM_dataread(EEPROM_PD_DRCONC_VALUE,4);

		dose.pat_weight = 	EEPROM_dataread(EEPROM_PD_DRPAT_WEIGHT,4);

		//INFUSED.ml =	EEPROM_dataread(EEPROM_PD_DRINFUSED_ml,2);

		dose.drug_dos =	EEPROM_dataread(EEPROM_PD_DRDRUG_DOS,4);

		//dose.flow_rate =	EEPROM_dataread(EEPROM_PD_DRFLOWRATE,4);

		dose.flow_rate=MOT.flowrate;

		for(int8_t lop_cnt=0;lop_cnt<10;lop_cnt++)
		{
			disp_para[lop_cnt] =EEPROM_dataread(EEPROM_PD_DRdisp_para+lop_cnt,1);
		}



		dose_option_no =EEPROM_dataread(EEPROM_PD_DRrate_unit,1);
		G_drug_unit_no=EEPROM_dataread(EEPROM_Drug_unit,1);

		memset(dose_rate_unit, 0, sizeof(dose_rate_unit));


		strcat(dose_rate_unit[0],drug_unit[G_drug_unit_no/4][G_drug_unit_no%4]);
		strcat(dose_rate_unit[0],"/");
		strcat(dose_rate_unit[0],"kg");
		strcat(dose_rate_unit[0],"/");


		strcpy(dose_rate_unit[1],dose_rate_unit[0]);
		strcpy(dose_rate_unit[2],dose_rate_unit[0]);

		strcat(dose_rate_unit[0],"h");
		strcat(dose_rate_unit[1],"min");
		strcat(dose_rate_unit[2],"24h");

	}
}

void  write_current_data(uint8_t mode)
{
	EEPROM_datawrite(EEPROM_LAST_IFMODE,mode,1);

		EEPROM_datawrite(EEPROM_PD_FRFLOWRATE,MOT.flowrate,2);//EEPROM_PD_FRFLOWRATE
		EEPROM_datawrite(EEPROM_PD_FRINFUSED_ml,INFUSED.ml,2);


	if(mode<2)
	{

		EEPROM_datawrite(EEPROM_PD_FRINFUSED_SEC,INFUSED.sec,2);
		if(mode==1)
		{
			EEPROM_datawrite(EEPROM_PD_FRVOLUME,MOT.volume,4);
		}
	}

		else if(mode==2)
		{
			EEPROM_datawrite(EEPROM_PD_DRDRUG_SELECT,dose.drug_select_no,2);
			EEPROM_datawrite(EEPROM_PD_DRCONC_VALUE,(uint16_t)dose.conc_value,4);
			EEPROM_datawrite(EEPROM_PD_DRPAT_WEIGHT,dose.pat_weight,4);
			EEPROM_datawrite(EEPROM_PD_DRDRUG_DOS,(uint16_t)dose.drug_dos,4);

			for(int8_t lop_cnt=0;lop_cnt<10;lop_cnt++)
			{
				EEPROM_datawrite(EEPROM_PD_DRdisp_para+lop_cnt,disp_para[lop_cnt],1);
			}

			EEPROM_datawrite(EEPROM_PD_DRrate_unit,dose_option_no,1);
			EEPROM_datawrite(EEPROM_Drug_unit,G_drug_unit_no,1);
		}
}

uint8_t  previous_data(uint8_t mode)
{
	uint8_t check_flag=0;
	uint8_t prev_loop=1;
//  previous_data_frame();
	auto_lock_time=AUTO_EXIT_TIME;
	POW.power_status_flag=1;


	while(prev_loop)
	{

		Power_off();

		if(!ON_Flag)
		{
			break;//26-03
		}

		else if(POW.power_status_flag)  //ZOOZ_1
		{
			POW.power_status_flag=0;
			previous_data_frame(mode);
			while(KEY.value);//28-03
		}

		if(KEY.value)//common_bug//ZOOZ_1
		{
			auto_lock_time=AUTO_EXIT_TIME;
		}

		 if((!auto_lock_time) && (G_last_infusion))
		{
			G_last_infusion=0;
			prev_loop=0;
		}


	switch(KEY.value)
	{

		case LEFT_KEY:

//		beep_sound();
		check_flag=1;
		G_mode_switch_flag=0;
		prev_loop=0;
		break;

		case RIGHT_KEY:

//		beep_sound();
		check_flag=0;
		prev_loop=0;
		break;

		case BACK_KEY:  //12-06-2024

//		beep_sound();
		prev_loop=0;
		break;
	}
 }
	return check_flag;
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void previous_data_frame(uint8_t mode)
{
//	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	CLRSCR(MAIN_FRAME_COL);
	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Infused data clear");


	CENTER_SCR(MAIN_FRAME_COL); 			//center green
	Rectangle(4,43,311,0,LIGHT_GRAY);       //TOP LINE
	Rectangle(315,44,0,140,LIGHT_GRAY);     //RIGHT LINE
	Rectangle(3,185,311,0,LIGHT_GRAY);		//BOTTOM LINE
	Rectangle(3,44,0,140,LIGHT_GRAY);		//LEFT LINE
	TFT_ST7789V__rectangle_curve(10,53,121,297,0,WHITE);
	if(mode==0)
	{
		Font_Select(ARIAL_BOLD_14);
		TFT_String(60,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)-15+6+8,(uint8_t*)"Continue With Old",WHITE,BLACK);
		TFT_String(60+30-5,INITIAL_TEXT_POS+(2*VER_TEXT_SPACE)-15+6+8,(uint8_t*)"Infusion Data",WHITE,BLACK);
	}
	else if(mode==1)
	{
		Font_Select(ARIAL_BOLD_14);
		TFT_String(60,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)-15+6+8,(uint8_t*)"Do you want clear",WHITE,BLACK);
		TFT_String(60+30-5,INITIAL_TEXT_POS+(2*VER_TEXT_SPACE)-15+6+8,(uint8_t*)"infused data?",WHITE,BLACK);

	}
	R_L_button();


	//TFT_String(55,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)-15,(uint8_t*)"?",WHITE,BLACK);
	//TFT_ST7789V_lcd_drawcircle1(30,223,15,BLACK,BLACK);    //	 Left Circle


}

void R_L_button()
{
	TFT_ST7789V__rectangle_curve(15,192,42,83,0,LIGHT_GRAY_5);     //KEY
	TFT_ST7789V__rectangle_curve(220,192,42,83,0,LIGHT_GRAY_5);
	Rectangle(91,192,138,41,MAIN_FRAME_COL);

	Font_Select(ARIAL_BOLD_14);
	TFT_String(38,204,(uint8_t *)"Yes",LIGHT_GRAY_5,MAIN_FRAME_COL);
	TFT_String(248,204,(uint8_t *)"No",LIGHT_GRAY_5,MAIN_FRAME_COL);


	TFT_ST7789V_left_Right_arrow_14_arial(207,23,0,LIGHT_GRAY_5,MAIN_FRAME_COL);    //	Left Arrow

	TFT_ST7789V_left_Right_arrow_14_arial(207,59+210+18,1,LIGHT_GRAY_5,MAIN_FRAME_COL);    //	Right Arrow
}

