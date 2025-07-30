  /*
 * Drug_category.c
 *
 *  Created on: Feb 21, 2024
 *      Author: Welcome
 */
#include "LPC55S16.h"
#include "stdio.h"
#include "stdint.h"
#include "Color.h"
#include "keypad.h"
#include "Shapes.h"
#include "fsl_clock.h"
//#include "Driver_I2C.h"
#include "TFT_Commands.h"
#include "TFT_String.h"
#include "shapes.h"
#include "Drug_List.h"
#include "Main_screen.h"
#include "Driver_Timer.h"
#include "Power_off.h"
#include "Dosage_mode.h"
#include  "Brand.h"
#include "sound.h"

extern uint8_t ON_Flag;
extern uint8_t TFT_ST7789V_tick_symbol_14pt(uint16_t xpos, uint16_t ypos,    //22-03
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);//ZOOZ_2
uint8_t category_screen(void);
void category_Frame(void);
void category_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt);
void category_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page, char* array[2][4]);

uint8_t category_list_fun(uint8_t cate_no);
void category_list_Frame(uint8_t cate_no);
void category_opt_list_Frame(uint16_t page,uint8_t start_opt,uint8_t end_opt,uint8_t cate_no);//ZOOZ_2
void category_list_cursor(uint8_t prev_option, uint8_t curr_option, uint16_t page,uint8_t cate_no);//, char* array[2][4]);//ZOOZ_2
extern uint8_t auto_lock_time;


//extern char* Drug_Names[150][4];
extern char Drug_Names[150][4][20];
extern uint8_t G_bootup_flag;
extern uint8_t fav_check[600];		//8-05-2024

//char* category_a[5][4]={Drug_Names[10/4][10%4],Drug_Names[11/4][11%4],Drug_Names[12/4][12%4],Drug_Names[13/4][13%4]};


char* category_list[2][4]={{"Benzodiazepines", "Muscle relaxant", "Opioids" ,"Anticholinergic Agents"},
		{"Vasopressor "}};
uint8_t total_cate_drug[4]={5,5,5,5};
uint16_t pin_drug_cate[4][5]={{18,157,201,458,497},{10,53,64,150,221},
								{97,192,285,356,477},{54,79,201,330,361}};

//Drug_Names[cat[lp]/4][cat[lp]%4];

uint16_t prev_cate_opt=0;//ZOOZ_2
uint8_t prev_cate_num=0;//ZOOZ_2

uint8_t category_screen(void)
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t exit_flag=1;
	uint8_t check_flag=0;
	uint8_t edit_flag=0;
	//uint8_t dose_longpress=0;
	uint8_t end_opt=3;
	uint8_t category_no=0;
	uint8_t max_cat_page=0;//25-03
	uint8_t max_cat_opt=0;
	//uint8_t time_interval=30;  //ZOOZ_1

	uint8_t total_cat=5;
	POW.power_status_flag=1;//27-03


	//dose.volume=1000;
	max_cat_page=(total_cat-1)/4;//25-03		//6-1=5 5/4=1
	max_cat_opt=(total_cat-1)%4;//25-03         //6-1=5 6%4=2
//	category_Frame();
//	category_opt_Frame(page_no,option_no,end_opt);


	//TFT_Number(200,60,category_no,YELLOW,BLACK);

