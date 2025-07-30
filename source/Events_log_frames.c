
/*
 * Events_log_frames.c
 *
 *  Created on: 09-Mar-2024
 *      Author: SoftwareRND
 */

#include "LPC55S16.h"
#include "Color.h"
#include "Keypad.h"
#include "TFT_String.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "Power_off.h"
#include "Driver_Timer.h"
#include "Events_log.h"
#include <string.h>
#include<FR_CALC.h>
#include <infused_calculate.h>
#include <Drug_List.h>
extern uint8_t G_ending_event;
extern uint8_t G_Motor_flag;
extern uint8_t G_Alarm_flag;
extern uint16_t array_month[50];
extern  uint16_t   array_mode_no[50];
extern  uint16_t array_drug_name[50];
volatile uint16_t month_number=0;
extern uint8_t G_infusion_modes;
extern char local_buffer[10];
char *event_alarm1[35]={
		 	 	 	 	 	 	 	 	 	 	 	    "No Alarm",//0
														"Device ON",//1
														"Device OFF",//2
														"Motor run",//3
														"Motor stop",//4
														"Occlusion",//5
														"Motor Error",//6
														"Plunger Distu.",//7
														"Syr load Err",//8
														"End of syringe",//9
														"Plunger not Eng"//10
														"Syringe not Eng"//11

//	                                                    "No Alarm      ",//0
//														"yes Alarm     ",//1
//														"Syr Distrbed  ",//2
//														"Tar Vol.Rech  ",//3
//														"Tar.TimeRech  ",//4
//														"Batt Low      ",//5
//														"Batt critical ",//6
//														"Syr End !     ",//7
//														"PCA Prb Unplg ",//8
//														"Occlusion !   ",//9
//														"Plung not eng ",//10
//                                                        "Inf.start     ",//11
//														"Inf.stop      ",//12
//														"Syr DisEng    ",//13
//														"FR Exceeds!   ",//14
//														"FR Exceeds!   ",//15
//														"FR is changed ",//16
//														"Occl Error!   ",//17
//														"FSR may Fail  ",//18
//														"FR is changed ",//19
//														"Motor reverse ",//20
//														"Motor undrive ",//21
//														"Machine Start ",//22
//														"Machine Stop  ",//23
//														"Syr distrub   ",//24
//														"calib error   ",//25
//														"charge IC fail",//26
//														"load cell fail",//27
//														"Syr.Load Err! ",//28
//														"Occl.lvlchang ",//29
//														"Occl.lvlchang ",//30
//														"Occl.lvlchang ",//31
//														"Occl.lvlchang " //32

													};

const char* Event_menu[1][6]={{"Event","F/R","Infml","Drug","Pres."}};
// Array to store month names
const char *month_names[13] = {
 " ","Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
const char *mode_names[] = {
		"Flowrate","VTBI","Dosage","Ramp"
};
extern char Drug_Names[150][4][20];


void events_option_scroll(uint8_t prev_option,uint8_t curr_option,uint8_t page,uint16_t array[20],
		uint16_t array1[20],uint16_t array2[20]);
void event_info_option_frame(uint8_t page,uint8_t start_opt,uint8_t end_opt);
void events_split_screen(void);
void events_option_frame1(uint8_t page,uint8_t start_opt,uint8_t end_opt);
void events_info_scroll(uint8_t prev_option,uint8_t curr_option,uint8_t page,uint16_t array[20],
		uint16_t array1[20],  char* array2[35]);
void Event_update_1_option_frame(uint16_t page);
void events_option_frame1(uint8_t page,uint8_t start_opt,uint8_t end_opt);

extern uint16_t event_filter_day[20];
extern uint16_t event_filter_month[20];
extern uint16_t event_filter_year[20];
extern uint16_t event_info[10][10];
extern uint16_t array_hour[50];
extern uint16_t array_minute[50];
extern uint16_t array_flowrate_no[50];
extern uint16_t array_infusedml_no[50];
extern uint16_t array_pressure_no[50];
extern uint8_t starting_event;
extern uint16_t array_event_no[50];
extern uint16_t array_date[50];

extern uint32_t back_colour ;
extern uint32_t text_colour ;
extern uint8_t E_main_screen;
extern uint8_t E_main_screen1;
extern uint8_t G_month_event;
extern uint8_t G_month_event1;

void events_split_screen(void)
{
	Frame_layout(1,3,0,(uint8_t*)"Event log",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}
void split_screen_info(uint16_t count)
{
	uint8_t menu_count = 0;
	char event_buffer[20];
	   	Rectangle(0,0,320,240,MAIN_FRAME_COL);

		//Rectangle(0,0,320,34,NEW_HDR_COL);


	   	separation_lines(1,3);
	//	TFT_Number(170,10,event_filter_day[count],NEW_HDR_COL,HDR_TXT_COL);
	    sprintf((char *)event_buffer,"Events  (%02d-%02d-%02d)",
		event_filter_day[count],
		event_filter_month[count],event_filter_year[count]);
	    Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)event_buffer);	// header
//		Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"Press        to Exit",(uint8_t*)"BK",2);

}



