/*
 * Rec_frame.c
 *
 *  Created on: Feb 16, 2024
 *      Author: Welcome
 */
#include "stdio.h"
#include "stdint.h"
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

extern char recent_drug_list[5][4][20];
extern uint16_t drug_num[11];
extern  char Drug_Names[150][4][20];//29-03

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void recent_drug_Frame(void)
{
	Frame_layout(1,3,(uint8_t*)"Recently used",(uint8_t*)"OKAY",(uint8_t*)"to confirm");

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void recent_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{

	uint8_t loop_count=0;
    uint32_t back_colour = 0;
   	uint32_t text_colour = 0;
   	uint32_t rec_star_colour=0;//27-03

	for(loop_count=0;loop_count<=end_opt;loop_count++)
	{
		if(loop_count == start_opt)//0==0
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			rec_star_colour=MAIN_FRAME_COL;
			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			rec_star_colour=GOLD;
		}


		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
				(uint8_t*)Drug_Names[drug_num[((page*4)+loop_count)]/4][drug_num[((page*4)+loop_count)]%4],back_colour,text_colour);//29-03

		if(dose.drug_select_no==drug_num[((page*4)+loop_count)])	//27-03

		  {
			  TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+(loop_count)*VER_TEXT_SPACE_1+8,250,0,back_colour,rec_star_colour);
		  }
		separation_lines(1,3);
	}

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void recent_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page,  char array[5][4][20])
{

	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
	uint32_t rec_star_colour=0;//27-03


	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour = MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			rec_star_colour=GOLD;
			opt_no		= prev_option;
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			rec_star_colour=MAIN_FRAME_COL;
			opt_no= curr_option;
		}

		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);
		Font_Select(ARIAL_BOLD_14);
			
		TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
								(uint8_t*)Drug_Names[drug_num[((page*4)+opt_no)]/4][drug_num[((page*4)+opt_no)]%4],back_colour,text_colour);//29-03
		if(dose.drug_select_no == drug_num[((page*4)+opt_no)])	//27-03
		{
			TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+(opt_no)*VER_TEXT_SPACE+6,250,0,back_colour,rec_star_colour);
		}
		separation_lines(1,3);
	}
}
