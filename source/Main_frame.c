#include <infused_calculate.h>

#include <Boot_up.h>
#include "LPC55S16.h"
#include "Color.h"
#include "Driver_SPI.h"
#include "TFT_Commands.h"
#include "Shapes.h"
#include "TFT_String.h"
#include "clock_config.h"
#include "Main_screen.h"
#include "Bolus_Header.h"
#include "Dosage_mode.h"
#include "Occlusion.h"
#include "Driver_ADC.h"
#include "Alarm.h"
#include "Power_off.h"
#include "Brand.h"
#include "Drug_List.h"
#include "Driver_Timer.h"
#include "math.h"
#include <FR_CALC.h>


#define FR_MODE_FRAME      0
#define VTBI_FRAME         1
#define DOSE_FRAME         2
#define RAMP_FRAME         3
#define BOL_FRAME          4

char local_buffer[10];
uint8_t dose_blink_flag=0;
uint8_t dose_blink_count=0;
extern uint16_t Brand_select_no;
extern uint8_t G_warn_flag;
extern uint8_t G_drug_unit_no;
//extern uint8_t xpos;
extern uint32_t MR_VALUE;
char* dose_para[5]={"FR","Conc.","Infu","P.Wt"};
uint32_t volume_ml=100;
extern uint8_t G_opt_no;
extern uint8_t G_page_no;
extern uint8_t drug_no;
extern uint8_t brand_select_no;
extern  char Brand_Names[6][4][15];
//uint8_t G_warning_flag;//28-03

extern char dose_rate_unit[3][13];
extern uint8_t dose_option_no;
//extern char* Drug_Names[150][4];
extern char Drug_Names[150][4][20];

extern uint8_t disc;
extern uint8_t temp_batt_level;

//extern char* disp_para[20];
extern void Warning_screen(uint8_t *Warning_name, volatile uint8_t mode);
extern uint8_t alarm_flag;
extern char* drug_unit[4][4];
void dose_rate_para(uint8_t start_cnt, uint8_t end_cnt);
extern void infused_data(uint8_t screen,uint8_t r_mode);   //28-03
//extern uint32_t pulse_per_ml;
void Battery_symbol(void);  //28-03
void FR_spliting_DD(uint16_t flowrate);
//void plug_symbol(void);
extern uint8_t G_occl_unit;
extern uint8_t home_flag;
extern uint8_t home_return_flag;
extern uint8_t G_Bolus_target_flag;
extern uint16_t G_Remaining_volume;
extern uint8_t G_Menu_Brand_flag;//17.09.24
extern uint16_t fav_Brand_list[11];
extern char disp_para[15];
extern uint8_t decimal_limit;
extern uint8_t sy_size;
extern uint8_t G_Menu_Brand_flag;//17.09.24
extern uint8_t G_occl_unit;
//uint8_t G_occl_unit1=0;

extern uint8_t G_occl_level;
extern uint8_t ramp_flag;
void ramp_shap(uint8_t mode);


void Frame_layout3(uint8_t start_pos, uint8_t end_pos, uint8_t *header_txt, uint8_t *footer_txt, uint8_t *nav_text, bool use_alternate_separation);
void draw_rem_frame();
extern float Ramp_FR[10];
extern uint8_t ramp_step;
extern void ramp_Fr_dispaly(float ramp_fr);

extern void lcd_drawline(uint16_t start_xpos, uint16_t start_ypos, uint16_t end_xpos, uint16_t end_ypos, uint32_t Color);