//	if(max_cat_page)
//	{
//	scroll_bar(page_no,option_no,total_cat);//25-03
//	}
	POW.power_status_flag=1;//27-03 //ZOOZ_1
	auto_lock_time=AUTO_EXIT_TIME;//common_bug

	while(exit_flag)
	{
		Power_off();//27-03


							if(!ON_Flag)
								{
									break;//26-03
								}

								else if(POW.power_status_flag)
							   {
									POW.power_status_flag=0;//26-03
									category_Frame();
									category_opt_Frame(page_no,option_no,end_opt);//27-03

									if(max_cat_page)
									{
									scroll_bar(page_no,option_no,total_cat);//25-03
									}
							   }
				if(KEY.value)//common_bug //ZOOZ_1
				{
				//	while(KEY.value);
					auto_lock_time=AUTO_EXIT_TIME;
				}
				else if(!auto_lock_time)
				{

					break;

				}//common_bug
		switch(KEY.value)
		{

		case DOWN_KEY :
			//beep_sound();
					if(!edit_flag)
					{

						if((option_no<3 && page_no<max_cat_page) //ZOOZ_2
								||(option_no<max_cat_opt ))
						{
							option_no++;
						}
						else
						{

							if(page_no<max_cat_page)//if(max_cat_page>page)
								{
									page_no++;
								 }

								else
								{
									page_no=0;
								}

							if(page_no==max_cat_page)
							{
								end_opt=max_cat_opt;

							}

							else
							{
								end_opt=3;
							}
							option_no=0;
							prev_option=option_no;
//							brand__drug_split_screen();
							//drug_search_layout();//19/07
						////	category_opt_Frame(page_no,option_no,end_opt);
							POW.power_status_flag=1;//27-03//ZOOZ_2
						}

						if(option_no!=prev_option)
						{
							//if(option_no!=0)
							//{
							category_cursor(prev_option,option_no,page_no,category_list);
							//}
							prev_option = option_no;
						}
					}//ZOOZ_2
					if(max_cat_page)//25-03
					{
						scroll_bar(page_no,option_no,total_cat); //23-03
					}

					while(KEY.value);
					break;

					case UP_KEY:
						//beep_sound();
					if(!edit_flag)
					{
						if(option_no>0)
						{
							option_no--;
						}
						else
						{
							if(page_no)
							{
								page_no--;
								end_opt=3;
							//option_no=3;
							//prev_option=3;
							}


							else
							{
								page_no=max_cat_page;
//								option_no=max_cat_opt;
								end_opt=max_cat_opt;
//								prev_option=0;

							}


							option_no=end_opt;
							prev_option=option_no;
//							brand__drug_split_screen();//ZOOZ_2

							//drug_search_layout();//19/07
						//   category_opt_Frame(page_no,option_no,end_opt);
							POW.power_status_flag=1;//27-03

						}

						if(option_no!=prev_option)
						{
							//if(option_no<3)
							//{
							category_cursor(prev_option,option_no,page_no,category_list);
							//}
							prev_option = option_no;
}
}
							if(max_cat_page)
								{
								scroll_bar(page_no,option_no,total_cat);//25-03
								}



					while(KEY.value);
					break;

					case  OK_KEY:
					//beep_sound();
					category_no=((page_no*4)+option_no)	;
					if(category_no!=4)	//IGNORE VASSOPRESSURE//25-03
					{
						check_flag=category_list_fun(category_no);
						if((check_flag)&&(G_bootup_flag))//ZOOZ_2
						{
						exit_flag=0;
						}
						else
						{
							POW.power_status_flag=1;//27-03

						}



//						category_Frame();
//						category_opt_Frame(page_no,option_no,end_opt);
//
//							if(max_cat_page)
//								{
//								scroll_bar(page_no,option_no,total_cat);//25-03
//								}//27-03


						//exit_flag=0;
					}
					//check_flag=1;
					while(KEY.value);
					break;
					case  BACK_KEY:
						//beep_sound();
						exit_flag=0;
						check_flag=0;
					while(KEY.value);
					break;


		}
	}
	return check_flag;
}

void category_layout(void)
{
	Rectangle(12,72-10-5-3,297,116+2+10+3,MAIN_FRAME_COL);//CENTRE GREEN


	Rectangle(17,54+(32*1),277,0,LIGHT_GRAY);
	Rectangle(17,54+(32*2),277,0,LIGHT_GRAY);
	Rectangle(17,54+(32*3),277,0,LIGHT_GRAY);
}
void category_Frame(void)
{
	Frame_layout(1,3,(uint8_t*)"Category",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}

void category_opt_Frame(uint8_t page,uint8_t start_opt,uint8_t end_opt)
{


    uint8_t loop_count=0;
    uint32_t back_colour = 0;
   	uint32_t text_colour = 0;

	for(loop_count=0;loop_count<=end_opt;loop_count++)
	{
		if(loop_count == start_opt)//0==0
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
		}

		Font_Select(ARIAL_BOLD_14);
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
		(uint8_t*)category_list[page][loop_count],back_colour,text_colour);

	}

}
void category_cursor(uint8_t prev_option, uint8_t curr_option, uint8_t page,  char* array[2][4])
{

	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
  uint8_t list_count=0;
	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour = MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			opt_no		= prev_option;
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			opt_no= curr_option;
		}


		//if ((page<=2) || ((page==3) && (opt_no==0)))
			//{
//			    Font_Select(ARIAL_9);
//				TFT_Number(50,60,page,YELLOW,BLACK);
//
//				Font_Select(ARIAL_9);
//				TFT_Number(70,60,opt_no,YELLOW,BLACK);


		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);
			Font_Select(ARIAL_BOLD_14);

			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,

			        (uint8_t*)array[page][opt_no],back_colour,text_colour);

			separation_lines(1,3);
			}
	}


