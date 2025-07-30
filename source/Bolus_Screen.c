/*
 * bolus_screen.c
 *
 *  Created on: Nov 25, 2023
 *      Author: Welcome
 */
#include <STEPPER.h>
#include <infused_calculate.h>
#include "LPC55S16.h"
#include "Color.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include"Keypad.h"
#include"Shapes.h"
#include "Bolus_Header.h"
#include "Driver_Timer.h"
#include "FR_CALC.h"
#include "Flowrate_Screen.h"
#include "Motor.h"
#include "Power_off.h"
#include  "sound.h"
#include "Occlusion.h"
#include "Alarm.h"
#include "Linear_Header.h"




// bolusflag_t BOL;

const char* bol_set[1][2]={{0,"Volume"}};
uint8_t Bol_fr_flag=0;
uint8_t G_Auto_Bolus_flag=0;
uint8_t G_bol_run=0;
uint32_t G_bol_vol=10;
uint8_t auto_bolus=0;
uint8_t Bol_prev_val=0;

extern uint8_t home_flag;

extern uint8_t G_run_count;
extern uint8_t  longpress;
extern uint8_t run_stop_flag;
extern volatile uint8_t normal_run;
extern uint8_t G_sound_level;
extern uint8_t ON_Flag;
extern uint8_t end_flag;
extern uint8_t disc;
extern uint8_t sy_size;
extern uint8_t G_Bolus_target_flag;
extern uint8_t G_Bolus_warning_flag;

extern uint16_t G_bol_fr;
extern uint16_t G_bol_fr;
extern const char* bolus_values[3][2];

extern void plunger_alarm_check();
extern void syringe_disp_status(uint8_t screen);
extern unsigned char detect_syringe_size(unsigned int sybrand);
extern void Running_symbol(uint8_t X_start, uint8_t Y_start,uint32_t col);

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void pop_up_info_frame(uint8_t screen, uint32_t color)//common_bug  //ZOOZ_1
{
	uint16_t XPOS=0;
	if(screen==5)
	{

	}

	TFT_ST7789V__rectangle_curve(20,60,117,280,0,color);//70//80//120
	Font_Select(ARIAL_14);
	if(!screen)
	{
//		XPOS=Letters_Alignments(20,20+280,(uint8_t *)"Purge Bolus",0);
//		TFT_String(XPOS,90,(uint8_t *)"Purge Bolus",color,BLACK);
	//	TFT_String_align(20,300,140,(uint8_t *)"Purge Bolus",0,color,BLACK);

//		XPOS=Letters_Alignments(20,20+280,(uint8_t *)"Completed",0);
//		TFT_String(XPOS,140,(uint8_t *)"Completed",color,BLACK);

	//	TFT_String_align(20,300,140,(uint8_t *)"Completed",0,color,BLACK);
	}

	else if(screen==1)
	{
		Font_Select(ARIAL_BOLD_14);
//		XPOS=Letters_Alignments(0,319,(uint8_t *)"EEPROM Fail",0);
//
//		TFT_String(XPOS,120,(uint8_t *)"EEPROM Fail",color,BLACK);
		//TFT_String(100,140,(uint8_t *)"Shutting Down ... ",WHITE,BLACK);
		TFT_String_align(0,319,120,(uint8_t *)"EEPROM Fail",0,color,BLACK);
		delay_secs(1);

	}

	else if(screen==2)
	{
//		XPOS=Letters_Alignments(15,20+270,(uint8_t *)"Passcode Invalid",0);
//	    TFT_String(XPOS,110,(uint8_t *)"Passcode Invalid",color,BLACK);
	    TFT_String_align(15,290,110,(uint8_t *)"Passcode Invalid",0,color,BLACK);
	}

	else if(screen==3)
	{
//		XPOS=Letters_Alignments(20,20+280,(uint8_t *)"Auto Bolus",0);
//		TFT_String(XPOS,90,(uint8_t *)"Auto Bolus",color,BLACK);
		 TFT_String_align(20,300,90,(uint8_t *)"Auto Bolus",0,color,BLACK);
		XPOS=Letters_Alignments(20,20+280,(uint8_t *)"Completed",0);
		TFT_String(XPOS,140,(uint8_t *)"Completed",color,BLACK);

		//delay_secs(1);
		WARN_SOUND(2,1,2);

	}
	else if(screen==4)
	{
//	 XPOS=Letters_Alignments(20,20+280,(uint8_t *)"Keypad Fail",0);
//	 TFT_String(XPOS,120,(uint8_t *)"Keypad Fail",color,BLACK);
	 TFT_String_align(20,300,120,(uint8_t *)"Keypad Fail",0,color,BLACK);
	 delay_secs(1);
	}

	else if(screen==5)
	{
//		TFT_ST7789V__rectangle_curve(17,57,123,286,0,WHITE);
//		TFT_ST7789V__rectangle_curve(20,60,117,280,0,RED);

//	XPOS=Letters_Alignments(20,20+280,(uint8_t *)"Auto Bolus",0);
//	TFT_String(XPOS,90,(uint8_t *)"Auto Bolus",RED,WHITE);
	TFT_String_align(20,300,90,(uint8_t *)"Auto Bolus",0,color,BLACK);
//	XPOS=Letters_Alignments(20,20+280,(uint8_t *)"Interrupted",0);
//	TFT_String(XPOS,140,(uint8_t *)"Interrupted",RED,WHITE);
	TFT_String_align(20,300,140,(uint8_t *)"Interrupted",0,color,BLACK);

	}
	delay_secs(1);

	}