void main_screen_frame(uint16_t flowrate1,uint8_t mode)
{

//	uint16_t loop_count=0;
//	uint32_t ml=1;
	uint16_t xpos=0;
//	uint16_t ml=100;
	char* disp_pres[5]={"mmHg","PSI","kPa","mbar"};
	uint8_t presure_ratio[5]={51.7,1,6.8,6.8};

	FR_Lines(mode);
//	FR_spliting(flowrate1);

	ramp_shap(mode);

	if((mode==0 ) && mode!=2)
		{


		FR_spliting(flowrate1);


		Font_Select(ARIAL_BOLD_14);
		TFT_String(120+25+8+1+4,116,(uint8_t* )"ml/h",LIGHT_GRAY_5,DARK_TEAL);
		Font_Select(ARIAL_BOLD_14);
		TFT_String(54,176,(uint8_t* )"Volume",LIGHT_TURQUI_1,DARK_TEAL_1);
		TFT_ST7789V_Summation_symbol(176,34,0,LIGHT_TURQUI_1,DARK_TEAL_1);
		TFT_String(222,176,(uint8_t* )"Time",LIGHT_TURQUI_1,DARK_TEAL_1);
		TFT_ST7789V_Summation_symbol(176,203,0,LIGHT_TURQUI_1,DARK_TEAL_1);
		Battery_symbol();

//		syringe_symbol();
		infused_data(1,mode);//28-03
//		if(G_Menu_Brand_flag)
//		{
//			Font_Select(ARIAL_12);
//			xpos=Letters_Alignments(95,30,(uint8_t*)Brand_Names[brand_select_no/4][brand_select_no%4],0);
//			TFT_String(xpos+65,15,(uint8_t*)Brand_Names[brand_select_no/4][brand_select_no%4],LIGHT_TURQUI,BLACK);
//			TFT_Number(80,40,99,RED,YELLOW);
//
//		}
//		else//17.9.24
//		{
//			Font_Select(ARIAL_12);
//			xpos=Letters_Alignments(95,30,(uint8_t*)Brand_Names[Brand_select_no/4][Brand_select_no%4],0);
//				//	[fav_Brand_list[(Brand_select_no/4)]][fav_Brand_list[(Brand_select_no%4)]],0);
//			TFT_String(xpos+65,15,(uint8_t*)Brand_Names[Brand_select_no/4][Brand_select_no%4],LIGHT_TURQUI,BLACK);
//				//	[fav_Brand_list[(Brand_select_no/4)]][fav_Brand_list[(Brand_select_no%4)]],LIGHT_TURQUI,BLACK);
//
//			TFT_Number(80,40,40,RED,YELLOW);
//
//		}
		}

		else if((mode==1)||(mode==3))
		{
			if(mode==1)
			{
			FR_spliting(flowrate1);
			}
			else
			{
				if(ramp_flag==1)
				{
					ramp_Fr_dispaly(MOT.plateau_rate/10.0);

				}
				else
				{
					ramp_Fr_dispaly(Ramp_FR[ramp_step-1]);
				}

			}
			Font_Select(ARIAL_BOLD_14);
			TFT_String(120+25+8+1+4,116,(uint8_t* )"ml/h",LIGHT_GRAY_5,DARK_TEAL);
			Font_Select(ARIAL_BOLD_14);
			TFT_String(38+10,176,(uint8_t* )"VTBI",LIGHT_TURQUI_1,DARK_TEAL_1);
			//TFT_ST7789V_Summation_symbol(176,34,0,LIGHT_TURQUI_1,DARK_TEAL_1);
			TFT_String(222,176,(uint8_t* )"Time",LIGHT_TURQUI_1,DARK_TEAL_1);
			TFT_ST7789V_Summation_symbol(176,203,0,LIGHT_TURQUI_1,DARK_TEAL_1);
					//TFT_ST7789V_Summation_symbol(176,203,0,LIGHT_TURQUI_1,DARK_TEAL_1);
			//TFT_String(25+9+3+2+2-1,165-4+3-1+3,(uint8_t* )"Rem. vol",LIGHT_TURQUI_1,DARK_TEAL_1);
			//TFT_String(175+4+5-1+30,165-4+3-1+3,(uint8_t* )"Time",LIGHT_TURQUI_1,DARK_TEAL_1);

//			syringe_symbol();
			infused_data(1,1);//28-03
		}

		else if(mode==2)
		{

			FR_spliting_DD(dose.drug_dos);

			Font_Select(ARIAL_BOLD_14);
//			TFT_String(120+25+8+1+4,116,(uint8_t* )"ml/h",LIGHT_GRAY_5,DARK_TEAL);
			Font_Select(ARIAL_12);
			TFT_ST7789V_Summation_symbol(210,174,0,LIGHT_GRAY_5,DARK_TEAL_1);
			TFT_String(190,212,(uint8_t* )"t:",LIGHT_GRAY_5,DARK_TEAL_1);
			TFT_ST7789V_Summation_symbol(177,174,0,LIGHT_GRAY_5,DARK_TEAL_1);
			TFT_String(190,179,(uint8_t* )"d:",LIGHT_GRAY_5,DARK_TEAL_1);
			Font_Select(ARIAL_12);
//			xpos=Letters_Alignments(95,95+215,(uint8_t*)Drug_Names[(dose.drug_select_no/4)][(dose.drug_select_no%4)],0);
//
//			TFT_String(55,129,(uint8_t*)Drug_Names[(dose.drug_select_no/4)][(dose.drug_select_no%4)],DARK_TEAL,LIGHT_GRAY_5);
			TFT_String_align(110,120,15,(uint8_t*)Drug_Names[(dose.drug_select_no/4)][(dose.drug_select_no%4)],0,LIGHT_TURQUI,DARK_TEAL_1);

			Font_Select(ARIAL_12);
			TFT_String_align(107,120,116,(uint8_t*)dose_rate_unit[dose_option_no],0,LIGHT_GRAY_5,DARK_TEAL);

//			TFT_Time_disp(215,212,(time.hour*100)+time.minute,LIGHT_GRAY_5,DARK_TEAL_1);//28-03

//			Font_Select(ARIAL_12);
//			TFT_String(35,177,(uint8_t* )"Flowrate",LIGHT_GRAY_5,DARK_TEAL_1);
			//TFT_String(35,212,(uint8_t* )"1145.0 ml/hr",LIGHT_GRAY_5,DARK_TEAL_1);
//			delay_secs(2);
//			TFT_String(25,177,(uint8_t* )"Concentraion",LIGHT_GRAY_5,DARK_TEAL_1);
//			//TFT_String(25,212,(uint8_t* )"31.00 mmol/ml",LIGHT_GRAY_5,DARK_TEAL_1);
//			delay_secs(2);

			  G_warn_flag=0;
//			  dose_rate_para(0,3);



		}


	/***********************************dynamic pressure display***************************/
		Font_Select(ARIAL_9);
		if( mode>=0)
		{
		xpos=Letters_Alignments(231+5+7+5+8+3+2+180,122+8,(uint8_t*)disp_pres[G_occl_unit],0);
		TFT_String(xpos,122+8-7,(uint8_t *)disp_pres[G_occl_unit],LIGHT_GRAY_5,DARK_TEAL);

			//TFT_String_align(345,122,117,(uint8_t *)local_buffer,0,LIGHT_GRAY_5,LIGHT_GRAY_5);

//		TFT_String_align(441,130,122,(uint8_t*)disp_pres[G_occl_unit],0,LIGHT_GRAY_5,DARK_TEAL);
//		Font_Select(ARIAL_12);
//		TFT_String(xpos,DYN_VAL_POS,(uint8_t*)local_buffer,MAIN_FRAME_COL,MAIN_FRAME_COL);

			memset(local_buffer,0,6);
				if(G_occl_unit!=1)
				{
					sprintf((char *)local_buffer,"%02d",Occl.PSI_VALUE*presure_ratio[G_occl_unit]);
				}

				else
				{
					sprintf((char *)local_buffer,"%.1f",Occl.PSI_VALUE*presure_ratio[G_occl_unit]);
				}
				Font_Select(ARIAL_12);
				TFT_String_align(355,122,117,(uint8_t*)local_buffer,0,LIGHT_GRAY_5,DARK_TEAL);

//	           xpos=Letters_Alignments(240+5+100,122,(uint8_t*)local_buffer,0);
//		       TFT_String(xpos,122+3,(uint8_t*)local_buffer,LIGHT_GRAY_5,DARK_TEAL);
//		       TFT_String_align(355,122,117,(uint8_t*)local_buffer,0,LIGHT_GRAY_5,DARK_TEAL);
	//         dynamic_pressure(x_poss,y_poss,press,screen);

		}


//		else if(mode==2)
//		{
////			xpos=Letters_Alignments(485,122+5-40+3,(uint8_t*)disp_pres[G_occl_unit],0);
////			TFT_String(xpos,122+5-40+3,(uint8_t *)disp_pres[G_occl_unit],LIGHT_GRAY_5,DARK_TEAL);
//			TFT_String_align(485,90,122,(uint8_t *)disp_pres[G_occl_unit],0,LIGHT_GRAY_5,DARK_TEAL);
//	//		Font_Select(ARIAL_12);
//	//		TFT_String(xpos,DYN_VAL_POS,(uint8_t*)local_buffer,MAIN_FRAME_COL,MAIN_FRAME_COL);
//			memset(local_buffer,0,6);
//			if(G_occl_unit!=1)
//			{
//				sprintf((char *)local_buffer,"%02d",Occl.PSI_VALUE*presure_ratio[G_occl_unit]);
//			}
//
//			else
//			{
//				sprintf((char *)local_buffer,"%.1f",Occl.PSI_VALUE*presure_ratio[G_occl_unit]);
//			}
//			Font_Select(ARIAL_12);
////			xpos=Letters_Alignments(485,120+2-58+3,(uint8_t*)local_buffer,0);
////			TFT_String(xpos,120+2-58+3,(uint8_t*)local_buffer,LIGHT_GRAY_5,DARK_TEAL);
//			TFT_String_align(355,122,117,(uint8_t*)local_buffer,0,LIGHT_GRAY_5,DARK_TEAL);
//		}
//		if(!BATT.mode)
//		{
//			Battery_indicate();
			//plug_symbol(PLUG_COL);
//		}


}

