/*
 * SYR_length_frame.c
 *
 *  Created on: 29-Feb-2024
 *      Author: Software4
 */
#include "Linear_Header.h"
#include "Syringe_calib_header.h"
#include "Driver_ADC.h"
#include "color.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "Keypad.h"
#include "Shapes.h"
#include "Power_off.h"
#include "Driver_Timer.h"
#include "Driver_I2C.h"
#include "Occlusion.h"
#include "SD_Calib.h"
#include "Boot_up.h"

uint16_t temp_sl_pull_in=0;
uint16_t temp_sl_pull_out=0;

//extern uint8_t prev_sysize;
extern uint16_t value_l[8];
extern uint16_t value_c[8];
extern uint16_t sl_pullout_ADC;
extern uint16_t sl_pullin_ADC;
extern uint16_t linear_temp_adc;

extern uint8_t brand_select_no;
void swap_sysize_glow(uint8_t currentOption,uint8_t prev_sysize);
extern void frame_footer_press_ok_to_edit();

/****************************************************************************
* Function Name : void frame_syr_len_and_plunger_adc_main(uint8_t cursor,uint8_t screen)
* Arguments     : cursor,screen
* Return type   : void
* Description   : these function display the main screen of and navigation sl calibration.

*****************************************************************************/
void frame_syr_len_and_plunger_adc_main(uint8_t cursor,uint8_t screen)
{
//	uint8_t sel=0;
	uint8_t loop_count=0;
	char* str[2]={"Syringe Length calib"};   //	,"Plunger ADC calibration"};
	uint32_t   txt_color[2]={MAIN_FRAME_COL,LIGHT_GRAY};
	uint32_t   bg_color[2]={LIGHT_GRAY,MAIN_FRAME_COL};
	//uint16_t   bor_color[2]={LIGHT_GRAY,BLACK};
	//uint16_t   str_color[2]={BLACK,LIGHT_GRAY};
    if(screen)
    {
    	    frame_footer_press_ok_to_edit();
    }

    else
    {
//    	if(cursor)
//    	{
//    		Rectangle(7,5+36+(43*0),290+16,40,MAIN_FRAME_COL);
    	//}
//    	else
//    	{
    	//	Rectangle(7,5+36+(43*1),290+16,40,MAIN_FRAME_COL);
    	//}
    }

	Font_Select(ARIAL_BOLD_14);

	Rectangle(9,48+(38*(0)),300,31,LIGHT_GRAY);

	separation_lines2(1,1);

		TFT_String(12,48+(37*0)+6,(uint8_t *)str[0],bg_color[0],MAIN_FRAME_COL);
	    TFT_String(12,48+(37*1)+6,(uint8_t *)str[1],bg_color[1],txt_color[0]);

}
/////////////////////////////////////////////////////////////////////////////////
void frame_syringe_size_glow(uint8_t state)
{
	int row=0,col=0;
	int sysize=1;

	uint8_t acutual_size;
	acutual_size=detect_syringe_size(1);            //detect_syringe_size(2);

	unsigned int   color[2]={GRAY,WHITE};
	int sel=0;
	char* mat[7]={"789"," 2 mL"," 5 mL","10 mL","20 mL","30 mL","50 mL"};
	//sysize=acutual_size=detect_syringe_size(1);
	Rectangle(0,0,320,240,MAIN_FRAME_COL);
	CENTER_SCR(MAIN_FRAME_COL); //center green
	if(state==1)
	{
				Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Syringe Length Calibration");

				Frame_Footer(NEW_FTR_COL,WHITE,(uint8_t*)"OKAY",(uint8_t*)"to confirm");

				CENTER_SCR(MAIN_FRAME_COL); 			//center green
				border_lines();
				right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);


	}
	else if((state==0))
	{

		Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"20 PSI Calibration");

		Frame_Footer(NEW_FTR_COL,WHITE,(uint8_t*)"OKAY",(uint8_t*)"to confirm");

		CENTER_SCR(MAIN_FRAME_COL); 			//center green
		border_lines();
		right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	}

	//Letters_Alignments(0,0,"opiu",2);

	 for(row=0;row<1;row++)
	 {
		 for(col=0;col<3;col++)
			{
//			  if(acutual_size == sysize && !sysize &&  !acutual_size)
//			  {
			 if(acutual_size == sysize)
			 {
			   sel=1;
			 }
			  else sel=0;

			 TFT_ST7789V__rectangle_curve(25+(95*col),62,50,80,0,color[sel]); // cursor box // caution window -- Y   X 1

			 TFT_String(40+(95*col),75,(uint8_t *)mat[sysize],color[sel],BLACK);

		//	 Letters_Alignments(0,0,mat[sysize],2);

			 if(acutual_size == sysize+3)
				 {
				   sel=1;
				 }

			 else
				 sel=0;

			 TFT_ST7789V__rectangle_curve(25+(95*col),130,50,80,0,color[sel]); // cursor box // caution window -- Y   X 1
			 TFT_String(35+(95*col),145,(uint8_t *)mat[sysize+3],color[sel],BLACK);
			 sysize++;
		   }
	  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void swap_sysize_glow(uint8_t currentOption,uint8_t prev_sysize1)
{
	uint8_t row1=0,col1=0;
		uint8_t row2=0,col2=0;

	//	uint8_t sysize=0;

		uint8_t acutual_size=currentOption;            //detect_syringe_size(2);

		uint32_t   color[2]={GRAY,WHITE};

		uint8_t sel=0;

		char* mat[7]={"789"," 2 mL"," 5 mL","10 mL","20 mL","30 mL","50 mL"};


		Font_Select(ARIAL_BOLD_14);


		if (acutual_size >= 1 && acutual_size <= 6)
		{
		         row1 = (acutual_size - 1) / 3;
		         col1 = (acutual_size - 1) % 3;
		}
		if (prev_sysize1 >= 1 && prev_sysize1 <= 6)
			{
			         row2 = (prev_sysize1 - 1) / 3;
			         col2 = (prev_sysize1 - 1) % 3;
			}
	  //  printf("\n acutual_size = %d prev_sysize = %d",acutual_size,prev_sysize);

		if(!acutual_size)
		{
			sel=0;
		}
		else
			{
			  sel=1;
			}


		TFT_ST7789V__rectangle_curve(25+(95*col1),62+(68*row1),50,80,0,color[sel]);

		if(acutual_size)
		{
			TFT_String(40+(row1*-5)+(95*col1),75+(row1*70),(uint8_t *)mat[acutual_size],color[sel],BLACK);
		}
		else
		{
			TFT_String(40+(row1*-5)+(95*col1),75+(row1*70),(uint8_t *)mat[1],color[sel],BLACK);
		}

		sel=0;

		if(prev_sysize1)
		{
			TFT_ST7789V__rectangle_curve(25+(95*col2),62+(68*row2),50,80,0,color[sel]);

			TFT_String(40+(row2*-5)+(95*col2),75+(row2*70),(uint8_t *)mat[prev_sysize1],color[sel],BLACK);
		}
		/*else
		{  if()
			TFT_ST7789V__rectangle_curve(25+(95*col2),62+(68*row2),50,80,0,color[sel]);

		    TFT_String(40+(row2*-5)+(95*col2),75+(row2*70),(uint8_t *)mat[1],color[sel],BLACK);
		}*/
	//		prev_sysize=acutual_size;
}


/****************************************************************************
* Function Name : void syringe_length_and_collar_main_screen(uint8_t cursor,uint8_t screen)
* Arguments     : cursor,screen
* Return type   : void
* Description   : these function display the main screen of syringe length and collar main screen

*****************************************************************************/
void syringe_length_and_collar_main_screen(uint8_t cursor,uint8_t screen)
{
	 uint8_t sel=0;
			uint8_t loop_count=0;
			char* str[2]={"Syr Length","Col Length"};
			unsigned int   txt_color[2]={LIGHT_GRAY,MAIN_FRAME_COL};
			unsigned int   bg_color[2]={LIGHT_GRAY,MAIN_FRAME_COL};
			float length_disp[8];
			float collar_disp[8];
			uint8_t ssize=0;

			ssize=detect_syringe_size(1);


			value_l[ssize]= EEPROM_dataread(EEPROM_SL_LEN_VAL+(brand_select_no)*12+((ssize-1)*2), 2);

			if(value_l[ssize] > 800)
			{
				value_l[ssize] = 800;
				EEPROM_datawrite(EEPROM_SL_LEN_VAL+(brand_select_no)*12+((ssize-1)*2),value_l[ssize], 2);
			}

			value_c[ssize]= EEPROM_dataread(EEPROM_SL_COL_VAL+(brand_select_no)*12+((ssize-1)*2), 2);
			if(value_c[ssize] > 300)
			{
				value_c[ssize] = 300;
				EEPROM_datawrite(EEPROM_SL_COL_VAL+(brand_select_no)*12+((ssize-1)*2),value_c[ssize], 2);
			}

			length_disp[ssize]=value_l[ssize];
			collar_disp[ssize]=value_c[ssize];


	        if(screen)
	        {

	        	Rectangle(0,0,320,240,MAIN_FRAME_COL);
	        	Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"SL Calibration");	// header
	        	frame_footer_press_ok_to_edit();

	        }
			if(cursor)
			{
				Rectangle(9,48+(38*(0)),301,30,MAIN_FRAME_COL);
			}
			else
			{
				Rectangle(9,48+(38*(1)),301,30,MAIN_FRAME_COL);
			}

			Rectangle(9,48+(38*(cursor)),301,30,LIGHT_GRAY);
			Font_Select(ARIAL_BOLD_14);

			separation_lines2(1,1);

	//		Rectangle(15,55+(31*(cursor)),280,31,LIGHT_GRAY);//cursor

			if(cursor==1)
			{
			   TFT_String(12,48+(37*0)+6,(uint8_t *)str[0],bg_color[1],txt_color[0]);
			   TFT_String(12,48+(37*1)+6,(uint8_t *)str[1],bg_color[0],txt_color[1]);

			   TFT_String(180,6+48,(uint8_t *)":",bg_color[1],txt_color[0]);
			   TFT_String(180,6+86,(uint8_t *)":",bg_color[0],txt_color[1]);

			   TFT_writenumber_float_1d(200,48+(37*0)+6,length_disp[ssize],bg_color[1],txt_color[0]);
			   TFT_writenumber_float_1d(200,48+(37*1)+6,collar_disp[ssize],bg_color[0],txt_color[1]);

			   TFT_String(250,6+48,(uint8_t *)"mm",bg_color[1],txt_color[0]);
			   TFT_String(250,6+85,(uint8_t *)"mm",bg_color[0],txt_color[1]);

			}
			else
			{
				TFT_String(12,48+(37*0)+6,(uint8_t *)str[0],bg_color[0],txt_color[1]);
			    TFT_String(12,48+(37*1)+6,(uint8_t *)str[1],bg_color[1],txt_color[0]);

			    TFT_String(180,6+48,(uint8_t *)":",bg_color[0],txt_color[1]);
			    TFT_String(180,6+85,(uint8_t *)":",bg_color[1],txt_color[0]);

			    TFT_writenumber_float_1d(200,48+(37*0)+6,length_disp[ssize],bg_color[0],txt_color[1]);
			    TFT_writenumber_float_1d(200,48+(37*1)+6,collar_disp[ssize],bg_color[1],txt_color[0]);

				TFT_String(250,6+48,(uint8_t *)"mm",bg_color[0],txt_color[1]);
				TFT_String(250,6+85,(uint8_t *)"mm",bg_color[1],txt_color[0]);
			}


}
/////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
* Function Name : void plunger_adc_live_main_screen()
* Arguments     : void
* Return type   : void
* Description   : this function display pull in pull out navigation screen  with cureent loaded value.

