/*
 * Dose_frame.c
 *
 *  Created on: Feb 16, 2024
 *      Author: Welcome
 */

#include "Color.h"
#include "keypad.h"
#include "Shapes.h"
#include "fsl_clock.h"
#include "Driver_I2C.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "shapes.h"
#include "Drug_List.h"
#include "Main_screen.h"
#include "Driver_Timer.h"
#include "Dosage_mode.h"
#include "Brand.h"

uint8_t drug_no;

uint8_t G_opt_no;
uint8_t G_page_no;

uint16_t G_concen;           //calculated data
uint8_t drug_no;
uint8_t dose_option_no=0;

uint8_t G_dose_exit_flag;

 char disp_para[15];    //ZOOZ_1

char dose_rate_unit[3][13];


extern uint8_t current_option_1;
extern uint8_t drug_new_no;
extern uint16_t waitingtime ;
extern uint8_t current_option;
//extern uint8_t normal_run;
extern const char* dosage_view[4];
extern  char Drug_Names[150][4][20];
extern const char* drug_unit[4][4];
extern uint8_t G_pat_para_opt_no;

/**************************************************************************
 *Function Name :void Main_Menu_Frame(void)
 *Arguments     :void
 *Return        :void
 *Description	:In this function for Main frame
***************************************************************************/