uint8_t category_list_fun(uint8_t cate_no)
{
	uint8_t page_no=0;
	uint8_t option_no=0;
	uint8_t prev_option = 0;
	uint8_t exit_flag=1;
	uint8_t edit_flag=0;
	uint8_t end_opt=3;
	uint8_t max_page=0;
	uint8_t max_opt=0;
	uint8_t check_flag=0;
	uint16_t category_num=0;

	POW.power_status_flag=1;//27-03

	//dose.volume=1000;
//	category_list_Frame(cate_no);
//
//
//
//	category_opt_list_Frame(page_no,option_no,end_opt,cate_no);
	//TFT_Number(300,60,cate_no,YELLOW,BLACK);


	max_page=(total_cate_drug[cate_no]-1)/4;
	max_opt=(total_cate_drug[cate_no]-1)%4;

//	if(max_page)
//	{
//	scroll_bar(page_no,option_no,total_cate_drug[cate_no]);//25-03
//	}

	while(exit_flag)
	{
		Power_off();//27-03

		if(!ON_Flag)
			{
				break;//26-03
			}

			else if(POW.power_status_flag)//27-03

		   {
				POW.power_status_flag=0;//26-03
				category_list_Frame(cate_no);
				category_opt_list_Frame(page_no,option_no,end_opt,cate_no);
				if(max_page)
					{
					scroll_bar(page_no,option_no,total_cate_drug[cate_no]);//25-03
					}
		   }
		switch(KEY.value)
		{

		case DOWN_KEY :
			//beep_sound();
					if(!edit_flag)
					{

						if((option_no<3 && page_no<max_page) || (option_no <max_opt))
						{
							option_no++;
						}
						else
						{
							if(page_no<max_page)
								{
									page_no++;


									if(page_no == max_page)
									{
										end_opt = max_opt;
									}
									else
									{
										end_opt = 3;
									}


									option_no=0;
									prev_option=0;

								 }


								else
								{
									page_no=0;
									option_no=0;
									end_opt=3;
									prev_option=0;


								}
//							brand__drug_split_screen();
//							category_opt_list_Frame(page_no,option_no,end_opt,cate_no);//ZOOZ_2
							POW.power_status_flag=1;

						}

						if(option_no!=prev_option)
						{
							//if(option_no!=0)
							//{
							category_list_cursor(prev_option,option_no,page_no,cate_no);//,Drug_Names);
							//}
							prev_option = option_no;
						}
					}
					if(max_page)
						{
						scroll_bar(page_no,option_no,total_cate_drug[cate_no]);//25-03
						}
					while(KEY.value);
					break;

					case UP_KEY:
						//beep_sound();
					if(!edit_flag)
					{
						if(option_no)
						{
							option_no--;
						}
						else
						{
							if(page_no)
							{
								page_no--;
								end_opt=3;

							}
							else
							{
								page_no=max_page;
								end_opt=max_opt;

							}
							option_no=end_opt;//25-03
							prev_option=option_no;
							brand__drug_split_screen(0);
							category_opt_list_Frame(page_no,option_no,end_opt,cate_no);
						}

						if(option_no!=prev_option)
						{
							//if(option_no<3)
							//{
							category_list_cursor(prev_option,option_no,page_no,cate_no);//,Drug_Names);
							//}
							prev_option = option_no;
						}
					}

					if(max_page)
						{
						scroll_bar(page_no,option_no,total_cate_drug[cate_no]);//25-03
						}
					while(KEY.value);
					break;

					case  OK_KEY:

						//beep_sound();
							if(prev_cate_num==cate_no)
							{
								TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS+((prev_cate_opt%4)*VER_TEXT_SPACE))+6,250,0,MAIN_FRAME_COL,MAIN_FRAME_COL);

							}
						dose.drug_select_no=pin_drug_cate[cate_no][((page_no*4)+option_no)];//ZOOZ_2
						TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+((option_no)*VER_TEXT_SPACE)+6,250,0,LIGHT_GRAY,GOLD);//ZOOZ_2

						prev_cate_opt=((page_no*4)+option_no);//ZOOZ_2

						prev_cate_num=cate_no;//ZOOZ_2

						recently_used_fun(dose.drug_select_no);//ZOOZ_2
						exit_flag=0;
						check_flag=1;

					while(KEY.value);
					break;

					case  BACK_KEY:
						//beep_sound();
						//drug_mode_opt_Frame(page_no,option_no,3);
						exit_flag=0;
						check_flag=0;
					while(KEY.value);
					break;

					case  MUTE_KEY :
						category_num=pin_drug_cate[cate_no][((page_no*4)+option_no)];//ZOOZ_2
						favourite_functionality(category_num,((page_no*4)+option_no));
						while(KEY.value);
						break;

		}
	}
	return check_flag;

}
void category_list_Frame(uint8_t cate_no)
{
		Frame_layout(1,3,(uint8_t*)category_list[cate_no/4][cate_no%4],(uint8_t*)"OKAY",(uint8_t*)"to confirm");
}

