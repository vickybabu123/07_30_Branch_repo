/*
 * plunger_alarm.c
 *
 *  Created on: 02-Mar-2024
 *      Author: Software4
 */
#include"plunger_alarm.h"
#include "Motor.h"
#include "Alarm.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "Color.h"
#include "Bolus_Header.h"
#include "plunger_alarm.h"
#include <stdio.h>
#include "stdint.h"
#include "fsl_clock.h"

uint8_t G_collar_cnt;
uint8_t collar_inc_cnt=0;
uint8_t disc=0;
uint8_t release=0;
uint8_t drv=0;
uint8_t disc_not_engaged=0;
uint8_t plunger_distrubed=0;
uint8_t drv_fault=0;
uint8_t siphonic_occured=0;
uint8_t siphonic_time=0;
uint8_t erase_screen=0;
extern uint8_t erase_screen;
extern uint8_t sy_size;
extern bool DISC_sample(uint8_t s_sample);
extern bool RLSW_sample(uint8_t s_sample);
extern uint8_t G_Loop_alarm;
extern uint8_t G_syringe_size_flag;
extern uint8_t erase_screen;
extern uint8_t sy_size;
extern uint8_t G_KVO_run_flag;

void plunger_alarm_check();

extern volatile uint8_t normal_run;
extern uint16_t press_run_alarm;

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void plunger_alarm_check()
{
	//uint8_t sy_size=0;
	//sy_size=detect_syringe_size(1);
	disc = DISC_sample(5);
	release=RLSW_sample(5);
	drv=DRV_sample(5);
//   Font_Select(ARIAL_BOLD_14);
//	TFT_Number(150,70,disc,BLACK,WHITE);
//////
//////    TFT_Number(150,110,release,BLACK,WHITE);
//	TFT_Number(150,90,release,BLACK,RED);
	if(!disc)
	{
//		press_run_alarm=10;
	}

	if(!disc)
	{
	//  TFT_String(30,120,(uint8_t *)"                                        ",MAIN_FRAME_COL,MAIN_FRAME_COL);
		disc_not_engaged=0;
		siphonic_time=30;
		siphonic_occured=0;
		if(erase_screen)
			{
				//Warning_Scr_Erase();
				erase_screen=0;
			}
	}
	else
		{
			if(normal_run && sy_size)
			{
			  //TFT_String(30,120,(uint8_t *)"disc not engaged",MAIN_FRAME_COL,RED);
				disc_not_engaged=1;
				siphonic_occured=0;
			}
			else if(!normal_run && sy_size && (!siphonic_time))
			{
				siphonic_occured=1;
				disc_not_engaged=0;
			}
			else
				{
					//TFT_String(30,120,(uint8_t *)"                                        ",MAIN_FRAME_COL,MAIN_FRAME_COL);
					disc_not_engaged=0;
					siphonic_occured=0;
				}
		}




	if(release)
	{

				if((normal_run && sy_size) || (!BOL.mode) ||(BOL.mode==1))
				{
					//TFT_String(30,150,(uint8_t *)"PLUNGER DISTRUBED",MAIN_FRAME_COL,RED);
					plunger_distrubed=1;
				}
				else
				{
				//	TFT_String(30,150,(uint8_t *)"                                                   ",MAIN_FRAME_COL,MAIN_FRAME_COL);
					plunger_distrubed=0;
				}

	}
	else
	   {
		   plunger_distrubed=0;
	   }

	if(drv)
		{

			if((normal_run ) || (!normal_run))
			{
				//TFT_String(30,150,(uint8_t *)"PLUNGER DISTRUBED",MAIN_FRAME_COL,RED);
				drv_fault=1;
			}
			else
			{
			//	TFT_String(30,150,(uint8_t *)"                                                   ",MAIN_FRAME_COL,MAIN_FRAME_COL);
				drv_fault=0;
			}

		}
		else
		   {
			drv_fault=0;
		   }

	plunger_alarm_frames();

}
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void plunger_alarm_frames()
{

	if(((BOL.run_mode) || (normal_run)) && (plunger_distrubed))
		{
		  G_syringe_size_flag=1;
		  G_KVO_run_flag=0;
		  Alarm(PLUNGER_DIS);
		  G_Loop_alarm=1;
		}
//	else if(disc_not_engaged)
//	{
//		disc_not_engaged=0;
//		Alarm(NOT_ENGAGE);
//		disc_not_engaged=0;
////		Warning_screen((uint8_t*)"Plunger Not Engaged");
////		normal_run=0;
////		erase_screen=1;
//	}
	else if(((BOL.run_mode) || (normal_run)) && (disc==1) )
		{
//			disc_not_engaged=0;
			Alarm(NOT_ENGAGE);
//			disc_not_engaged=0;
////			Warning_screen((uint8_t*)"Plunger Not Engaged");
//			normal_run=0;
//			erase_screen=1;
		}

	if(siphonic_occured)
	{
		G_syringe_size_flag=1;
		Alarm(SIPHONIC);
		siphonic_occured=0;

	}
}

uint8_t collar_fun(void)
{
static uint8_t collar_confirm_flag=0;


	if(!G_collar_cnt)
	{
		if(GPIO->PIN[0] & (1<<9))
		 {
			collar_inc_cnt++;
		 }

		 else
		 {
			 collar_inc_cnt=0;
		 }

	G_collar_cnt=1;

		if(collar_inc_cnt>5)
		{
			collar_inc_cnt=5;
			collar_confirm_flag=1;
		}
		else
		{
			collar_confirm_flag=0;
		}
  }

	return collar_confirm_flag;
}