void drug_unit_Frame(void)
{

	Frame_layout(1,3,(uint8_t*)"Dose unit",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}

/**************************************************************************
 *Function Name :void Main_Menu_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void drug_unit_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{



    uint8_t loop_count=0;
    uint32_t back_colour = 0;
    uint32_t text_colour = 0;

	for(loop_count=0;loop_count<=3;loop_count++)
	{
		if(loop_count == start_opt)//0==0
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
		}

     //   if((page<=2)||((page==3)&&(loop_count==0)))


		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
				(uint8_t*)drug_unit[page][loop_count],back_colour,text_colour);

	}
}

/**************************************************************************
 *Function Name :void Menu_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4]
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void drug_unit_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page)//, const char* array[4][4])
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
			opt_no		= prev_option;
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			opt_no= curr_option;
		}


		if ((page<=2) || ((page==3) && (opt_no==0)))
			{
//			    Font_Select(ARIAL_9);
//				TFT_Number(50,60,page,YELLOW,BLACK);
//
//				Font_Select(ARIAL_9);
//				TFT_Number(70,60,opt_no,YELLOW,BLACK);
			Rectangle(9,48+(38*(opt_no)),288,30,back_colour);
		    Font_Select(ARIAL_BOLD_14);
			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
			(uint8_t*)drug_unit[page][opt_no],back_colour,text_colour);
			}

	}
	separation_lines(1,3);
}


/**************************************************************************
 *Function Name :void Main_Menu_Frame(void)
 *Arguments     :void
 *Return        :void
 *Description	:In this function for Main frame
***************************************************************************/

void dose_unit_parameter_Frame(void)
{
	Frame_layout(1,3,(uint8_t*)"Select dose unit",(uint8_t*)"OKAY",(uint8_t*)"to confirm");


}

/**************************************************************************
 *Function Name :void Main_Menu_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void dose_unit_parameter_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{


    uint8_t loop_count=0;
    uint32_t back_colour = 0;
    uint32_t text_colour = 0;

	for(loop_count=0;loop_count<end_opt;loop_count++)  //ZOOZ_1
	{
		if(loop_count == start_opt)//0==0
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
		}

    	Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
				(uint8_t*)dose_rate_unit[loop_count],back_colour,text_colour);

	}

}

/**************************************************************************
 *Function Name :void Menu_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4]
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void dose_unit_cursor(uint8_t prev_option, uint8_t curr_option,  char array[3][13])
{

	uint8_t loop = 0;
	static uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;

	for(loop = 0; loop<2; loop++)
	{

		if(!loop)
		{
			back_colour = MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			opt_no		= prev_option;
		}

		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			opt_no		= curr_option;
		}


		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);

			Font_Select(ARIAL_BOLD_14);
			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
			(uint8_t*)array[opt_no],back_colour,text_colour);
			separation_lines(1,3);


	}
}
/**************************************************************************
 *Function Name :void Main_Menu_Frame(void)
 *Arguments     :void
 *Return        :void
 *Description	:In this function for Main frame
***************************************************************************/

void dosage_overview_parameter_Frame(void)
{

	CLRSCR(MAIN_FRAME_COL);


		Frame_Header(NEW_HDR_COL,HDR_TXT_COL,"Parameter");

		Frame_Footer(NEW_FTR_COL,WHITE,"","");

		CENTER_SCR(MAIN_FRAME_COL); 			//center green
		border_lines();
		separation_lines(1,3);

//	TFT_ST7789V_lcd_shapes(38+163,38,300+15,306,LIGHT_GRAY_3);  	//Scroll bar                                                                                           //SCROLLING BOX OUTSIDE LAYER
//	TFT_ST7789V_lcd_shapes(40+57,40,303+12,306,BLACK);

	//Rectangle(0,206,320,34,NEW_FTR_COL);
	Font_Select(ARIAL_12);
	TFT_String(10+7,213-4,(uint8_t*)"FR",ORANGE_DARK,HDR_TXT_COL);  //ZOOZ_1
	TFT_String(40+7,213-4,(uint8_t*)"=",ORANGE_DARK,HDR_TXT_COL);
	TFT_writenumber_float(55+7,213-4,dose.flow_rate,ORANGE_DARK,HDR_TXT_COL );
//	Font_Select(ARIAL_12);
	TFT_String(120+12+15,213-4,(uint8_t*)"ml/h",ORANGE_DARK,HDR_TXT_COL);


//	Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"        Start Infusion",(uint8_t*)"ST",1);

//	TFT_ST7789V_lcd_drawcircle1(230+7,223-6,15,DARK_TEAL,DARK_TEAL);  //ZOOZ_1
	TFT_ST7789V__rectangle_curve(221,201,33,37,10,LIGHT_GRAY_5);
	Font_Select(ARIAL_9);
	TFT_String(231,218-6,(uint8_t*)"ST",LIGHT_GRAY_5,DARK_TEAL);
	Font_Select(ARIAL_12);
	TFT_String(268-3,213-4,(uint8_t*)"Run",ORANGE_DARK,HDR_TXT_COL );
//	TFT_ST7789V_lcd_drawcircle1(150,223,15,MAIN_FRAME_COL,MAIN_FRAME_COL);
//	Font_Select(ARIAL_9);

//	TFT_ST7789V_lcd_drawcircle1(223,210,14,MAIN_FRAME_COL,MAIN_FRAME_COL);
//	TFT_String(200,210,(uint8_t*)"OK",MAIN_FRAME_COL,WHITE);
}

/**************************************************************************
 *Function Name :void Main_Menu_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void dosage_parameter_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{


	uint8_t loop_count=0;



//	char* arr[20];
	uint16_t value;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
	uint16_t dose_data[4]={dose.drug_name,dose.conc_value,dose.drug_dos,dose.pat_weight};

   // memset(disp_para,0,20);
//	if(G_pat_para_opt_no)
//		{
//		 dose_data[4]={dose.drug_name,dose.conc_value,dose.drug_dos,dose.BSA};
//		}
//		else
//		{
//			dose_data[4]={dose.drug_name,dose.conc_value,dose.drug_dos,dose.pat_weight};
//		}

	for(loop_count=0;loop_count<=3;loop_count++)  //local var
	{
		if(loop_count == start_opt)
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
		}

		value = dose_data[loop_count];

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
		(uint8_t*)dosage_view[loop_count],back_colour,text_colour);

		TFT_String(80,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
		(uint8_t*)":",back_colour,text_colour);

		if(loop_count==0)
		{
			TFT_String(95,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
					(uint8_t*)Drug_Names[(dose.drug_select_no/4)][(dose.drug_select_no%4)],back_colour,text_colour);
		}
		else
		{
			Font_Select(ARIAL_BOLD_14);
			TFT_writenumber_float(95,INITIAL_TEXT_POS+((loop_count)*VER_TEXT_SPACE)+6,
					  			     value,back_colour,text_colour);
		}


		switch(loop_count)
		{
//		case 0:
////					TFT_String(180,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),
////										(uint8_t*)"Abilify",back_colour,text_colour);
//					break;
		Font_Select(ARIAL_BOLD_14);
		case 1:
//			strcat((char *)disp_para,(char *)drug_unit[G_page_no][G_opt_no]);
//			strcat((char *)disp_para,(char *)"/mL");

			TFT_String(176,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
										(uint8_t*)disp_para,back_colour,text_colour);
//			TFT_String(230,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),
//					(uint8_t*)"/mL",back_colour,text_colour);
			break;

		case 3:
			//Font_Select(ARIAL_12);
			if(G_pat_para_opt_no==1)
			{
			TFT_String(176,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
								(uint8_t*)"BSA",back_colour,text_colour);
			}
			else
			{
			TFT_String(176,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
								(uint8_t*)"kg",back_colour,text_colour);
			}
			break;

		case 2:
			//Font_Select(ARIAL_12);
			TFT_String(176,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
								(uint8_t*)dose_rate_unit[dose_option_no],back_colour,text_colour);
			break;




		}



	}

}

/**************************************************************************
 *Function Name :void Menu_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4])
 *Arguments     :uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[2][4]
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void dosage_overview_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, const char* array[4])
{

	    uint8_t loop = 0;
		uint8_t opt_no  = 0;
		uint32_t back_colour = 0;
		uint32_t text_colour = 0;
		//char* arr[20];
//		if(G_pat_para_opt_no)
//		{
		uint16_t dose_data[4]={dose.drug_name,dose.conc_value,dose.drug_dos,dose.pat_weight};
//		}
//		else
//		{
//		uint16_t dose_data[4]={dose.drug_name,dose.conc_value,dose.drug_dos,dose.BSA};
//		}
		//memset(disp_para,0,20);

		for(loop = 0; loop<2; loop++)
		{
			if(!loop)
			{
				back_colour = MAIN_FRAME_COL;
				text_colour = LIGHT_GRAY;
				opt_no		= prev_option;
			}
			else
			{
				back_colour = LIGHT_GRAY;
				text_colour = MAIN_FRAME_COL;
				opt_no= curr_option;
			}
			Rectangle(9,48+(38*(opt_no)),288,30,back_colour);

			Font_Select(ARIAL_BOLD_14);
			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
			(uint8_t*)array[opt_no],back_colour,text_colour);


			TFT_String(80,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
			(uint8_t*)":",back_colour,text_colour);

			if(opt_no==0)
			{
				TFT_String(95,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
									(uint8_t*)Drug_Names[(dose.drug_select_no/4)][(dose.drug_select_no%4)],back_colour,text_colour);
			}
//			else if(opt_no==2)
//			{
//				TFT_String(95,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE),
//													(uint8_t*)dose.pat_weight,back_colour,text_colour);
//			}
			else
			{

				TFT_writenumber_float(95,INITIAL_TEXT_POS+((opt_no)*VER_TEXT_SPACE)+6,
								  			    dose_data[opt_no],back_colour,text_colour);
			}

			switch(opt_no)
				{
//				case 0:
////					TFT_String(180,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE),
////										(uint8_t*)"Abilify",back_colour,text_colour);
//					break;
			Font_Select(ARIAL_BOLD_14);
				case 1:
//					strcat((char *)disp_para,(char *)drug_unit[G_page_no][G_opt_no]);
//					strcat((char *)disp_para,(char *)"/mL");
					Font_Select(ARIAL_BOLD_14);
					TFT_String(176,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
							(uint8_t*)disp_para,back_colour,text_colour);
//					TFT_String(230,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE),
//							(uint8_t*)"/mL",back_colour,text_colour);
					break;

				case 3:
					//Font_Select(ARIAL_12);
					if(G_pat_para_opt_no==1)
					{

					TFT_String(176,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
																	(uint8_t*)"BSA",back_colour,text_colour);
					}
					else
					{
						TFT_String(176,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
																(uint8_t*)"kg",back_colour,text_colour);
					}
					break;

				case 2:
					//Font_Select(ARIAL_12);
					TFT_String(176,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
										(uint8_t*)dose_rate_unit[dose_option_no],back_colour,text_colour);
					break;




				}
			separation_lines(1,3);


       }
}


