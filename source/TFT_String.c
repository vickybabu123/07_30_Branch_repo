#include <stdio.h>
#include "LPC55S16.h"
#include "TFT_Commands.h"
#include <string.h>
#include "stdint.h"
#include "TFT_String.h"
#include "Color.h"
#include "Shapes.h"
#include "Font.h"
#include "Driver_SPI.h"
#include "Driver_Timer.h"

uint8_t font_size;
uint8_t decimal_limit=0;
extern uint16_t G_stand_sec;
//typedef struct
//			{
//			uint16_t chr_width;
//			uint16_t chr_height;
//			uint16_t chr_starting_byte;
//			}FONT_CHAR_INFO;


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

extern const uint8_t segoeUISymbol_18ptBitmaps[];

extern uint8_t right_14(uint16_t xpos, uint16_t ypos,    //22-03
	       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);

uint8_t brand_width_disp=0;

const FONT_CHAR_INFO *local_font_array_descriptor;
const uint8_t *local_font_array_select;
struct get_char_info ascii_details(char numeric);

extern const uint8_t Left_Right_Arrow_14ptBitmaps[];
extern const uint8_t up_down_arrow_20ptBitmaps[];
extern const uint8_t Left_arrow_35ptBitmaps[];
extern const uint8_t segoeFluentIcons_25ptBitmaps[] ;
extern const uint8_t segoeUIEmoji_24ptBitmaps[] ;
extern const uint8_t segoeFluentIcons_12ptBitmaps[];
extern const uint8_t segoeFluentIcons_9ptBitmaps[];
extern const uint8_t ZOOZ_LOGO_PNG[];
extern const uint8_t wingdings3_24ptBitmaps[];
extern const uint8_t arial_7ptBitmaps[];
extern const uint8_t webdings_22ptBitmaps[];
extern const uint8_t wingdings3_R_ARROW_20ptBitmaps[];
extern const uint8_t Mute_Symbol_12pt[];
extern const uint8_t wingdings3_L_ARROW_13ptBitmaps[];
extern const uint8_t segoeUISymbol_22ptBitmaps[];
extern const FONT_CHAR_INFO arial_9ptDescriptors[];
extern const uint8_t arial_9ptBitmaps[];

extern const FONT_CHAR_INFO arial_12ptDescriptors[];
extern const uint8_t arial_12ptBitmaps[];

extern const FONT_CHAR_INFO arial_Regular_14ptDescriptors[];
extern const uint8_t arial_Regular_14ptBitmaps[];

extern const FONT_CHAR_INFO arial_20ptDescriptors[];
extern const uint8_t arial_20ptBitmaps[] ;


extern const FONT_CHAR_INFO arialNarrow_36ptDescriptors[];
extern const uint8_t arialNarrow_36ptBitmaps[];


extern const uint8_t bebasNeuePro_43ptBitmaps[];
extern const FONT_CHAR_INFO bebasNeuePro_43ptDescriptors[];

extern const FONT_CHAR_INFO bebasNeuePro_36ptDescriptors[];
extern const uint8_t bebasNeuePro_36ptBitmaps[];

extern const uint8_t arial_12boldptBitmaps[];
extern const FONT_CHAR_INFO arial_12boldptDescriptors[];

extern const FONT_CHAR_INFO calibri_48ptDescriptors[ ];
extern const uint8_t calibri_48ptBitmaps[ ];
extern const uint8_t Warning_symbol_18ptBitmaps[];
extern const uint8_t segoeUISymbol_14ptBitmaps[];
extern const uint8_t star_14ptBitmaps[];

extern const uint8_t keylock4[];
extern const uint8_t keyunlock4[];


extern const uint8_t tick_14[];  //22-03
extern const uint8_t search[];  //22-03
extern const uint8_t segoeFluentIcons_18ptBitmaps[];
extern const uint8_t power_28ptBitmaps[];
extern const uint8_t standby_28ptBitmaps[];
extern const uint16_t WARNING[];
extern const uint8_t arial_14ptBitmaps[];
extern const FONT_CHAR_INFO arial_14ptDescriptors[];
extern const uint8_t warn_triangle[];
extern const uint8_t warn_exclaim[];
extern const uint8_t warn_triangle_2[];
extern const uint8_t warn_exclaim_2[] ;
extern const uint8_t right_14ptBitmaps[];
extern const uint8_t lOGO_1100[];


uint8_t AKAS_LOGO(uint16_t xpos, uint16_t ypos,
				uint8_t arrow, uint32_t boarder_color, uint32_t fill_color);
 void Frame_Footer_drug(uint32_t BG_color, uint32_t TXT_color,uint8_t *word);

extern const FONT_CHAR_INFO bebasNeuePro_60ptDescriptors[];
extern const uint8_t bebasNeuePro_60ptBitmaps[];
extern const uint8_t segoeUISymbol_12ptBitmaps[];
extern const uint8_t segoeUISymbol_9ptBitmaps[];
uint16_t debug_temp=0;


void Font_Select(uint8_t Font_Size)
{
	font_size=Font_Size;

	switch(Font_Size)
	{

		case ARIAL_9:
		local_font_array_descriptor =  arial_9ptDescriptors;
		local_font_array_select     =  (unsigned char*)arial_9ptBitmaps;
		break;

		case ARIAL_12:
		local_font_array_descriptor = arial_12ptDescriptors;
		local_font_array_select     = (unsigned char*)arial_12ptBitmaps;
		break;

		case ARIAL_14:
		local_font_array_descriptor  =   arial_Regular_14ptDescriptors;
		local_font_array_select      =  (unsigned char*)arial_Regular_14ptBitmaps;
		break;

		case ARIAL_20:
		local_font_array_descriptor    =   arial_20ptDescriptors;
		local_font_array_select        =  (unsigned char*)arial_20ptBitmaps;
		break;

//		case ARIAL_36:
//		local_font_array_descriptor    = arialNarrow_36ptDescriptors;
//		local_font_array_select        = (unsigned char*)arialNarrow_36ptBitmaps;
//		break;


		case BEBAS_60:
		local_font_array_descriptor   =  bebasNeuePro_60ptDescriptors;                                  // Not in the Descriptors Method
		local_font_array_select       =  (unsigned char*)bebasNeuePro_60ptBitmaps;
		break;

//		case ARIAL_BOLD_12:
//		local_font_array_descriptor  =  arial_12boldptDescriptors;                                // Not in the Descriptor Method
//		local_font_array_select      = (unsigned char*)arial_12boldptBitmaps;
//		break;

		case ARIAL_BOLD_14:
		local_font_array_descriptor    = arial_14ptDescriptors;                                 // Not in the Descriptor Method
		local_font_array_select        = (unsigned char*)arial_14ptBitmaps;
		break;

//		case BEBAS_36:
//		local_font_array_descriptor    = bebasNeuePro_36ptDescriptors;                                 // Not in the Descriptor Method
//		local_font_array_select        = (unsigned char*)bebasNeuePro_36ptBitmaps;
//		break;

		case BEBAS_43:
		local_font_array_descriptor    = bebasNeuePro_43ptDescriptors;                                 // Not in the Descriptor Method
		local_font_array_select        = (unsigned char*)bebasNeuePro_43ptBitmaps;
		break;

	}
}


uint8_t TFT_ST7789V_sercch_symbol_24pt(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color) //22-03
{

	uint8_t ascii_buff[112];
	uint32_t add=arrow*112;
	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	uint8_t result_multiplier=0;
	uint16_t __16_bit_color=0;
	uint16_t __16_bit_color_1=0;

	__16_bit_color	=__24_bit_to_16_bit_color(fill_color);
	__16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);




//	for(row=0;row<112;row++)
//	{
//		//ascii_buff[row]= tick_14[add+row];
//		ascii_buff[row]= search[add+row];
//	}

	TFT_ST7789V_BlockWrite(xpos,xpos+27,ypos,ypos+110);

	A0_DATA;

	for(column_no=0;column_no<4;column_no++)
	{
		ascii_inc=column_no;

		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
		{
			result_multiplier=multiplier[No_of_bits];
			for(row_wise_column_no=ascii_inc;row_wise_column_no<(112);row_wise_column_no+=4)
			{
				data =ascii_buff[row_wise_column_no];

				if((data & result_multiplier)==result_multiplier)
				{
					//    spi_write_data(fill_color>>16);
					//    spi_write_data(fill_color>>8);
					//    spi_write_data(fill_color);
					SPI_transfer( __16_bit_color,_16_BIT);
				}
				else
				{
					//    spi_write_data(boarder_color>>16);
					//    spi_write_data(boarder_color>>8);
					//    spi_write_data(boarder_color);
					SPI_transfer( __16_bit_color_1,_16_BIT);
				}
			}
		}
	}
	return ypos;   //+16;
}