void Event_update_1(uint16_t count)
{
//	uint8_t menu_count = 0;
//	char event_buffer[20];
//	   	Rectangle(0,0,320,240,MAIN_FRAME_COL);
   	Rectangle(0,0,320,240,MAIN_FRAME_COL);
//		Font_Select(ARIAL_20);
//		Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"EVENTS");	// header
//		Frame_Footer(ORANGE_DARK,WHITE,(uint8_t*)"");
//	    Rectangle(11,47,298,145,MAIN_FRAME_COL); //center green
//		Rectangle(12,53,297,1,LIGHT_GRAY);
//		Rectangle(308,53,1,131,LIGHT_GRAY);
//		Rectangle(12,185,297,1,LIGHT_GRAY);
//		Rectangle(11,54,1,131,LIGHT_GRAY);
	TFT_ST7789V__rectangle_curve(10,10,220,300,50,LIGHT_GRAY);
	Rectangle(12,30,296,26,MAIN_FRAME_COL);

//	Font_Select(ARIAL_14);
//	Rectangle(17,28,165,26,COMB_RED);
//	TFT_String(20,31,(uint8_t* )"06-DEC   17:54",COMB_RED,LIGHT_GRAY);
//	TFT_String(20,63,(uint8_t* )"Run mode",LIGHT_GRAY,COMB_RED);
//	TFT_String(20,95,(uint8_t* )"Alarm",LIGHT_GRAY,COMB_RED);
//	TFT_String(20,127,(uint8_t* )"Flow rate",LIGHT_GRAY,COMB_RED);
//	TFT_String(20,159,(uint8_t* )"Infused ml",LIGHT_GRAY,COMB_RED);
//	TFT_String(20,191,(uint8_t* )"Pressure",LIGHT_GRAY,COMB_RED);
//
//	TFT_String(145,63,(uint8_t* )":  5ml/h",LIGHT_GRAY,COMB_RED);
//	TFT_String(145,95,(uint8_t* )":  40ml",LIGHT_GRAY,COMB_RED);
//	TFT_String(145,127,(uint8_t* )": 5psi",LIGHT_GRAY,COMB_RED);
//	TFT_String(145,159,(uint8_t* )": 50%",LIGHT_GRAY,COMB_RED);
//	TFT_String(145,191,(uint8_t* )": 50%",LIGHT_GRAY,COMB_RED);
}

/**************************************************************************
 *Function Name :void Main_Menu_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
 *Arguments     :uint8_t page,uint8_t start_opt,uint8_t end_opt
 *Return        :void
 *Description	:In this function goto the next parameter in current position
***************************************************************************/

