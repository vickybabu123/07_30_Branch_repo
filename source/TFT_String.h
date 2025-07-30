#include "LPC55S16.h"
#include <stdio.h>

//typedef struct
//{
//	uint8_t  run_mode;
//	uint8_t  auto_purg_flag;
//	uint32_t flowrate;
//	uint16_t target_ml;
//	uint16_t infused_ml;
//	uint16_t temp_flowrate;
//	uint8_t  mode;
//	uint32_t bolus_limit;
//	uint32_t two_ml;
//	uint32_t five_ml;
//	uint32_t ten_ml;
//	uint32_t twent_ml;
//	uint32_t limit[7];
//	//uint16_t max_lmt[7];
//} bolus_flag;
//
////#define _RPM 50
//
//extern bolus_flag BOL;
uint16_t TFT_writenumber_float_1d(uint16_t xpos,uint16_t ypos,float number,uint32_t boarder_color,uint32_t fill_color);

uint16_t TFT_writenumber_float(uint16_t xpos,uint16_t ypos,float number,uint32_t boarder_color,uint32_t fill_color);
uint16_t TFT_Number(uint16_t xpos,uint16_t ypos,uint32_t number,uint32_t boarder_color,uint32_t fill_color);
uint16_t TFT_writechar(uint16_t xpos, uint16_t ypos,
				  	   uint8_t asciichar, uint32_t BG_COL, uint32_t TXT_COL);
uint16_t TFT_String(uint16_t xpos, uint16_t ypos,const uint8_t *asciistr,uint32_t BG_COL,uint32_t TXT_COL);
void Frame_Footer_new(uint32_t BG_color, uint32_t TXT_color,uint8_t *word);
struct get_char_info ascii_details(char numeric);
void Font_Select(uint8_t Font_Size);
void TFT_ST7789V__Bottom_curve(uint16_t xpos1, uint16_t ypos1, uint16_t width,
							  uint16_t height,uint16_t radius,uint32_t colour);
void Frame_Header(uint32_t BG_color, uint32_t TXT_color ,uint8_t *word);
void Frame_Footer(uint32_t BG_color, uint32_t TXT_color,uint8_t *word,uint8_t *string);
void Frame_Footer_duplicate(uint32_t BG_color, uint32_t TXT_color,uint8_t *word,uint8_t *circle_word,uint8_t circle_pos);
void Frame_Footer_NC(uint32_t BG_color, uint32_t TXT_color,uint8_t *word );
uint16_t Letters_Alignments(uint16_t X_Start,uint16_t X_End,uint8_t *asciichar,uint8_t Type);

uint8_t TFT_ST7789V_left_Right_arrow_14_arial(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint8_t TFT_ST7789V_up_down_arrow_20_arial(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint8_t TFT_ST7789V_Left_Arrow_35pt(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint8_t TFT_ST7789V_mains_symbol_25pt(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint8_t ZOOZ_LOGO(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint8_t TFT_ST7789V_power_10pt(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint16_t Right_Alignments(uint8_t *asciichar,uint16_t pos);
uint16_t Flow_rate_Alignments(uint8_t *asciichar);
uint8_t TFT_ST7789V_Triangle_24pt(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint32_t TFT_ST7789V_ALARM_ARROW_20pt(uint16_t xpos, uint16_t ypos,
                    uint8_t arrow, uint32_t boarder_color, uint32_t fill_color,uint8_t L_R_Flag);
uint8_t TFT_ST7789V_Warning_symbol_18pt(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color) ;
uint8_t TFT_ST7789V_Mute_Symbol_12pt(uint16_t xpos, uint16_t ypos,
                    uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)   ;
uint8_t TFT_ST7789V_Exclamatory_9pt_arial(uint16_t xpos, uint16_t ypos,
                    uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint16_t Bolus_rate_Alignments(uint8_t *asciichar);
uint8_t TFT_ST7789V_Left_Arrow_22pt(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint8_t TFT_ST7789V_star_14(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color) ;
uint8_t TFT_ST7789V_war_symbol(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)  ;
uint8_t TFT_ST7789V_thunder_8_pt(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)  ;
uint8_t TFT_ST7789V_tick_symbol_14pt(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);//22-03
uint8_t TFT_ST7789V_sercch_symbol_24pt(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);				//22-03
void TFT_Time_disp(uint16_t xpos,uint16_t ypos,
					uint32_t number,uint32_t boarder_color,uint32_t fill_color);
void TFT_screen_saver_Time_disp(uint16_t xpos,uint16_t ypos,
					uint32_t number,uint32_t boarder_color,uint32_t fill_color);
void TFT_Number_month(uint16_t xpos,uint16_t ypos,
					uint32_t number,uint32_t boarder_color,uint32_t fill_color);
uint8_t TFT_ST7789V_BOL_search(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint8_t TFT_ST7789V_keyunlock4_symbol_16pt(uint16_t xpos, uint16_t ypos,
					uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
void Frame_Footer_warning(uint32_t BG_color, uint32_t TXT_color,uint8_t *word,
					uint8_t *circle_word,uint8_t circle_pos);

uint8_t TFT_ST7789V_power_28pt_arial(uint16_t xpos, uint16_t ypos,
                                         uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint8_t TFT_ST7789V_Summation_symbol(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint8_t right_14(uint16_t xpos, uint16_t ypos,    //22-03
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint8_t TFT_ST7789V_standby_28pt_arial(uint16_t xpos, uint16_t ypos,
                                         uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
uint16_t TFT_writenumber_float_com(uint16_t xpos,uint16_t ypos,float number,uint32_t boarder_color,uint32_t fill_color);
uint8_t AKAS_LOGO(uint16_t xpos, uint16_t ypos,
											uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