*****************************************************************************/
void plunger_adc_screen(uint8_t cursor,uint8_t screen)
{
//	int loop_count=0;
//	         if(screen)
//	         {
//	        	 Rectangle(0,0,320,240,MAIN_FRAME_COL);
//
//				Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Plunger Calibration");
//
//				Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
//				CENTER_SCR(MAIN_FRAME_COL);  //center green
//				right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
//				Font_Select(ARIAL_BOLD_14);
//				TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
//
//				 //Font_Select(BEBAS_36);
//				  //TFT_Number(100,60,65535,MAIN_FRAME_COL,LIGHT_GRAY);
//				 Rectangle(7,117+(31*(cursor)),290+16,31,LIGHT_GRAY);
//				 if(cursor)
//				 {
//					 Rectangle(7,117+(31*0),290+16,31,MAIN_FRAME_COL);
//				 }
//
//				 else
//				 {
//					 Rectangle(7,117+(31*1),290+16,31,MAIN_FRAME_COL);
//				 }
//		 //*********************************4_Menu_Alignments***********************************//
//	//			  Rectangle(7,118,290,0,BLACK);
//	//			  Rectangle(7,162,290,0,BLACK);
//	//			  Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"Press          to Calibrate",(uint8_t*)"OK",2);
//	         }
////	     	for(loop_count = 1; loop_count <=2 ; loop_count++)
////	     		{
////	     		 Rectangle(7,115,290+16,0, LIGHT_GRAY);// line
////				 Rectangle(7,115+(31*loop_count),290+16,0, LIGHT_GRAY);// line
////				 Rectangle(7,115+(31*loop_count),290+16,0, LIGHT_GRAY);// line
////	     		}
//	         separation_lines(2,3);
//	         Font_Select(ARIAL_14);
//
//	         if(!cursor)
//					  {
//	//        	 Rectangle(7,117+(31*0),290+16,31,MAIN_FRAME_COL);
//						  TFT_String(16,125,(uint8_t *)"Pull out ADC",LIGHT_GRAY,MAIN_FRAME_COL); //-1
//						  TFT_String(200,125,(uint8_t *)":",LIGHT_GRAY,MAIN_FRAME_COL);
//						  TFT_Number(15+220,125,sl_pullout_ADC,LIGHT_GRAY,MAIN_FRAME_COL);
//						  TFT_String(16,155+5,(uint8_t *)"Pull in ADC",MAIN_FRAME_COL,LIGHT_GRAY);
//						  TFT_String(200,155+5,(uint8_t *)":",MAIN_FRAME_COL,LIGHT_GRAY);
//						  TFT_Number(15+220,155+5,sl_pullin_ADC,MAIN_FRAME_COL,LIGHT_GRAY);
//					  }
//			  else
//				  {
//	//			  Rectangle(7,117+(31*1),290+16,31,MAIN_FRAME_COL);
//					  TFT_String(16,125,(uint8_t *)"Pull out ADC",MAIN_FRAME_COL,LIGHT_GRAY);//-2
//					  TFT_String(16,125,(uint8_t *)"Pull out ADC",MAIN_FRAME_COL,LIGHT_GRAY);
//					  TFT_String(200,125,(uint8_t *)":",MAIN_FRAME_COL,LIGHT_GRAY);
//					  TFT_Number(16+220,125,sl_pullout_ADC,MAIN_FRAME_COL,LIGHT_GRAY);
//					  TFT_String(16,155+5,(uint8_t *)"Pull in ADC",LIGHT_GRAY,MAIN_FRAME_COL);
//					  TFT_String(200,155+5,(uint8_t *)":",LIGHT_GRAY,MAIN_FRAME_COL);
//					  TFT_Number(16+220,155+5,sl_pullin_ADC,LIGHT_GRAY,MAIN_FRAME_COL);
//				  }
	if(screen)
			{

	      Rectangle(0,0,320,240,MAIN_FRAME_COL);
	//		CLRSCR(MAIN_FRAME_COL);
		//	Rectangle(0,0,320,35,NEW_HDR_COL);

			Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Plunger Tuning");

		//	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Accuracy Calibration");
			Font_Select(ARIAL_BOLD_14);

			Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
			CENTER_SCR(MAIN_FRAME_COL);  //center green
			right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
			Font_Select(ARIAL_BOLD_14);
			TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);

			//*********************************4_Menu_Alignments***********************************//
			for(int loop_count = 1; loop_count <=2 ; loop_count++)
			{
				Rectangle(7,114,290+16,0, LIGHT_GRAY);// line
				Rectangle(7,114+(37*loop_count),290+16,0, LIGHT_GRAY);// line
				//Rectangle(7,114+(37*loop_count),290+16,0, LIGHT_GRAY);// line
			}

		///	Frame_Footer(NEW_FTR_COL,HDR_TXT_COL,(uint8_t*)"Press          to Calibrate",(uint8_t*)"OK",2,14);

		}