//void TFT_screen_saver_Time_disp(uint16_t xpos,uint16_t ypos,uint32_t number,uint32_t boarder_color,uint32_t fill_color) //23-03
//{
//	char local_buffer[6];
//
//	if(number>9959)
//	{
//		number=9959;
//	}
//
//	sprintf(local_buffer,"%02d",number/100);    //store the integer variables in to string using sprintf
//	Font_Select(CALIBRI_48);
//	xpos=TFT_String(xpos,ypos,(uint8_t*)local_buffer,boarder_color,fill_color);
//	//Font_Select(ARIAL_36);
//	//xpos=TFT_String(xpos+4,ypos+10,(uint8_t*)":",boarder_color,fill_color);
//	sprintf(local_buffer,"%02d",number%100);    //store the integer variables in to string using sprintf
//	Font_Select(CALIBRI_48);
//	TFT_String(xpos+2+10+4+5+5,ypos,(uint8_t*)local_buffer,boarder_color,fill_color);
//
//}

//void TFT_Number_month(uint16_t xpos,uint16_t ypos,uint32_t number,uint32_t boarder_color,uint32_t fill_color)  //23-03
//{
//	char local_buffer[6];
//	sprintf(local_buffer,"%02d",number);    //store the integer variables in to string using sprintf
//	TFT_String(xpos,ypos,(uint8_t*)local_buffer,boarder_color,fill_color);
//}

uint8_t TFT_ST7789V_tick_symbol_14pt(uint16_t xpos, uint16_t ypos,    //22-03
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)
{

	uint8_t ascii_buff[26];
	uint32_t add=arrow*26;
	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	uint8_t result_multiplier=0;
	uint16_t __16_bit_color=0;
	uint16_t __16_bit_color_1=0;

	__16_bit_color	=__24_bit_to_16_bit_color(fill_color);
	__16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);




	for(row=0;row<26;row++)
	{
		ascii_buff[row]= tick_14[add+row];
	}

	TFT_ST7789V_BlockWrite(xpos,xpos+12,ypos,ypos+26);

	A0_DATA;

	for(column_no=0;column_no<2;column_no++)
	{
		ascii_inc=column_no;
		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
		{
			result_multiplier=multiplier[No_of_bits];
			for(row_wise_column_no=ascii_inc;row_wise_column_no<(26);row_wise_column_no+=2)
			{
				data =ascii_buff[row_wise_column_no];

				if((data & result_multiplier)==result_multiplier)
				{
					//    spi_write_data(fill_color>>16);
					//    spi_write_data(fill_color>>8);
					//    spi_write_data(fill_color);
					SPI_transfer( __16_bit_color,_16_BIT);
				}
				else
				{
					//    spi_write_data(boarder_color>>16);
					//    spi_write_data(boarder_color>>8);
					//    spi_write_data(boarder_color);
					SPI_transfer( __16_bit_color_1,_16_BIT);
				}
			}
		}
	}
	return ypos;   //+16;
}
uint8_t TFT_ST7789V_keylock4_symbol_16pt(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
{
	uint8_t ascii_buff[81];
	uint32_t add=arrow*81;
	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	uint8_t result_multiplier=0;
	uint16_t __16_bit_color=0;
	uint16_t __16_bit_color_1=0;

	__16_bit_color	=__24_bit_to_16_bit_color(fill_color);
	__16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);

	for(row=0;row<81;row++)
	{
		ascii_buff[row]= keylock4[add+row];
	}

	TFT_ST7789V_BlockWrite(xpos,xpos+26,ypos,ypos+81);

	A0_DATA;

	for(column_no=0;column_no<3;column_no++)
	{
		ascii_inc=column_no;
		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
		{
			result_multiplier=multiplier[No_of_bits];
			for(row_wise_column_no=ascii_inc;row_wise_column_no<(81);row_wise_column_no+=3)				//57 wid
			{
				data =ascii_buff[row_wise_column_no];

				if((data & result_multiplier)==result_multiplier)
				{
					//    spi_write_data(fill_color>>16);
					//    spi_write_data(fill_color>>8);
					//    spi_write_data(fill_color);
					SPI_transfer( __16_bit_color,_16_BIT);
				}
				else
				{
					//    spi_write_data(boarder_color>>16);
					//    spi_write_data(boarder_color>>8);
					//    spi_write_data(boarder_color);
					SPI_transfer( __16_bit_color_1,_16_BIT);
				}
			}
		}
	}
	return ypos;   //+16;
}
uint8_t TFT_ST7789V_keyunlock4_symbol_16pt(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
{
	uint8_t ascii_buff[81];
	uint32_t add=arrow*81;
	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	uint8_t result_multiplier=0;
	uint16_t __16_bit_color=0;
	uint16_t __16_bit_color_1=0;

	__16_bit_color	=__24_bit_to_16_bit_color(fill_color);
	__16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);

	for(row=0;row<81;row++)
	{
		ascii_buff[row]= keyunlock4[add+row];
	}

	TFT_ST7789V_BlockWrite(xpos,xpos+26,ypos,ypos+81);

	A0_DATA;

	for(column_no=0;column_no<3;column_no++)
	{
		ascii_inc=column_no;

		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
		{
			result_multiplier=multiplier[No_of_bits];

			for(row_wise_column_no=ascii_inc;row_wise_column_no<(81);row_wise_column_no+=3)				//57 wid
			{
				data =ascii_buff[row_wise_column_no];

				if((data & result_multiplier)==result_multiplier)
				{
					//    spi_write_data(fill_color>>16);
					//    spi_write_data(fill_color>>8);
					//    spi_write_data(fill_color);
					SPI_transfer( __16_bit_color,_16_BIT);
				}
				else
				{
					//    spi_write_data(boarder_color>>16);
					//    spi_write_data(boarder_color>>8);
					//    spi_write_data(boarder_color);
					SPI_transfer( __16_bit_color_1,_16_BIT);
				}
			}
		}
	}
	return ypos;   //+16;
}
uint16_t TFT_writenumber_float(uint16_t xpos,uint16_t ypos,float number,uint32_t boarder_color,uint32_t fill_color)
{
	char local_buffer[10];
	sprintf(local_buffer,"%0.2f",number/100);
	TFT_String(xpos,ypos,(uint8_t*)local_buffer,boarder_color,fill_color);
	return xpos;
}
uint16_t TFT_writenumber_float_com(uint16_t xpos,uint16_t ypos,float number,uint32_t boarder_color,uint32_t fill_color)
{
	char local_buffer[10];
	sprintf(local_buffer,"%0.1f",number);
	TFT_String(xpos,ypos,(uint8_t*)local_buffer,boarder_color,fill_color);
	return xpos;
}
uint16_t TFT_writenumber_float_1d(uint16_t xpos,uint16_t ypos,float number,uint32_t boarder_color,uint32_t fill_color)
{
	char local_buffer[10];
	sprintf(local_buffer,"%0.1f",number/10);
	TFT_String(xpos,ypos,(uint8_t*)local_buffer,boarder_color,fill_color);
	return xpos;
}

uint16_t TFT_Number(uint16_t xpos,uint16_t ypos,uint32_t number,uint32_t boarder_color,uint32_t fill_color)
{
	uint16_t x_pos;

	char local_buffer[20];
	sprintf(local_buffer,"%0d",number);    //store the integer variables in to string using sprintf
	x_pos=TFT_String(xpos,ypos,(uint8_t*)local_buffer,boarder_color,fill_color);
	return x_pos;
}