void category_opt_list_Frame(uint16_t page,uint8_t start_opt,uint8_t end_opt,uint8_t cate_no)//ZOOZ_2
{


    uint8_t loop_count=0;
    uint32_t back_colour = 0;
   	uint32_t text_colour = 0;
   	uint32_t tick_color=0;

	for(loop_count=0;loop_count<=end_opt;loop_count++)
	{
		if(loop_count == start_opt)//0==0
		{
			back_colour	= LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			tick_color=MAIN_FRAME_COL;
			Rectangle(9,48+(38*(start_opt)),288,30,back_colour);
		}
		else
		{
			back_colour	= MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			tick_color= GOLD;
		}

		Font_Select(ARIAL_BOLD_14);
		/*TFT_String(9,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE),
		(uint8_t*)Drug_Names   [  pin_drug_cate[  ((page*4)+loop_count)   ]/4  ]    [pin_drug_cate[((page*4)+loop_count)]%4],
		back_colour,text_colour);*/
		TFT_String(12,INITIAL_TEXT_POS+(loop_count*VER_TEXT_SPACE)+6,
				(uint8_t*)Drug_Names[(pin_drug_cate[cate_no][((page*4)+loop_count)])/4] [(pin_drug_cate[cate_no][((page*4)+loop_count)])%4],back_colour,text_colour);

		if((pin_drug_cate[cate_no][((page*4)+loop_count)])==dose.drug_select_no)  //22-03

		{
			TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+((loop_count)*VER_TEXT_SPACE)+6,250,0,back_colour,tick_color);//ZOOZ_2
		}

		 if(fav_check[pin_drug_cate[cate_no][((page*4)+loop_count)]]==1)
			  {
				 // fav_check[(page*4)+loop_count]=1;
				  TFT_ST7789V_star_14((INITIAL_TEXT_POS)+((loop_count)*VER_TEXT_SPACE)+6,270,1,back_colour,tick_color);

			  }


	}

}

void category_list_cursor(uint8_t prev_option, uint8_t curr_option, uint16_t page,uint8_t cate_no)//,  char* array[2][4])//ZOOZ_2
{

	uint8_t loop = 0;
	uint8_t opt_no  = 0;
	uint32_t back_colour = 0;
	uint32_t text_colour = 0;
	uint32_t tick_color = 0;

	for(loop = 0; loop<2; loop++)
	{
		if(!loop)
		{
			back_colour = MAIN_FRAME_COL;
			text_colour = LIGHT_GRAY;
			opt_no		= prev_option;
			tick_color= GOLD;
		}
		else
		{
			back_colour = LIGHT_GRAY;
			text_colour = MAIN_FRAME_COL;
			opt_no= curr_option;
			tick_color= MAIN_FRAME_COL;
		}


		//if ((page<=2) || ((page==3) && (opt_no==0)))
			//{
//			    Font_Select(ARIAL_9);
//				TFT_Number(50,60,page,YELLOW,BLACK);
//
//				Font_Select(ARIAL_9);
//				TFT_Number(70,60,opt_no,YELLOW,BLACK);


		Rectangle(9,48+(38*(opt_no)),288,30,back_colour);
			Font_Select(ARIAL_BOLD_14);

			/*TFT_String(9,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE),

			        (uint8_t*)array[pin_drug_cate[((page*4)+opt_no)]/4][pin_drug_cate[((page*4)+opt_no)]%4],back_colour,text_colour);*/
			TFT_String(12,INITIAL_TEXT_POS+(opt_no*VER_TEXT_SPACE)+6,
							(uint8_t*)Drug_Names[(pin_drug_cate[cate_no][((page*4)+opt_no)])/4] [(pin_drug_cate[cate_no][((page*4)+opt_no)])%4],back_colour,text_colour);

			if(  pin_drug_cate[cate_no][((page*4)+opt_no)]==dose.drug_select_no) //22-03

			{
				TFT_ST7789V_tick_symbol_14pt((INITIAL_TEXT_POS)+((opt_no)*VER_TEXT_SPACE)+6,250,0,back_colour,tick_color);//ZOOZ_2
			}

		//	}

			 if(fav_check[pin_drug_cate[cate_no][((page*4)+opt_no)]]==1)
				  {
					 // fav_check[(page*4)+loop_count]=1;
					  TFT_ST7789V_star_14((INITIAL_TEXT_POS)+((opt_no)*VER_TEXT_SPACE)+6,270,1,back_colour,tick_color);

				  }




	}
}