void FR_spliting(uint16_t flowrate)
{

	char FR_buffer[6];
	uint16_t xpos=0;
	uint16_t flowrate_view = 0;

    Font_Select(BEBAS_60);

	flowrate_view = flowrate/10;//400

	if(flowrate_view == 0)
	{
	   xpos=164;
	}

	else
	{
     sprintf(FR_buffer,"%.d",flowrate_view);
     xpos=Flow_rate_Alignments((uint8_t*)FR_buffer);
    }

     //while(1);
//	if(decimal_limit==4)
//	{
//	  Font_Select(BEBAS_60);
//
//	  xpos=TFT_Number(xpos+25,40-30+5-2+5+30+5,flowrate_view,LIGHT_GRAY_5,DARK_TEAL);
//	}
//	else
	{
		Font_Select(BEBAS_60);

		xpos=TFT_Number(xpos+2-35,50,flowrate_view,LIGHT_GRAY_5,DARK_TEAL);
	}

//   	 if(decimal_limit<4)
	{
		 Font_Select(BEBAS_60);  //2-04
		 xpos=TFT_String(xpos+3,50,(uint8_t *)".",LIGHT_GRAY_5,DARK_TEAL);		//2-04

		 Font_Select(BEBAS_60);
		 TFT_Number(xpos+3,50,flowrate%10,LIGHT_GRAY_5,DARK_TEAL);
	}


  }


void FR_spliting_DD(uint16_t flowrate)
{

	char FR_buffer[6];
	char FR_buffer1[6];

	uint16_t xpos=0;
	uint16_t flowrate_view = 0;

	flowrate_view = flowrate/100;

//		if(flowrate_view == 0)
//		{
//		   xpos=100;
//		}

//		else
		{
	     sprintf(FR_buffer,"%.2d",flowrate_view);
	     xpos=Flow_rate_Alignments((uint8_t*)FR_buffer);
	    }


		{
			Font_Select(BEBAS_60);

			xpos=TFT_Number(xpos+2-100-15,50,flowrate_view,LIGHT_GRAY_5,DARK_TEAL);
		}

	//   	 if(decimal_limit<4)
		{
			 Font_Select(BEBAS_60);  //2-04
			 xpos=TFT_String(xpos+3,50,(uint8_t *)".",LIGHT_GRAY_5,DARK_TEAL);		//2-04

			 Font_Select(BEBAS_60);
			 TFT_Number(xpos+3,50,flowrate%100,LIGHT_GRAY_5,DARK_TEAL);
		}


//	flowrate_view = flowrate/100;
//
//	if(flowrate_view == 0)
//		{
//		   xpos=110;
//		}
//
//		else
//		{
//	     sprintf(FR_buffer1,"%02d",flowrate_view);
//	     xpos=Flow_rate_Alignments((uint8_t*)FR_buffer1);
//	    }
//
//	Font_Select(BEBAS_60);
//
//	sprintf(FR_buffer1,"%02d",flowrate%100);
//	TFT_String(xpos+2,50,(uint8_t *)FR_buffer1,LIGHT_GRAY_5,DARK_TEAL);
//
//	Font_Select(BEBAS_60);
//
//	sprintf(FR_buffer,"%d .",flowrate_view);
//	xpos=Right_Alignments((uint8_t*)FR_buffer,100);
//	TFT_String(xpos+3,50,(uint8_t *)FR_buffer,LIGHT_GRAY_5,DARK_TEAL);		//2-04


}