void events_option_frame1(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{

    uint8_t loop_count=0;
    uint32_t back_colour=0;
    uint32_t text_colour=0;
    char event_buffer[20];
	for(loop_count=0;loop_count<=end_opt;loop_count++)
	{
		if(loop_count == start_opt)
		{
			back_colour	= LIGHT_GRAY;
			text_colour = BLACK;
			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = WHITE;
		}

		Font_Select(ARIAL_BOLD_14);
		sprintf((char *)event_buffer,"%02d",event_filter_day[(page*4)+loop_count]);
		TFT_String(21,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,(uint8_t*)event_buffer,back_colour,text_colour);
		TFT_String(42,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7
				       				,(uint8_t*)"-",back_colour,text_colour);
        sprintf((char *)event_buffer,"%02d",event_filter_month[(page*4)+loop_count]);

		TFT_String(55,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,(uint8_t*)event_buffer,back_colour,text_colour);
		TFT_String(85,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7
						       				,(uint8_t*)"-",back_colour,text_colour);
//        TFT_Number(9,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),event_filter_day[(page*4)+loop_count],back_colour,text_colour);
//        TFT_String(40,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)
//        				,(uint8_t*)"-",back_colour,text_colour);
//        TFT_Number(55,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),event_filter_month[(page*4)+loop_count],back_colour,text_colour);
//        TFT_String(80,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)
//               				,(uint8_t*)"-",back_colour,text_colour);
        TFT_Number(100,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,event_filter_year[(page*4)+loop_count],back_colour,text_colour);

	//	TFT_Number(9,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),array_day[page][loop_count],back_colour,text_colour);
//		TFT_String(130,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)
//				,(uint8_t*)":",back_colour,text_colour);
//		TFT_String(150,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),(uint8_t*)events_info[page][loop_count],back_colour,text_colour);



	}

}
void event_info_option_frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{
	 	uint8_t loop_count=0;
	    uint32_t back_colour=0;
	    uint32_t text_colour=0;
	   // char local_buffer[6];
		for(loop_count=0;loop_count<=end_opt;loop_count++)
		{
			if(loop_count == start_opt)
			{
				back_colour	= LIGHT_GRAY;
				text_colour = BLACK;
				Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
			}
			else
			{
				back_colour	= MAIN_FRAME_COL;
				text_colour = WHITE;
			}
			Font_Select(ARIAL_BOLD_14);
//			TFT_Number(9,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),array_hour[starting_event+loop_count],back_colour,text_colour);
//			TFT_String(40,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)
//						,(uint8_t*)":",back_colour,text_colour);
//			TFT_Number(55,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),array_minute[starting_event+loop_count],back_colour,text_colour);

		//	sprintf(local_buffer,"%04d",	((array_hour[starting_event+loop_count]*100)+array_minute[starting_event+loop_count]));    //store the integer variables in to string using sprintf
//			TFT_Time_disp(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
	//		((array_hour[(G_ending_event+starting_event)-((page*4)+loop_count)]*100)+array_minute[starting_event+((page*4)+loop_count)]),back_colour,text_colour);    //25-03  for Time display
//


			TFT_Time_disp(15,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,
						((array_hour[(G_ending_event+starting_event)-((page*4)+loop_count)]*100)+array_minute[(G_ending_event+starting_event)-((page*4)+loop_count)]),back_colour,text_colour);    //25-03  for Time display

			TFT_String(105,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+7,
				    	   							(uint8_t*)event_alarm1[array_event_no[(G_ending_event+starting_event)-((page*4)+loop_count)]],back_colour,text_colour);

		}
}
void events_option_scroll(uint8_t prev_option,uint8_t curr_option,uint8_t page,uint16_t array[20],
		uint16_t array1[20],uint16_t array2[20])
 {
	uint32_t back_colour = 0;
    uint32_t text_colour = 0;
	uint8_t loop		 = 0;
	uint8_t opt_no		 = 0;
    char event_buffer[20];


	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour     = MAIN_FRAME_COL;
			text_colour 	= WHITE;
			opt_no			= prev_option;
		}
		else
		{
			back_colour   = LIGHT_GRAY;
			text_colour   = BLACK;
			opt_no		  = curr_option;
    	}


		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);

	    Font_Select(ARIAL_BOLD_14);
		sprintf((char *)event_buffer,"%02d",event_filter_day[(page*4)+opt_no]);
	    		TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7,(uint8_t*)event_buffer,back_colour,text_colour);
	    		TFT_String(42,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7
	    				       				,(uint8_t*)"-",back_colour,text_colour);
        sprintf((char *)event_buffer,"%02d",event_filter_month[(page*4)+opt_no]);

	    		TFT_String(55,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7,(uint8_t*)event_buffer,back_colour,text_colour);
	    		TFT_String(85,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7
	    						       				,(uint8_t*)"-",back_colour,text_colour);
	    //        TFT_Number(9,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),event_filter_day[(page*4)+loop_count],back_colour,text_colour);
	    //        TFT_String(40,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)
	    //        				,(uint8_t*)"-",back_colour,text_colour);
	    //        TFT_Number(55,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),event_filter_month[(page*4)+loop_count],back_colour,text_colour);
	    //        TFT_String(80,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)
	    //               				,(uint8_t*)"-",back_colour,text_colour);
	            TFT_Number(100,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+7,event_filter_year[(page*4)+opt_no],back_colour,text_colour);