uint16_t TFT_String(uint16_t xpos, uint16_t ypos,const uint8_t *asciistr,uint32_t BG_COL,uint32_t TXT_COL)
{
	while(*asciistr)
	{
		if(*asciistr!=194)
		xpos = TFT_writechar(xpos,ypos,*asciistr,BG_COL,TXT_COL);
		asciistr++;
	}
	return xpos;
}
uint16_t TFT_writechar(uint16_t xpos, uint16_t ypos,
				  uint8_t asciichar, uint32_t BG_COL, uint32_t TXT_COL)
{
	int  No_of_bits;
	uint16_t  row_wise_column_no=0,column_no;
	uint16_t data=0;
	struct get_char_info char_info=ascii_details(asciichar);
	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	uint8_t result_multiplier=0;
	uint16_t __16_bit_color=0;
	uint16_t __16_bit_color_1=0;

	__16_bit_color	=__24_bit_to_16_bit_color(TXT_COL);
	__16_bit_color_1=__24_bit_to_16_bit_color(BG_COL);

	//uint8_t red = 0;
	//uint8_t green = 0;
	//uint8_t blue = 0;

	//uint16_t __16_bit_color = 0;
	//uint16_t __16_bit_color1 = 0;
	//
	//		red = (TXT_COL&0xFF0000)>>16;
	//		green =(TXT_COL&0x00FF00)>>8;
	//		blue = (TXT_COL&0x0000FF);
	//		__16_bit_color = (red<<11)|(green<<5)|(blue);
	//
	//
	//		red = (BG_COL&0xFF0000)>>16;
	//		green =(BG_COL&0x00FF00)>>8;
	//		blue = (BG_COL&0x0000FF);
	//		__16_bit_color1 = (red<<11)|(green<<5)|(blue);


	//	TFT_ST7789V_BlockWrite(ypos,ypos+((char_info.chr_width*8)-1),xpos,xpos+char_info.chr_height-1);

	TFT_ST7789V_BlockWrite(ypos,ypos+char_info.chr_height-1,xpos,xpos+((char_info.chr_width*8)-1));
	//	TFT_ST7789V_BlockWrite(xpos,xpos+((char_info.chr_width*8)-1),ypos,ypos+char_info.chr_height-1);

	//  TFT_ST7789V_BlockWrite(ypos,ypos+char_info.chr_height-1,xpos,xpos+((char_info.chr_width*8)-1));
	//	TFT_ST7789V_BlockWrite(ypos,ypos+((char_info.chr_width*8)-1),xpos,xpos+char_info.chr_height-1);


	A0_DATA;

	for(column_no=0;column_no<char_info.chr_width;column_no++)
	{
		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
		{
			result_multiplier=multiplier[No_of_bits];

			for(row_wise_column_no=char_info.chr_starting_byte;row_wise_column_no<char_info.chr_ending_byte;row_wise_column_no+=char_info.chr_width)
			{
				data=char_info.font_array_select[row_wise_column_no+column_no];

				if((data & result_multiplier)==result_multiplier)
				{

					//			  SPI_data(TXT_COL>>16);
					//			  SPI_data(TXT_COL>>8);
					//			  SPI_data(TXT_COL);
					SPI_transfer(__16_bit_color,_16_BIT);

				}
				else
				{
					// SPI_data(BG_COL>>16);
					//  SPI_data(BG_COL>>8);
					//  SPI_data(BG_COL);
					SPI_transfer(__16_bit_color_1,_16_BIT);
				}
			}
		}
	}
	return xpos+char_info.chr_width_in_pixels+3;
}

struct get_char_info ascii_details(char numeric)
{
	uint8_t array_location=0;
		struct get_char_info collect_char_info;
		uint8_t numeric1=numeric;
		if(numeric1<=127 || numeric1==181 )
		{
			if(numeric1==181  && (font_size == ARIAL_14 || font_size == ARIAL_12 ))
			{
				array_location=95;
			}
			else if((font_size <= ARIAL_BOLD_14)  ||  (numeric1==32))
			{
				array_location=numeric1-32;
			}

			else
			{
			 if (font_size <= BEBAS_60)
			 {   // font_size >= BEBAS_36 &&
			     if (numeric1 == 32)
			     {
			         array_location = 0;
			     }
			     else if (numeric1 == 38)
			     {
			         array_location = 1;
			     }
			     else if (numeric1 >= 38 && numeric1 <= 58)
			     {
			         array_location = numeric1 - (font_size >= BEBAS_43 ? 45 : 44);
			     }
			     else if (numeric1 >= 65 && numeric1 <= 90)
			     {
			         array_location = numeric1 - 50;
			     }
			     else if (numeric1 >= 97 && numeric1 <= 122)
			     {
			         array_location = numeric1 - 56;
			     }
			 }


					else
					{
						array_location=numeric1-47;

					}


			}
			collect_char_info.chr_width_in_pixels = local_font_array_descriptor[array_location].chr_width;

			if((local_font_array_descriptor[array_location].chr_width%8) == 0)
			collect_char_info.chr_width         = (local_font_array_descriptor[array_location].chr_width)/8;

			else
			{
				collect_char_info.chr_width         =(((local_font_array_descriptor[array_location].chr_width)/8)+1);
			}
			collect_char_info.chr_height     	    =   local_font_array_descriptor[array_location].chr_height;
			collect_char_info.chr_starting_byte	=   local_font_array_descriptor[array_location].chr_starting_byte;
			collect_char_info.chr_ending_byte   =   ((collect_char_info.chr_starting_byte+(collect_char_info.chr_width*collect_char_info.chr_height)));//3570
			collect_char_info.font_array_select	=	  local_font_array_select;
		}
		return collect_char_info;
}


//void TFT_ST7789V__Bottom_curve(uint16_t xpos1, uint16_t ypos1, uint16_t width,uint16_t height,uint16_t radius,uint32_t colour)
//{
//	int tempx,tempy,tempz;
//	height+= xpos1;
//	width+= ypos1;
//	if((height>xpos1) & (width > ypos1))
//	{
//		tempx = ((height - xpos1)/((height - xpos1)/10));
//		tempy = ((width - ypos1)/((width - ypos1)/10));
//		tempz = ((tempx+tempy)/2);
//	}
//	TFT_ST7789V_lcd_shapes(xpos1,height-tempx,ypos1+1,width-1,AKAS_GREEN);
//	TFT_ST7789V_lcd_shapes(xpos1,height-1,ypos1+tempy+1,width-tempy-1,AKAS_GREEN);
//	TFT_ST7789V_lcd_drawcircle1(height-tempx,ypos1+tempy,tempz,BLACK,AKAS_GREEN);
//	TFT_ST7789V_lcd_drawcircle1(height-tempx,width-tempy,tempz,BLACK,AKAS_GREEN);
//}

void Frame_Header(uint32_t BG_color, uint32_t TXT_color ,uint8_t *word)
{
	TFT_ST7789V__rectangle_curve(2,2,42,315,50,BG_color);
	Font_Select(ARIAL_BOLD_14);
	TFT_String(Letters_Alignments(0,0,word,2),12,word,BG_color,TXT_color);
}
void Frame_Footer_new(uint32_t BG_color, uint32_t TXT_color,uint8_t *word)
{
	uint8_t x_pos1=0;
	TFT_ST7789V__rectangle_curve(2,185,51,315,50,BG_color);

	Font_Select(ARIAL_BOLD_14);
	x_pos1=Letters_Alignments(0,20,word,2);
	TFT_String(85-3,209,word,BG_color,TXT_color);

//	right_14(202,100-40-5,0,ORANGE_DARK,WHITE);
}

void Frame_Footer(uint32_t BG_color, uint32_t TXT_color,uint8_t *word,uint8_t *string)
{
	uint16_t x_pos1=0;
	TFT_ST7789V__rectangle_curve(2,185,51,315,50,BG_color);

	Font_Select(ARIAL_BOLD_14);
	x_pos1=Letters_Alignments(0,20,word,2);
	TFT_String(85-3,209,word,BG_color,TXT_color);
	Font_Select(ARIAL_BOLD_14);
	TFT_String(163,211,string,BG_color,TXT_color);
}

//void Frame_Footer_drug(uint32_t BG_color, uint32_t TXT_color,uint8_t *word)
//{
//	uint16_t x_pos1=0;
//	TFT_ST7789V__rectangle_curve(10,190-20+30,40-10,300,50,BG_color);
//
//	Font_Select(ARIAL_BOLD_14);
//	x_pos1=Letters_Alignments(0,20,word,2);
//	TFT_String(85-3,202,word,BG_color,TXT_color);
//
////	right_14(202,100-40-5,0,ORANGE_DARK,WHITE);
//
//}
void Frame_Footer_NC(uint32_t BG_color, uint32_t TXT_color,uint8_t *word )
{
	Rectangle(0,206,320,34,NEW_FTR_COL);
	Font_Select(ARIAL_14);
	TFT_String(Letters_Alignments(0,0,word,2),210,word,BG_color,TXT_color);
}

void Frame_Footer_warning(uint32_t BG_color, uint32_t TXT_color,uint8_t *word,uint8_t *circle_word,uint8_t circle_pos)
{
	uint16_t x_pos=0;
	uint8_t YPOS1=0;
//	if(circle_pos==2)
//	{
//		YPOS1=83;
//	}
//	else  if(circle_pos==1)
//	{
//		YPOS1=15;
//	}

	TFT_ST7789V__rectangle_curve(2,185,51,315,50,BG_color);
	Font_Select(ARIAL_BOLD_14);
	x_pos=Letters_Alignments(0,20,word,2);
	TFT_String(x_pos,212,word,BG_color,TXT_color);

	TFT_ST7789V_Warning_symbol_18pt(210,x_pos-40,0,WARNING_COLOR,BLACK);
	TFT_ST7789V_Exclamatory_9pt_arial(216,x_pos-40+9,0,WARNING_COLOR,BLACK);

//	if(circle_pos)
//	{
//		TFT_ST7789V_lcd_drawcircle1(x_pos+YPOS1,223,14,MAIN_FRAME_COL,MAIN_FRAME_COL);
//		Font_Select(ARIAL_9);
//		TFT_String(x_pos+YPOS1-9+1,218,circle_word,MAIN_FRAME_COL,WHITE);
//	}

}