void FR_Lines(uint8_t mode)
{

	Rectangle(0,0,320,240,MAIN_FRAME_COL);


	if(mode==FR_MODE_FRAME || mode==VTBI_FRAME || mode==RAMP_FRAME)
	{


	TFT_ST7789V__upside_rectangle_curve(4,4,45+26,311,10,LIGHT_TURQUI);  //upside rec

	TFT_ST7789V__rectangle_curve(4,194,42,311,35,LIGHT_GRAY_5);   //INFUSED DATA
	Rectangle(5,170,309,30,LIGHT_TURQUI_1);   //INFUSED PARAMETER
	Rectangle(5,145,309,22,LIGHT_GRAY_5);  //REM VOL BOX
//	if(sy_size && disc==0)
	{
		draw_rem_frame();
	}
//	draw_rem_frame();

	Rectangle(22,200,120,0,GRAY);   //INFUSE PARA seperate line
	Rectangle(176,200,120,0,GRAY);  //INFUSE PARA seperate line

	Rectangle(5,40,309,102,LIGHT_GRAY_5);  //FR BOX



	Rectangle(200+10,2,1,140,MAIN_FRAME_COL);//fr and pressure bar seperation line

//	Font_Select(ARIAL_12);
//
//	TFT_String(251,148,(uint8_t* )"50.0 ",LIGHT_GRAY_5,DARK_TEAL);
//	Font_Select(ARIAL_9);
//	TFT_String(294,154,(uint8_t* )"ml",LIGHT_GRAY_5,DARK_TEAL);
	}
	else if(mode==DOSE_FRAME)
	{
		TFT_ST7789V__upside_rectangle_curve(4,4,45+26,311,10,LIGHT_TURQUI);  //upside rec

			TFT_ST7789V__rectangle_curve(4,194,42,311,35,LIGHT_GRAY_5);   //INFUSED DATA
			Rectangle(5,170,309,34,LIGHT_GRAY_5);   //INFUSED PARAMETER


			Rectangle(10,202,148,0,GRAY);   //INFUSE PARA seperate line
			Rectangle(169,202,140,0,GRAY);  //INFUSE PARA seperate line

			Rectangle(163,172,1,61,GRAY);
			Rectangle(5,40,309,102,LIGHT_GRAY_5);  //FR BOX



			Rectangle(200+10,2,1,140,MAIN_FRAME_COL);//fr and pressure bar seperation line
			Rectangle(5,145,309,22,LIGHT_GRAY_5);  //REM VOL BOX
//			if(sy_size && disc==0)
			{
				draw_rem_frame();
			}
			//rem_vol_frame(238,238-20,1);
			 //if((sy_size!=0))
			 {
				 dose_rate_para(0,3);
			 }



	}
	else if(mode==BOL_FRAME )
		{

		Rectangle(0,0,320,240,0xFF8321);
		TFT_ST7789V__upside_rectangle_curve(4,4,45+26,311,10,ORANGE);  //upside rec

		TFT_ST7789V__rectangle_curve(4,194,42,311,35,0xFFE7C4);   //INFUSED DATA
		Rectangle(5,170,309,30,ORPIMENT_YELLOW);   //INFUSED PARAMETER
		Rectangle(5,145,309,22,0xFFE7C4);  //REM VOL BOX
	//	if(sy_size && disc==0)
		{
			draw_rem_frame();
		}
	//	draw_rem_frame();

		Rectangle(22,200,120,0,GRAY);   //INFUSE PARA seperate line
		Rectangle(176,200,120,0,GRAY);  //INFUSE PARA seperate line

		Rectangle(5,40,309,102,0xFFE7C4);  //FR BOX   0xFFD1B2



		//Rectangle(200+10,2,1,140,ORANGE_DARK);//fr and pressure bar seperation line

	//	Font_Select(ARIAL_12);
	//
	//	TFT_String(251,148,(uint8_t* )"50.0 ",LIGHT_GRAY_5,DARK_TEAL);
	//	Font_Select(ARIAL_9);
	//	TFT_String(294,154,(uint8_t* )"ml",LIGHT_GRAY_5,DARK_TEAL);
		}


}


void scroll_bar(uint8_t page_no,uint8_t option_no,uint8_t total_option)     //23-03                                  //(uint8_t prev_option, uint8_t curr_option, uint8_t page)
{
	uint8_t total_scroll=143,updated_option=0;
	float y_pos_bar;
	uint8_t height_of_bar=(total_scroll/total_option);


	if(height_of_bar<5)
	{
		height_of_bar=5;
		total_scroll=(total_scroll-height_of_bar);

	}

	        y_pos_bar=((float)total_scroll/total_option);
		    updated_option=(page_no*4)+option_no;
			Rectangle(302,50,10,143,LIGHT_GRAY);
			Rectangle(304,(50+(int)((y_pos_bar)*updated_option)),6,height_of_bar,BLACK);
}