//				TFT_ST7789V__rectangle_curve(5 ,120+(40*0),40,310,0,MAIN_FRAME_COL);
//				TFT_ST7789V__rectangle_curve(5 ,120+(40*1),40,310,0,MAIN_FRAME_COL);
//				TFT_ST7789V__rectangle_curve(5 ,120+(40*cursor),40,310,0,LIGHT_GRAY);  //Rectangle Curve Option 1

			if(cursor)
			 {
				 Rectangle(7,118+(36*0),290+16,30,MAIN_FRAME_COL);
			 }

			 else
			 {
				 Rectangle(7,118+(36*1),290+16,30,MAIN_FRAME_COL);
			 }

			Rectangle(7,118+(36*(cursor)),290+16,30,LIGHT_GRAY);

				Font_Select(ARIAL_BOLD_14);

			if(!cursor)
			{

				TFT_String(16,125,(uint8_t *)"Pull out ADC",LIGHT_GRAY,MAIN_FRAME_COL); //-1
				TFT_String(180,125,(uint8_t *)":",LIGHT_GRAY,MAIN_FRAME_COL);
				TFT_Number(210,125,sl_pullout_ADC,LIGHT_GRAY,MAIN_FRAME_COL);

				TFT_String(10,160,(uint8_t *)"Pull in ADC",MAIN_FRAME_COL,LIGHT_GRAY);
				TFT_String(180,160,(uint8_t *)":",MAIN_FRAME_COL,LIGHT_GRAY);
				TFT_Number(210,160,sl_pullin_ADC,MAIN_FRAME_COL,LIGHT_GRAY);

			}

			else
			{

				//TFT_String(10,131,(uint8_t *)"Pull out ADC",MAIN_FRAME_COL,LIGHT_GRAY);//-2
				TFT_String(10,125,(uint8_t *)"Pull out ADC",MAIN_FRAME_COL,LIGHT_GRAY);
				TFT_String(180,125,(uint8_t *)":",MAIN_FRAME_COL,LIGHT_GRAY);
				TFT_Number(210,125,sl_pullout_ADC,MAIN_FRAME_COL,LIGHT_GRAY);

				TFT_String(10,160,(uint8_t *)"Pull in ADC",LIGHT_GRAY,MAIN_FRAME_COL);
				TFT_String(180,160,(uint8_t *)":",LIGHT_GRAY,MAIN_FRAME_COL);
				TFT_Number(210,160,sl_pullin_ADC,LIGHT_GRAY,MAIN_FRAME_COL);

			}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
