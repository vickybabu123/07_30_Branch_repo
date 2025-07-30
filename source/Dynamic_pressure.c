/****************************************************************************
* File Name    :
* Developed by :
* Version      :
* Description  :

*****************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include "LPC55S16.h"
//#include "Occlusion.h"
#include "Driver_Timer.h"
//#include "Driver_I2C.h"
//#include "Driver_PINT.h"
//#include "Driver_ADC.h"
#include "Driver_SPI.h"
#include "Color.h"
#include "stdint.h"
#include "TFT_String.h"
#include "Flowrate_Screen.h"
#include "keypad.h"
//#include "Main_Menu.h"
//#include "../Menu_Setting/Main_Menu.h"
#include "Dynamic_Pressure.h"
#include "shapes.h"
#include "Main_screen.h"
//#include "Syringe_calib_header.h"


/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/


void dynamic_pressure(uint8_t dy_xpos,uint8_t dy_ypos,uint8_t pressure,uint8_t screen) //curved disp
{
	uint8_t loop_count;
	uint8_t loop_count1=0;
	uint8_t dynamic_data[6][2]={
			{9,18},
			{6,12},
			{4,8},
			{2,4},
			{1,2},
			{0,0}
	};

	uint8_t dynamic_data1[6][2]={
			{9,18},
			{6,12},
			{4,8},
			{2,4},
			{1,2},
			{0,0}
	};

	if(pressure>52)
	{
		pressure=52;
	}
	else if(pressure<2)
	{
		pressure=2;
	}
	pressure=54-pressure;
	if(screen)
	{
//		TFT_ST7789V_lcd_drawcircle1(39+dy_xpos,dy_ypos,16,BAR_COLOR,GRAY);
//		Rectangle(24+dy_xpos,dy_ypos,30,28,GRAY);
//		TFT_ST7789V_lcd_drawcircle1(39+dy_xpos,27+dy_ypos,16,WHITE,GRAY);
		TFT_ST7789V_lcd_drawcircle1(39+dy_xpos,dy_ypos,16,DARK_TEAL,DARK_TEAL);
		Rectangle(24+dy_xpos,dy_ypos,30,28,DARK_TEAL);
		TFT_ST7789V_lcd_drawcircle1(39+dy_xpos,25+dy_ypos,16,DARK_TEAL,DARK_TEAL);
//		TFT_ST7789V_lcd_drawcircle1(240,55,20,DARK_TEAL,DARK_TEAL); //23-04
//		Rectangle(221,60,38,25,DARK_TEAL);
//		TFT_ST7789V_lcd_drawcircle1(240,85,20,DARK_TEAL,DARK_TEAL);
               //**COLOUR**//
//		TFT_ST7789V_lcd_drawcircle1(40+dy_xpos-1,dy_ypos+1-1,12,LIGHT_GRAY_5,LIGHT_GRAY_5);
//			Rectangle(27+dy_xpos+1,dy_ypos,30-8,28,LIGHT_GRAY_5);//white
//			TFT_ST7789V_lcd_drawcircle1(40+dy_xpos-1,25+dy_ypos-1,12,LIGHT_GRAY_5,LIGHT_GRAY_5);


		TFT_ST7789V_lcd_drawcircle1(40+dy_xpos-1,dy_ypos+1-1,12,LIGHT_GRAY_5,LIGHT_GRAY_5);
			Rectangle(27+dy_xpos+1,dy_ypos+5,30-8,28-5,DARK_TEAL);//white
	}

		loop_count1=0;
		for(loop_count=53;loop_count>0;loop_count--)//53
			{
	          if(loop_count>pressure && loop_count<52)
	          {
//				Rectangle(26+2+dynamic_data[loop_count1][0]+dy_xpos,loop_count+dy_ypos-14,
//						                26-dynamic_data[loop_count1][1]-4,0,(((100+((54-loop_count)*2))<<16)+((200-((54-loop_count)*3))<<8)+(30<<0)));
	          }

	          else if(loop_count%2==0)
	      			{
//	      				Rectangle(29+dynamic_data[loop_count1][0]+dy_xpos-6,loop_count+dy_ypos-12,
//	      										26-dynamic_data[loop_count1][1]+8,0,BAR_COLOR);
	      			}

	      		else
	      		 {
//	      			Rectangle(26+dynamic_data1[loop_count1][0]+dy_xpos,+loop_count+dy_ypos-13-2,
//	      					  26-dynamic_data1[loop_count1][1],0,LIGHT_GRAY_5);
	      		 }

//           	delay_ms(50);

				if(loop_count1>0 && loop_count%2==0 && loop_count<11)
				{
					loop_count1--;
				}

				else if(loop_count1<5 && loop_count%2==0 && loop_count>43)
				{
					loop_count1++;
				}


			}

}

