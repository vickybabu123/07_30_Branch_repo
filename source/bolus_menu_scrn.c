#include "LPC55S16.h"
#include "Shapes.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_Commands.h"
#include "Main_Menu.h"
#include "TFT_String.h"
#include "Flowrate_Screen.h"
#include "Brand.h"
#include "Occlusion.h"
#include "Bolus_Header.h"
#include "Power_off.h"
#include"Driver_I2C.h"
#include "Power_off.h"
#include "Driver_Timer.h"
#include  "sound.h"





uint8_t G_bolus_mode=0;
uint8_t Bolus_rate_flag=0;
uint16_t G_bol_fr=1000;
extern uint8_t edit_flag;
extern uint8_t ON_Flag;
extern uint8_t auto_bolus;
extern void Frame_layout3(uint8_t start_pos, uint8_t end_pos, uint8_t *header_txt, uint8_t *footer_txt, uint8_t *nav_text, bool use_alternate_separation);



const char* bolus_menu[4]={"Bolus Mode","Bolus Rate","Bolus limit"};

const char* bolus_values[3][2]={{"Auto","Purge"},
		                        {0},{0}
								};

void Bolus_Screen();
void Bolus_Frame(void);
void Bolus_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt);
void bolus_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page);//, const char* array[4])
void bolus_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag);

extern uint8_t auto_lock_time;    //ZOOZ_1

/**************************************************************************
 *Function Name :void Bolus_Screen()
 *Arguments     :void
 *Return        :void
 *Description	:In this function to enter the Bolus option
***************************************************************************/