//	    Font_Select(ARIAL_14);
//
////	    TFT_Number(9,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE),
////	    		array[page],back_colour,text_colour);
//	    TFT_Number(9,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE),event_filter_day[(page*4)+opt_no],back_colour,text_colour);
//	          TFT_String(40,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)
//	          				,(uint8_t*)"-",back_colour,text_colour);
//	          TFT_Number(55,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE),event_filter_month[(page*4)+opt_no],back_colour,text_colour);
//	          TFT_String(80,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)
//	                 				,(uint8_t*)"-",back_colour,text_colour);
//	          TFT_Number(100,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE),event_filter_year[(page*4)+opt_no],back_colour,text_colour);

//	        Font_Select(ARIAL_14);

 }
}

void events_info_scroll(uint8_t prev_option,uint8_t curr_option,uint8_t page,uint16_t array[20],
		uint16_t array1[20], char* array2[35])
 {
	uint32_t back_colour = 0;
    uint32_t text_colour = 0;
	uint8_t loop		 = 0;
	uint8_t opt_no		 = 0;
//	char local_buffer[6];


	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour     = MAIN_FRAME_COL;
			text_colour 	= WHITE;
			opt_no			= prev_option;
		}
		else
		{
			back_colour   = LIGHT_GRAY;
			text_colour   = BLACK;
			opt_no		  = curr_option;
    	}


		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);
	    Font_Select(ARIAL_BOLD_14);