uint16_t Letters_Alignments(volatile uint16_t X_Start,volatile uint16_t X_End,uint8_t *asciichar,volatile uint8_t Type)
{
	volatile uint16_t total_pixel=0;
	volatile uint8_t count=0;
	volatile uint16_t chr_width=0;
	volatile uint16_t TFT_centre_pxl=160;
	volatile uint16_t xpos=0;
	struct get_char_info char_info;

	while(*asciichar)
	{
		char_info=	ascii_details(*asciichar);
		total_pixel=char_info.chr_width_in_pixels+total_pixel;
		count++;
		asciichar++;
	}

	if(Type==0)
	{
		brand_width_disp=chr_width	=  total_pixel + ((count-1)*2);
		chr_width	=  chr_width/2;
		xpos		=  X_Start+(((X_End - X_Start)/2) - chr_width);
	}

	else
	{
		brand_width_disp=chr_width	=  total_pixel + ((count-1)*3);
		chr_width	=  chr_width/2;
		xpos		=  TFT_centre_pxl - chr_width;
//		Font_Select(ARIAL_14);
//		TFT_Number(0,70,xpos,YELLOW,RED);
	}
	return xpos;
}
uint16_t  TFT_String_align(uint16_t x_start,uint16_t x_end,uint16_t YPOS,uint8_t *popup_message,uint8_t center,uint32_t back_col,uint32_t txt_col)    //REW1
{
	uint16_t XPOS=0;
	XPOS=Letters_Alignments(x_start,x_end,(uint8_t *)popup_message,center);
	TFT_String(XPOS,YPOS,popup_message,back_col,txt_col);

	return XPOS;
}
//uint16_t Letters_Alignments_HF( uint8_t *asciichar )
//	{
//	uint16_t total_pixel=0;
//	uint8_t count=0;
//	uint16_t chr_width=0;
//
//	uint16_t TFT_centre_pxl=160;
//	uint16_t ypos=0;
//
//	struct get_char_info char_info;
//
//	while(*asciichar)
//		{
//			char_info=	ascii_details(*asciichar);
//			total_pixel=char_info.chr_width_in_pixels+total_pixel;
//			count++;
//			asciichar++;
//		}
//
//			chr_width		= 	total_pixel + ((count-1)*2);
//			chr_width		=	chr_width/2;
//			ypos				=	TFT_centre_pxl - chr_width;
//
//			return ypos;
//	}

uint8_t TFT_ST7789V_left_Right_arrow_14_arial(uint16_t xpos, uint16_t ypos,
								 uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)
{
	uint8_t ascii_buff[15];
	uint32_t add=arrow*15;
	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;
	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	uint8_t result_multiplier=0;
	uint16_t __16_bit_color=0;
	uint16_t __16_bit_color_1=0;

	__16_bit_color	=__24_bit_to_16_bit_color(fill_color);
	__16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);

	for(row=0;row<15;row++)
	{
		ascii_buff[row]=Left_Right_Arrow_14ptBitmaps [add+row];
	}
	TFT_ST7789V_BlockWrite(xpos,xpos+14,ypos,ypos+13);
	A0_DATA;

	for(column_no=0;column_no<1;column_no++)
	{
		ascii_inc=column_no;
		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
		{
			result_multiplier=multiplier[No_of_bits];
			for(row_wise_column_no=ascii_inc;row_wise_column_no<(15);row_wise_column_no+=1)				//57 wid
			{
				data =ascii_buff[row_wise_column_no];

				if((data & result_multiplier)==result_multiplier)
				{
					//														SPI_data(fill_color>>16);
					//														SPI_data(fill_color>>8);
					//														SPI_data(fill_color);
					SPI_transfer(__16_bit_color,_16_BIT);
				}
				else
				{
					//																SPI_data(boarder_color>>16);
					//																SPI_data(boarder_color>>8);
					//																SPI_data(boarder_color);
					SPI_transfer(__16_bit_color_1,_16_BIT);
				}
			}
		}
	}
	return ypos;
}

uint8_t TFT_ST7789V_up_down_arrow_20_arial(uint16_t xpos, uint16_t ypos,
									uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)
{
	uint8_t ascii_buff[27];
	uint32_t add=arrow*27;
	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;
	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	uint8_t result_multiplier=0;
	uint16_t __16_bit_color=0;
	uint16_t __16_bit_color_1=0;

     __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
     __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


		for(row=0;row<27;row++)
		{
			ascii_buff[row]= up_down_arrow_20ptBitmaps[add+row];
		}
		TFT_ST7789V_BlockWrite(xpos,xpos+8,ypos,ypos+26);
		A0_DATA;
		for(column_no=0;column_no<3;column_no++)
		{
			ascii_inc=column_no;
			for(No_of_bits=0;No_of_bits<8;No_of_bits++)
			{
				result_multiplier=multiplier[No_of_bits];
				for(row_wise_column_no=ascii_inc;row_wise_column_no<(27);row_wise_column_no+=3)				//57 wid
				{
				data =ascii_buff[row_wise_column_no];

				if((data & result_multiplier)==result_multiplier)
				{
					//	SPI_data(fill_color>>16);
					//	SPI_data(fill_color>>8);
					//	SPI_data(fill_color);
						SPI_transfer(__16_bit_color,_16_BIT);
				}
				else
				{
				//	SPI_data(boarder_color>>16);
				//	SPI_data(boarder_color>>8);
				//	SPI_data(boarder_color);
					SPI_transfer(__16_bit_color_1,_16_BIT);
				}
				}
			}
		}
	return ypos;
}

//uint8_t TFT_ST7789V_Left_Arrow_35pt(uint16_t xpos, uint16_t ypos,
//											uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)
//{
//	uint8_t ascii_buff[64];
//	uint32_t add=0;
//	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
//	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//	uint8_t result_multiplier=0;
//	uint16_t __16_bit_color=0;
//	uint16_t __16_bit_color_1=0;
//
//	__16_bit_color	=__24_bit_to_16_bit_color(fill_color);
//	__16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);
//
//	for(row=0;row<64;row++)
//	{
//		ascii_buff[row]=Left_arrow_35ptBitmaps[row+add];
//	}
//	TFT_ST7789V_BlockWrite(xpos,xpos+31,ypos,ypos+63);
//	A0_DATA;
//	for(column_no=0;column_no<2;column_no++)
//	{
//		ascii_inc=column_no;
//		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
//		{
//			result_multiplier=multiplier[No_of_bits];
//			for(row_wise_column_no=ascii_inc;row_wise_column_no<(64);row_wise_column_no+=2)				//57 wid
//			{
//				data =ascii_buff[row_wise_column_no];
//
//				if((data & result_multiplier)==result_multiplier)
//					{
//					//	SPI_data(fill_color>>16);
//					//	SPI_data(fill_color>>8);
//					//	SPI_data(fill_color);
//					SPI_transfer(__16_bit_color,_16_BIT);
//
//				}
//				else
//				{
//					//													SPI_data(boarder_color>>16);
//					//													SPI_data(boarder_color>>8);
//					//													SPI_data(boarder_color);
//					SPI_transfer(__16_bit_color_1,_16_BIT);
//
//				}
//			}
//		}
//	}
//	return ypos;
//}

//uint8_t TFT_ST7789V_mains_symbol_25pt(uint16_t xpos, uint16_t ypos,
//											uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)
//	{
//		uint8_t ascii_buff[30];
//		uint32_t add=0;
//		uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
//		uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//		uint8_t result_multiplier=0;
//
//		for(row=0;row<30;row++)
//			{
//				ascii_buff[row]=segoeFluentIcons_9ptBitmaps[row+add];
//			}
//				TFT_ST7789V_BlockWrite(xpos,xpos+14,ypos,ypos+29);
//				A0_DATA;
//				for(column_no=0;column_no<2;column_no++)
//					{
//						ascii_inc=column_no;
//						for(No_of_bits=0;No_of_bits<8;No_of_bits++)
//							{
//								result_multiplier=multiplier[No_of_bits];
//									for(row_wise_column_no=ascii_inc;row_wise_column_no<(30);row_wise_column_no+=2)				//57 wid
//										{
//											data =ascii_buff[row_wise_column_no];
//
//												if((data & result_multiplier)==result_multiplier)
//													{
//														SPI_data(fill_color>>16);
//														SPI_data(fill_color>>8);
//														SPI_data(fill_color);
//													}
//												else
//													{
//													SPI_data(boarder_color>>16);
//													SPI_data(boarder_color>>8);
//													SPI_data(boarder_color);
//													}
//										}
//							}
//					}
//				return ypos;
//	}