void Bolus_Screen()
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t end_op_no=3;
	uint8_t bolus_variable=1;
	uint8_t edit_flag=0;
    uint8_t exit_flag=0;

    uint8_t bolus_mode=0;//common_bug
    uint8_t bolus_fr=0;//common_bug

    bolus_mode=BOL.mode;//common_bug				//ZOOZ_1    bolus_fr=BOL.flowrate;//common_bug
    POW.power_status_flag=1;

	while(KEY.value);			//ZOOZ_1
	while(bolus_variable)
	{

		Power_off();

	if(!ON_Flag)
	{
		BOL.mode=bolus_mode;	//common_bug		//ZOOZ_1
		BOL.flowrate=bolus_fr;	//common_bug
		break;//26-03
	}

					else if(POW.power_status_flag)
				   {
						POW.power_status_flag=0;//26-03
						Bolus_Frame();
						Bolus_Opt_Frame(page_no,option_no,end_op_no);
					//	scroll_bar(page_no,option_no,3);
						if(edit_flag)
							{
								TFT_ST7789V_left_Right_arrow_14_arial(58,162,0,LIGHT_GRAY,BLACK);
								TFT_ST7789V_left_Right_arrow_14_arial(58,253,1,LIGHT_GRAY,BLACK);

								//edit_flag = 1;
							}

							else
							{
								//edit_flag = 0;
								if(option_no==0)
								{
								TFT_ST7789V_left_Right_arrow_14_arial(58,162,0,LIGHT_GRAY,LIGHT_GRAY);
								TFT_ST7789V_left_Right_arrow_14_arial(58,253,1,LIGHT_GRAY,LIGHT_GRAY);
								}
							}

				   }
		if(KEY.value)//common_bug//ZOOZ_1
		{

			auto_lock_time=AUTO_EXIT_TIME;

		}
		else if(!auto_lock_time)
		{
			break;
		}//common_bug


		switch(KEY.value)
		{
			case DOWN_KEY :
//				//beep_sound();
			if(!edit_flag)
			{
				if(option_no<2)
				{
					option_no++;
				}
				else
				{
					option_no=0;
				}

				if(option_no!=prev_option)
				{

					bolus_cursor(prev_option,option_no,page_no);//,bolus_menu);
					prev_option = option_no;
				}

			}
		//	scroll_bar(page_no,option_no,3);
			while(KEY.value);
			break;

			case UP_KEY:
				//beep_sound();
			if(!edit_flag)
			{
				if(option_no)
				{
					option_no--;
				}


				if(option_no!=prev_option)
				{
					bolus_cursor(prev_option,option_no,page_no);//,bolus_menu);
					prev_option = option_no;
				}
			}
		//	scroll_bar(page_no,option_no,3);

			while(KEY.value);
			break;

			case RIGHT_KEY:
				//beep_sound();
			//	if(option_no==0)
//						{
//							if(!edit_flag)
//							{
//								Font_Select(ARIAL_BOLD_14);
//								TFT_String(162,50+10,(uint8_t* )"<",LIGHT_GRAY,BLACK);
//								TFT_String(250-10,50+10,(uint8_t* )">",LIGHT_GRAY,BLACK);
//
////								TFT_ST7789V_left_Right_arrow_14_arial(50+10+3,162,0,LIGHT_GRAY,BLACK);
////								TFT_ST7789V_left_Right_arrow_14_arial(50+10+3,250-10,1,LIGHT_GRAY,BLACK);
//
//								edit_flag = 1;
//							}
//						}
			if(edit_flag)
			{
				bolus_subsetting( option_no,page_no,1);
			}
			while(KEY.value);
			break;

			case LEFT_KEY:
				//beep_sound();
//				if(option_no==0)
//						{
//							if(!edit_flag)
//							{
//
//								edit_flag = 1;
//							}
//
//						}
			if(edit_flag)
			{
				bolus_subsetting( option_no,page_no,0);
			}
			while(KEY.value);
			break;

			case OK_KEY:
				//beep_sound();
			if(option_no==0)
			{
				if(!edit_flag)
				{
					Font_Select(ARIAL_BOLD_14);
					TFT_ST7789V_left_Right_arrow_14_arial(58,162,0,LIGHT_GRAY,BLACK);
					TFT_ST7789V_left_Right_arrow_14_arial(58,253,1,LIGHT_GRAY,BLACK);

					edit_flag = 1;
				}


				else
				{
					Font_Select(ARIAL_BOLD_14);
//					TFT_String(158,50+10,(uint8_t* )"<",LIGHT_GRAY,LIGHT_GRAY);
//					TFT_String(250-6,50+10,(uint8_t* )">",LIGHT_GRAY,LIGHT_GRAY);
					TFT_ST7789V_left_Right_arrow_14_arial(58,162,0,LIGHT_GRAY,LIGHT_GRAY);
					TFT_ST7789V_left_Right_arrow_14_arial(58,253,1,LIGHT_GRAY,LIGHT_GRAY);


//					Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"        to edit",(uint8_t*)"OK",1);//8/07
					edit_flag = 0;

				}
			}

			else if(option_no==1)
			{
				//Bolus_rate_flag=1;
				exit_flag=Flow_rate_screen(&BOL.flowrate,0,3000,2);
				POW.power_status_flag=1;

//				if(!exit_flag)
//				{
//
//				//	bolus_variable=0;
//				//	Rectangle(0,36,320,169,MAIN_FRAME_COL);
//
//
//				}//exit for menu setting
			}

	else if(option_no==2)
		{

			bolus_limit();
			Bolus_Frame();
			Bolus_Opt_Frame(page_no,option_no,end_op_no);

		}
			EEPROM_datawrite(EEPROM_G_bolus_mode,BOL.mode ,1);
			EEPROM_datawrite(EEPROM_G_bol_fr,BOL.flowrate ,2);



			while(KEY.value);
			break;


			case BACK_KEY:

				//beep_sound();

				if(edit_flag)
				{
					BOL.mode=bolus_mode;//common_bug
					BOL.flowrate=bolus_fr;//common_bug
				}
				EEPROM_datawrite(EEPROM_G_bolus_mode,BOL.mode ,1);
				EEPROM_datawrite(EEPROM_G_bol_fr,BOL.flowrate ,2);

			bolus_variable=0;


			while(KEY.value);
			break;

		}
		}

	}


/**************************************************************************
 *Function Name :void Bolus_Frame(void)
 *Arguments     :void
 *Return        :void
 *Description	:In this function for Bolus main frame
***************************************************************************/

//void Bolus_Frame(void)
//{
//	uint8_t bolus_count = 0;
//	Rectangle(3,36,317,169,MAIN_FRAME_COL);
//	Rectangle(0,0,320,34,NEW_HDR_COL);
//
//	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Bolus");	// header
//
//	for(bolus_count = 1; bolus_count <=2 ; bolus_count++)				//no of lines
//	{
//		Rectangle(7,34+(43*bolus_count),290,0, BLACK);		// no of rect line
//	}
//
////	TFT_ST7789V_lcd_shapes(38,38+163,306,300+15,LIGHT_GRAY_3);  	//Scroll bar                                                                                           //SCROLLING BOX OUTSIDE LAYER
////	TFT_ST7789V_lcd_shapes(40,40+57,306,303+12,BLACK);
//
//	Rectangle(0,206,320,34,NEW_FTR_COL);
//	Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"        to edit",(uint8_t*)"OK",1);
////	TFT_ST7789V_lcd_drawcircle1(140,223,15,MAIN_FRAME_COL,MAIN_FRAME_COL);
//
////	Font_Select(ARIAL_9);
////	TFT_String(130,218,(uint8_t*)"OK",MAIN_FRAME_COL,WHITE);
//}