void dose_rate_para(uint8_t start_cnt, uint8_t end_cnt)
{

	uint8_t loop_count = 0;
	uint16_t xpos=0;
	char local_buffer[10];

//	for(loop_count=start_cnt;loop_count<=end_cnt;loop_count++)
//	{
//		Font_Select(ARIAL_12);
//		TFT_String(95,(135+(25*loop_count)),(uint8_t*)dose_para[loop_count-start_cnt],MAIN_FRAME_COL,WHITE);


		//if((start_cnt+loop_count)<=4)
		{
		Font_Select(ARIAL_12);
				TFT_ST7789V_Summation_symbol(210,174,0,LIGHT_GRAY_5,DARK_TEAL_1);
				TFT_String(190,212,(uint8_t* )"t:",LIGHT_GRAY_5,DARK_TEAL_1);
				TFT_ST7789V_Summation_symbol(177,174,0,LIGHT_GRAY_5,DARK_TEAL_1);
				TFT_String(190,179,(uint8_t* )"d:",LIGHT_GRAY_5,DARK_TEAL_1);

				if(!dose_blink_count)
				{
					if(dose_blink_flag==0)
					{
						//if(loop_count==start_cnt)
						{
							Font_Select(ARIAL_12);
							TFT_String_align(70,92,177,(uint8_t*)"Concentration",0,LIGHT_GRAY_5,LIGHT_GRAY_5);  //remove
							TFT_String_align(70,95,177,(uint8_t*)"Flowrate",0,LIGHT_GRAY_5,DARK_TEAL_1);

							TFT_ST7789V__rectangle_curve(4,204,32,152,10,LIGHT_GRAY_5);
							Font_Select(ARIAL_12);
							sprintf(local_buffer,"%.1f",((float)dose.flow_rate/100.0));   	 //store the integer variables in to string using sprintf
							xpos=Right_Alignments((uint8_t*)local_buffer,80);   				 //(uint8_t*)letter
							TFT_writenumber_float(xpos,212,dose.flow_rate,LIGHT_GRAY_5,DARK_TEAL_1);


	//						TFT_ST7789V__rectangle_curve(4,204,32,152,10,LIGHT_GRAY_5);

							xpos=Right_Alignments((uint8_t*)"ml/h",140);    				//(uint8_t*)letter
							TFT_String(xpos,212,(uint8_t*)"ml/h",LIGHT_GRAY_5,DARK_TEAL_1);

						}
	//					start_cnt=1;
						dose_blink_flag=1;
						dose_blink_count=10;

						}
					else
					{
	//
							Font_Select(ARIAL_12);
							TFT_String_align(70,95,177,(uint8_t*)"Flowrate",0,LIGHT_GRAY_5,LIGHT_GRAY_5);
							TFT_String_align(70,92,177,(uint8_t*)"Concentration",0,LIGHT_GRAY_5,DARK_TEAL_1);

							Font_Select(ARIAL_12);
							sprintf(local_buffer,"%.1f",((float)dose.flow_rate/100.0));   	 //store the integer variables in to string using sprintf
							xpos=Right_Alignments((uint8_t*)local_buffer,80);   				 //(uint8_t*)letter
							TFT_writenumber_float(xpos,212,dose.flow_rate,LIGHT_GRAY_5,LIGHT_GRAY_5);
	//						TFT_ST7789V__rectangle_curve(4,204,32,152,10,LIGHT_GRAY_5);
							Font_Select(ARIAL_12);
							sprintf(local_buffer,"%.1f",((float)dose.conc_value/10.0));    	//store the integer variables in to string using sprintf
							xpos=Right_Alignments((uint8_t*)local_buffer,80);    				//(uint8_t*)letter
							TFT_writenumber_float(xpos,211,dose.conc_value,LIGHT_GRAY_5,DARK_TEAL_1);
							Font_Select(ARIAL_12);
							xpos=Right_Alignments((uint8_t*)disp_para,155);    				//(uint8_t*)letter
		//					TFT_String_align(265,310,184,(uint8_t*)disp_para,0,LIGHT_GRAY_5,DARK_TEAL_1);
							TFT_String(xpos,211,(uint8_t*)disp_para,LIGHT_GRAY_5,DARK_TEAL_1);

							//TFT_writenumber_float(xpos,211,45.5,LIGHT_GRAY_5,DARK_TEAL_1);

						dose_blink_flag=0;
						dose_blink_count=10;
					}

				}



				// if(loop_count==start_cnt+2)
					{
					Font_Select(ARIAL_12);
					sprintf(local_buffer,"%.1f",((float)dose.drug_dos/10.0));    	//store the integer variables in to string using sprintf
					xpos=Right_Alignments((uint8_t*)local_buffer,244);    				//(uint8_t*)letter
					TFT_writenumber_float(xpos+27-15,179,(float)dose.drug_dos,LIGHT_GRAY_5,DARK_TEAL_1);
					Font_Select(ARIAL_12);
					xpos=Right_Alignments((uint8_t*)drug_unit[G_page_no][G_opt_no],308);    				//(uint8_t*)letter

//					TFT_String_align(265,310,184,(uint8_t*)drug_unit[G_page_no][G_opt_no],0,LIGHT_GRAY_5,DARK_TEAL_1);
					TFT_String(xpos,179,(uint8_t*)drug_unit[G_page_no][G_opt_no],LIGHT_GRAY_5,DARK_TEAL_1);
//					TFT_String_align(265,310,184,(uint8_t*)"mmol",0,LIGHT_GRAY_5,DARK_TEAL_1);

					}
//				 if(loop_count==start_cnt+3)
//					{
////						Font_Select(ARIAL_BOLD_14);
////						TFT_String(26-8+4+70,194+3+2-3+6+2,(uint8_t* )"Kg",LIGHT_GRAY_5,DARK_TEAL);
////
////						sprintf(local_buffer,"%.1f",((float)dose.pat_weight/10.0));   	 //store the integer variables in to string using sprintf
////						xpos=Right_Alignments((uint8_t*)local_buffer,230);   				 //(uint8_t*)letter
////						TFT_writenumber_float(26-8+4,194+3+2-3+6+2,dose.pat_weight,LIGHT_GRAY_5,DARK_TEAL);  //28-03
//					}
//			}

	}
	Font_Select(ARIAL_12);
	TFT_Time_disp(215,212,(time.hour*100)+time.minute,LIGHT_GRAY_5,DARK_TEAL_1);

	TFT_String(255+25,211,(uint8_t* )"hrs",LIGHT_GRAY_5,DARK_TEAL_1);//28-03


}


