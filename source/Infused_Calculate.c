/*
 * INFUSED_CALCULATE.c
 *
 *  Created on: 21-Sep-2023
 *      Author: Software4
 */
#include "LPC55S16.h"
#include <infused_calculate.h>
#include "Shapes.h"
#include "TFT_String.h"
#include "TFT_Commands.h"
#include "Color.h"
#include "FR_CALC.h"
#include "Alarm.h"
#include "Bolus_Header.h"
#include "Driver_Timer.h"
//typedef struct tim
//{
	//uint8_t hour;
   // uint8_t minute;
   // uint8_t seconds;
//} clk;
//clk time;
infuse INFUSED;
clk time;
uint8_t war_disp=0;
uint8_t near_to_target_flag=0;
extern uint8_t G_warn_flag;//28-03  //ZOOZ_1
extern float rem_volume;
extern uint8_t near_tar_timer;
static uint8_t near_toggle=0;
uint32_t G_Remaining_volume;
void infuse_status(void);
void infused_data(uint8_t screen,uint8_t r_mode);  //28-03
void infuse_clr();
void infuseD_ml(uint32_t pulse_to_one_ml);
void near_to_target();

//unsigned short stepsper_point_1ml=0; //for 0.1 ml
/*infused ml variables*/

//uint16_t step_count=0;

/*infused ml variables ends*/

clk sec_to_time_format(uint32_t sec);
//extern uint8_t G_warn_flag;//28-03
extern uint8_t G_infusion_modes;

extern volatile uint8_t normal_run;
extern void KVOmodeRun(void);
extern void infuse_clr();
extern uint8_t G_Bolus_target_flag;
extern uint8_t Bolus_status_flag;
extern uint8_t home_flag;
extern uint8_t war_status[3];

void infuseD_ml(uint32_t pulse_to_one_ml)
{
	INFUSED.stepsper_point_1ml  = pulse_to_one_ml/10;  //0.1

	if(INFUSED.step_count>INFUSED.stepsper_point_1ml)
	{
		INFUSED.step_count = INFUSED.stepsper_point_1ml;
	}
	//printf("\n ->infused.stepsper_point_1ml = %d ",INFUSED.stepsper_point_1ml);
}

clk sec_to_time_format(uint32_t sec)
{

    time.hour = (sec / 3600);
    time.minute = (sec - (3600 * time.hour)) / 60;
    time.seconds = (sec - (3600 * time.hour) - (time.minute * 60));
    //printf("hour : %d minute : %d seconds : %d ",time.hour,time.minute,time.seconds);
    return time;
}