void Bolus_Frame(void)
{
	Frame_layout3(1,3,(uint8_t*)"Bolus",(uint8_t*)"OKAY",(uint8_t*)"to confirm",1);
}

/**************************************************************************
 *Function Name :void Bolus_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/


void Bolus_Opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{
	uint16_t bolus[4]={BOL.mode,BOL.flowrate,BOL.bolus_limit};
		uint16_t value = 0;
		uint32_t back_colour = 0;
		uint32_t text_colour = 0;
	    uint8_t loop_count=0;


		for(loop_count=0;loop_count<=end_opt;loop_count++)
		{
			if(loop_count == start_opt)//0==0
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
			value = bolus[loop_count];


			Font_Select(ARIAL_BOLD_14);
			TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
					(uint8_t*)bolus_menu[loop_count],back_colour,text_colour);

			separation_lines(1,3);




		if(loop_count<2)
			{
	//			Font_Select(ARIAL_12);
	//			TFT_String(150,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),(uint8_t*)":",back_colour,text_colour);
			}

			if(loop_count==0)
				{


				TFT_String(180,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
				(uint8_t*)bolus_values[loop_count][value],back_colour,text_colour);
				}

			else if(loop_count==1)
				{
				TFT_writenumber_float_1d(180,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,
				value,back_colour,text_colour);
				//TFT_String(255,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE),(uint8_t*)"ml/h",back_colour,text_colour);

				TFT_String(245,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,
						(uint8_t*)"ml/h",back_colour,text_colour);

				}

		}

}
/**************************************************************************
 *Function Name :void bolus_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[3]
 *Return        :void
 *Description	:In this function to change the current cursor position
***************************************************************************/



void bolus_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page)//, const char* array[4])
{
	const uint16_t bol[4]={BOL.mode,BOL.flowrate,BOL.bolus_limit};
	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
	uint16_t value=0;

	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour =  MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			opt_no		= prev_option; //1

		}
		else  //!1
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			opt_no= curr_option;
		}


		value = bol[opt_no];

		Rectangle(9,48+(38*(opt_no)),300,31,back_colour);


		//TFT_ST7789V__rectangle_curve(3,35+(43*(opt_no)),41,308,0,back_colour);


		Font_Select(ARIAL_BOLD_14);

		TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
		(uint8_t*)bolus_menu[opt_no],back_colour,text_colour);




		if(opt_no==0)
		{
			TFT_String(180,INITIAL_TEXT_POS+((opt_no)*VER_TEXT_SPACE)+6,
			(uint8_t*)bolus_values[opt_no][bol[opt_no]],back_colour,text_colour);
		}
		else if(opt_no==1)
		{
//			TFT_Number(INITIAL_TEXT_POS+(1*VER_TEXT_SPACE),200,
//			value,back_colour,text_colour);
			TFT_writenumber_float_1d(180,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,
			value,back_colour,text_colour);

			TFT_String(245,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)+6,(uint8_t*)"ml/h",back_colour,text_colour);

			separation_lines(1,3);

		}



	}

}

/**************************************************************************
 *Function Name :void bolus_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag)
 *Arguments     :uint8_t option_no,uint8_t page_no, uint8_t INC_flag
 *Return        :void
 *Description	:In this function array arguments print as right side
***************************************************************************/


void bolus_subsetting(uint8_t option_no,uint8_t page_no, uint8_t INC_flag)
{

	//uint16_t* bolus_data[4] = {&BOL.mode,&BOL.flowrate,&BOL.bolus_limit};
	uint8_t max_value[3] = {1,0,0};
	uint8_t cur_value = 0;
	uint8_t pre_value = 0;

	cur_value	= BOL.mode;//*bolus_data[option_no];
	pre_value	= cur_value;

	if(INC_flag)
	{
		if(cur_value < max_value[option_no])
		{
			cur_value++;
		}
		else
		{
			cur_value=0;
		}
	}
	else
	{
		if(cur_value)
		{
			cur_value--;
		}
		else
		{
			cur_value=1;
		}
	}

	if(pre_value != cur_value)
	{

		Font_Select(ARIAL_BOLD_14);
		TFT_String(180,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)bolus_values[option_no][pre_value],LIGHT_GRAY,LIGHT_GRAY);

		Font_Select(ARIAL_BOLD_14);
		TFT_String(180,INITIAL_TEXT_POS+((option_no)*VER_TEXT_SPACE)+6,
		(uint8_t*)bolus_values[option_no][cur_value], LIGHT_GRAY,MAIN_FRAME_COL);

	}
	BOL.mode = cur_value;

}

