/****************************************************************************
* File Name    : Alarm
* Developed by :
* Version      : V 1.00
* Description  : i)

*****************************************************************************/
#include "stdint.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include <stdio.h>
#include "LPC55S16.h"
#include "Color.h"
#include <FR_CALC.h>
#include "Keypad.h"
#include "Dynamic_Pressure.h"
#include "Shapes.h"
#include "Alarm.h"
#include "Linear_Header.h"
#include "Motor.h"
#include  "sound.h"
#include "STEPPER.h"
#include "Driver_Timer.h"
#include "Bolus_Header.h"
#include <math.h>
#include "Power_off.h"
#include "Symbol.h"
#include "Events_log.h"
#include "TFT_String.h"
#include "infused_calculate.h"
#include "Main_screen.h"
#include "Alarm.h"

uint8_t standby_key=0;
uint8_t G_alarm_ok_flag=0;
volatile uint8_t G_warn_flag=0;//28-03
extern uint8_t warn_timer;
static uint8_t warn_toggle=0;
extern uint8_t G_infusion_modes;
extern uint8_t sy_size;
extern volatile uint8_t normal_run;
extern uint8_t warn_disp_count;
extern uint8_t warn_disp_flag;
extern uint8_t ON_Flag;
extern uint8_t end_flag;
extern uint16_t Pump_idle;
extern uint16_t press_run_alarm;
extern uint8_t G_add;
extern uint8_t G_Battery_error_flag;

extern uint8_t G_run_flag;
extern void main_screen_frame(uint16_t flowrate1,uint8_t mode);
extern void syringe_disp_status(uint8_t screen);