void bottom_para_frame(uint8_t mode)
{

	if(mode==0 )
	{


			TFT_ST7789V__rectangle_curve(4,194,42,311,35,LIGHT_GRAY_5);   //INFUSED DATA
				Rectangle(5,170,309,30,LIGHT_TURQUI_1);   //INFUSED PARAMETER
		//
		//		rem_vol_frame(238,238,0);
		//
				Rectangle(22,200,120,0,GRAY);   //INFUSE PARA seperate line
				Rectangle(176,200,120,0,GRAY);  //INFUSE PARA seperate line

				Font_Select(ARIAL_BOLD_14);
				TFT_String(54,176,(uint8_t* )"Volume",LIGHT_TURQUI_1,DARK_TEAL_1);
				TFT_ST7789V_Summation_symbol(176,34,0,LIGHT_TURQUI_1,DARK_TEAL_1);
				TFT_String(222,176,(uint8_t* )"Time",LIGHT_TURQUI_1,DARK_TEAL_1);
				TFT_ST7789V_Summation_symbol(176,203,0,LIGHT_TURQUI_1,DARK_TEAL_1);

		infused_data(1,mode);
	}
	else if(mode==1|| mode==3 )
	{


		TFT_ST7789V__rectangle_curve(4,194,42,311,35,LIGHT_GRAY_5);   //INFUSED DATA
		Rectangle(5,170,309,30,LIGHT_TURQUI_1);   //INFUSED PARAMETER
//
//		rem_vol_frame(238,238,0);
//
		Rectangle(22,200,120,0,GRAY);   //INFUSE PARA seperate line
		Rectangle(176,200,120,0,GRAY);  //INFUSE PARA seperate line

		Font_Select(ARIAL_BOLD_14);
		TFT_String(38+10,176,(uint8_t* )"VTBI",LIGHT_TURQUI_1,DARK_TEAL_1);
		//TFT_ST7789V_Summation_symbol(176,34,0,LIGHT_TURQUI_1,DARK_TEAL_1);
		TFT_String(222,176,(uint8_t* )"Time",LIGHT_TURQUI_1,DARK_TEAL_1);
		TFT_ST7789V_Summation_symbol(176,203,0,LIGHT_TURQUI_1,DARK_TEAL_1);

		infused_data(1,1);//28-03
	}
	if(mode==2)
		{


		TFT_ST7789V__rectangle_curve(4,194,42,311,35,LIGHT_GRAY_5);   //INFUSED DATA
					Rectangle(5,170,309,34,LIGHT_GRAY_5);   //INFUSED PARAMETER

					Rectangle(163,172,1,61,GRAY);
					Rectangle(10,202,148,0,GRAY);   //INFUSE PARA seperate line
					Rectangle(169,202,140,0,GRAY);  //INFUSE PARA seperate line

					dose_rate_para(0,3);
		//	Rectangle(0,0,320,240,LIGHT_TURQUI);

//			TFT_ST7789V__upside_rectangle_curve(4,4,115,311,50,LIGHT_GRAY_5);  //upside rec
//
//
//			TFT_ST7789V__rectangle_curve(4,190,42,311,50,LIGHT_GRAY_5);   //bottom rec
//				Rectangle(5,155-2+3-1,309,40,LIGHT_GRAY_5);   //elaptime box
//
//			Rectangle(166,155,1,76,LIGHT_TURQUI);// down seperation line
//
//			Rectangle(200+10,2,1,120+28,LIGHT_TURQUI);
//
//			Rectangle(5,122,309,29,DARK_TEAL);  // drug box
//
//			Rectangle(15,193,141,0,GRAY);   //sepration single line right
//				Rectangle(174,193,132,0,GRAY);  //sepration single line left

}
}
void Frame_layout(uint8_t start_pos,uint8_t end_pos,uint8_t *header_txt,uint8_t *footer_txt,uint8_t *nav_text)
{


	CLRSCR(MAIN_FRAME_COL);


	Frame_Header(NEW_HDR_COL,HDR_TXT_COL,header_txt);

	Frame_Footer(NEW_FTR_COL,WHITE,footer_txt,nav_text);

	CENTER_SCR(MAIN_FRAME_COL); 			//center green
	border_lines();
	separation_lines(start_pos,end_pos);

	right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);

}


void Frame_layout3(uint8_t start_pos, uint8_t end_pos, uint8_t *header_txt, uint8_t *footer_txt, uint8_t *nav_text, bool use_alternate_separation)
{
    CLRSCR(MAIN_FRAME_COL);

    Frame_Header(NEW_HDR_COL, HDR_TXT_COL, header_txt);
    Frame_Footer(NEW_FTR_COL, WHITE, footer_txt, nav_text);

    CENTER_SCR(MAIN_FRAME_COL); // center green
    border_lines();

    // Use appropriate separation lines function based on flag
    if (use_alternate_separation)
    {
        separation_lines2(start_pos, end_pos);
    }
    else
    {
        separation_lines(start_pos, end_pos);
    }

    right_14(207, 100 - 40 - 5 - 3, 0, ORANGE_DARK, WHITE);
}


void border_lines()
{
	Rectangle(4,43,311,0,LIGHT_GRAY);       //TOP LINE
	Rectangle(315,44,0,151,LIGHT_GRAY);     //RIGHT LINE
	Rectangle(3,196,311,0,LIGHT_GRAY);		//BOTTOM LINE
	Rectangle(3,44,0,151,LIGHT_GRAY);		//LEFT LINE
}

void separation_lines(uint8_t start_pos,uint8_t end_pos)
{
	uint8_t loop_count = 0;
	for(loop_count = start_pos; loop_count <=end_pos ; loop_count++)				//no of lines
		{
			Rectangle(8,44+(38*loop_count),289,0,LIGHT_GRAY);//LINE
		}
}