void infused_data(uint8_t screen,uint8_t r_mode)		//28-03
{

	//uint32_t ml=1;
	uint16_t xpos=0;
	uint16_t ypos=0;
	uint16_t r_pos=0;
	//uint16_t ypos_time[4]={160+(25*1),160+(25*2)};//28-03
	//uint16_t ypos_inf[4]={160,160+(25*1),160+(25*2)};
	static uint32_t prev_infused_ml=1,prev_rem_vol=2;
	char local_buffer[6];
	static uint8_t prev_elapsed_min=1,prev_elapsed_hr=1;

//	if(G_infusion_modes!=2)

		if(prev_infused_ml!=INFUSED.ml || screen)
		{
			if(r_mode<4)
			{
				ypos=0;
				r_pos=0;
			}

			else
			{

				if(G_warn_flag)
				{
					ypos=2;
				}
				else
				{
					ypos=1;
				}
				r_pos=33;

			}
			if(!r_mode)
			{
			Font_Select(ARIAL_BOLD_14);
			sprintf(local_buffer,"%.1f",((float)prev_infused_ml/10.0));    //store the integer variables in to string using sprintf

			xpos=Right_Alignments((uint8_t*)local_buffer,80-r_pos);                 //(uint8_t*)letter
			TFT_writenumber_float_1d(xpos,208,(float)prev_infused_ml,LIGHT_GRAY_5,LIGHT_GRAY_5);   //Infused_ml

			sprintf(local_buffer,"%.1f",((float)INFUSED.ml/10.0));         //store the integer variables in to string using sprintf
			xpos=Right_Alignments((uint8_t*)local_buffer,80-r_pos);                 //(uint8_t*)letter
			TFT_writenumber_float_1d(xpos,208,(float)INFUSED.ml,LIGHT_GRAY_5,DARK_TEAL);   //Infused_ml


			Font_Select(ARIAL_BOLD_14);
			TFT_String(94,208,(uint8_t* )"ml",LIGHT_GRAY_5,DARK_TEAL);
			prev_infused_ml=INFUSED.ml;
			}


//			Font_Select(ARIAL_12);
//			TFT_String(100,160-(25*1)+5,(uint8_t*)"Remaining",MAIN_FRAME_COL,WHITE);//28-03
//			TFT_writenumber_float_1d(237,135+5,rem_volume,MAIN_FRAME_COL,WHITE);
//			TFT_String(285,135+5,(uint8_t*)"mL",MAIN_FRAME_COL,WHITE);


		}


	if(r_mode==2 ) return;	//28-03
	time=sec_to_time_format(INFUSED.sec);

	if((time.minute!=prev_elapsed_min) || screen)
	{
		Font_Select(ARIAL_BOLD_14);
		sprintf(local_buffer,"%04d",(prev_elapsed_hr*100)+prev_elapsed_min);    //store the integer variables in to string using sprintf
		xpos=Right_Alignments((uint8_t*)local_buffer,240);
		TFT_Time_disp(xpos,208,(prev_elapsed_hr*100)+prev_elapsed_min,LIGHT_GRAY_5,LIGHT_GRAY_5);//28-03

		sprintf(local_buffer,"%04d",(time.hour*100)+time.minute);    //store the integer variables in to string using sprintf
		xpos=Right_Alignments((uint8_t*)local_buffer,240);
		TFT_Time_disp(xpos,208,(time.hour*100)+time.minute,LIGHT_GRAY_5,DARK_TEAL);//28-03

		prev_elapsed_hr=time.hour;
		prev_elapsed_min=time.minute;

		Font_Select(ARIAL_BOLD_14);
		TFT_String(255,208,(uint8_t* )"hrs",LIGHT_GRAY_5,DARK_TEAL);
	}

	if(r_mode==0 ) return;	//28-03

	if(((prev_rem_vol!=(MOT.volume-INFUSED.ml))&&MOT.volume)|| screen)
	{

	    G_Remaining_volume=MOT.volume-INFUSED.ml;
	    Rectangle(10,200+5,20+40+30+20,25,LIGHT_GRAY_5);
		if(MOT.volume>=INFUSED.ml)
		{
			Font_Select(ARIAL_BOLD_14);
			sprintf(local_buffer,"%.1f",((float)(MOT.volume-prev_rem_vol)/10.0));    							//store the integer variables in to string using sprintf
			xpos=Right_Alignments((uint8_t*)local_buffer,83);                 						//(uint8_t*)letter
			TFT_writenumber_float_1d(xpos+20,208,(float)prev_rem_vol,LIGHT_GRAY_5,LIGHT_GRAY_5);   //Infused_ml

			sprintf(local_buffer,"%.1f",((float)(MOT.volume-INFUSED.ml)/10.0));        				 //store the integer variables in to string using sprintf
			xpos=Right_Alignments((uint8_t*)local_buffer,83);                						 //(uint8_t*)letter
			TFT_writenumber_float_1d(xpos+20,208,(float)G_Remaining_volume,LIGHT_GRAY_5,DARK_TEAL);  //Infused_ml

			//Font_Select(ARIAL_BOLD_14);
			TFT_String(94+20,208,(uint8_t* )"ml",LIGHT_GRAY_5,DARK_TEAL);
//			 Font_Select(ARIAL_14);
//					   TFT_Number(100,50,xpos,YELLOW,RED);
		}

		else
		{
			Font_Select(ARIAL_BOLD_14);
			sprintf(local_buffer,"%.1f",0.0);         												//store the integer variables in to string using sprintf
			xpos=Right_Alignments((uint8_t*)local_buffer,83);                 						//(uint8_t*)letter
			TFT_writenumber_float_1d(xpos,208,0.0,LIGHT_GRAY_5,DARK_TEAL);  						    //Infused_ml
		}

		prev_rem_vol=(MOT.volume-INFUSED.ml);
		if(near_toggle==1)
		{
			if(near_to_target_flag)
			{
				//TFT_writenumber_float_1d(48,202,(float)(MOT.volume-INFUSED.ml),YELLOW,YELLOW);
//				TFT_String(94,208,(uint8_t* )"ml",YELLOW,YELLOW);
			}

		}
	}


}