//uint8_t ZOOZ_LOGO(uint16_t xpos, uint16_t ypos,
//											uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)
//{
//	uint8_t ascii_buff[2160];
//	uint32_t add=0;
//	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
//	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//	uint8_t result_multiplier=0;
//
//	uint8_t red = 0;
//	uint8_t green = 0;
//	uint8_t blue = 0;
//
//	uint16_t __16_bit_color,__16_bit_color1 = 0;
//
//
//	red = (fill_color&0xFF0000)>>16;
//	green =(fill_color&0x00FF00)>>8;
//	blue = (fill_color&0x0000FF);
//	red = ((float)red * 0.12);
//	green = ((float)green * 0.24);
//	blue = ((float)blue * 0.12);
//	__16_bit_color = (red<<11)|(green<<5)|(blue);
//
//	red = (boarder_color&0xFF0000)>>16;
//	green =(boarder_color&0x00FF00)>>8;
//	blue = (boarder_color&0x0000FF);
//	red = ((float)red * 0.12);
//	green = ((float)green * 0.24);
//	blue = ((float)blue * 0.12);
//	__16_bit_color1 = (red<<11)|(green<<5)|(blue);
//
//	for(row=0;row<2160;row++)
//	{
//		ascii_buff[row]=ZOOZ_LOGO_PNG[row+add];
//	}
//	TFT_ST7789V_BlockWrite(xpos,xpos+89,ypos,ypos+500);
//	A0_DATA;
//	for(column_no=0;column_no<24;column_no++)
//	{
//		ascii_inc=column_no;
//		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
//		{
//			result_multiplier=multiplier[No_of_bits];
//			for(row_wise_column_no=ascii_inc;row_wise_column_no<(2160);row_wise_column_no+=24)				//57 wid
//			{
//				data =ascii_buff[row_wise_column_no];
//
//				if((data & result_multiplier)==result_multiplier)
//				{
//					SPI_transfer(__16_bit_color,_16_BIT);
//					//														SPI_data(fill_color>>16);
//					//														SPI_data(fill_color>>8);
//					//														SPI_data(fill_color);
//				}
//				else
//				{
//
//					SPI_transfer(__16_bit_color1,_16_BIT);
//					//													SPI_data(boarder_color>>16);
//					//													SPI_data(boarder_color>>8);
//					//													SPI_data(boarder_color);
//				}
//			}
//		}
//	}
//	return ypos;
//}

uint8_t AKAS_LOGO(uint16_t xpos, uint16_t ypos,
											uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)
{
	uint8_t ascii_buff[1500];
	uint32_t add=0;
	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	uint8_t result_multiplier=0;

	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;

	uint16_t __16_bit_color,__16_bit_color1 = 0;


	red = (fill_color&0xFF0000)>>16;
	green =(fill_color&0x00FF00)>>8;
	blue = (fill_color&0x0000FF);
	red = ((float)red * 0.12);
	green = ((float)green * 0.24);
	blue = ((float)blue * 0.12);
	__16_bit_color = (red<<11)|(green<<5)|(blue);

	red = (boarder_color&0xFF0000)>>16;
	green =(boarder_color&0x00FF00)>>8;
	blue = (boarder_color&0x0000FF);
	red = ((float)red * 0.12);
	green = ((float)green * 0.24);
	blue = ((float)blue * 0.12);
	__16_bit_color1 = (red<<11)|(green<<5)|(blue);

	for(row=0;row<1500;row++)
	{
		ascii_buff[row]=lOGO_1100[row+add];
	}
	TFT_ST7789V_BlockWrite(xpos,xpos+59,ypos,ypos+500);
	A0_DATA;
	for(column_no=0;column_no<25;column_no++)
	{
		ascii_inc=column_no;
		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
		{
			result_multiplier=multiplier[No_of_bits];
			for(row_wise_column_no=ascii_inc;row_wise_column_no<(1500);row_wise_column_no+=25)				//57 wid
			{
				data =ascii_buff[row_wise_column_no];

				if((data & result_multiplier)==result_multiplier)
				{
					SPI_transfer(__16_bit_color,_16_BIT);
					//														SPI_data(fill_color>>16);
					//														SPI_data(fill_color>>8);
					//														SPI_data(fill_color);
				}
				else
				{

					SPI_transfer(__16_bit_color1,_16_BIT);
					//													SPI_data(boarder_color>>16);
					//													SPI_data(boarder_color>>8);
					//													SPI_data(boarder_color);
				}
			}
		}
	}
	return ypos;
}

//uint8_t TFT_ST7789V_power_10pt(uint16_t xpos, uint16_t ypos,
//											uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)
//	{
//		uint8_t ascii_buff[24];
//		uint32_t add=0;
//		uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
//		uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//		uint8_t result_multiplier=0;
//
//		for(row=0;row<24;row++)
//			{
//				ascii_buff[row]=Left_arrow_35ptBitmaps[row+add];
//			}
//				TFT_ST7789V_BlockWrite(xpos,xpos+11,ypos,ypos+23);
//				A0_DATA;
//				for(column_no=0;column_no<2;column_no++)
//					{
//						ascii_inc=column_no;
//						for(No_of_bits=0;No_of_bits<8;No_of_bits++)
//							{
//								result_multiplier=multiplier[No_of_bits];
//									for(row_wise_column_no=ascii_inc;row_wise_column_no<(24);row_wise_column_no+=2)				//57 wid
//										{
//											data =ascii_buff[row_wise_column_no];
//
//												if((data & result_multiplier)==result_multiplier)
//													{
//														SPI_data(fill_color>>16);
//														SPI_data(fill_color>>8);
//														SPI_data(fill_color);
//													}
//												else
//													{
//													SPI_data(boarder_color>>16);
//													SPI_data(boarder_color>>8);
//													SPI_data(boarder_color);
//													}
//										}
//							}
//					}
//				return ypos;
//	}

uint16_t Right_Alignments(uint8_t *asciichar,uint16_t pos)
{
	uint16_t total_pixel=0;
	uint8_t count=0;
	uint16_t chr_width=0;
	//		uint16_t TFT_centre_pxl=160;
	uint16_t xpos=0;
	struct get_char_info char_info;

	while(*asciichar)
	{
		char_info=	ascii_details(*asciichar);
		total_pixel=char_info.chr_width_in_pixels+total_pixel;
		count++;
		asciichar++;
	}
	chr_width=total_pixel+((count-1)*3);
	xpos=pos-chr_width;

	//			Font_Select(ARIAL_9);
	//		    TFT_Number(50,60,xpos,YELLOW,BLACK);

	return xpos;
}

uint16_t Dosage_Alignments(uint8_t *asciichar)
{
	uint16_t total_pixel=0;
	uint8_t count=0;
	uint16_t xpos=0;
	uint16_t chr_width=0;
	//		uint16_t TFT_centre_pxl=160;
	struct get_char_info char_info;

	while(*asciichar)
	{
		char_info=	ascii_details(*asciichar);
		total_pixel=char_info.chr_width_in_pixels+total_pixel;
		count++;
//		Font_Select(ARIAL_14);
//		TFT_Number(150,150,count,BLACK,RED);
		asciichar++;
	}
	chr_width=total_pixel+((count-1)*3);
	xpos=chr_width;

	//Font_Select(ARIAL_9);
	//TFT_Number(50,60,xpos,YELLOW,BLACK);

	return xpos;
}

uint16_t Flow_rate_Alignments(uint8_t *asciichar)
{
		uint16_t total_pixel=0;
		uint8_t count=0;
		uint16_t chr_width=0;
//		uint16_t TFT_centre_pxl=160;
		uint16_t xpos=0;
		struct get_char_info char_info;

//		if(*asciichar==0)
//		{
//			total_pixel=31;
//		}
	//	else
		//{
		while(*asciichar)
			{

			 char_info=	ascii_details(*asciichar);
			 total_pixel=char_info.chr_width_in_pixels+total_pixel;
			 count++;


			 asciichar++;

			}
		decimal_limit=count;
		//}
		    chr_width=total_pixel+((count-1)*3);
		    xpos=191-chr_width;
//		    if(chr_width<195)
//		    {
//		    xpos=195-chr_width;
//		    }
		    //debug_temp=total_pixel;
//		    Font_Select(ARIAL_9);
//		    TFT_Number(0,20,total_pixel,YELLOW,BLACK);
//			Font_Select(ARIAL_9);
//		    TFT_Number(50,60,xpos,YELLOW,BLACK);
//		    delay_secs(1);

		    return xpos;

}


