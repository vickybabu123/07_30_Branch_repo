/*
 * Alarm.h
 *
 *  Created on: 27-Nov-2023
 *      Author: Software3
 */

#ifndef ALARM_H_
#define ALARM_H_

#include "stdint.h"

#define OCCL       1
#define PLUNGER_DIS    2
#define SIPHONIC   3
#define NOT_ENGAGE 4
#define SYR_END    5
#define MOT_ERR_1  6
#define MOT_ERR_2  7
#define MOT_ERR_3  8
#define MOT_ERR_4  9
#define TAR_REACH   10
#define BATTERY_ALM  11
//
//#define COLLAR_ERR            12
//#define PLUNGER_ERR           13
#define MFB_ERR               14
//#define MOV_ERR               15
#define CONNECT_120MM_ERR     16
#define LNPOT_CONNECT_ERR     17
#define USB_CONNECT_ERR       18
#define DISPLAY_CONNECT_ERR   19

#define KEY_CONNECT_ERR       20
#define BAT_REPLACE_ERR       21
#define LD_CELL_ERR_2         22
#define LD_CELL_ERR_3         23
#define LD_CELL_ERR_1         24
#define RLSW_ERR              25
#define PSI20_ERR             26
#define PSI0_ERR              27

#define EPROM_ERR             28
//#define IO_EXP                29
//#define UP_KEY_FAIL           30
//#define DOWN_KEY_FAIL         31
//#define RIGHT_KEY_FAIL        32
//#define LEFT_KEY_FAIL         33
//#define OK_KEY_FAIL           34
//#define BOLUS_KEY_FAIL        35
//#define START_STOP_FAIL       36
//#define POWER_KEY_FAIL        37
//#define MENU_KEY_FAIL         38
//#define MUTE_KEY_FAIL         39
//#define BACK_KEY_FAIL         40

//#define ALARM_LED_FAIL_1      41
//#define ALARM_LED_FAIL_2      42
//#define ALARM_LED_FAIL_3      43
//#define LED_DRIVER_FAIL       44
//#define EFUSE_FAIL_1          45
//#define EFUSE_FAIL_2          46
//#define EFUSE_FAIL_3          47
//#define ACC_FAIL              48
//#define ALS_FAIL              49

#define LIN_POT_ERR           50
#define SD_POT_ERR            51

//#define BATTERY_FULL_1  12

//#define SIPHONIC 12
#define SYR_DIS               52
#define DISC_ERR              53
#define SYR_NOT_ENGAGE        54
#define PRESS_RUN             55
#define CALIBRATION_ERR       56

#define DRV_ERR               57
#define TAR_VOL_REACH         58
#define TAR_TIME_REACH        59
#define BAT_CRITICAL          60
//#define BAT_DRAIN         	  61
//#define FALL_DETECT           62
//#define ORIENT_CHANGE         63





/************Notification Macros**************************/

#define KEY_LOCK        1
#define KEY_UNLOCK      2
#define MAINS_PLUGIN    3
#define MAINS_PLUGOUT   4
#define SET_FLOWRATE    5
#define SET_FLOWRATE_1  6
#define SET_TARGET      7
#define PUMP_IDLE	    8
#define BOLUS_REACH  	9
#define BATTERY_FULL_1 10

////////////////pop up///////////////////

#define TIME_EXCEED		1
#define SET_RAMP_FR 	2
#define SET_RAMP		3
#define NO_BOLUS		4
#define NO_SYR   		5
#define NO_DIS 			6
#define TAR_REACH_1		7
#define BOL_SET         8

void Alarm_screen(uint8_t *alarm_code,uint8_t *alarm_name);
void Alarm(uint8_t alarm_no);
void Warning_screen(uint8_t *Warning_name, volatile uint8_t mode);
void Warning_Scr_Erase(uint8_t mode);
void caution_info(uint8_t *Warning_name);
void warn_tone(uint8_t warn_dura_sec);
void Notification(uint8_t notif);
void Notif_screen(uint8_t *Notif_name);
void pop_up(uint8_t pop_up_mgs);
void pop_screen(uint8_t *Notif_name);
void Warning_Scr_Erase_1();
#endif /* ALARM_H_ */