//	    TFT_Number(9,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE),array_hour[starting_event+opt_no],back_colour,text_colour);
//	    TFT_String(40,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)
//	    						,(uint8_t*)":",back_colour,text_colour);
//
//
//
//	    TFT_Number(55,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE),array_minute[starting_event+opt_no],back_colour,text_colour);

	  //  sprintf(local_buffer,"%04d",	((array_hour[starting_event+((page*4)+opt_no)]*100)+array_minute[starting_event+((page*4)+opt_no)]));    //store the integer variables in to string using sprintf
		TFT_Time_disp(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
			((array_hour[(G_ending_event+starting_event)-((page*4)+opt_no)]*100)+array_minute[(G_ending_event+starting_event)-((page*4)+opt_no)]),back_colour,text_colour);    //25-03  for Time display





	   /* TFT_String(130,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)
	    									,(uint8_t*)":",back_colour,text_colour);*/
	    TFT_String(105,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
	    	   							(uint8_t*)event_alarm1[array_event_no[(G_ending_event+starting_event)-((page*4)+opt_no)]],back_colour,text_colour);


 }
}
void Event_update_1_option_frame(uint16_t page)
{

    uint8_t loop_count=0;
    char event_buffer[20];
    uint8_t xpos=0;
	back_colour	= LIGHT_GRAY;
	text_colour = BLACK;


	Font_Select(ARIAL_BOLD_14);

      month_number=array_month[page];

	TFT_String(60,33,(uint8_t *)month_names[month_number],MAIN_FRAME_COL,back_colour);
	TFT_String(110,30,(uint8_t*)"-",MAIN_FRAME_COL,back_colour);


	sprintf((char *)event_buffer,"%02d",array_date[page]);
    		TFT_String(20,33,(uint8_t*)event_buffer,MAIN_FRAME_COL,back_colour);
    	TFT_String(50,33,(uint8_t*)"-",MAIN_FRAME_COL,back_colour);


TFT_Time_disp(110,33,((array_hour[page]*100)+array_minute[page]),MAIN_FRAME_COL,back_colour);



	TFT_String(190,33,(uint8_t*)mode_names[array_mode_no[page]],MAIN_FRAME_COL,back_colour);


	for(loop_count=0;loop_count<=4;loop_count++)
	{
	TFT_String(20, EVENT_POS +(loop_count*EVENT_POS_LOOP),(uint8_t*)Event_menu[0][loop_count], back_colour, text_colour);
	 TFT_String(90,EVENT_POS+(loop_count*EVENT_POS_LOOP),(uint8_t*)":",back_colour,text_colour);

	if(loop_count==0)
{

    TFT_String(100, EVENT_POS +2+(loop_count*EVENT_POS_LOOP),
   (uint8_t*)event_alarm1[array_event_no[page]],back_colour,text_colour);


}
if(loop_count==1)
{

	sprintf(local_buffer,"%.1f",((float)array_flowrate_no[page]/10.0));


     xpos=Right_Alignments((uint8_t*)local_buffer,0);

	TFT_writenumber_float_1d(100, EVENT_POS +2+(loop_count*EVENT_POS_LOOP),
	array_flowrate_no[page],back_colour,text_colour);

    TFT_String(100+xpos+10, EVENT_POS +2+(loop_count*EVENT_POS_LOOP),
   (uint8_t*)"ml",back_colour,text_colour);




}
if(loop_count==2)
{
//	TFT_writenumber_float_1d(100, EVENT_POS +2+(loop_count*EVENT_POS_LOOP),
//	array_infusedml_no[page],back_colour,text_colour);


	sprintf(local_buffer,"%.1f",((float)array_infusedml_no[page]/10.0));

     xpos=Right_Alignments((uint8_t*)local_buffer,0);

	TFT_writenumber_float_1d(100, EVENT_POS +2+(loop_count*EVENT_POS_LOOP),
			array_infusedml_no[page],back_colour,text_colour);

    TFT_String(100+xpos+10, EVENT_POS +2+(loop_count*EVENT_POS_LOOP),
   (uint8_t*)"ml",back_colour,text_colour);

}
if(loop_count==3)
{


	TFT_String(100,EVENT_POS +2+(loop_count*EVENT_POS_LOOP),(uint8_t*)
	Drug_Names[(array_drug_name[page]/ 4)][(array_drug_name[page]% 4)],back_colour,text_colour);
}

if(loop_count==4)
{


	sprintf(local_buffer,"%.1f",((float)array_pressure_no[page]));

     xpos=Right_Alignments((uint8_t*)local_buffer,0);

	TFT_writenumber_float_1d(100, EVENT_POS +2+(loop_count*EVENT_POS_LOOP),
			array_pressure_no[page],back_colour,text_colour);

    TFT_String(100+xpos+10, EVENT_POS +2+(loop_count*EVENT_POS_LOOP),
   (uint8_t*)"mmhg",back_colour,text_colour);
}



}
}




