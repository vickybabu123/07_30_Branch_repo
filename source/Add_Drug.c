
#include "LPC55S16.h"			//22-03
#include "Color.h"
#include "keypad.h"
#include "Shapes.h"
#include "TFT_Commands.h"
#include "Driver_Timer.h"
#include "TFT_String.h"
#include <string.h>
#include "Driver_I2C.h"
#include "Power_off.h"
#include "sound.h"
#define KEY_PRESSED    0
#define KEY_RELEASED   1



struct get_char_info
{
	uint16_t chr_width;
	uint16_t chr_height;
	uint16_t chr_starting_byte;
	uint16_t chr_ending_byte;
	uint16_t chr_width_in_pixels;
	uint32_t fore_color;
	uint32_t back_color;
	const uint8_t *font_array_select;
};


uint8_t drug_L=0;
uint8_t drug_R=0;
uint16_t blink_drug_delay=0;
uint8_t drug_add=0;
uint8_t blink_drug_rect=0;
uint8_t drug_char_change_flg=0;//ZOOZ_2
uint8_t mute_char_change_flg=0;//ZOOZ_2
uint8_t symbol_flg=0;//ZOOZ_2
//volatile uint8_t KEY_STATE = KEY_RELEASED;

char drug_array_R[15]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
char drug_array_L[15]={'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};

char drug_alpha;
char drug_chr_change_arr[4][2]={
								{97,122},
								{65,90},
								{48,57},
								{45,47}
						};//ZOOZ_2

extern uint16_t total_drugs;
extern  uint16_t custom_new_no;
extern uint8_t brand_select_no;//ZOOZ_2
extern uint8_t total_brand;//ZOOZ_2
extern char Drug_Names[150][4][20];
extern uint8_t auto_lock_time;    //ZOOZ_1
extern char Brand_Names[6][4][15];//ZOOZ_2

extern void Frame_Header(uint32_t BG_color, uint32_t TXT_color ,uint8_t *word);
//extern void Frame_Footer(uint32_t BG_color, uint32_t TXT_color,uint8_t *word,uint8_t *circle_word,uint8_t circle_pos);
extern void TFT_ST7789V__rectangle_curve(uint16_t xpos1, uint16_t ypos1, uint16_t width,uint16_t height,uint16_t radius,uint32_t colour);
extern void Font_Select(uint8_t Font_Size);
extern uint16_t TFT_String(uint16_t xpos, uint16_t ypos,const uint8_t *asciistr,uint32_t BG_COL,uint32_t TXT_COL);
extern void Rectangle(uint16_t x_axis,uint16_t y_axis,uint16_t Length,uint16_t Height,uint32_t Color);
//extern uint16_t TFT_Number(uint16_t xpos,uint16_t ypos,uint32_t number,uint32_t boarder_color,uint32_t fill_color);
extern uint8_t ON_Flag;
//void add_drug_dis(void);
void add_drug_dis(uint8_t ID_flag);//ZOOZ_2

uint8_t key_delay_time=0;
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void add_data_base (uint8_t ID_flag,uint16_t db_sel_no)
{
//	uint8_t drug_prev_flg=0;
	uint8_t blink_drug_rect=0;
	uint8_t add_drug_variable=1;
 // uint8_t time_interval=30;//common_bug    //ZOOZ_1
	char drug_alpha=0;
//	uint8_t menu_flg=0;
	uint8_t cnt=0;
	uint16_t cursor_end=0;
	uint16_t str_return=18;
	uint16_t loop_count=0;
//	uint16_t increment_cursor=0;
//  int KEY_CODE = 0;
//	uint8_t key_code=0;
	uint8_t key_release_flag=0;
	//struct get_char_info char_info;//=ascii_details(asciichar);

	char* custom_data1="(+).Add Brand";
	char* custom_data2="(+).Add Drug";

	drug_L=0;
	drug_R=0;

	symbol_flg=0;
	mute_char_change_flg=0;
	drug_char_change_flg=0;
	POW.power_status_flag=1;//26-03

	if(!ID_flag)
	{
		if((db_sel_no!=total_drugs))	// which denoted already edited drug name to present in text field
		{

			for(loop_count=0;loop_count<15;loop_count++)
			{
				drug_array_L[loop_count]=Drug_Names[db_sel_no/4][db_sel_no%4][loop_count];

				drug_array_R[loop_count]='\0';

				if(drug_array_L[loop_count]!='\0')
				{
					drug_L++;

				}
				else
				{
					drug_array_R[loop_count]='\0';
					break;
				}


			}


		}
		else
		{


			for(loop_count=0;loop_count<15;loop_count++)
			{
				drug_array_L[loop_count]='\0';
				drug_array_R[loop_count]='\0';
			}
		}
	}
	else
	{
		if((db_sel_no!=total_brand))
		{
			for(loop_count=0;loop_count<15;loop_count++)
			{
				drug_array_L[loop_count]=Brand_Names[db_sel_no/4][db_sel_no%4][loop_count];
				drug_array_R[loop_count]='\0';

				if(drug_array_L[loop_count]!='\0')
				{
					drug_L++;

				}
				else
				{
					drug_array_R[loop_count]='\0';
					break;
				}


			}


		}
		else
		{


			for(loop_count=0;loop_count<15;loop_count++)
			{
				drug_array_L[loop_count]='\0';
				drug_array_R[loop_count]='\0';
			}
		}

	}



	while(add_drug_variable)
	{
		Power_off();	//02-04
		if(!ON_Flag)
		{
			break;
		}

		else if(POW.power_status_flag)
		{
			POW.power_status_flag=0;
			add_drug_dis(ID_flag);
			Font_Select(ARIAL_14);
			str_return=TFT_String(16,80,(uint8_t *)drug_array_L,WHITE,BLACK);


			if(str_return<18)
			{
				str_return=18;
			}

			TFT_String(str_return,80,(uint8_t *)drug_array_R,WHITE,BLACK);
			while(KEY.value);
		}


		if(!blink_drug_delay)
		{

			if(blink_drug_rect==0)
			{
				Rectangle((str_return-1),108,15,1,BLACK);
				blink_drug_rect=1;

			}
			else if(blink_drug_rect==1)
			{
				Rectangle((str_return-1),108,15,1,WHITE);
				blink_drug_rect=0;
			}
			blink_drug_delay=5;

		}

		if(!key_delay_time)
				{
				if(key_release_flag)
				{
					key_release_flag=0;
					if(drug_array_R[0]!='\0')
				{
					if(drug_L<=12)
					{
						Font_Select(ARIAL_14);
					//	TFT_Number(80,40,drug_L,RED,YELLOW);
						drug_array_L[drug_L]=drug_array_R[0];						//	right array first posi shift to left array last posi
						drug_L+=1;										//	left array vari will be inc
						cursor_end=drug_L;
							if(drug_R>0)										//	left array element in greater than 0
							{
								//TFT_Number(80,60,drug_R,RED,YELLOW);

								for(cnt=0;cnt<drug_R;cnt++)					//	variable will be decr defs on right side array variable
								{

									drug_array_R[cnt]=drug_array_R[cnt+1];			//added right side varible to store right side data

								}
								drug_array_R[drug_R]='\0';						//right array will be null
								drug_R--;									// right array variable will be dec

									if(!drug_R)
									{
										drug_array_R[0]=65;
									}

									drug_alpha=drug_array_R[0];							//right array first posi to load alpha

							}
							else
							{
								drug_alpha=drug_chr_change_arr[drug_char_change_flg][0];
								drug_alpha=drug_alpha-1;
								drug_array_R[0]='\0';
							}

						//	TFT_Number(80,80,str_return,RED,YELLOW);

							Rectangle(str_return-1,108,15,1,WHITE);
							str_return=TFT_String(16,80,(uint8_t *)drug_array_L,WHITE,BLACK);


						//	TFT_Number(80,80,str_return,RED,YELLOW);

					}
				}
			}
			//	key_delay_time=4;
		}

//		***if (KEY_STATE == KEY_RELEASED)  // Only check for UP or DOWN key release
//		{
////		    if (KEY_CODE == UP_KEY || KEY_CODE == DOWN_KEY)  // Ensure only UP and DOWN keys trigger this
////		    {
//			    KEY_CODE = UP_KEY;  // Manually force UP key to check if logic works
//
//			    if (KEY_STATE == KEY_RELEASED)
//			    {
//			        printf("Key Released: %d\n", KEY_CODE);
//
//			        if (KEY_CODE == UP_KEY || KEY_CODE == DOWN_KEY)
//			        {
//			            printf("UP/DOWN Key Released\n");
//			        }
//			    }
//		        auto_inc_pos = 1;      // Key is released, trigger cursor increment automatically
//		        cursor_pos++;          // Increment cursor position
//
//		        if (cursor_pos > 12)   // Adjust 12 based on your max cursor position
//		        {
//		            cursor_pos = 0;
//		        }
//
//		        int prev_cursor_pos = cursor_pos * 16;  // Store previous cursor position
//		        cursor_pos = str_return * 16;           // Update cursor position
//
//		        Rectangle(prev_cursor_pos, 108, 15, 1, WHITE);  // Erase the old cursor
//		        Rectangle(cursor_pos, 108, 15, 1, BLACK);       // Draw the new cursor at the updated position
//
//		        blink_drug_delay = 5;
//		    }
//		}
//		else
//		{
//		    auto_inc_pos = 0;
//		    cursor_pos = 0;
//		}***


//		if (auto_inc_pos)
//		   {
//		        cursor_pos++;                                  // Move the cursor position
//		        if (cursor_pos > 12)
//		        {
//		            cursor_pos = 0;
//		        }
//		           auto_inc_pos = 0;                             // Reset flag after cursor is moved
//
//		    }
//		if (auto_inc_pos)
//		{
//		        cursor_pos++; // Increment cursor position
//		        increment_cursor(); // Ensure it's within valid bounds
//		        auto_inc_pos = 0; // Reset auto increment flag after moving cursor
//		    }







//		* if (auto_inc_pos)
//		 {
//		        cursor_pos++; // Increment cursor position
//		        TFT_String(str_return, 80, (uint8_t *)drug_array_L, WHITE, BLACK);
//
//                if (cursor_pos > 12)
//		        {
//		            cursor_pos = 0; // Reset to the first position after exceeding 12
//		        }
//
//		           auto_inc_pos = 0; // Reset auto increment flag after moving cursor
//		    }*

//	   if (!KEY.value)                  // Detect if a key was pressed and handle cursor increment after release
//	    {
//		   if(key_code)
//		   {
//		    key_code=0;
//	        auto_inc_pos = 1;            // Check if a key is pressed, do not increment the cursor yetKey is released, trigger cursor increment automatically
//	        cursor_pos++;                // Increment cursor position
//	        if (cursor_pos > 12)         // Adjust 12 based on your max cursor position
//	            {
//	                cursor_pos = 0;
//	            }
//	    //    cursor_pos = str_return * 16;      // Adjust 16 for the horizontal spacing between cursor positions
//
//	        Rectangle((str_return-16), 108, 15, 1, WHITE); // Erase the old cursor
//	    //    TFT_String(str_return,80,(uint8_t *)drug_array_R,WHITE,BLACK);
//	        Rectangle(cursor_pos, 108, 15, 1, BLACK);      // Draw the new cursor at the updated position
//
//	        blink_drug_delay = 5;
//
////	        Font_Select(ARIAL_14);
////	        TFT_Number(180,70,str_return,YELLOW,RED);
////	        Font_Select(ARIAL_14);
////	        TFT_Number(str_return,108,5,YELLOW,RED);
////	        Font_Select(ARIAL_14);
////	        TFT_Number(str_return+1,108,6,YELLOW,RED);
//		   }
//	    }
//
//         else
//	    {
//        	 key_code=1;
////	        auto_inc_pos = 0;             // Check if a key is pressed, do not increment the cursor yet
////	        cursor_pos = 0;
////			Rectangle((str_return-1),108,15,1,WHITE)
//	    }

	   // increment_cursor();              // Automatically increment the cursor if needed


	  //  TFT_String(str_return, 80, (uint8_t *)drug_array_L, WHITE, BLACK);


	    if(KEY.value)
			{
				auto_lock_time=AUTO_EXIT_TIME;
			}
			if(!auto_lock_time)
			{
				add_drug_variable=0;
				break;

					}


		switch(KEY.value)
		{

		case UP_KEY:
				//

				    Font_Select(ARIAL_14);

					if(drug_array_R)
					{

						TFT_String(str_return,80,(uint8_t *)drug_array_R,WHITE,WHITE);
					}



					if(symbol_flg==0)
					{
						if((drug_alpha<drug_chr_change_arr[mute_char_change_flg+2][1])&&
						(drug_alpha>=drug_chr_change_arr[mute_char_change_flg+2][0]))	//max
						{

							drug_alpha+=1;

						}
						else
						{
							drug_alpha=drug_chr_change_arr[mute_char_change_flg+2][0];

						}
					}
					else
					{
						if((drug_alpha<drug_chr_change_arr[drug_char_change_flg][1])&&
						(drug_alpha>=drug_chr_change_arr[drug_char_change_flg][0]))	//max
						{

							drug_alpha+=1;

						}
						else
						{
							drug_alpha=drug_chr_change_arr[drug_char_change_flg][0];

						}

					}
					drug_array_R[0]=drug_alpha;

                    key_delay_time=3;
                    key_release_flag=1;

                    Font_Select(ARIAL_14);
                //    TFT_Number(280,40,str_return,RED,YELLOW);
                  //  Font_Select(ARIAL_BOLD_14);
				   TFT_String(str_return,80,(uint8_t *)drug_array_R,WHITE,BLACK);



					while(KEY.value);
					break;



					case DOWN_KEY:

				 Font_Select(ARIAL_14);
				//	Font_Select(ARIAL_20);
					if(drug_array_R)
					{
						TFT_String(str_return,80,(uint8_t *)drug_array_R,WHITE,WHITE);
					}

					if(symbol_flg==0)
					{
						if((drug_alpha>drug_chr_change_arr[mute_char_change_flg+2][0])&&
						(drug_alpha<=drug_chr_change_arr[mute_char_change_flg+2][1]))
						{
							drug_alpha-=1;
						}
						else
						{
							drug_alpha=drug_chr_change_arr[mute_char_change_flg+2][1];
						}
					}

					else
					{
						if((drug_alpha>drug_chr_change_arr[drug_char_change_flg][0])&&
						(drug_alpha<=drug_chr_change_arr[drug_char_change_flg][1]))
						{
							drug_alpha-=1;
						}
						else
						{
							drug_alpha=drug_chr_change_arr[drug_char_change_flg][1];
						}
					}

					drug_array_R[0]=drug_alpha;
					key_delay_time=3;
					key_release_flag=1;
					TFT_String(str_return,80,(uint8_t *)drug_array_R,WHITE,BLACK);

					while(KEY.value);
					break;



			case MENU_KEY:




				 Font_Select(ARIAL_14);

			TFT_String(str_return,80,(uint8_t *)drug_array_R,WHITE,WHITE);
			//if(drug_prev_flg<2)
			//drug_prev_flg=drug_char_change_flg;
			if(!symbol_flg)
			{
				symbol_flg=1;

				TFT_ST7789V__rectangle_curve(108+8,140,40,103-12,0,WHITE);
				TFT_ST7789V_lcd_drawcircle1(140,160,17,MAIN_FRAME_COL,MAIN_FRAME_COL);
				TFT_ST7789V_Mute_Symbol_12pt(153,133,0,MAIN_FRAME_COL,WHITE);

				Font_Select(ARIAL_14);

				if(!mute_char_change_flg)
				{
					TFT_String(170,150,(uint8_t*)"12",WHITE,BLACK);
				}
				else
				{
					TFT_String(170,150,(uint8_t*)"/#",WHITE,BLACK);
				}

				Font_Select(ARIAL_9);
				TFT_ST7789V__rectangle_curve(16,140,40,103-12,0,ORANGE);
				TFT_ST7789V_lcd_drawcircle1(37,160,17,ORANGE,MAIN_FRAME_COL);
				Font_Select(ARIAL_9);
				TFT_String(12+12,155,(uint8_t*)"Men",MAIN_FRAME_COL,WHITE);


			}

			else
			{
				if(drug_char_change_flg)
				{
					drug_char_change_flg=0;
				}
				else
				{
					drug_char_change_flg=1;
				}
			}

			Font_Select(ARIAL_14);

			if(drug_char_change_flg==0)
			{
				drug_alpha=97;
			//	drug_char_change_flg=1;
				TFT_String(60,150,(uint8_t*)"A-Z",ORANGE,ORANGE);
				TFT_String(60,150,(uint8_t*)"a-z",ORANGE,BLACK);


			}
			else
			{
				drug_alpha=65;
			//	drug_char_change_flg=0;
				TFT_String(60,150,(uint8_t*)"a-z",ORANGE,ORANGE);
				TFT_String(60,150,(uint8_t*)"A-Z",ORANGE,BLACK);

			}

			drug_array_R[0]=drug_alpha;
			key_delay_time=3;
			key_release_flag=1;
		//	Font_Select(ARIAL_BOLD_14);
			TFT_String(str_return,80,(uint8_t *)drug_array_R,WHITE,BLACK);

			while(KEY.value);
			break;


			case MUTE_KEY:


				//	symbol_flg=0;
				Font_Select(ARIAL_14);

				TFT_String(str_return,80,(uint8_t *)drug_array_R,WHITE,WHITE);

				//if(drug_prev_flg>2)
				//drug_prev_flg=drug_char_change_flg;
				if(symbol_flg)
				{
					symbol_flg=0;
					TFT_ST7789V__rectangle_curve(16,140,40,103-12,0,WHITE);
					TFT_ST7789V_lcd_drawcircle1(37,160,17,WHITE,MAIN_FRAME_COL);
					Font_Select(ARIAL_9);
					TFT_String(12+12,155,(uint8_t*)"Men",MAIN_FRAME_COL,WHITE);

					Font_Select(ARIAL_14);

					if(!drug_char_change_flg)
					{
						TFT_String(60,150,(uint8_t*)"a-z",WHITE,BLACK);
					}
					else
					{
						TFT_String(60,150,(uint8_t*)"A-Z",WHITE,BLACK);
					}

					Font_Select(ARIAL_9);
					TFT_ST7789V__rectangle_curve(108+8,140,40,103-12,0,ORANGE);
					TFT_ST7789V_lcd_drawcircle1(140,160,17,MAIN_FRAME_COL,MAIN_FRAME_COL);
					TFT_ST7789V_Mute_Symbol_12pt(153,133,0,MAIN_FRAME_COL,WHITE);
				}

				else
				{
					if(mute_char_change_flg)
					{
						mute_char_change_flg=0;
					}
					else
					{
						mute_char_change_flg=1;
					}
				}

				Font_Select(ARIAL_14);
				if(mute_char_change_flg==0)
				{
					drug_alpha=48;
					//	mute_char_change_flg=1;
					TFT_String(170,150,(uint8_t*)"/#",ORANGE,ORANGE);
					TFT_String(170,150,(uint8_t*)"12",ORANGE,BLACK);
				}

				else
				{
					drug_alpha=45;
					//	mute_char_change_flg=0;
					TFT_String(170,150,(uint8_t*)"12",ORANGE,ORANGE);
					TFT_String(170,150,(uint8_t*)"/#",ORANGE,BLACK);
				}
				drug_array_R[0]=drug_alpha;
				key_delay_time=3;
				key_release_flag=1;
				//Font_Select(ARIAL_BOLD_14);
				TFT_String(str_return,80,(uint8_t *)drug_array_R,WHITE,BLACK);


				while(KEY.value);
				break;

			case BOLUS_KEY:


	      if((drug_array_R[0]!='\0')&&(drug_L))
	//   if ((drug_array_R[0] == '\0') && (drug_L))
		//2 if (drug_L>0)
 //3 		if ((drug_array_L[0] != '\0')&&(drug_R>0))
		//	4	if ((drug_array_R[0] != '\0') && (drug_L > 0))
			//	if((drug_L>0)&&(drug_array_R[0]=='\0'))
			{

			    Font_Select(ARIAL_14);
               //drug_L--;
//			    TFT_String (160,140,(uint8_t*)drug_array_R,RED,YELLOW);

//			    TFT_String (160,140,(uint8_t*)drug_array_R,RED,YELLOW);
	           drug_array_L[drug_L]='\0';

	           str_return = TFT_String(16,80,(uint8_t *)drug_array_L,WHITE,BLACK);//ERASE THE LETTERS

	         //   Rectangle(str_return,80,20,25,BLUE);
	     	 //  TFT_String (str_return,80,(uint8_t*)drug_array_R,WHITE,WHITE);


		       TFT_String (str_return,80,(uint8_t*)drug_array_R,WHITE,WHITE);

			   drug_alpha=drug_chr_change_arr[drug_char_change_flg][0];
			   drug_alpha=drug_alpha-1;




	        	 drug_array_R[drug_R]='\0';

              //  Rectangle(str_return-1,108,15,1,WHITE);//TO CREATE A ANOTHER RECTANGLE
              //  Rectangle(str_return,80,15,20,BLUE);
              //  str_return = TFT_String(16,80,(uint8_t *)drug_array_R,WHITE,WHITE);//ERASE THE LETTERS

              //  cursor_end = drug_R;
			//	TFT_Number(10,120,str_return,RED,WHITE);
			//	TFT_Number(100,120,drug_L,RED,WHITE);


			    // drug_array_L[drug_L]='\0';
			//	TFT_String(16,80,(uint8_t *)drug_array_R,WHITE,BLACK);
			}
			while(KEY.value);
			break;


			case RIGHT_KEY:

				if(drug_array_R[0]!='\0')
				{
					if(drug_L<=12)
					{
						Font_Select(ARIAL_14);
					//	TFT_Number(80,40,drug_L,RED,YELLOW);
						drug_array_L[drug_L]=drug_array_R[0];						//	right array first posi shift to left array last posi
						drug_L+=1;										//	left array vari will be inc
						cursor_end=drug_L;
							if(drug_R>0)										//	left array element in greater than 0
							{
								for(cnt=0;cnt<drug_R;cnt++)					//	variable will be decr defs on right side array variable
								{

									drug_array_R[cnt]=drug_array_R[cnt+1];			//added right side varible to store right side data

								}
								drug_array_R[drug_R]='\0';						//right array will be null
								drug_R--;									// right array variable will be dec

									if(!drug_R)
									{
										drug_array_R[0]=65;
									}

									drug_alpha=drug_array_R[0];							//right array first posi to load alpha

							}
							else
							{
								drug_alpha=drug_chr_change_arr[drug_char_change_flg][0];
								drug_alpha=drug_alpha-1;
								drug_array_R[0]='\0';
							}

							Rectangle(str_return-1,108,15,1,WHITE);
							str_return=TFT_String(16,80,(uint8_t *)drug_array_L,WHITE,BLACK);
					}
				}

			while(KEY.value);
			break;
//
//			case LEFT_KEY:
//
//			    if (drug_L > 0) 									 // Check if drug_array_L is not empty (has at least one character)
//			    {
//			                                                          // Move the last character from drug_array_L to the first position of drug_array_R
//			        drug_array_R[drug_R] = drug_array_L[drug_L - 1];  // Move the last character to the right array
//			        drug_R++;                                         // Increment the right array index
//
//			        drug_L--;                                           // Decrease the left array size (move the index back)
//
//			        cursor_end = drug_L;                              // Update the cursor position (cursor_end will be at the new end of drug_array_L)
//
//
//			        if (drug_L > 0)                                    // Shift characters in drug_array_L to the left (remove the last character)
//			        {
//			            for (int cnt = drug_L; cnt < 12; cnt++)
//			            {
//			                drug_array_L[cnt] = drug_array_L[cnt + 1]; // Shift left by 1 position
//			            }
//			        }
//			        else
//			        {
//
//			            drug_array_L[0] = '\0';                        // If drug_L is 0, reset drug_array_L (optional, if you need to)
//			        }
//
//
//			        if (drug_R > 0)                                  // Update the right array character (drug_alpha), if there are any characters in drug_array_R
//			        {
//			            drug_alpha = drug_array_R[0];                 // Set drug_alpha to the first character of drug_array_R
//			        }
//			        else
//			        {
//
//			            drug_alpha = drug_chr_change_arr[drug_char_change_flg][0] - 1;      // If drug_R is empty, reset to a default character or behavior
//			            drug_array_R[0] = '\0';                                            // Clear drug_array_R
//			        }
//
//
//			        Rectangle(str_return - 1, 108, 15, 1, WHITE);                            // Clear the previous area of drug_array_L
//			        str_return = TFT_String(16, 80, (uint8_t *)drug_array_L, WHITE, BLACK);  // Update the display with new drug_array_L content
//
//			    if (drug_R == 0)  // Check if drug_array_R is empty
//		        {
//		            // Move the cursor left to the last character of drug_array_L if drug_array_R is empty
//		            cursor_end = drug_L - 1;  // Set cursor to the last position in drug_array_L
//		        }
//			    }
//



//			case LEFT_KEY:
//
//				if (drug_L > 0)                                                            // Check if `drug_array_L` is not empty
//				{
//					// Font_Select(ARIAL_BOLD_14);
//					//			       if (drug_array_R[0] == '\0')                                // If right array is empty, perform deletion
//					//			    	    {
//					//						    drug_L--;                                                // Reduce left array size (delete last character)
//					//							drug_array_L[drug_L] = '\0';
//					//							Rectangle(str_return - 1, 108, 15, 1, WHITE);             // Erase the character
//					//							str_return = TFT_String(16, 80, (uint8_t *)drug_array_L, WHITE, BLACK);
//					//			    	     }
//					if (drug_array_R[0] == '\0') // right array is empty, delete the character above the cursor
//					{
//						drug_array_L[drug_L-1] = '\0';            // Remove the last character in left array
//						drug_L--;
//					 //   TFT_Number(100, 100, drug_L, WHITE, BLACK);
//						cursor_end = drug_L; // Move the cursor left
//						Rectangle(str_return-1 , 108, 15, 1, WHITE); // Erase the character
//						str_return = TFT_String(16, 80, (uint8_t *)drug_array_L, WHITE, BLACK);
//					}
//					else
//					{
//						// Move last character from left array to right array
//						drug_R++;
//
//						for (int cnt = drug_R - 1; cnt > 0; cnt--)                        // Move last character of left array to first position of right array
//						{
//							drug_array_R[cnt] = drug_array_R[cnt - 1];
//						}
//						drug_array_R[0] = drug_array_L[drug_L - 1];                         // Assign last left character to right array
//						drug_L--;                                                          // Decrease left array size
//						cursor_end = drug_L;                                                 // Update cursor position
//						drug_array_L[drug_L] = '\0';                                          // Clear the last position in `drug_array_L`
//
//						if (drug_R > 0) {
//						        drug_R--; // Decrease the index in the right array
//						        drug_array_R[drug_R] = '\0'; // Null-terminate the last character in drug_array_R
//						    }
//
//						if (drug_R > 0)
//						{
//							drug_alpha = drug_array_R[0];                                         // Set `drug_alpha` to first right character
//						}
//						else
//						{
//							drug_alpha = drug_chr_change_arr[drug_char_change_flg][0] - 1;
//						}
//							Rectangle(str_return - 1, 108, 15, 1, WHITE);
//							str_return = TFT_String(16, 80, (uint8_t *)drug_array_L, WHITE, BLACK);
//							TFT_String(str_return, 80, (uint8_t *)drug_array_R, WHITE, BLACK);
//					}
//				}
            case LEFT_KEY:

                if (drug_array_R[0] == '\0')
                {

                    if (drug_L > 0)
                    {
                        //drug_array_R[0] = drug_L;
                        //drug_array_L[drug_L]=drug_array_R[0];
                     	//drug_array_L[drug_L] = '\0';
                    //    drug_array_L[drug_L]=drug_array_R[0];
						//TFT_String (80,80,(uint8_t*)drug_array_L,RED,YELLOW);

                       // drug_array_R[0]=drug_array_L[drug_L-1];
//                    	drug_array_R[0]=drug_array_L[drug_L];    // need to delete
//                    	drug_array_L[drug_L-1]='\0';
                        drug_L--;
                        drug_alpha = drug_array_L[drug_L];
                        drug_array_R[0]=drug_alpha;


                        struct get_char_info char_info = ascii_details(drug_alpha);
        				Rectangle((str_return-1),108,15,1,WHITE);

                        str_return = str_return - char_info.chr_width_in_pixels - 3;

                      //drug_array_L[drug_L-1]='\0';
                       cursor_end = drug_L;  // Update the cursor position

                  //      Rectangle(str_return - 1 , 108, 15, 1, RED);

                    //    str_return = TFT_String(16, 80, (uint8_t *)drug_array_L, WHITE, BLACK);
                     //   TFT_String(str_return, 80, (uint8_t *)drug_array_R, WHITE, WHITE);
                       key_delay_time=3;
                       key_release_flag=1;

                    }
                }

                blink_drug_delay = 5;


                while (KEY.value);
                break;



			case OK_KEY:


				strcat((char *)drug_array_L,(char *)drug_array_R);
				Font_Select(ARIAL_14);
				drug_add=strlen((char *)drug_array_L);
			    key_release_flag=0;

				if(drug_add) // 27-03
				{
					add_drug_variable=0;
					if(!ID_flag)
					{
						for(loop_count=0;loop_count<15;loop_count++)
						{
							if(loop_count<drug_add)
							{

								Drug_Names[db_sel_no/4][db_sel_no%4][loop_count]=drug_array_L[loop_count];
							}

							else
							{
								Drug_Names[db_sel_no/4][db_sel_no%4][loop_count]='\0';
							}

							EEPROM_datawrite(EEPROM_DRUG_NAMES+((db_sel_no-500)*15)+loop_count,Drug_Names[db_sel_no/4][db_sel_no%4][loop_count],1);  //22-03
						}

							if(db_sel_no==total_drugs)
							{

								total_drugs++;
								Font_Select(ARIAL_14); //22-03
								EEPROM_datawrite(EEPROM_TOTAL_DRUG,total_drugs,2);

							}

							for(loop_count=0;loop_count<15;loop_count++)
							{
								Drug_Names[total_drugs/4][total_drugs%4][loop_count]=custom_data2[loop_count];
							}
							Rectangle(0,0,320,240,MAIN_FRAME_COL);

					}

					else
					{
						for(loop_count=0;loop_count<15;loop_count++)
						{
							if(loop_count<drug_add)
							{

								Brand_Names[db_sel_no/4][db_sel_no%4][loop_count]=drug_array_L[loop_count];
							}
							else
							{
								Brand_Names[db_sel_no/4][db_sel_no%4][loop_count]='\0';
							}


							EEPROM_datawrite(EEPROM_BRAND_NAMES+((db_sel_no-8)*15)+loop_count,Brand_Names[db_sel_no/4][db_sel_no%4][loop_count],1); //22-03
						}
						if(db_sel_no==total_brand)   //22-03
						{
							if(db_sel_no<19)
							{
								total_brand++;
								for(loop_count=0;loop_count<15;loop_count++) //22-03
								{
									Brand_Names[total_brand/4][total_brand%4][loop_count]=custom_data1[loop_count];
								}
							}
							Font_Select(ARIAL_14);

							EEPROM_datawrite(EEPROM_TOTAL_BRAND,total_brand,1);


						}
					}
				}


			while(KEY.value);
			break;


			case BACK_KEY:

			add_drug_variable=0;
			//Rectangle(3,36,320,169,MAIN_FRAME_COL);
			while(KEY.value);
			break;


		}
	}

	}//ZOOZ_2
/****************************************************************************
* Function Name :
* Arguments     :
* Return type   :
* Description   :

*****************************************************************************/
void add_drug_dis(uint8_t ID_flag)//ZOOZ_2
{

	Rectangle(0,0,320,240,MAIN_FRAME_COL);

	if(!ID_flag)
	{
		Frame_Header(NEW_HDR_COL,HDR_TXT_COL,"Custom Drug");

		Frame_Footer_new(ORANGE_DARK,WHITE,(uint8_t*)"OKAY");
		CENTER_SCR(MAIN_FRAME_COL);
		right_14(207,100-40-5-3,0,ORANGE_DARK,WHITE);
		Font_Select(ARIAL_BOLD_14);
		TFT_String(195-30-5+10-7,210,(uint8_t* )"to confirm",ORANGE_DARK,WHITE);

						//center green
			border_lines();
	}
	else
	{
		Frame_layout(1,3,(uint8_t*)"Add Brand",(uint8_t*)"OKAY",(uint8_t*)"to confirm");
	}

    Rectangle(5,60,310,70,WHITE);

	TFT_ST7789V__rectangle_curve(213,140,40,103-12,0,WHITE);
	TFT_ST7789V_lcd_drawcircle1(235,160,17,MAIN_FRAME_COL,MAIN_FRAME_COL);
	Font_Select(ARIAL_9);
	TFT_String(224,155,(uint8_t*)"Bol",MAIN_FRAME_COL,WHITE);
	Font_Select(ARIAL_12);
	TFT_String(265,150,(uint8_t*)"Del",WHITE,BLACK);

//	TFT_ST7789V__rectangle_curve(108+8,140,40,103-12,0,ORANGE);
//						TFT_ST7789V_lcd_drawcircle1(140,160,17,MAIN_FRAME_COL,MAIN_FRAME_COL);
//						TFT_ST7789V_Mute_Symbol_12pt(153,133,0,MAIN_FRAME_COL,WHITE);

		if(symbol_flg)
		{
			TFT_ST7789V__rectangle_curve(108+8,140,40,103-12,0,WHITE);
			TFT_ST7789V_lcd_drawcircle1(140,160,17,MAIN_FRAME_COL,MAIN_FRAME_COL);
			Font_Select(ARIAL_9);
			TFT_ST7789V_Mute_Symbol_12pt(153,133,0,MAIN_FRAME_COL,WHITE);
			Font_Select(ARIAL_12);

			if(!drug_char_change_flg)
			{
				TFT_String(60,150,(uint8_t*)"a-z",WHITE,BLACK);
			}
			else
			{
				TFT_String(60,150,(uint8_t*)"A-Z",WHITE,BLACK);
			}

			Font_Select(ARIAL_9);
			TFT_ST7789V__rectangle_curve(16,140,40,103-12,0,ORANGE);
			TFT_ST7789V_lcd_drawcircle1(37,160,17,ORANGE,MAIN_FRAME_COL);
			Font_Select(ARIAL_9);
			TFT_String(12+12,155,(uint8_t*)"Men",MAIN_FRAME_COL,WHITE);

				Font_Select(ARIAL_12);
				if(mute_char_change_flg==0)
				{
					TFT_String(170,150,(uint8_t*)"12",WHITE,BLACK);
				}
				else
				{
					TFT_String(170,150,(uint8_t*)"/#",WHITE,BLACK);
				}
		}

		else
		{
			TFT_ST7789V__rectangle_curve(16,140,40,103-12,0,WHITE);
			TFT_ST7789V_lcd_drawcircle1(37,160,17,WHITE,MAIN_FRAME_COL);
			Font_Select(ARIAL_9);
			TFT_String(12+12,155,(uint8_t*)"Men",MAIN_FRAME_COL,WHITE);
			Font_Select(ARIAL_12);
				if(mute_char_change_flg==0)
				{
					TFT_String(60,150,(uint8_t*)"a-z",WHITE,BLACK);
				}
				else
				{
					TFT_String(60,150,(uint8_t*)"A-Z",WHITE,BLACK);
				}


				//Font_Select(ARIAL_9);
				TFT_ST7789V__rectangle_curve(108+8,140,40,103-12,0,ORANGE);
				TFT_ST7789V_lcd_drawcircle1(140,160,17,MAIN_FRAME_COL,MAIN_FRAME_COL);
				TFT_ST7789V_Mute_Symbol_12pt(153,133,0,MAIN_FRAME_COL,WHITE);

				Font_Select(ARIAL_12);

					if(!drug_char_change_flg)
					{
						TFT_String(170,150,(uint8_t*)"12",ORANGE,BLACK);
					}
					else
					{
						TFT_String(170,150,(uint8_t*)"/#",ORANGE,BLACK);
					}

		}
}