void separation_lines2(uint8_t start_pos,uint8_t end_pos)
{
	uint8_t loop_count = 0;
	for(loop_count = start_pos; loop_count <=end_pos ; loop_count++)				//no of lines
		{
			Rectangle(8,44+(38*loop_count),300,0,LIGHT_GRAY);//LINE
		}
}
void border_lines_small()
{
	Rectangle(4,43+20,311,0,LIGHT_GRAY);       //TOP LINE
			Rectangle(315,44+20,0,151,LIGHT_GRAY);     //RIGHT LINE
			Rectangle(3,196+20,311,0,LIGHT_GRAY);		//BOTTOM LINE
			Rectangle(3,44+20,0,151,LIGHT_GRAY);		//LEFT LINE
}

//void rem_vol_frame(uint8_t pos,uint8_t end_pos,uint8_t state)
//{
//
//	uint8_t mov=0;
//	uint8_t start_pos=238;
//	//uint8_t end_pos=14;
//	uint8_t circle2_start_pos=15;
//
//
//
//	if(state==0)
//	{
//		Rectangle(5,145,309,22,LIGHT_GRAY_5);  //REM VOL BOX
//
//		Rectangle(15,149,222,14,BLACK);
//		TFT_ST7789V_circle(14,156,8,BLACK,BLACK);
//		TFT_ST7789V_circle1(14,156,8,BLACK,BLACK);
//		TFT_ST7789V_circle2(238,156,8,BLACK,BLACK);
//		TFT_ST7789V_circle3(238,156,8,BLACK,BLACK);
//
//		Rectangle(15,151,222,10,0xD9D9D9);
//		TFT_ST7789V_circle((14),156,6,0xD9D9D9,0xD9D9D9 ); //0X00BAD1,0X00BAD1
//		TFT_ST7789V_circle1((14),156,6,0xD9D9D9,0xD9D9D9 );
//		TFT_ST7789V_circle2(238,156,6,0xD9D9D9,0xD9D9D9 );
//		TFT_ST7789V_circle3(238,156,6,0xD9D9D9,0xD9D9D9 );
//
//		Rectangle(15,151,222,10,0xD9D9D9);
//		TFT_ST7789V_circle((14),156,6,0xD9D9D9,0xD9D9D9 );
//		TFT_ST7789V_circle1((14),156,6,0xD9D9D9,0xD9D9D9 );
//		TFT_ST7789V_circle2(238,156,6,0xD9D9D9,0xD9D9D9 );
//		TFT_ST7789V_circle3(238,156,6,0xD9D9D9,0xD9D9D9 );
//
////		Rectangle(pos,151,start_pos-pos,10,0xD9D9D9);
////		delay_secs(1);
//	}
//
//	if(state==1)
//	{
//		//Rectangle(15,151,222,10,0xD9D9D9);
////		TFT_ST7789V_circle((pos),156,6,0xD9D9D9,0xD9D9D9);
////		TFT_ST7789V_circle1((pos),156,6,0xD9D9D9,0xD9D9D9);
////		Rectangle(pos,151,start_pos-pos,10,0xD9D9D9);
//		TFT_ST7789V_circle2(start_pos,156,6,0xD9D9D9,0xD9D9D9);
//		TFT_ST7789V_circle3(start_pos,156,6,0xD9D9D9,0xD9D9D9);
//		//if(pos>=start_pos)
//		{
//
//		}
////		Rectangle(start_pos,151,start_pos-end_pos,10,0xD9D9D9);
//		for( mov=pos-1;mov>end_pos;mov--)
//			{
//
//			TFT_ST7789V_circle((mov),156,6,0xD9D9D9,0xD9D9D9);
//			TFT_ST7789V_circle1((mov),156,6,0xD9D9D9,0xD9D9D9);
//
////			delay_ms(50);
//			}
//	}
//
////SKY_BLUE,AQUAMARINE,0X00BAD1,CHLORIDE,TURQUOISE,OEM_BLUE,
//}

void semi_circle(uint8_t R_L_flag,uint8_t xpos,uint8_t y_pos,uint8_t radius,uint32_t bg_clr,uint32_t in_clr)
{

	if(R_L_flag==0)
	{
		TFT_ST7789V_circle(xpos,y_pos,radius,bg_clr,in_clr);
		TFT_ST7789V_circle1(xpos,y_pos,radius,bg_clr,in_clr);
	}
	else
	{
		TFT_ST7789V_circle2(xpos,y_pos,radius,bg_clr,in_clr);
		TFT_ST7789V_circle3(xpos,y_pos,radius,bg_clr,in_clr);
	}

}

void draw_rem_frame()
{
	//if(sy_size!=5)
	{
		Rectangle(15,149,222,14,BLACK);
			semi_circle(0,14,156,8,BLACK,BLACK);
			semi_circle(1,238,156,8,BLACK,BLACK);

			Rectangle(15,151,222,10,ROYAL_BLUE);      //BLUE
			semi_circle(0,14,156,6,ROYAL_BLUE,ROYAL_BLUE);
			semi_circle(1,238,156,6,ROYAL_BLUE,ROYAL_BLUE);
	}


}