* Function Name : void warning_screen_ADC( uint8_t disp_1)
* Arguments     : disp_1
* Return type   : void
* Description   : these function display the warning screen for pull in & pull out

*****************************************************************************/
void warning_screen_ADC( uint8_t disp_1)
{
	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	Frame_Header(LIGHT_GRAY,DARK_TEAL,(uint8_t*)"Plunger Tuning");	// header


	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
	//CENTRE GREEN
	CENTER_SCR(MAIN_FRAME_COL);  //center green
	border_lines();

	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
	Font_Select(ARIAL_BOLD_14);
	TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);

	//	    TFT_ST7789V__rectangle_curve(7,45,150,305,0,WHITE); // SCREEN
	TFT_ST7789V__rectangle_curve(10,50,140,300,0,WHITE);//70//80//120
			Font_Select(ARIAL_BOLD_14);
			TFT_String(30-10,80+15,(uint8_t*)" 1.  Ensure the plunger is",WHITE,BLACK);

			if(disp_1)
			{
				TFT_String(43,105+15,(uint8_t*)" fully in",WHITE,BLACK);
			}
			else
			{
				TFT_String(43,105+15,(uint8_t*)" fully out",WHITE,BLACK);
			}

		    TFT_String(30-10,140+15,(uint8_t*)" 2. Ensure no Syringe",WHITE,BLACK);

		    TFT_ST7789V_Triangle_24pt(62,90,0,WHITE,WARNING_COLOR);
		    TFT_ST7789V_Exclamatory_9pt_arial(70,100,0,WARNING_COLOR,BLACK);
		    Font_Select(ARIAL_BOLD_14);
			TFT_String(130,65,(uint8_t*)"Warning",WHITE,ORANGE);

}