extern uint8_t G_KVO_run_flag;
extern uint16_t G_KVO_set_fr;
extern void shutting_down(void);
void warning_screen_erase();
void Alarm_Running(uint8_t L_R_flag);
void Alarm_war_screen(uint8_t state);
void Notif_screen(uint8_t *Notif_name);
extern uint8_t G_run_count;
void Notif_screen(uint8_t *Notif_name);
void Mains_symbol(uint8_t x_pos_adj,uint8_t y_pos_adj);
void alarm_string(uint8_t alarm_no);
uint8_t G_warn_count=0;
uint8_t G_syringe_size_flag=0;
extern uint8_t siphonic_time;
extern uint8_t G_fr_wrn_flag;
extern uint8_t home_flag;
extern uint8_t near_to_target_flag;
extern uint8_t Led_Alarm_flag;
extern uint8_t TFT_ST7789V_keylock4_symbol_16pt(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
extern uint8_t ramp_flag;
/****************************************************************************
* Function Name : void Alarm_screen(uint8_t *alarm_name)
* Arguments     : Name of the alarm
* Return type   : void
* Description   : i)   Display the alarm name
*                 ii)  Alarm instruction

*****************************************************************************/
void alarm_frameline(uint32_t bg_clr,uint32_t line_clr)
	{
	Rectangle(0,0,320,240,bg_clr);
		Rectangle(31,30,220+3,1,line_clr);
	 	Rectangle(31,30,1,150,line_clr);
	 	Rectangle(285,60,1,150,line_clr);
	 	Rectangle(61,210,230-5,1,line_clr);


	 	draw_line(31,180,63,211,line_clr);//
		draw_line(250+5,30,282+5-1,63-1,line_clr);
//	 	draw_line(248-2,38+2,280-2,69+2,COMB_RED);
//	 	draw_line(29,196,54,221,COMB_RED);//

	 	TFT_ST7789V_triangle2(212,192,0,bg_clr,line_clr);
		TFT_ST7789V_triangle2(212,263,0,bg_clr,line_clr);
		Rectangle(200,212,72,8,line_clr);

		TFT_ST7789V_triangle2(32,45,0,bg_clr,line_clr);
		TFT_ST7789V_triangle2(32,116,0,bg_clr,line_clr);
		Rectangle(53,32,72,8,line_clr);
}

void Alarm_screen(uint8_t *alarm_code,uint8_t *alarm_name)
{


 	alarm_frameline(FALU_RED,COMB_RED);

 	draw_line(248-2,38+2,280-2,69+2,COMB_RED);
 	draw_line(29,196,54,221,COMB_RED);//

 	Rectangle(54,221,133,1,COMB_RED);

 	TFT_ST7789V__rectangle_curve(100,80,65,122,50,WHITE);
 	TFT_ST7789V__rectangle_curve(101,81,63,120,50,FALU_RED);

	Font_Select(ARIAL_20);
	TFT_String_align(0,0,100,alarm_code,2,FALU_RED,WHITE);
	Font_Select(ARIAL_BOLD_14);
	TFT_String_align(0,0,160,alarm_name,2,FALU_RED,WHITE);

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void Alarm_screen_target(uint8_t *alarm_name,uint8_t *alarm_name_para)
{

	alarm_frameline(FALU_RED,COMB_RED);

	draw_line(248-2,38+2,280-2,69+2,COMB_RED);
	draw_line(29,196,54,221,COMB_RED);//
	Rectangle(54,221,133,1,COMB_RED);

	Font_Select( ARIAL_20);

	//TFT_String(85+5-25,87-5,alarm_name,FALU_RED,WHITE);
	TFT_String_align(0,0,82,alarm_name,2,FALU_RED,WHITE);
	// TFT_String(100+30+5,138,(uint8_t* )alarm_name_para,FALU_RED,0XFFCFCF);
	TFT_ST7789V__rectangle_curve(67,129,40,185,50,BEIGE);
	Font_Select( ARIAL_BOLD_14);
	 TFT_String_align(0,0,140,alarm_name_para,2,BEIGE,BOL_GREEN);
}

/****************************************************************************
* Function Name : void Error_screen(uint8_t *Error_name)
* Arguments     : Name of the alarm
* Return type   : void
* Description   : i)   Display the Error code
*                 ii)  Alarm instruction

*****************************************************************************/

void Error_screen(uint8_t *Error_code,uint8_t *Error_name)//28-03

{

	uint32_t bg_clr=0x182cd4 ;       //0xB44FFF     0x4B0082            0xcfa25b    0x182cd4    0x3433e0
	uint32_t LINE_clr=YELLOW ;      //BLACK        WHITE/0xD6F71B      BLACK       0xD6F71B    0xE710F7

	alarm_frameline(bg_clr,LINE_clr);

 	TFT_ST7789V__rectangle_curve(100,80,65,122,50,LINE_clr);
 	TFT_ST7789V__rectangle_curve(101,81,63,120,50,bg_clr);

	Font_Select(ARIAL_BOLD_14);

	TFT_String_align(0,0,105,Error_code,2,bg_clr,WHITE);
	TFT_String_align(0,0,160,Error_name,2,bg_clr,WHITE);
}

/****************************************************************************
* Function Name : void Error_screen(uint8_t *Error_name)
* Arguments     : Name of the alarm
* Return type   : void
* Description   : i)   Display the Error code
*                 ii)  Alarm instruction

*****************************************************************************/

void alarm_string(uint8_t alarm_no)
{
	uint8_t YPOS=88;
	uint8_t XPOS=105;
	switch(alarm_no)
		{
			case OCCL:
				//event_no_update(5);
				WARN_SOUND(4,1,2);
				Alarm_screen((uint8_t*)"AL 01",(uint8_t*)"Occlusion");
				break;

			case SYR_DIS:
				//event_no_update(5);
				WARN_SOUND(4,1,2);
				Alarm_screen((uint8_t*)"AL 02",(uint8_t*)"Syringe disturbed");
				break;


			case PLUNGER_DIS:
				//event_no_update(7);

				Alarm_screen((uint8_t*)"AL 03",(uint8_t*)"Plunger disturbed");
				break;

			case SYR_END:
				//event_no_update(10);

				Alarm_screen((uint8_t*)"AL 04",(uint8_t*)"Syringe empty");
				break;

			case TAR_REACH:
				//event_no_update(10);
				if(!G_KVO_run_flag)
				{
				WARN_SOUND(4,1,2);
				Alarm_screen((uint8_t*)"AL 05",(uint8_t*)"Target reached");
				}
				else
				{
				  Alarm_war_screen(0);
				}
				break;

			case TAR_VOL_REACH:
				//event_no_update(10);

				Alarm_screen((uint8_t*)"AL 06",(uint8_t*)"Target volume reached");
				break;


			case TAR_TIME_REACH:
				//event_no_update(10);
				Alarm_screen((uint8_t*)"AL 07",(uint8_t*)"Target time reached");
				break;

			case SIPHONIC:
			//	event_no_update(10);
				Alarm_screen((uint8_t*)"AL 08",(uint8_t*)"Siphonic alarm");

				break;

			case BAT_CRITICAL:
				Alarm_screen((uint8_t*)"AL 09",(uint8_t*)"Battery critical");
				break;

			case NOT_ENGAGE:
				//event_no_update(7);

				Alarm_screen((uint8_t*)"AL 10",(uint8_t*)"Syringe disengaged");
				break;
//			case BAT_DRAIN:
//				Alarm_screen((uint8_t*)"AL 10",(uint8_t*)"Battery drained");
//				break;

//			case FALL_DETECT:
//				Alarm_screen((uint8_t*)"AL 11",(uint8_t*)"Fall detection");
//				break;
//
//			case ORIENT_CHANGE:
//				Alarm_screen((uint8_t*)"ALS12",(uint8_t*)"Orientation changed");
//				break;

	          case MOT_ERR_1://28-03
	        //	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1001",(uint8_t*)"Motor unable to run"); //Motor unable to run
				break;

	          case MFB_ERR://28-03
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1002",(uint8_t*)"Motor function not detected"); //Motor unable to run
				break;

	          case LIN_POT_ERR://28-03
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1003",(uint8_t*)"SYR Linear pot error"); //Motor unable to run
				break;

	          case SD_POT_ERR://28-03
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1004",(uint8_t*)"SYR Linear pot error"); //Motor unable to run
				break;

			case MOT_ERR_2:
				//event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1005",(uint8_t*)"Motor reverse"); //Motor reverse
				break;

			case MOT_ERR_3:
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1006",(uint8_t*)"Less cuts"); //Less cuts
				break;

			case MOT_ERR_4:
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1007",(uint8_t*)"Excess cuts"); //Excess cuts
				break;

			case PSI0_ERR:
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1008",(uint8_t*)"Calibration error"); //DISC ERROR
				break;

			case PSI20_ERR:
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1009",(uint8_t*)"Calibration error"); //DISC ERROR
				break;

			case DISC_ERR:
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1010",(uint8_t*)"Disc Switch Fails"); //DISC ERROR
				break;

			case RLSW_ERR:
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1011",(uint8_t*)"Threaded clamp Fail"); //DISC ERROR
				break;

	          case LD_CELL_ERR_1://28-03
	        //	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1026",(uint8_t*)"Load cell failed"); //Motor unable to run
				break;

	          case LD_CELL_ERR_2://28-03
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1027",(uint8_t*)"Load cell failed"); //Motor unable to run
				break;

	          case LD_CELL_ERR_3://28-03
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1028",(uint8_t*)"Load cell failed"); //Motor unable to run
				break;

	          case BAT_REPLACE_ERR://28-03
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1029",(uint8_t*)"Battery failure"); //Motor unable to run
				break;

			case KEY_CONNECT_ERR:
				//event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1030",(uint8_t*)"Connector Detachment detected"); //Motor reverse
				break;

			case DISPLAY_CONNECT_ERR:
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1031",(uint8_t*)"Connector Detachment detected"); //Less cuts
				break;

			case USB_CONNECT_ERR:
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1032",(uint8_t*)"Connector Detachment detected"); //Excess cuts
				break;

			case LNPOT_CONNECT_ERR:
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1033",(uint8_t*)"Connector Detachment detected"); //DISC ERROR
				break;

			case CONNECT_120MM_ERR:
			//	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1034",(uint8_t*)"Connector Detachment detected"); //DISC ERROR
				break;

//			case MOV_ERR:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1035",(uint8_t*)"Connector Detachment detected"); //DISC ERROR
//				break;
//
//			case COLLAR_ERR:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1036",(uint8_t*)"Connector Detachment detected"); //DISC ERROR
//				break;
//
//			case PLUNGER_ERR:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1037",(uint8_t*)"Connector Detachment detected"); //DISC ERROR
//				break;

			case DRV_ERR:
			//event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1038",(uint8_t*)"Motor unable to run"); //drv
				break;





	          case EPROM_ERR://28-03
	        //	event_no_update(6);
				WARN_SOUND(4,1,2);
				Error_screen((uint8_t*)"Err.1039",(uint8_t*)"Memory overload"); //Motor unable to run
				break;

//	          case IO_EXP://28-03
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1040",(uint8_t*)"IO Expander failure"); //Motor unable to run
//				break;
//
//	          case UP_KEY_FAIL://28-03
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1041",(uint8_t*)"Key fault failure"); //Motor unable to run
//				break;
//
//	          case DOWN_KEY_FAIL://28-03
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1042",(uint8_t*)"Key fault failure"); //Motor unable to run
//				break;
//
//			case RIGHT_KEY_FAIL:
//				//event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1043",(uint8_t*)"Key fault failure"); //Motor reverse
//				break;
//
//			case LEFT_KEY_FAIL:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1044",(uint8_t*)"Key fault failure"); //Less cuts
//				break;
//
//			case OK_KEY_FAIL:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1045",(uint8_t*)"Key fault failure"); //Excess cuts
//				break;
//
//			case BOLUS_KEY_FAIL:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1046",(uint8_t*)"Key fault failure"); //DISC ERROR
//				break;
//
//			case START_STOP_FAIL:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1047",(uint8_t*)"Key fault failure"); //DISC ERROR
//				break;
//
//			case POWER_KEY_FAIL:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1048",(uint8_t*)"Key fault failure"); //DISC ERROR
//				break;
//
//			case MENU_KEY_FAIL:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1049",(uint8_t*)"Key fault failure"); //DISC ERROR
//				break;
//
//	          case MUTE_KEY_FAIL://28-03
//	        //	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1050",(uint8_t*)"Key fault failure"); //Motor unable to run
//				break;
//
//	          case BACK_KEY_FAIL://28-03
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1051",(uint8_t*)"Key fault failure"); //Motor unable to run
//				break;
//
//	          case ALARM_LED_FAIL_1://28-03
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1052",(uint8_t*)"Led fault failure"); //Motor unable to run
//				break;
//
//	          case ALARM_LED_FAIL_2://28-03
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1053",(uint8_t*)"Led fault failure"); //Motor unable to run
//				break;
//
//			case ALARM_LED_FAIL_3:
//				//event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1054",(uint8_t*)"Led fault failure"); //Motor reverse
//				break;
//
//			case LED_DRIVER_FAIL:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1055",(uint8_t*)"Keypad led failure"); //Less cuts
//				break;
//
//			case EFUSE_FAIL_1:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1058",(uint8_t*)"Efuse failure"); //Excess cuts
//				break;
//
//			case EFUSE_FAIL_2:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1059",(uint8_t*)"Efuse failure"); //DISC ERROR
//				break;
//
//			case EFUSE_FAIL_3:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1060",(uint8_t*)"Efuse failure"); //DISC ERROR
//				break;
//
//			case ACC_FAIL:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1061",(uint8_t*)"Accelerometer failed"); //DISC ERROR
//				break;
//
//			case ALS_FAIL:
//			//	event_no_update(6);
//				WARN_SOUND(4,1,2);
//				Error_screen((uint8_t*)"Err.1062",(uint8_t*)"ALS failed"); //DISC ERROR
//				break;


		}
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Alarm(uint8_t alarm_no)
{
	uint8_t alarm_loop=1;
	POW.power_status_flag=1;//26-03
	home_flag=0;
	uint8_t batt_cnt=0;
	Led_Alarm_flag=1;

	if(!G_KVO_run_flag)        // kvo RUNNING CONCEPT
	{
	 Timer_disable();
	}
	while(alarm_loop)
	{
		//alarm_sound_tone();
		//Alarm_LED(Led_Alarm_flag);
		Power_off();

			if(!ON_Flag)
			{
				break;//26-03
			}

			else if(POW.power_status_flag)
		    {
				POW.power_status_flag=0;//26-03
				alarm_string(alarm_no);
				while(KEY.value);
		    }
			if(alarm_no==60)
					{
						batt_cnt++;
						if(batt_cnt>5)
						{
							batt_cnt=0;
							delay_secs(2);
							//shutting_down();
//							Rectangle(0,0,320,240,BLACK);
//							SUPPLY_TRIGGER_OFF;
						}
					}


			 Font_Select(ARIAL_14);
		     TFT_Number(100,140,G_add,YELLOW,RED);


		if(G_KVO_run_flag)
		{
			Alarm_Running(0);

		}
//		if(!G_syringe_size_flag)
//			{
//				syringe_disp_status(0);
//		     	plunger_alarm_check();
//			}

		if((KEY.value==START_STOP_KEY)  || KEY.value==OK_KEY)
		{

			if(G_KVO_run_flag)
			{
				G_KVO_run_flag=0;
				 Timer_disable();
			}
			G_Battery_error_flag=0;
			G_alarm_ok_flag=1;
//			MFB_count_reset(0);
//			MFB_count_reset(1);
//			MFB_count_reset(2);
			alarm_loop=0;
			MFB.Motor_struck=0;
//			MOT.flowrate=1;
			LIN_POT.syr_end_flag=0;
			normal_run=0;
			POW.power_status_flag=1;
	//		siphonic_time=15;
			BOL.run_mode=0;//28-03

			RAMP.up_flag=0;
	//		press_run_alarm=180;
			Pump_idle=10;
//			Led_Alarm_flag=1;
			while(KEY.value);
			break;
		}
//		else if(KEY.value==MUTE_KEY)
//		{
//			WARN_SOUND(0,0,0);
//			while(KEY.value);
//			break;
//		}

	}

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Warning_screen(uint8_t *Warning_name, volatile uint8_t mode)
{
	volatile uint16_t YPOS=0;

	if(mode==0)
	{

		TFT_ST7789V__rectangle_curve(4,194,42,311,35,YELLOW);   //INFUSED DATA
		Rectangle(5,170,309,30,YELLOW);   //INFUSED PARAMETER

//
//			//TFT_ST7789V__rectangle_curve(86,92,62,229,0,WARNING_COLOR);
			Font_Select(ARIAL_20);
			YPOS=Letters_Alignments(170,190,Warning_name,0);
			TFT_String(YPOS,190,Warning_name,YELLOW,BLACK);
//			TFT_String_align(150,200,176,Warning_name_C1,0,YELLOW,BLACK);
//			TFT_String_align(150,210,208,Warning_name_C2,0,YELLOW,BLACK);
			warning_triangle_1(180,22,0,YELLOW,BLACK);
			warning_exclaimatory_1(193,48,0,YELLOW,BLACK);

			//TFT_ST7789V__rectangle_curve(4,194,42,311,35,LIGHT_GRAY_5);   //INFUSED DATA
			//Rectangle(5,170,309,30,LIGHT_TURQUI_1);   //INFUSED PARAMETER
//	TFT_ST7789V__rectangle_curve(86,92,62,229,0,WARNING_COLOR);
//	Font_Select(ARIAL_12);
//	YPOS=Letters_Alignments(86,315-30,Warning_name,0);
//	TFT_String(YPOS,97+2,Warning_name,WARNING_COLOR,BLACK);

//	TFT_ST7789V_Warning_symbol_18pt(183,40,0,YELLOW,BLACK);      //19-12
//	TFT_ST7789V_Exclamatory_9pt_arial(183+6,40+8,0,YELLOW,BLACK);//19-12
//	TFT_ST7789V_Triangle_24pt(97-3,275-5,0,WARNING_COLOR,WHITE);


//	YPOS=Letters_Alignments(86,315-15,(uint8_t*)"        Mute Alarm",0);
//	TFT_String(YPOS,131-2,(uint8_t*)"        Mute Alarm",WARNING_COLOR,BLACK);
//
//	TFT_ST7789V_lcd_drawcircle1(YPOS+15,138,14,WARNING_COLOR,BLACK);
//	TFT_ST7789V_lcd_drawcircle1(YPOS+15,138,12,WARNING_COLOR,LIGHT_GRAY);
//
//	TFT_ST7789V_Mute_Symbol_12pt(138-7,YPOS+15-8,0,LIGHT_GRAY,BLACK);
//	Rectangle(91,122,219,0,BLACK);
	G_warn_flag=1;//19-03

//	if(G_infusion_modes==2)//19-03
//	{
//		//dose_rate_para(1,3);
//		infused_data(1,G_infusion_modes);
//	}

	}
//	else if(mode==1)    //19-03
//	{
//		Frame_Footer(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
//		right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
//		Font_Select(ARIAL_BOLD_14);
//		TFT_String(195-30-5+10-7,202+3,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
//	}

}

void warning_screen_erase(uint32_t colour)
{
	TFT_ST7789V__rectangle_curve(4,194,42,311,35,colour);   //INFUSED DATA
		Rectangle(5,170,309,30,colour);   //INFUSED PARAMETER
}
void disp_warning_screen(uint8_t *Warning_msg,uint8_t mode)
{
	if(warn_disp_count==0)
	{
		if(warn_disp_flag==0)
		{
			Warning_screen((uint8_t*)Warning_msg,0);   //REW 1
			warn_disp_flag=1;
			warn_disp_count=8;
//			Font_Select(ARIAL_14);
//			TFT_Number(20,40,warn_disp_count,BLACK,WHITE);
		}
		else
		{
			Warning_Scr_Erase(mode);
			warn_disp_flag=0;
			warn_disp_count=4;
		}
	}
}
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void warn_tone(uint8_t warn_dura_sec)
{
	//G_warn_flag=1;
	if(G_warn_flag)
	{
		if(!G_warn_count)
		{
			WARN_SOUND(4,1,2);
			G_warn_count=warn_dura_sec;
		}

	}
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void pop_screen(uint8_t *Notif_name)
{
	TFT_ST7789V__rectangle_curve(36,92,62,250,0,YELLOW);
	Font_Select(ARIAL_14);
//	YPOS=Letters_Alignments(36,315+30,Notif_name,0);
	TFT_String(100,115-3,Notif_name,YELLOW,BLACK);
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Notif_screen(uint8_t *Notif_name)
{
	uint16_t YPOS=0;

	TFT_ST7789V__upside_rectangle_curve(4,4,35,205,55,ORANGE);
		Rectangle(199,5,10,33,ORANGE);

	//TFT_ST7789V__rectangle_curve(86,92,62,229,0,WARNING_COLOR);
	Font_Select(ARIAL_12);
//	YPOS=Letters_Alignments(74+80,183,Notif_name,0);
//	TFT_String(YPOS,183,Notif_name,WHITE,DARK_TEAL);
	TFT_String_align(165,35,12,Notif_name,0,ORANGE,BLACK);

////	TFT_ST7789V_lcd_drawcircle1(YPOS+15,138,14,WARNING_COLOR,BLACK);
////	TFT_ST7789V_lcd_drawcircle1(YPOS+15,138,12,WARNING_COLOR,LIGHT_GRAY);
//
////	TFT_ST7789V_Mute_Symbol_12pt(138-6,YPOS+15-8,0,LIGHT_GRAY,BLACK);
////	Rectangle(91,122,219,0,BLACK);
//
//
//}
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Warning_Scr_Erase(uint8_t mode)
{


	if((normal_run) && !G_warn_flag)     ////REW 1 NEED TO CHECK
	{
		G_warn_flag=1;
	}
	else if(normal_run)
	{
		G_warn_flag=0;
		bottom_para_frame(G_infusion_modes);

	}                                    //REW 1  NEED TO CHECK
	else if(!normal_run && G_warn_flag )
		{
			G_warn_flag=0;
			bottom_para_frame(G_infusion_modes);

		}

	if(G_warn_flag)
	{
		if(!warn_timer)
		{

			if(!warn_toggle)
			{
				warn_toggle=1;

			}
			else
			{
				warn_toggle=0;
				bottom_para_frame(mode);


				if(G_infusion_modes==2)
				{
					bottom_para_frame(G_infusion_modes);
					dose_rate_para(0,3);
				}
			}

			warn_timer=4;

		}
	}


}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Alarm_Running(uint8_t L_R_flag)
{
	uint16_t Y_pos[2][3]={{126,150,173},{126,150,173}};
	uint32_t colour[2][4][3]={
			{{FALU_RED,FALU_RED,WHITE},{FALU_RED,WHITE,WHITE},
									{WHITE,WHITE,WHITE},{FALU_RED,FALU_RED,FALU_RED}
				},
			{{FALU_RED,FALU_RED,FALU_RED},{WHITE,FALU_RED,FALU_RED},
								{WHITE,WHITE,FALU_RED},{WHITE,WHITE,WHITE}
	}

	};
	uint8_t pos;
	uint32_t col=FALU_RED;
	uint8_t X_pos[2]={175,175};
	static uint8_t state=0;
	 if(!G_run_count)
	 {
		for (pos = 0; pos <= 2; pos++)
		 {
			TFT_ST7789V_ALARM_ARROW_20pt(X_pos[L_R_flag],Y_pos[L_R_flag][pos],0,col,colour[L_R_flag][state][pos],L_R_flag);
		 }
		 if(state<3)
		 {
			 state++;
		 }
		 else
		 {
			 state=0;
		 }
		 G_run_count=5;

	 }

}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Alarm_war_screen(uint8_t state)
{
	uint16_t war_sym_pos[2]={260,210};
	Rectangle(0,0,320,240,ALARM_COLOR);
	TFT_ST7789V__rectangle_curve(10,40,120,300,0,WARNING_COLOR);
	TFT_ST7789V__rectangle_curve(15,80,40,290,0,LIGHT_GRAY);


	Rectangle(10,193+10,301,1,BLACK);  // Footer Boarder line

	TFT_ST7789V_Warning_symbol_18pt(51,war_sym_pos[state],0,WARNING_COLOR,BLACK);
    TFT_ST7789V_Exclamatory_9pt_arial(51+6,war_sym_pos[state]+9,0,WARNING_COLOR,BLACK);


    TFT_ST7789V_lcd_drawcircle1(60+35,141,14,WARNING_COLOR,BLACK);
	TFT_ST7789V_lcd_drawcircle1(60+35,141,12,WARNING_COLOR,LIGHT_GRAY);
	TFT_ST7789V_Mute_Symbol_12pt(141-7,60+35-8,0,LIGHT_GRAY,BLACK);

	Font_Select(ARIAL_14);
	TFT_String(90+30,130,(uint8_t*)"Mute Alarm ",WARNING_COLOR,BLACK);
	if(!state)
	{
//		TFT_String(70,50,(uint8_t*)"Target Reached",WARNING_COLOR,BLACK);
//		TFT_String(100,90,(uint8_t*)"KVO in progress",LIGHT_GRAY,BLACK);
//		Alarm_screen_target(uint8_t *alarm_name)
		Alarm_screen_target((uint8_t*)"Target Reached",(uint8_t*)"KVO");
	}

	else
	{
		Alarm_screen_target((uint8_t*)"Occlusion",(uint8_t*)"Anti Bolus");
	}

//	Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
//	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
//	Font_Select(ARIAL_BOLD_14);
//	TFT_String(195-30-5+10-7,202+3,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Notification(uint8_t notif)
{
//	beep_sound();
	WARN_SOUND(2,1,2);
	switch(notif)
	{
		case KEY_LOCK:
						Notif_screen((uint8_t *)"Keypad locked");
//						TFT_ST7789V_keylock4_symbol_16pt(178,44,0,WHITE,DARK_TEAL);
//						delay_secs(2);
//						Warning_Scr_Erase();
						break;
		case KEY_UNLOCK:
						Notif_screen((uint8_t *)"Keypad unlocked");
//						TFT_ST7789V_keyunlock4_symbol_16pt(178,35,0,WHITE,DARK_TEAL);
//						delay_secs(2);
//						Warning_Scr_Erase();
						break;
		case MAINS_PLUGIN:
		             	Notif_screen((uint8_t *)"Mains connected");
		             	Mains_symbol(44,186);
//		             	delay_secs(2);
//		             	Warning_Scr_Erase();
					    break;
		case MAINS_PLUGOUT:
						Notif_screen((uint8_t *)"Mains disconnected");
//						delay_secs(2);
//						Warning_Scr_Erase();
						break;

		case SET_FLOWRATE:
						Notif_screen((uint8_t *)"Set Flowrate");
					//	TFT_ST7789V_keylock4_symbol_16pt(110,100,0,WARNING_COLOR, BLACK);
//						TFT_ST7789V_Warning_symbol_18pt(178+6,44,0,WHITE,DARK_TEAL);
//						TFT_ST7789V_Exclamatory_9pt_arial(178+12,44+9,0,WHITE,DARK_TEAL);
//						delay_secs(2);
//						Warning_Scr_Erase();
						break;

	    case SET_FLOWRATE_1:
						//Notif_screen_1((uint8_t *)"Set Flowrate");
					//	TFT_ST7789V_keylock4_symbol_16pt(110,100,0,WARNING_COLOR, BLACK);
//						TFT_ST7789V_Warning_symbol_18pt(110,70,0,WARNING_COLOR,BLACK);
//						TFT_ST7789V_Exclamatory_9pt_arial(110+6,70+9,0,WARNING_COLOR,BLACK);
//						delay_secs(2);
//						Warning_Scr_Erase();
						break;
		case SET_TARGET:
						//Notif_screen_1((uint8_t *)"Set Target");
					//	TFT_ST7789V_keylock4_symbol_16pt(110,100,0,WARNING_COLOR, BLACK);
//						TFT_ST7789V_Warning_symbol_18pt(110,70,0,WARNING_COLOR,BLACK);
//						TFT_ST7789V_Exclamatory_9pt_arial(110+6,70+9,0,WARNING_COLOR,BLACK);
//						delay_secs(2);
//						Warning_Scr_Erase();
						break;

		case PUMP_IDLE:
						Notif_screen((uint8_t *)"Pump Idle");
						//TFT_ST7789V_keylock4_symbol_16pt(110,100,0,WARNING_COLOR, BLACK);
//						delay_secs(2);
//						Warning_Scr_Erase();
						break;

		case BATTERY_FULL_1:
						Notif_screen((uint8_t *)"Battery full");
						Mains_symbol(90,118);
						break;


	}
//	press_run_alarm=180;
	Pump_idle=10;
	delay_secs(2);
	notif_Scr_Erase();
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void Mains_symbol(uint8_t x_pos_adj,uint8_t y_pos_adj)
{
	TFT_ST7789V_lcd_drawcircle1(x_pos_adj+20,y_pos_adj+5,17,DARK_TEAL,DARK_TEAL);
	TFT_ST7789V_lcd_drawcircle1(x_pos_adj+20,y_pos_adj+5,14,WHITE,WHITE);

    TFT_ST7789V_circle1(x_pos_adj+20,y_pos_adj+3,9,DARK_TEAL,DARK_TEAL);   //left quadrant circle for mains symbol
    TFT_ST7789V_circle3(x_pos_adj+20,y_pos_adj+3,9,DARK_TEAL,DARK_TEAL);   //right quadrant circle for mains symbol

	Rectangle(x_pos_adj+10,y_pos_adj,20,2,DARK_TEAL);            //Bar type line
	Rectangle(x_pos_adj+19,y_pos_adj+10,3,7,DARK_TEAL);       //mains symbol line
	Rectangle(x_pos_adj+15,y_pos_adj-5,2,4,DARK_TEAL);      //left plugin line
	Rectangle(x_pos_adj+23,y_pos_adj-5,2,4,DARK_TEAL);    //right plugin line
	//TFT_ST7789V_thunder_8_pt(y_pos_adj+2,x_pos_adj+17,0,BLACK,WHITE);          //Thunder symbol
}

/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/

void caution_info(uint8_t *Warning_name)
{
	uint16_t xpos=0;
	TFT_ST7789V__rectangle_curve(40,80,82,236,0,WARNING_COLOR);
	Font_Select(ARIAL_BOLD_14);
//	xpos=Letters_Alignments(60,315-61,Warning_name,0);
//	TFT_String(xpos,97+2+15,Warning_name,WARNING_COLOR,BLACK);
	TFT_String_align(70,254,112,Warning_name,0,WARNING_COLOR,BLACK);
	TFT_ST7789V_Warning_symbol_18pt(86+12+12,46,0,WARNING_COLOR,BLACK);
	TFT_ST7789V_Exclamatory_9pt_arial(86+12+6+12,46+9,0,WARNING_COLOR,BLACK);
}


void notif_Scr_Erase()
{
	char* syr_size[8]={"N/A","2ml","5ml","10ml","20ml","30ml","50ml"};

		sy_size=detect_syringe_size(1);
	G_warn_flag=0;//28-03
	near_to_target_flag=0;

	TFT_ST7789V__upside_rectangle_curve(4,4,35,205,55,LIGHT_TURQUI);
	Rectangle(199,5,10,33,LIGHT_TURQUI);
//	syringe_disp_status(0);
	Font_Select(ARIAL_BOLD_14);

	if(G_infusion_modes!=2)
	{

		TFT_String_align(55,15,15,(uint8_t *)syr_size[sy_size],0,LIGHT_TURQUI,DARK_TEAL_1);
	}

}
void pop_up(uint8_t pop_up_mgs)
{
	WARN_SOUND(2,1,2);

		switch(pop_up_mgs)
		{
				case TIME_EXCEED:
				pop_screen((uint8_t *)"Invalid Time");
				break;

				case SET_RAMP :
				pop_screen((uint8_t *)"Invalid Ramp");
				break;

				case SET_RAMP_FR :
				pop_screen((uint8_t *)"> Max FR");
				break;

				case NO_BOLUS :
				pop_screen((uint8_t *)"No Bolus");
				break;

				case NO_SYR :
				pop_screen((uint8_t *)"No Syringe");
				break;

				case NO_DIS :
				pop_screen((uint8_t *)"Syr disengage");
				break;

				case TAR_REACH_1 :
				pop_screen((uint8_t *)"Target Reached");
				break;

				case BOL_SET :
				pop_screen((uint8_t *)"Set Bolus FR");
				break;


		}
		delay_secs(2);
		Warning_Scr_Erase_1();
}
void Warning_Scr_Erase_1()
{


	if((normal_run) && !G_warn_flag)     ////REW 1 NEED TO CHECK
	{
		G_warn_flag=1;
	}
	else if(normal_run)
	{
		G_warn_flag=0;
//		bottom_para_frame(0);

	}                                    //REW 1  NEED TO CHECK
	else if(!normal_run && G_warn_flag )
		{
			G_warn_flag=0;
//			bottom_para_frame(0);

		}

	if(G_warn_flag)
	{
		if(!warn_timer)
		{

			if(!warn_toggle)
			{
				warn_toggle=1;

			}
			else
			{
				warn_toggle=0;
//				bottom_para_frame(0);


				if(G_infusion_modes==2)
				{
					dose_rate_para(0,3);
				}
			}

			warn_timer=1;

		}
	}


}