void speedometer3(uint16_t pressure,uint8_t screen)
{
    uint16_t xpos = 260;   // Center of the speedometer
    uint16_t ypos = 101;
    uint8_t radius = 20;

    uint16_t valueee = 0;
    float theta;
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    uint32_t color = 0;
    uint32_t degree_value = 0;
    uint16_t angle = 0;
    uint16_t current_angle = 0;

    static uint16_t prev_angle = 180;

    const uint16_t unit_value[4][10]={{1000,2000,3000,4000,5000,6000,7000,8000,9000,10000},
    							   {20,40,60,80,100,120,140,160,180,200},
    							   {133,266,399,533,666,799,933,1066,1199,1322},
    							   {13,26,39,50,60,70,90,100,110,130}
    							  };

    if(screen)
	{
		prev_angle = 180;
	}
    Font_Select(ARIAL_BOLD_14);
//    valueee = Pressure_ADC();
//    if(valueee>=Occl.strain_20PSI_value[sy_size])
//    {
//    	valueee=Occl.strain_20PSI_value[sy_size];
//    }
//    else
//    {
//
//   degree_value = (180 * (valueee - Occl.Zero_PSI) / (Occl.strain_20PSI_value[sy_size] - Occl.Zero_PSI));
//
//    }

    degree_value = (pressure*180/unit_value[G_occl_unit][G_occl_level]);

    if(degree_value>=180)
    {

    	degree_value=1;
        current_angle =  degree_value;
    }
    else
    {

    current_angle =180- degree_value;
    if(!degree_value)
    	{
    		degree_value=1;
    	}
    }
   // current_angle = 180 - degree_value;

//
//    TFT_Number(50, 50, Occl.Zero_PSI, YELLOW, BLACK);
//    TFT_Number(50, 100, valueee, YELLOW, BLACK);
//    TFT_Number(150, 50, current_angle, YELLOW, BLACK);

    if (degree_value >= 1)
    {

        if (current_angle < prev_angle)
        {



            for (angle = prev_angle; angle > current_angle; angle--)
            {
                theta = angle * (3.14159 / 180);


                if (angle > 170)
                 {
                     red = 179;
                     green = 233;
                     blue = 2;
                 }

                 else if (angle > 60)
                 {

                     red = 179 + (255 - 179) * ((170 - angle) / 110.0);

                     green = 233 - (233 - 206) * ((170 - angle) / 110.0);

                     blue = 2;
                 }

                 else
                 {

                     red = 253;

                     green = 206 - (206 - 97) * ((60 - angle) / 59.0);

                     blue = 4;
                 }



                color = ((red << 16) | (green << 8) | (blue << 0));

            //    color = ((red << 16) | (green << 8) | (blue << 0));





                for (int thickness = 0; thickness < 8; thickness++)
                {
                    lcd_drawline(
                        xpos + cos(theta) * (radius + thickness),
                        ypos - sin(theta) * (radius + thickness),
                        xpos + cos(theta) * (radius + thickness + 5),
                        ypos - sin(theta) * (radius + thickness + 5),
						color


                    );

                }
            }
        }

        else if (current_angle > prev_angle)
        {
            for (angle = prev_angle; angle < current_angle; angle++)
            {
                theta = angle * (3.14159 / 180);


                for (int thickness = 0; thickness < 8; thickness++)
                {
                    lcd_drawline(
                        xpos + cos(theta) * (radius + thickness),
                        ypos - sin(theta) * (radius + thickness),
                        xpos + cos(theta) * (radius + thickness + 5),
                        ypos - sin(theta) * (radius + thickness + 5),
						THIN_ICE
                    );

                }
            }
        }


        prev_angle = current_angle;
    }


//    TFT_ST7789V_lcd_drawcirclehalf(xpos - 1, ypos, radius, LIGHT_GRAY_5, LIGHT_GRAY_5);

delay_ms(5);
}




void speedometer_shape(void)
{
    uint16_t xpos = 260;
    uint16_t ypos = 101;
    uint8_t radius = 20;
    float theta;

        for (uint16_t angle = 180; angle >= 1; angle--)
        {
            theta = angle * (3.14159 / 180);

            for (uint16_t thickness = 0; thickness < 8; thickness++)
            {
                lcd_drawline(
                    xpos + cos(theta) * (radius + thickness),
                    ypos - sin(theta) * (radius + thickness),
                    xpos + cos(theta) * (radius + thickness + 5),
                    ypos - sin(theta) * (radius + thickness + 5),
					THIN_ICE
                );
            }

        }
    }
void ramp_shap(uint8_t mode)
{
	uint8_t loop_count=0;


///////////////Ramp up//////////////////////////////////////////
	if(mode==3 )
	{
		if(MOT.ramp_up)
		{
			if(!ramp_flag)
			{

				Rectangle(8,120-3,25+15+3+5,25-2,LIGHT_TURQUI);  //box
				Rectangle(8,135,10+5-5,2,BLACK); //side line

				for(loop_count=0;loop_count<3;loop_count++)
				{
					//lcd_drawline(260+loop_count,100,290+loop_count,150,BLACK);

					lcd_drawline(22-5+loop_count,136+1,40-5-5+loop_count,95+5+5+10+5,BLACK);
				}
				Rectangle(34-4,115+5,10+5+5-3,2,BLACK); //top line
			}
		}
		else
		{
			ramp_flag=1;
		}
///////////////plateau//////////////////////////////////////////
		if(ramp_flag==1)
		{
			Rectangle(8,120-3,25+15+3+5+2,25-2,LIGHT_TURQUI);  //box
			//Rectangle(8,135,10+5-5,2,BLACK); //side line

			for(loop_count=0;loop_count<3;loop_count++)
			{
				//lcd_drawline(260+loop_count,100,290+loop_count,150,BLACK);

				lcd_drawline(18+loop_count,120,8+loop_count,139,BLACK);
			}
			for(loop_count=0;loop_count<3;loop_count++)
				{
					//lcd_drawline(260+loop_count,100,290+loop_count,150,BLACK);

					lcd_drawline(45+loop_count,120,55+loop_count,139,BLACK);
				}
			Rectangle(15+3,115+5,10+5+5+5+3,2,BLACK); //top line
		}
///////////////Ramp down//////////////////////////////////////////
		if(ramp_flag==2)
		{
			Rectangle(8,120-3,25+15+3+5,25-2,LIGHT_TURQUI);  //box
			Rectangle(42+1,134,10+5-5,2,BLACK); //side line

			for(loop_count=0;loop_count<3;loop_count++)
			{

				lcd_drawline(28+loop_count,120,43+loop_count,136,BLACK);
			}
			Rectangle(20-7,115+5,10+5+5-3,2,BLACK); //top line
		}


	}

	///////////////////////////////////////////////////////////////////////
}