/*************************************************************
  Function Name : infuse_status(void)
  Description   :	To Track the target infuse status
  Input         : Void
  Output        : Void
*************************************************************/

void infuse_status(void)
{
	static uint8_t war_disp=0;
	 uint16_t Remaining_volume=0;

	 if(MOT.volume>INFUSED.ml)
	 {
		 Remaining_volume =(MOT.volume-INFUSED.ml);

	 }

	 else
	 {

		 if( ( (!BOL.run_mode) && (normal_run)) && (!Bolus_status_flag) )
		 {
			 G_Bolus_target_flag=1;
		 }

	 }


	 if(normal_run && (G_infusion_modes==1)||(G_infusion_modes==3) )//&& !bolusflag && syringe_size)
	  {
		 //  if(MOT.flowrate)
		  // {
			// if(MOT.volume)
			//{
			//  if(MOT.time)
				//{
				if (((time.hour * 100 + time.minute) >= MOT.time)
					 || !(Remaining_volume))                         //!(MOT.volume-INFUSED.ml))
				  {

					if((!(Remaining_volume)) && (G_Bolus_target_flag) )					//(!(MOT.volume-INFUSED.ml))
					  {
						//	Alarm(TARGET);
						    KVOmodeRun( );
					    	infused_data(1,1);		//28-03
					  }
				  }
				  	else if((MOT.time-(time.hour * 100 +time.minute))<=3  && (!war_disp))
					{
//						Warning_screen((uint8_t*)"Near to ",(uint8_t*)"Target",0);
						WARN_SOUND(3,1,3);
						war_disp=1;
					}

					else if(war_disp &&( MOT.time-(time.hour * 100 +time.minute))>3 )
					{
						//Warning_Scr_Erase();//CHECK
						war_disp=0;
					}
				//}
			//}
		//}

	  }

}


void infuse_clr()
{
	INFUSED.ml=0;
	INFUSED.sec=0;
	G_warn_flag=0;  //which clear the warning screen
}


void near_to_target()
{


	if( (((MOT.time-(time.hour * 100 +time.minute))<=3) && (normal_run)) && (!near_to_target_flag) )
			{

				near_to_target_flag=1;

			}

		else if((( MOT.time-(time.hour * 100 +time.minute))>3) &&(normal_run) )
		{
	//		if(home_flag)
	//		{
	//			Warning_Scr_Erase();
	//		//	home_flag=0;
	//		}
	//		Font_Select(ARIAL_14);
	//		TFT_Number(150,70,200,YELLOW,RED);
			near_to_target_flag=0;
			war_status[1]=0;
//			bottom_para_frame(1);
		//	infused_data(1,1);

		}
		else if(!normal_run && near_to_target_flag)
		{
			near_to_target_flag=0;
			war_status[1]=0;
			bottom_para_frame(G_infusion_modes);
		//	infused_data(1,1);
		}

		if(near_to_target_flag)
		{
			war_status[1]=1;
//			if(!near_tar_timer)
//					{
//						if(!near_toggle)
//						{
//							Warning_screen((uint8_t*)"Near to Target",0);
//							near_toggle=1;
//
//
//						}
//						else
//						{
//							near_toggle=0;
//							bottom_para_frame(G_infusion_modes);
//					//		infused_data(1,1);
//						}
//
//						near_tar_timer=4;
//
//
//					}
		}
}