/*uint16_t Bolus_rate_Alignments(uint8_t *asciichar)
	{
		uint16_t total_pixel=0;
		uint8_t count=0;
		uint16_t chr_width=0;
//		uint16_t TFT_centre_pxl=160;
		uint16_t xpos=0;
		struct get_char_info char_info;

//		if(*asciichar==0)
//		{
//			total_pixel=31;
//		}
	//	else
		//{
		while(*asciichar)
			{

			 char_info=	ascii_details(*asciichar);
			 total_pixel=char_info.chr_width_in_pixels+total_pixel;
			 count++;
			 asciichar++;

			}
		//}
		    chr_width=total_pixel+((count-1)*3);
		    xpos=245-chr_width;

//		    Font_Select(ARIAL_9);
//		    TFT_Number(0,20,total_pixel,YELLOW,BLACK);
//			Font_Select(ARIAL_9);
//		    TFT_Number(50,60,xpos,YELLOW,BLACK);

		    return xpos;

	}*/

uint8_t TFT_ST7789V_Triangle_24pt(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)
{
	uint8_t ascii_buff[92];//28
    uint32_t add=0;//28
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
	uint16_t __16_bit_color_1=0;

	__16_bit_color	=__24_bit_to_16_bit_color(fill_color);
	__16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);
    for(row=0;row<92;row++)
	{
			ascii_buff[row]=wingdings3_24ptBitmaps[row+add];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+22,ypos,ypos+92);

    A0_DATA;

	for(column_no=0;column_no<4;column_no++)
	{
	ascii_inc=column_no;
   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
  	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(92);row_wise_column_no+=4)				//57 wid
    {
 	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
    {
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
		SPI_transfer( __16_bit_color,_16_BIT);
    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
    SPI_transfer( __16_bit_color_1,_16_BIT);
    }
    }
    }
    }
    return ypos;   //+16;
    }//Size 20
//   {
//	uint8_t ascii_buff[130];//28
//    uint32_t add=0;//28
//    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
//    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//    uint8_t result_multiplier=0;
//    uint16_t __16_bit_color=0;
//    uint16_t __16_bit_color_1=0;
//
//    __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
//    __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);
//
//
//    for(row=0;row<130;row++)
//	{
//			ascii_buff[row]=wingdings3_24ptBitmaps[row+add];
//	}
//
//    TFT_ST7789V_BlockWrite(xpos,xpos+25,ypos,ypos+129);
//
//    A0_DATA;
//
//	for(column_no=0;column_no<5;column_no++)
//	{
//	ascii_inc=column_no;
//   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
//	{
//  	result_multiplier=multiplier[No_of_bits];
//	for(row_wise_column_no=ascii_inc;row_wise_column_no<(130);row_wise_column_no+=5)				//57 wid
//    {
// 	data =ascii_buff[row_wise_column_no];
//
//	if((data & result_multiplier)==result_multiplier)
//    {
//   // spi_write_data(fill_color>>16);
//  //  spi_write_data(fill_color>>8);
//   // spi_write_data(fill_color);
//      SPI_transfer(__16_bit_color,_16_BIT);
//
//    }
//    else
//    {
// //  spi_write_data(boarder_color>>16);
// //  spi_write_data(boarder_color>>8);
// //  spi_write_data(boarder_color);
//     SPI_transfer( __16_bit_color_1,_16_BIT);
//
//    }
//    }
//    }
//    }
//    return ypos;   //+16;
//   }

uint8_t TFT_ST7789V_Exclamatory_9pt_arial(uint16_t xpos, uint16_t ypos,
                                         uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[12];

	uint32_t add=arrow;
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;

    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

        __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
        __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);

    for(row=0;row<12;row++)
	{
			ascii_buff[row]=arial_7ptBitmaps [add+row];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+11,ypos,ypos+5);

    A0_DATA;

	for(column_no=0;column_no<1;column_no++)
	{
	ascii_inc=column_no;
	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(12);row_wise_column_no+=1)				//57 wid
    {
	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
	{
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
    SPI_transfer(__16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
      SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }

//uint8_t TFT_ST7789V_Left_Arrow_22pt(uint16_t xpos, uint16_t ypos,
//       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
//   {
//	uint8_t ascii_buff[44];//28
//    uint32_t add=0;//28
//    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
//    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//    uint8_t result_multiplier=0;
//    uint16_t __16_bit_color=0;
//    uint16_t __16_bit_color_1=0;
//
//   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
//   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);
//
//
//   for(row=0;row<44;row++)
//	{
//			ascii_buff[row]=webdings_22ptBitmaps[row+add];
//	}
//
//    TFT_ST7789V_BlockWrite(xpos,xpos+21,ypos,ypos+44);
//
//    A0_DATA;
//
//	for(column_no=0;column_no<2;column_no++)
//	{
//	ascii_inc=column_no;
//   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
//	{
//  	result_multiplier=multiplier[No_of_bits];
//	for(row_wise_column_no=ascii_inc;row_wise_column_no<(44);row_wise_column_no+=2)				//57 wid
//    {
// 	data =ascii_buff[row_wise_column_no];
//
//	if((data & result_multiplier)==result_multiplier)
//    {
////    spi_write_data(fill_color>>16);
////    spi_write_data(fill_color>>8);
////    spi_write_data(fill_color);
//	  SPI_transfer(__16_bit_color,_16_BIT);
//
//    }
//    else
//    {
////    spi_write_data(boarder_color>>16);
////    spi_write_data(boarder_color>>8);
////    spi_write_data(boarder_color);
//      SPI_transfer( __16_bit_color_1,_16_BIT);
//
//    }
//    }
//    }
//    }
//    return ypos;   //+16;
//    }

uint32_t TFT_ST7789V_ALARM_ARROW_20pt(uint16_t xpos, uint16_t ypos,
                                         uint8_t arrow, uint32_t boarder_color, uint32_t fill_color,uint8_t L_R_Flag)                               //Size 20
   {
	uint8_t ascii_buff[34];

	uint32_t add=0;
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;

    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

    __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
    __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);

    for(row=0;row<34;row++)
	{
			if(L_R_Flag)ascii_buff[row]=wingdings3_R_ARROW_20ptBitmaps[add+row];
			else  ascii_buff[row]=wingdings3_L_ARROW_13ptBitmaps[add+row];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+16,ypos,ypos+34);

    A0_DATA;

	for(column_no=0;column_no<2;column_no++)
	{
	ascii_inc=column_no;
	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(34);row_wise_column_no+=2)				//57 wid
    {
	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
	{
//   spi_write_data(fill_color>>16);
//   spi_write_data(fill_color>>8);
//   spi_write_data(fill_color);
    SPI_transfer( __16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
    SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }
uint8_t TFT_ST7789V_Warning_symbol_18pt(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[66];

	uint32_t add=arrow*66;
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

    __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
    __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);
    for(row=0;row<66;row++)
	{
			ascii_buff[row]= Warning_symbol_18ptBitmaps[add+row];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+21,ypos,ypos+66);

    A0_DATA;

	for(column_no=0;column_no<3;column_no++)
	{
	ascii_inc=column_no;
	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(66);row_wise_column_no+=3)				//57 wid
    {
	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
	{
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
		 SPI_transfer( __16_bit_color,_16_BIT);
    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
    	 SPI_transfer( __16_bit_color_1,_16_BIT);
   }
   }
   }
  }
    return ypos;   //+16;
    }
uint8_t TFT_ST7789V_Mute_Symbol_12pt(uint16_t xpos, uint16_t ypos,
                                         uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[30];

	uint32_t add=arrow*30;
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

    __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
    __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);

    for(row=0;row<30;row++)
	{
			ascii_buff[row]=Mute_Symbol_12pt [add+row];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+14,ypos,ypos+30);

    A0_DATA;

	for(column_no=0;column_no<2;column_no++)
	{
	ascii_inc=column_no;
	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(30);row_wise_column_no+=2)				//57 wid
    {
	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
	{
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
		 SPI_transfer( __16_bit_color,_16_BIT);
    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
    	 SPI_transfer( __16_bit_color_1,_16_BIT);
    }
    }
    }
    }
    return ypos;   //+16;
    }

//TFT_Time_disp(20,60,(((G_stand_min/60)*100)+(G_stand_min%60)),MAIN_FRAME_COL,WHITE);
void TFT_Time_disp(uint16_t xpos,uint16_t ypos,uint32_t number,uint32_t boarder_color,uint32_t fill_color)
	{
    	char local_buffer[8];

    	if(number>9959)
    	{
    		number=9959;
    	}
		sprintf(local_buffer,"%02d",number/100);    //store the integer variables in to string using sprintf
		xpos=TFT_String(xpos,ypos,(uint8_t*)local_buffer,boarder_color,fill_color);
		xpos=TFT_String(xpos,ypos,(uint8_t*)":",boarder_color,fill_color);
		sprintf(local_buffer,"%02d",number%100);    //store the integer variables in to string using sprintf
		TFT_String(xpos,ypos,(uint8_t*)local_buffer,boarder_color,fill_color);
//		xpos=TFT_String(190,60,(uint8_t*)":",boarder_color,fill_color);

//		Font_Select(BEBAS_36);
//
//		TFT_Number(200,60,G_stand_sec,LIGHT_GRAY,MAIN_FRAME_COL);


	}