/****************************************************************************
* Function Name : void warning_calibrated_screen( uint8_t cursor)
* Arguments     : cursor
* Return type   : void
* Description   : these function display the warning calibrated screen for pull in & pull out

*****************************************************************************/
void warning_calibrated_screen( uint8_t cursor)
{
	CLRSCR(MAIN_FRAME_COL);
			Font_Select(BEBAS_43);
		 Frame_Header(NEW_HDR_COL,HDR_TXT_COL,(uint8_t*)"Plunger Tuning");

			 	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
				right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
				Font_Select(ARIAL_BOLD_14);
				TFT_String(163,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
			 	//CENTRE GREEN
			 	CENTER_SCR(MAIN_FRAME_COL);  //center green
			 	border_lines();

				Font_Select(ARIAL_BOLD_14);
				TFT_ST7789V__rectangle_curve(10,50+10,138-20,300,0,WHITE);

	//	TFT_String(95+5,120+10-15,(uint8_t*)"Calibrated",WHITE,BLACK);
	// TFT_String(53,130,(uint8_t*)"Warning",WHITE,BLACK);

	// TFT_ST7789V__rectangle_curve(50,100,25,25,0,YELLOW);
  //   TFT_String(53,112,(uint8_t*)"!",YELLOW,BLACK);
	 if(cursor)
	 {
		 sl_pullin_ADC=linear_temp_adc;
	 }
	 else
	 {
		 sl_pullout_ADC=linear_temp_adc;
	 }

	 if(!cursor)
	 {
		 sl_pullout_ADC=linear_temp_adc;
		 EEPROM_datawrite(EEPROM_SL_PULL_OUT,sl_pullout_ADC,2);

		 if( (sl_pullout_ADC>60800))
		 {

			 Font_Select(ARIAL_BOLD_14);
			 TFT_String(30+10+10,110,(uint8_t*)"           Calibrated   ",WHITE,BLACK);
		 }
		 else if((sl_pullout_ADC<60800))
		 {
			 Font_Select(ARIAL_BOLD_14);
			 TFT_String(30+10+10,110,(uint8_t*)"   Calibration Failed",WHITE,BLACK);
		 }

	 }

	 else if(cursor)
	 {
		 sl_pullin_ADC=linear_temp_adc;
		 EEPROM_datawrite(EEPROM_SL_PULL_IN,sl_pullin_ADC,2);
		 if( (sl_pullin_ADC<4500) && (sl_pullin_ADC>2000) )
		 {

			 Font_Select(ARIAL_BOLD_14);
			 TFT_String(30+10+10,110,(uint8_t*)"        Calibrated   ",WHITE,BLACK);
		 }

		 else
		 {
			 Font_Select(ARIAL_BOLD_14);
			 TFT_String(30+10+10,110,(uint8_t*)"   Calibration Failed",WHITE,BLACK);
		 }

	}

}