//}//common_bug

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void bolus_frame()
{
	Font_Select(ARIAL_14);
	TFT_writenumber_float_1d(170,213,BOL.flowrate,NEW_FTR_COL,BLACK);//195//common_bug
	TFT_String(245,212,(uint8_t*)"ml/h",NEW_FTR_COL,BLACK);//260
	TFT_writenumber_float_1d(180,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)-5,G_bol_vol,LIGHT_GRAY,BLACK);
//	Font_Select(ARIAL_14);
	TFT_String(220,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)-5,(uint8_t*)"ml",LIGHT_GRAY,BLACK);//common_bug
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void bolus_fun(void)
{
	uint8_t exit_flag=1;
	uint8_t check_flag=0;
//	uint8_t status=0;
	uint32_t syr_max=0;

	POW.power_status_flag=1;
	BOL.run_mode=0;
	Bol_fr_flag=1;
	while(exit_flag)
	{
		syringe_disp_status(0);
		//plunger_alarm_check();
		sy_size=detect_syringe_size(1);
		syr_max=Bolus_Limit_syringe(sy_size);
		Occlusion_Status();
		if((!home_flag)&&(check_flag))
		{
			Auto_bolus_timedisplay(INFUSED.sec);
		}

		Power_off();

		if(!ON_Flag)
		{
			break;//26-03
		}

		else if(POW.power_status_flag)  //ZOOZ_1
		{

			POW.power_status_flag=0;
//			Font_Select(ARIAL_14);
//			TFT_Number(90,50,syr_max,NAVY,WHITE);
			check_flag=Flow_rate_screen(&G_bol_vol,10,syr_max,1);
			Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Volume");

//			if(BOL.flowrate==0)
//				{
//					//Alarm(MOT_ERR_1);
//					pop_up(BOL_SET);
//					//exit_flag=0;
//					//POW.power_status_flag=1;
//				}
			Bol_fr_flag=0;

			while(KEY.value);//28-03

		}

//		Auto_bolus_timedisplay();


		if(check_flag)
		{
			if(!BOL.run_mode)
			{
					Rectangle(5,38,320,170,MAIN_FRAME_COL);
					bolus(BOL.mode);
					bolus_run_frame();
//					Font_Select(ARIAL_14);
//				    TFT_Number(1000,70,G_Bolus_warning_flag,YELLOW,RED);
					bolus_FR_spliting(BOL.infused_ml,BLACK);

					Auto_bolus_timedisplay();
			}
		}

		else
		{
			exit_flag=0;
			break;
		}



		if(RAMP.up_flag)
		{
			RAMP_UP(_MILLISEC,RAMP_RPM,RAMP.RPM_control);  //ZOOZ_1
		}
		else if(RAMP.down_flag)
		{
			RAMP_DOWN(_MILLISEC,RAMP_RPM);//ZOOZ_1
		}

		if((MOT.flowrate>BOL.flowrate) && (!G_Auto_Bolus_flag) )
		   {
//				Frame_Footer_warning(WARNING_COLOR,BLACK,(uint8_t*)"Less Bolus FR",(uint8_t*)"",0);
				G_Auto_Bolus_flag=1;
		   }

		if(BOL.run_mode)
		{

			bolus_fr_screen( BOL.infused_ml);
		// 	Running_symbol(56,3,MAIN_FRAME_COL);
			ADC_to_PSI_conversion();
			Occlusion_Status();


		     	if(BOL.infused_ml >= G_bol_vol)
				 {
					   MOT.flowrate=BOL.temp_flowrate;
					   BOL.run_mode=0;
					   INFUSED.sec=0;
					   exit_flag=0;
					   Timer_disable();
					  // Rectangle(0,0,320,240,MAIN_FRAME_COL);		//21-03-2024
					  // pop_up_info_frame(3, ORANGE);//common_bug  //ZOOZ_1
					   G_Bolus_target_flag=1;
					   //delay_secs(2);

						   if(normal_run==1)
						   		{
							   bolus_to_normal_run();
								exit_flag=0;
						   		}
					   	      }
							}

		switch(KEY.value)
		{

		case BOLUS_KEY:

//		bolus_complete_screen(5,ORANGE);
//		beep_sound();
	   if(normal_run==1)
		{
		   bolus_to_normal_run();
		   //exit_flag=0;
		}
		else
		{
		  Timer_disable();
		}
		   G_Bolus_target_flag=1;
		   BOL.run_mode=0;
		   INFUSED.sec=0;
		   exit_flag=0;

		while(KEY.value);
		break;

		case START_STOP_KEY:

			pop_up_info_frame(5,ORANGE);
//		   beep_sound();
		   if(normal_run==1)
			{
			   bolus_to_normal_run();
			  //exit_flag=0;
			}
			else
			{
			  Timer_disable();
			}
		   	   G_Bolus_target_flag=1;
			   BOL.run_mode=0;
			   INFUSED.sec=0;

			   exit_flag=0;

		while(KEY.value);
		break;

		}
	}
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

 void bolus_fr_screen(uint16_t flowrate )
 {

	 static uint16_t prev_value=0;

	if(prev_value!=flowrate)
	{
		bolus_FR_spliting(prev_value,0xFFE7C4);
		bolus_FR_spliting(flowrate,BLACK);
		prev_value=flowrate;
	}

 }

 /****************************************************************************
 * Function Name :
 * Arguments     :
 * Return type   :
 * Description   :

 *****************************************************************************/

void bolus_run_frame(void)
{
	FR_Lines(4);
//	Rectangle(0,0,320,240,ORANGE);
//		TFT_ST7789V__rectangle_curve(10,10,220,300,50,WHITE);
//		TFT_ST7789V__rectangle_curve(14,14,212,292,50,ORPIMENT_YELLOW);
		Font_Select(ARIAL_BOLD_14);
		TFT_String(70,15,(uint8_t* )"Automatic Bolus",ORANGE,BLACK);
//		YAMSORANGE
		Font_Select(ARIAL_BOLD_14);
//		TFT_String(218,115+20,(uint8_t* )"ml",ORPIMENT_YELLOW,BLACK);
		TFT_String(205,98,(uint8_t* )"ml",0xFFE7C4,BLACK);
		Font_Select(ARIAL_BOLD_14);
//		TFT_String(40,160+8,(uint8_t* )"Target",ORPIMENT_YELLOW,BLACK);
		TFT_String(33,176,(uint8_t* )"Flowrate",ORPIMENT_YELLOW,BLACK);


//		TFT_writenumber_float_1d(35+17,192+3,G_bol_vol,ORPIMENT_YELLOW,BLACK);
		Font_Select(ARIAL_BOLD_14);


		TFT_writenumber_float_1d(33,208,BOL.flowrate,0xFFE7C4,BLACK);
		Font_Select(ARIAL_12);
//		TFT_String(45+47,195+1,(uint8_t* )"ml",ORPIMENT_YELLOW,BLACK);
		TFT_String(100,210,(uint8_t*)"ml/h",0xFFE7C4,BLACK);
		TFT_String(113,122,(uint8_t* )"of",0xFFE7C4,BLACK);
		TFT_writenumber_float_1d(143,122,G_bol_vol,0xFFE7C4,BLACK);
		TFT_String(173,122,(uint8_t* )"ml",0xFFE7C4,BLACK);



		Font_Select(ARIAL_BOLD_14);
//		TFT_String(215-10,160+8,(uint8_t* )"Time",ORPIMENT_YELLOW,BLACK);
		TFT_String(222,176,(uint8_t* )"Time",ORPIMENT_YELLOW,BLACK);
//		TFT_String(205-4,192+3,(uint8_t* )"15.2 sec",ORANGE,BLACK);
		TFT_ST7789V_Summation_symbol(176,203,0,ORPIMENT_YELLOW,BLACK);

		//syringe_size_display();

		Bolus_Running_symbol(70, 10,70, 240,0xFFE7C4);

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void purge_frame(void)
{
	FR_Lines(4);

			Font_Select(ARIAL_BOLD_14);
			TFT_String(95,15,(uint8_t* )"Purge Bolus",ORANGE,BLACK);

			Font_Select(ARIAL_BOLD_14);

			TFT_String(205,102,(uint8_t* )"ml",0xFFE7C4,BLACK);
			Font_Select(ARIAL_BOLD_14);

			TFT_String(33,176,(uint8_t* )"Flowrate",ORPIMENT_YELLOW,BLACK);

			Font_Select(ARIAL_BOLD_14);


			TFT_writenumber_float_1d(33,208,BOL.flowrate,0xFFE7C4,BLACK);
			Font_Select(ARIAL_12);

			TFT_String(100,210,(uint8_t*)"ml/h",0xFFE7C4,BLACK);

			Font_Select(ARIAL_BOLD_14);

			TFT_String(222,176,(uint8_t* )"Time",ORPIMENT_YELLOW,BLACK);

			TFT_ST7789V_Summation_symbol(176,203,0,ORPIMENT_YELLOW,BLACK);



			Bolus_Running_symbol(70, 10,70, 240,0xFFE7C4);

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void purge_bolus(void)
{

	purge_frame();
	bolus_FR_spliting(BOL.infused_ml,BLACK);
	bolus(BOL.mode);


}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void bolus_layout(void)
{

//	Rectangle(0,0,320,240,MAIN_FRAME_COL);
//		Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Bolus");    // to create header
//
//
//		Rectangle(0,206,320,34,NEW_FTR_COL);
//		TFT_String(65,212,(uint8_t*)"Bolus rate :",NEW_FTR_COL,BLACK);
//
//
//		 TFT_ST7789V__rectangle_curve(10,68,(41*2)+1+20,300,0,LIGHT_GRAY);
//
//		 Rectangle(15,120,285,0, BLACK);
//		 TFT_ST7789V_lcd_drawcircle1(30,56+(1*VER_TEXT_SPACE)-3,16,MAIN_FRAME_COL,MAIN_FRAME_COL);
//		 TFT_ST7789V_lcd_drawcircle1(30,56+(1*VER_TEXT_SPACE)-3,14,LIGHT_GRAY,LIGHT_GRAY);
//		 Font_Select(ARIAL_9);
//		 TFT_String(22,56+6+(1*VER_TEXT_SPACE)-13,(uint8_t*)"OK",LIGHT_GRAY,MAIN_FRAME_COL);
//		 TFT_ST7789V_lcd_drawcircle1(30,56+(2*VER_TEXT_SPACE)+5,17,MAIN_FRAME_COL,MAIN_FRAME_COL);
//		 Font_Select(ARIAL_9);
//		 TFT_String(15,56+(2*VER_TEXT_SPACE),(uint8_t*)"BOL",MAIN_FRAME_COL,WHITE);
//	//   TFT_ST7789V_lcd_drawcircle1(56+(2*VER_TEXT_SPACE)+5,30,13,LIGHT_GRAY,LIGHT_GRAY);
//
//		 Font_Select(ARIAL_14);
//		 TFT_String(55,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)-5,(uint8_t*)"Volume",LIGHT_GRAY,BLACK);
//		 TFT_String(150,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)-5,(uint8_t*)":",LIGHT_GRAY,BLACK);
//
//		 TFT_String(55,INITIAL_TEXT_POS+(2*VER_TEXT_SPACE)+5,(uint8_t*)"Run",LIGHT_GRAY,BLACK);
//	//   TFT_String(INITIAL_TEXT_POS+(1*VER_TEXT_SPACE),150,(uint8_t*)":",LIGHT_GRAY,BLACK);
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void bolus_popup_frame()
{
//		Rectangle(5,39,320,170,MAIN_FRAME_COL);
//		Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Do you want exit?");    // to create header
//
//		Rectangle(0,206,320,34,NEW_FTR_COL);
////		Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"Bolus rate  :",(uint8_t*)"",0);
//
//		TFT_ST7789V__rectangle_curve(10,68,(41*2)+1+20,300,0,LIGHT_GRAY);
//		Rectangle(15,120,285,0, BLACK);
//		TFT_ST7789V_lcd_drawcircle1(30,56+(1*VER_TEXT_SPACE)-3,18,MAIN_FRAME_COL,MAIN_FRAME_COL);
//		TFT_ST7789V_lcd_drawcircle1(30,56+(1*VER_TEXT_SPACE)-3,16,LIGHT_GRAY,LIGHT_GRAY);
//
//		Font_Select(ARIAL_9);
//		TFT_String(22,56+6+(1*VER_TEXT_SPACE)-13,(uint8_t*)"OK",LIGHT_GRAY,MAIN_FRAME_COL);
//		TFT_ST7789V_lcd_drawcircle1(30,56+(2*VER_TEXT_SPACE)+5,18,MAIN_FRAME_COL,MAIN_FRAME_COL);
//		TFT_String(18,56+(2*VER_TEXT_SPACE),(uint8_t*)"BAK",MAIN_FRAME_COL,WHITE);
//
//		Font_Select(ARIAL_14);
//		TFT_String(55,INITIAL_TEXT_POS+(1*VER_TEXT_SPACE)-5,(uint8_t*)"To Stop",LIGHT_GRAY,BLACK);
//		TFT_String(55,INITIAL_TEXT_POS+(2*VER_TEXT_SPACE)+5,(uint8_t*)"To Continue",LIGHT_GRAY,BLACK);
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

uint8_t bolus_popup(void)
{
//	uint8_t check_flag=0;
//	uint8_t exit_flag=1;
//
//	bolus_popup_frame();
//	while(exit_flag)
//	{
//		switch(KEY.value)
//		{
//		case OK_KEY:
//
//		beep_sound();  //ZOOZ_1
//		bolus_mode_stop();
//	//  auto_bolus_stop();
//		check_flag=0;
//		exit_flag=0;
//
//		while(KEY.value);
//		break;
//
//		case BACK_KEY:
//
//			beep_sound();  //ZOOZ_1
//			check_flag=1;
//			exit_flag=0;
//
//			while(KEY.value);
//			break;
//
//		}
//
//	}
//
//	return  check_flag;
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void bolus_FR_spliting(uint16_t flowrate,uint32_t color)
{

		uint16_t xpos=0;
		char BR_buffer[6];
		uint16_t flowrate_view = 0;

		Font_Select(BEBAS_60);
		flowrate_view = flowrate/10;//400
//		if(flowrate_view == 0)
//		{
//		   xpos=150;
//		}

		//else
		{
			sprintf(BR_buffer,"%d",flowrate_view);
			xpos=Flow_rate_Alignments((uint8_t*)BR_buffer);
		}


		xpos=TFT_Number(xpos-50,58,flowrate_view,0xFFE7C4,color);
		 xpos=TFT_String(xpos+3,58,(uint8_t *)".",0xFFE7C4,color);
		TFT_Number(xpos+3,58,flowrate%10,0xFFE7C4,color);

//		if(flowrate_view==1)
//		{
//
//			//Rectangle(xpos-50,85,34,82,RED);
//			Font_Select(BEBAS_60);
//			TFT_Number(xpos-50,85,Bol_prev_val,ORPIMENT_YELLOW,ORPIMENT_YELLOW);
//			TFT_Number(xpos-50,85,flowrate_view,ORPIMENT_YELLOW,BLACK);
//	//		if(bolus_values[0][0])
////		   {
////				TFT_Number(xpos,85,flowrate_view,ORPIMENT_YELLOW,BLACK);
////		   }
//		}
//
//		else
//		{
//			Font_Select(BEBAS_60);
//			TFT_Number(xpos-50,85,flowrate_view,ORPIMENT_YELLOW,BLACK);
//			//Bol_prev_val=flowrate_view;
//		}
//		TFT_String(159,85,(uint8_t *)".",ORPIMENT_YELLOW,BLACK);
//		if( flowrate%10 == 1)
//		{
//
//			Font_Select(BEBAS_60);
//			Rectangle(178,85,34,82,ORPIMENT_YELLOW);
//			TFT_Number(178,85,flowrate%10,ORPIMENT_YELLOW,BLACK);
//		}
//
//		else
//		{
//			TFT_Number(178,85,flowrate%10,ORPIMENT_YELLOW,BLACK);
//		}

//		if(prev_infused_ml!=flowrate)
//				{
//				//flowrate_view = flowrate/10;//400
//
//				sprintf(BR_buffer,"%d",prev_infused_ml/10);
//				xpos=Flow_rate_Alignments((uint8_t*)BR_buffer);
//				TFT_Number(xpos-50,85,prev_infused_ml,ORPIMENT_YELLOW,BLACK);
//				TFT_String(159,85,(uint8_t *)".",ORPIMENT_YELLOW,BLACK);
//				TFT_Number(178,85,prev_infused_ml%10,ORPIMENT_YELLOW,BLACK);
//
//				sprintf(BR_buffer,"%d",flowrate/10);
//				xpos=Flow_rate_Alignments((uint8_t*)BR_buffer);
//				TFT_Number(xpos-50,85,flowrate,ORPIMENT_YELLOW,BLACK);
//				TFT_String(159,85,int8_t *)".",ORPIMENT_YELLOW,BLACK);
//				TFT_Number(178,85,flowrate%10,ORPIMENT_YELLOW,BLACK);
//
//				prev_infused_ml=flowrate;
//				}

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

uint16_t Bolus_Limit_syringe(uint8_t size)
{
	uint16_t syr_max_limit=0;
	if(size==1)
	{
		syr_max_limit=20;
	}
	else if(size==2)
	{
		syr_max_limit=50;
	}
	else if(size==3)
	{
		syr_max_limit=100;
	}
	else if(size==4)
	{
		syr_max_limit=200;
	}
	else if(size==5)
	{
		syr_max_limit=300;
	}
	else if(size==6)
	{
		syr_max_limit=500;
	}
	return syr_max_limit;
}

void Bolus_Running_symbol(uint8_t X_start, uint8_t Y_start,uint8_t X_start_1, uint8_t Y_start_1,uint32_t col)
{
	uint8_t Y_pos[3]={5,25,45};
	//uint32_t col=0x505050;
//	uint32_t colour[4][3]={{LIGHT_GRAY,LIGHT_GRAY,YELLOW},{LIGHT_GRAY,YELLOW,YELLOW},
//			               {YELLOW,YELLOW,YELLOW},{LIGHT_GRAY,LIGHT_GRAY,LIGHT_GRAY}};

	uint32_t colour[4][3]= {
		    {YAMSORANGE, GOMANGO, TANGERINE},
		    {GOMANGO, TANGERINE, YAMSORANGE},
		    {TANGERINE, YAMSORANGE, GOMANGO},
			{YAMSORANGE, GOMANGO, TANGERINE},
		};


	uint8_t pos,pos_1;
	uint8_t X_pos=10,X_pos_1=10;
	static uint8_t state=0;

	 if(!G_run_count)
	 {

		 for(pos=0;pos<=2;pos++)
		 {
//			 TFT_ST7789V_Left_Arrow_22pt(X_pos+X_start,Y_pos[pos]+Y_start,0,col,colour[state][pos]);		//corner
			 TFT_ST7789V_rightarrow_symbol(X_pos+X_start, Y_pos[pos]+Y_start, 0, col, colour[state][pos]);
		 }
		 for(pos_1=0;pos_1<=2;pos_1++)
		 {
//			 TFT_ST7789V_Left_Arrow_22pt(X_pos+X_start,Y_pos[pos]+Y_start,0,col,colour[state][pos]);		//corner
			 TFT_ST7789V_rightarrow_symbol(X_pos_1+X_start_1, Y_pos[pos_1]+Y_start_1, 0, col, colour[state][pos_1]);
		 }
		 if(state<3)
		 {
			 state++;
		 }
		 else
		 {
			 state=0;
		 }

		 G_run_count=2;

	 }
}

void Auto_bolus_timedisplay()
{

	char local_buffer_1[10];
	static uint8_t prev_elapsed_min=1;
	static uint8_t prev_elapsed_sec=1;
    uint16_t xpos=0;
	time=sec_to_time_format(INFUSED.sec);

	if((time.seconds!=prev_elapsed_sec))
	{
		Font_Select(ARIAL_BOLD_14);
		sprintf(local_buffer_1,"%04d",(prev_elapsed_min*100)+prev_elapsed_sec);    //store the integer variables in to string using sprintf
		xpos=Right_Alignments((uint8_t*)local_buffer_1,240);
		TFT_Time_disp(xpos,208,(prev_elapsed_min*100)+prev_elapsed_sec,0xFFE7C4,0xFFE7C4);//28-03  ORPIMENT_YELLOW

		sprintf(local_buffer_1,"%04d",(time.minute*100)+time.seconds);    //store the integer variables in to string using sprintf
		xpos=Right_Alignments((uint8_t*)local_buffer_1,240);
		TFT_Time_disp(xpos,208,(time.minute*100)+time.seconds,0xFFE7C4,BLACK);//28-03

		prev_elapsed_sec=time.seconds;
		prev_elapsed_min=time.minute;

		Font_Select(ARIAL_12);

		TFT_String(255,208,(uint8_t* )"mins",0xFFE7C4,BLACK);
	}
}