//uint8_t TFT_ST7789V_micro_14pt(uint16_t xpos, uint16_t ypos,
//                                         uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
//   {
//	uint8_t ascii_buff[32];
//
//	uint32_t add=arrow*32;
//    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
//    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//    uint8_t result_multiplier=0;
//    uint16_t __16_bit_color=0;
//    uint16_t __16_bit_color_1=0;
//
//    __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
//    __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);
//
//    for(row=0;row<32;row++)
//	{
//			ascii_buff[row]=segoeUISymbol_14ptBitmaps [add+row];
//	}
//
//    TFT_ST7789V_BlockWrite(xpos,xpos+15,ypos,ypos+32);
//
//    A0_DATA;
//
//	for(column_no=0;column_no<2;column_no++)
//	{
//	ascii_inc=column_no;
//	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
//	{
//	result_multiplier=multiplier[No_of_bits];
//	for(row_wise_column_no=ascii_inc;row_wise_column_no<(32);row_wise_column_no+=2)				//57 wid
//    {
//	data =ascii_buff[row_wise_column_no];
//
//	if((data & result_multiplier)==result_multiplier)
//	{
////    spi_write_data(fill_color>>16);
////    spi_write_data(fill_color>>8);
////    spi_write_data(fill_color);
//		 SPI_transfer( __16_bit_color,_16_BIT);
//    }
//    else
//    {
////    spi_write_data(boarder_color>>16);
////    spi_write_data(boarder_color>>8);
////    spi_write_data(boarder_color);
//    	 SPI_transfer( __16_bit_color_1,_16_BIT);
//    }
//    }
//    }
//    }
//    return ypos;   //+16;
//    }


uint8_t TFT_ST7789V_star_14(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[30];//28

    uint32_t add=0;//28
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


   for(row=0;row<30;row++)
	{
			ascii_buff[row]=star_14ptBitmaps[row+add];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+14,ypos,ypos+30);

    A0_DATA;

	for(column_no=0;column_no<2;column_no++)	//no.of column
	{
	ascii_inc=column_no;
   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
  	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(30);row_wise_column_no+=2)				//total cal value..array[index value]
    {
 	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
    {
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
	  SPI_transfer(__16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
      SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }


//uint8_t TFT_ST7789V_thunder_8_pt(uint16_t xpos, uint16_t ypos,
//       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
//   {
//	uint8_t ascii_buff[10];//28
//
//    uint32_t add=0;//28
//    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
//    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//    uint8_t result_multiplier=0;
//    uint16_t __16_bit_color=0;
//    uint16_t __16_bit_color_1=0;
//
//   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
//   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);
//
//
//   for(row=0;row<10;row++)
//	{
//			ascii_buff[row]=segoeFluentIcons_9ptBitmaps[row+add];
//	}
//
//    TFT_ST7789V_BlockWrite(xpos,xpos+9,ypos,ypos+10);
//
//    A0_DATA;
//
//	for(column_no=0;column_no<1;column_no++)	//no.of column
//	{
//	ascii_inc=column_no;
//   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
//	{
//  	result_multiplier=multiplier[No_of_bits];
//	for(row_wise_column_no=ascii_inc;row_wise_column_no<(10);row_wise_column_no+=1)				//total cal value..array[index value]
//    {
// 	data =ascii_buff[row_wise_column_no];
//
//	if((data & result_multiplier)==result_multiplier)
//    {
////    spi_write_data(fill_color>>16);
////    spi_write_data(fill_color>>8);
////    spi_write_data(fill_color);
//	  SPI_transfer(__16_bit_color,_16_BIT);
//
//    }
//    else
//    {
////    spi_write_data(boarder_color>>16);
////    spi_write_data(boarder_color>>8);
////    spi_write_data(boarder_color);
//      SPI_transfer( __16_bit_color_1,_16_BIT);
//
//    }
//    }
//    }
//    }
//    return ypos;   //+16;
//    }

uint8_t TFT_ST7789V_BOL_search(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[66];//28

    uint32_t add=0;//28
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


   for(row=0;row<66;row++)
	{
			ascii_buff[row]=segoeFluentIcons_18ptBitmaps[row+add];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+21,ypos,ypos+66);

    A0_DATA;

	for(column_no=0;column_no<3;column_no++)	//no.of column
	{
	ascii_inc=column_no;
   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
  	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(66);row_wise_column_no+=3)				//total cal value..array[index value]
    {
 	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
    {
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
	  SPI_transfer(__16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
      SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }

uint8_t TFT_ST7789V_power_28pt_arial(uint16_t xpos, uint16_t ypos,
                                         uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[276];//28

	    uint32_t add=0;//28
	    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
	    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	    uint8_t result_multiplier=0;
	    uint16_t __16_bit_color=0;
	    uint16_t __16_bit_color_1=0;

	   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
	   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


	   for(row=0;row<276;row++)
		{
				ascii_buff[row]=power_28ptBitmaps[row+add];
		}

	    TFT_ST7789V_BlockWrite(xpos,xpos+45,ypos,ypos+276);

	    A0_DATA;

		for(column_no=0;column_no<6;column_no++)	//no.of column
		{
		ascii_inc=column_no;
	   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
		{
	  	result_multiplier=multiplier[No_of_bits];
		for(row_wise_column_no=ascii_inc;row_wise_column_no<(276);row_wise_column_no+=6)				//total cal value..array[index value]
	    {
	 	data =ascii_buff[row_wise_column_no];

		if((data & result_multiplier)==result_multiplier)
	    {
	//    spi_write_data(fill_color>>16);
	//    spi_write_data(fill_color>>8);
	//    spi_write_data(fill_color);
		  SPI_transfer(__16_bit_color,_16_BIT);

	    }
	    else
	    {
	//    spi_write_data(boarder_color>>16);
	//    spi_write_data(boarder_color>>8);
	//    spi_write_data(boarder_color);
	      SPI_transfer( __16_bit_color_1,_16_BIT);

	    }
	    }
	    }
	    }
	    return ypos;   //+16;
	    }
uint8_t TFT_ST7789V_standby_28pt_arial(uint16_t xpos, uint16_t ypos,
                                         uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
{
	uint8_t ascii_buff[276];//28

	uint32_t add=0;//28
	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	uint8_t result_multiplier=0;
	uint16_t __16_bit_color=0;
	uint16_t __16_bit_color_1=0;

   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


   for(row=0;row<276;row++)
	{
			ascii_buff[row]=standby_28ptBitmaps[row+add];
	}

	TFT_ST7789V_BlockWrite(xpos,xpos+45,ypos,ypos+276);

	A0_DATA;

	for(column_no=0;column_no<6;column_no++)	//no.of column
	{
		ascii_inc=column_no;
		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
		{
			result_multiplier=multiplier[No_of_bits];
			for(row_wise_column_no=ascii_inc;row_wise_column_no<(276);row_wise_column_no+=6)				//total cal value..array[index value]
			{
				data =ascii_buff[row_wise_column_no];

				if((data & result_multiplier)==result_multiplier)
				{
			//    spi_write_data(fill_color>>16);
			//    spi_write_data(fill_color>>8);
			//    spi_write_data(fill_color);
				  SPI_transfer(__16_bit_color,_16_BIT);

				}
				else
				{
			//    spi_write_data(boarder_color>>16);
			//    spi_write_data(boarder_color>>8);
			//    spi_write_data(boarder_color);
				  SPI_transfer( __16_bit_color_1,_16_BIT);

				}
			}
		}
	}
	return ypos;   //+16;
}
uint8_t right_14(uint16_t xpos, uint16_t ypos,    //22-03
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)
{

	uint8_t ascii_buff[54];
	uint32_t add=arrow*54;
	uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
	uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	uint8_t result_multiplier=0;
	uint16_t __16_bit_color=0;
	uint16_t __16_bit_color_1=0;

	__16_bit_color	=__24_bit_to_16_bit_color(fill_color);
	__16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);




	for(row=0;row<54;row++)
	{
		ascii_buff[row]= right_14ptBitmaps[add+row];
	}

	TFT_ST7789V_BlockWrite(xpos,xpos+17,ypos,ypos+54);

	A0_DATA;

	for(column_no=0;column_no<3;column_no++)
	{
		ascii_inc=column_no;
		for(No_of_bits=0;No_of_bits<8;No_of_bits++)
		{
			result_multiplier=multiplier[No_of_bits];
			for(row_wise_column_no=ascii_inc;row_wise_column_no<(54);row_wise_column_no+=3)
			{
				data =ascii_buff[row_wise_column_no];

				if((data & result_multiplier)==result_multiplier)
				{
					//    spi_write_data(fill_color>>16);
					//    spi_write_data(fill_color>>8);
					//    spi_write_data(fill_color);
					SPI_transfer( __16_bit_color,_16_BIT);
				}
				else
				{
					//    spi_write_data(boarder_color>>16);
					//    spi_write_data(boarder_color>>8);
					//    spi_write_data(boarder_color);
					SPI_transfer( __16_bit_color_1,_16_BIT);
				}
			}
		}
	}
	return ypos;   //+16;
}

void syringe_symbol()

{
	uint8_t startx=0;
		uint8_t  endy;
		endy=3;

	Rectangle(30+45+3+2,124-2,1,12,BLACK);
	Rectangle(31+45+3+2,126-2,7-3,1,BLACK);
	Rectangle(31+45+3+2,133-2,7-3,1,BLACK);
	Rectangle(39+45+2,120-2,1,20,BLACK);

	Rectangle(40+45+2,124-2,38,1,BLACK);
	Rectangle(40+45+2,135-2,38,1,BLACK);
	Rectangle(78+45+2,124-2,1,3,BLACK);
	Rectangle(78+45+2,133-2,1,3,BLACK);

	Rectangle(78+45+2,127-2,5,1,BLACK);
	Rectangle(78+45+2,132-2,5,1,BLACK);
	Rectangle(83+45+2,127-2,1,6,BLACK);

	Rectangle(83+45+2,130-2,20,0,BLACK);


	for(startx=85+2;startx<=123;startx=startx+2)
	{
		if(startx==95+2||startx==105+2||startx==115+2)
		{
			endy=endy+2;
		}
		Rectangle(startx,124-2,0,endy,BLACK);
		endy=3;
	}
}

uint8_t TFT_ST7789V_Summation_symbol(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[36];//28

    uint32_t add=0;//28
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


   for(row=0;row<36;row++)
	{
		ascii_buff[row]=segoeUISymbol_12ptBitmaps[row+add];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+17,ypos,ypos+36);

    A0_DATA;

	for(column_no=0;column_no<2;column_no++)	//no.of column
	{
	ascii_inc=column_no;
   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
  	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(36);row_wise_column_no+=2)				//total cal value..array[index value]
    {
 	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
    {
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
	  SPI_transfer(__16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
      SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }

uint8_t TFT_ST7789V_triangle2(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[27];//28

    uint32_t add=0;//28
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


   for(row=0;row<27;row++)
	{
		ascii_buff[row]=segoeUISymbol_9ptBitmaps[row+add];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+8,ypos,ypos+27);

    A0_DATA;

	for(column_no=0;column_no<3;column_no++)	//no.of column
	{
	ascii_inc=column_no;
   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
  	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(27);row_wise_column_no+=3)				//total cal value..array[index value]
    {
 	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
    {
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
	  SPI_transfer(__16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
      SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }

uint8_t TFT_ST7789V_Triangle1(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[140];//28

    uint32_t add=0;//28
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


   for(row=0;row<140;row++)
	{
		ascii_buff[row]=segoeUISymbol_18ptBitmaps[row+add];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+27,ypos,ypos+140);

    A0_DATA;

	for(column_no=0;column_no<5;column_no++)	//no.of column
	{
	ascii_inc=column_no;
   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
  	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(140);row_wise_column_no+=5)				//total cal value..array[index value]
    {
 	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
    {
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
	  SPI_transfer(__16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
      SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }

uint8_t TFT_ST7789V_rightarrow_symbol(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[75];//28

    uint32_t add=0;//28
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


   for(row=0;row<75;row++)
	{
		ascii_buff[row]=segoeUISymbol_22ptBitmaps[row+add];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+24,ypos,ypos+75);

    A0_DATA;

	for(column_no=0;column_no<3;column_no++)	//no.of column
	{
	ascii_inc=column_no;
   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
  	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(75);row_wise_column_no+=3)				//total cal value..array[index value]
    {
 	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
    {
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
	  SPI_transfer(__16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
      SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }

uint8_t warning_triangle_2(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[175];//28

    uint32_t add=0;//28
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


   for(row=0;row<175;row++)
	{
		ascii_buff[row]=warn_triangle[row+add];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+34,ypos,ypos+75);

    A0_DATA;

	for(column_no=0;column_no<5;column_no++)	//no.of column
	{
	ascii_inc=column_no;
   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
  	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(175);row_wise_column_no+=5)				//total cal value..array[index value]
    {
 	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
    {
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
	  SPI_transfer(__16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
      SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }

//uint8_t warning_exclaimatory_1(uint16_t xpos, uint16_t ypos,
//       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
//   {
//	uint8_t ascii_buff[27];//28
//
//    uint32_t add=0;//28
//    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
//    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
//    uint8_t result_multiplier=0;
//    uint16_t __16_bit_color=0;
//    uint16_t __16_bit_color_1=0;
//
//   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
//   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);
//
//
//   for(row=0;row<39;row++)
//	{
//		ascii_buff[row]=warn_exclaim[row+add];
//	}
//
//    TFT_ST7789V_BlockWrite(xpos,xpos+26,ypos,ypos+75);
//
//    A0_DATA;
//
//	for(column_no=0;column_no<1;column_no++)	//no.of column
//	{
//	ascii_inc=column_no;
//   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
//	{
//  	result_multiplier=multiplier[No_of_bits];
//	for(row_wise_column_no=ascii_inc;row_wise_column_no<(27);row_wise_column_no+=1)				//total cal value..array[index value]
//    {
// 	data =ascii_buff[row_wise_column_no];
//
//	if((data & result_multiplier)==result_multiplier)
//    {
////    spi_write_data(fill_color>>16);
////    spi_write_data(fill_color>>8);
////    spi_write_data(fill_color);
//	  SPI_transfer(__16_bit_color,_16_BIT);
//
//    }
//    else
//    {
////    spi_write_data(boarder_color>>16);
////    spi_write_data(boarder_color>>8);
////    spi_write_data(boarder_color);
//      SPI_transfer( __16_bit_color_1,_16_BIT);
//
//    }
//    }
//    }
//    }
//    return ypos;   //+16;
//    }

uint8_t warning_triangle_1(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[343];//28

    uint32_t add=0;//28
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


   for(row=0;row<343;row++)
	{
		ascii_buff[row]=warn_triangle_2[row+add];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+48,ypos,ypos+75);

    A0_DATA;

	for(column_no=0;column_no<7;column_no++)	//no.of column
	{
	ascii_inc=column_no;
   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
  	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(343);row_wise_column_no+=7)				//total cal value..array[index value]
    {
 	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
    {
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
	  SPI_transfer(__16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
      SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }

uint8_t warning_exclaimatory_1(uint16_t xpos, uint16_t ypos,
       uint8_t arrow, uint32_t boarder_color, uint32_t fill_color)                               //Size 20
   {
	uint8_t ascii_buff[29];//28

    uint32_t add=0;//28
    uint16_t row,column_no,row_wise_column_no,No_of_bits,data=0,ascii_inc=0;   //shift_count;
    uint8_t multiplier[8]={0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    uint8_t result_multiplier=0;
    uint16_t __16_bit_color=0;
    uint16_t __16_bit_color_1=0;

   __16_bit_color	=__24_bit_to_16_bit_color(fill_color);
   __16_bit_color_1=__24_bit_to_16_bit_color(boarder_color);


   for(row=0;row<29;row++)
	{
		ascii_buff[row]=warn_exclaim_2[row+add];
	}

    TFT_ST7789V_BlockWrite(xpos,xpos+28,ypos,ypos+75);

    A0_DATA;

	for(column_no=0;column_no<1;column_no++)	//no.of column
	{
	ascii_inc=column_no;
   	for(No_of_bits=0;No_of_bits<8;No_of_bits++)
	{
  	result_multiplier=multiplier[No_of_bits];
	for(row_wise_column_no=ascii_inc;row_wise_column_no<(29);row_wise_column_no+=1)				//total cal value..array[index value]
    {
 	data =ascii_buff[row_wise_column_no];

	if((data & result_multiplier)==result_multiplier)
    {
//    spi_write_data(fill_color>>16);
//    spi_write_data(fill_color>>8);
//    spi_write_data(fill_color);
	  SPI_transfer(__16_bit_color,_16_BIT);

    }
    else
    {
//    spi_write_data(boarder_color>>16);
//    spi_write_data(boarder_color>>8);
//    spi_write_data(boarder_color);
      SPI_transfer( __16_bit_color_1,_16_BIT);

    }
    }
    }
    }
    return ypos;   //+16;
    }
