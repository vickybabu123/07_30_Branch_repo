#include "LPC55S16.h"
#include "TFT_Commands.h"
#include <stdio.h>
#include "stdint.h"
#include <string.h>
#include <math.h>
#include <color.h>
#include <Shapes.h>
#include "Driver_SPI.h"
#include "Driver_Timer.h"

void lcd_drawline(uint16_t start_xpos, uint16_t start_ypos, uint16_t end_xpos, uint16_t end_ypos, uint32_t Color);

void Rectangle(uint16_t x_axis,uint16_t y_axis,uint16_t Length,uint16_t Height,uint32_t Color)
	{
		uint16_t x=0,y=0;
		uint16_t __16_bit_color=0;
		__16_bit_color=__24_bit_to_16_bit_color(Color);
		TFT_ST7789V_BlockWrite(y_axis,y_axis+Height,x_axis,x_axis+Length);
	 	A0_DATA;

		for(x=0 ; x<=Length; x++)
		{
		for(y=0; y<= Height ; y++)
		{
	//	A0_DATA;
	//	SPI_data(Color>>16);
	//	SPI_data(Color>>8);
	//	SPI_data(Color);

        SPI_transfer(__16_bit_color,_16_BIT);

	//	SPI_transfer(NAVY);
	//	delay(1);
	//	SPI_transfer(Color>>16);
	//	SPI_transfer(Color>>8);
	//	SPI_transfer(Color);
	     }
//		 delay_100us(10);
	   }
//        delay_us(1);

	}

void TFT_ST7789V__rectangle_curve_down(uint16_t xpos1, uint16_t ypos1, uint16_t width,
		uint16_t height,uint16_t radius,uint32_t colour)
	{
	uint16_t tempx=0;
	uint16_t tempy=0;
	uint16_t tempz=0;
	//uint16_t  length;

		height+= xpos1;
		width+= ypos1;

			if((height>xpos1) & (width > ypos1))
				{

					tempx = ((height - xpos1)/((height - xpos1)/10));

					tempy = ((width - ypos1)/((width - ypos1)/10));

					tempz = ((tempx+tempy)/2);
				}

					TFT_ST7789V_lcd_shapes(xpos1+tempx+1,height-tempx-1,ypos1+1,width-1,colour);
					TFT_ST7789V_lcd_shapes(xpos1+1,xpos1+tempx+1-1,ypos1+tempy+1,width-tempy-1,colour);
					TFT_ST7789V_lcd_shapes(height-tempx,height-1,ypos1+tempy+1,width-tempy-1,colour);



			//		TFT_ST7789V_circle(xpos1+tempx,ypos1+tempy,tempz,colour,colour);
					TFT_ST7789V_circle1(xpos1+tempx,width-tempy,tempz,colour,colour);
		//			TFT_ST7789V_circle2(height-tempx,ypos1+tempy,tempz,colour,colour);
					TFT_ST7789V_circle3(height-tempx,width-tempy,tempz,colour,colour);
	}

uint16_t  __24_bit_to_16_bit_color(uint32_t Color)
{
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
	uint16_t __16_bit_color=0;

	red	 	= (Color&0xFF0000)>>16;
	green 	= (Color&0x00FF00)>>8;
	blue 	= (Color&0x0000FF)>>0;

	red 	= ((float)red * 0.12);
	green 	= ((float)green * 0.24);
	blue 	= ((float)blue * 0.12);

  //__16_bit_color = (green<<5)|(blue;)
	__16_bit_color = (red<<11)|(green<<5)|(blue<<0);
	return __16_bit_color;
}


//void TFT_ST7789V__rectangle_curve(uint16_t xpos1, uint16_t ypos1, uint16_t width,uint16_t height,uint16_t radius,uint32_t colour)
//	{
//		int tempx,tempy,tempz;
////		uint16_t  length;
//		height+= xpos1;
//		width+= ypos1;
//			if((height>xpos1) & (width > ypos1))
//				{
//
//					tempx = ((height - xpos1)/((height - xpos1)/10));//50/5
//					tempy = ((width - ypos1)/((width - ypos1)/10));  //150/10
//					tempz = ((tempx+tempy)/2);//10+10=20
//				}
//
//					TFT_ST7789V_lcd_shapes(xpos1+tempx+1,height-tempx-1,ypos1+1,width-1,colour);
//					TFT_ST7789V_lcd_shapes(xpos1+1,height-1,ypos1+tempy+1,width-tempy-1,colour);
//					TFT_ST7789V_lcd_drawcircle1(xpos1+tempx,ypos1+tempy,tempz,colour,colour);
//					TFT_ST7789V_lcd_drawcircle1(xpos1+tempx,width-tempy,tempz,colour,colour);
//					TFT_ST7789V_lcd_drawcircle1(height-tempx,ypos1+tempy,tempz,colour,colour);
//					TFT_ST7789V_lcd_drawcircle1(height-tempx,width-tempy,tempz,colour,colour);
//
////				    radius=tempz;
////
////				     for(i=1;i<radius;i++)
////				      {
////				    	length = sqrt((radius+i)*(radius-i));
////
////
////				    	TFT_ST7789V_lcd_shapes(xpos1-length+tempx, xpos1 , ypos1-i, ypos1 - i, RED); //first
////
//////				    TFT_ST7789V_lcd_shapes(xpos1-length+tempx, xpos1 , ypos1-i+tempy, ypos1 - i, RED); //first
//////				    TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos, ypos+i,fill_color);     // second
//////				    TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos-length, ypos,fill_color);     //third
//////				    TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos, ypos+length, fill_color);   //four
////				    			}
////					TFT_ST7789V_lcd_shapes(xpos1+tempx+1,height-tempx-1,ypos1+1,width-1,colour);
////					TFT_ST7789V_lcd_shapes(xpos1+1,height-1,ypos1+tempy+1,width-tempy-1,colour);
//	}
void TFT_ST7789V__rectangle_curve(uint16_t xpos1, uint16_t ypos1, uint16_t width,uint16_t height,uint16_t radius,uint32_t colour)
	{
	uint16_t tempx=0;
	uint16_t tempy=0;
	uint16_t tempz=0;
	//uint16_t  length;

		height+= xpos1;
		width+= ypos1;

			if((height>xpos1) & (width > ypos1))
				{

					tempx = ((height - xpos1)/((height - xpos1)/10));

					tempy = ((width - ypos1)/((width - ypos1)/10));

					tempz = ((tempx+tempy)/2);
				}

					TFT_ST7789V_lcd_shapes(xpos1+tempx+1,height-tempx-1,ypos1+1,width-1,colour);
					TFT_ST7789V_lcd_shapes(xpos1+1,xpos1+tempx+1-1,ypos1+tempy+1,width-tempy-1,colour);
					TFT_ST7789V_lcd_shapes(height-tempx,height-1,ypos1+tempy+1,width-tempy-1,colour);


				//	TFT_ST7789V_lcd_shapes(xpos1+1,height-1,ypos1+tempy+1,width-tempy-1,colour);
//					TFT_ST7789V_lcd_drawcircle1(xpos1+tempx,ypos1+tempy,tempz,colour,colour);
//					TFT_ST7789V_lcd_drawcircle1(xpos1+tempx,width-tempy,tempz,colour,colour);
//					TFT_ST7789V_lcd_drawcircle1(height-tempx,ypos1+tempy,tempz,colour,colour);
//					TFT_ST7789V_lcd_drawcircle1(height-tempx,width-tempy,tempz,colour,colour);

					TFT_ST7789V_circle(xpos1+tempx,ypos1+tempy,tempz,colour,colour);
					TFT_ST7789V_circle1(xpos1+tempx,width-tempy,tempz,colour,colour);
					TFT_ST7789V_circle2(height-tempx,ypos1+tempy,tempz,colour,colour);
					TFT_ST7789V_circle3(height-tempx,width-tempy,tempz,colour,colour);
	}

void TFT_ST7789V__upside_rectangle_curve(uint16_t xpos1, uint16_t ypos1, uint16_t width,
		uint16_t height,uint16_t radius,uint32_t colour)
	{
	uint16_t tempx=0;
	uint16_t tempy=0;
	uint16_t tempz=0;
	//uint16_t  length;

		height+= xpos1;
		width+= ypos1;

			if((height>xpos1) & (width > ypos1))
				{

					tempx = ((height - xpos1)/((height - xpos1)/10));

					tempy = ((width - ypos1)/((width - ypos1)/10));

					tempz = ((tempx+tempy)/2);
				}
                                 	TFT_ST7789V_lcd_shapes(xpos1+tempx+1-5,height-tempx-1,ypos1+1,width-1,colour);
									TFT_ST7789V_lcd_shapes(xpos1+1,xpos1+tempx+1-1,ypos1+tempy+1,width-tempy-1+10,colour);
									TFT_ST7789V_lcd_shapes(height-tempx,height-1,ypos1+tempy+1,width-tempy-1+10,colour);



									TFT_ST7789V_circle(xpos1+tempx,ypos1+tempy,tempz,colour,colour);
									TFT_ST7789V_circle1(xpos1+tempx,width-tempy,tempz,colour,colour);
									TFT_ST7789V_circle2(height-tempx,ypos1+tempy,tempz,colour,colour);
									TFT_ST7789V_circle3(height-tempx,width-tempy,tempz,colour,colour);
	}

void TFT_ST7789V__rectangle_curve2(uint16_t xpos1, uint16_t ypos1, uint16_t width,
		uint16_t height,uint16_t radius,uint32_t colour)
	{
	uint16_t tempx=0;
	uint16_t tempy=0;
	uint16_t tempz=0;
	//uint16_t  length;

		height+= xpos1;
		width+= ypos1;

			if((height>xpos1) & (width > ypos1))
				{

					tempx = ((height - xpos1)/((height - xpos1)/10));

					tempy = ((width - ypos1)/((width - ypos1)/10));

					tempz = ((tempx+tempy)/2);
				}

					TFT_ST7789V_lcd_shapes(xpos1+tempx+1,height-tempx-1,ypos1+1,width-1,colour);
					TFT_ST7789V_lcd_shapes(xpos1+1,xpos1+tempx+1-1,ypos1+tempy+1,width-tempy-1,colour);
					TFT_ST7789V_lcd_shapes(height-tempx,height-1,ypos1+tempy+1,width-tempy-1,colour);



					TFT_ST7789V_circle(xpos1+tempx,ypos1+tempy,tempz,colour,colour);
					TFT_ST7789V_circle1(xpos1+tempx,width-tempy,tempz,colour,colour);
					TFT_ST7789V_circle2(height-tempx,ypos1+tempy,tempz,colour,colour);
					TFT_ST7789V_circle3(height-tempx,width-tempy,tempz,colour,colour);
	}


void TFT_ST7789V__Top_rectangle_curve(uint16_t xpos1, uint16_t ypos1, uint16_t width,uint16_t height,uint16_t radius,uint32_t colour)//23.09.24
	{
	uint16_t tempx=0;
	uint16_t tempy=0;
	uint16_t tempz=0;
	//uint16_t  length;

		height+= xpos1;
		width+= ypos1;

			if((height>xpos1) & (width > ypos1))
				{

					tempx = ((height - xpos1)/((height - xpos1)/10));

					tempy = ((width - ypos1)/((width - ypos1)/10));

					tempz = ((tempx+tempy)/2);
				}

					TFT_ST7789V_lcd_shapes(xpos1+tempx+1,height-tempx-1,ypos1+1,width-1,GRAY);
					TFT_ST7789V_lcd_shapes(xpos1+1,xpos1+tempx+1-1,ypos1+tempy+1,width-tempy-1+10,GRAY);
					TFT_ST7789V_lcd_shapes(height-tempx,height-1,ypos1+tempy+1,width-tempy-1,GRAY);
					TFT_ST7789V_lcd_shapes(height-tempx,height-1,ypos1+tempy+1,width-tempy-1+10,GRAY);


				//	TFT_ST7789V_lcd_shapes(xpos1+1,height-1,ypos1+tempy+1,width-tempy-1,colour);
//					TFT_ST7789V_lcd_drawcircle1(xpos1+tempx,ypos1+tempy,tempz,colour,colour);
//					TFT_ST7789V_lcd_drawcircle1(xpos1+tempx,width-tempy,tempz,colour,colour);
//					TFT_ST7789V_lcd_drawcircle1(height-tempx,ypos1+tempy,tempz,colour,colour);
//					TFT_ST7789V_lcd_drawcircle1(height-tempx,width-tempy,tempz,colour,colour);

					TFT_ST7789V_circle(xpos1+tempx,ypos1+tempy,tempz,BLACK,GRAY);
				//	TFT_ST7789V_circle1(xpos1+tempx,width-tempy,tempz,BLACK,BLACK);
					TFT_ST7789V_circle2(height-tempx,ypos1+tempy,tempz,BLACK,GRAY);
				//	TFT_ST7789V_circle3(height-tempx,width-tempy,tempz,BLACK,BLACK);
	}


void TFT_ST7789V_circle(uint16_t xpos, uint16_t ypos, uint16_t radius, uint32_t boarder_color, uint32_t fill_color)	//11-01-2022
	{
		uint16_t i, length;
		i=0;
	//	TFT_ST7789V_BlockWrite(xpos,xpos+radius,ypos,ypos+radius);
	//	A0_DATA;
		length = sqrt((radius+i)*(radius-i));
		TFT_ST7789V_lcd_shapes(xpos-(length-1), (xpos) , ypos-i, ypos - i, fill_color); //first

		for(i=1;i<radius;i++)
			{
				length = sqrt((radius+i)*(radius-i));
				TFT_ST7789V_lcd_shapes(xpos-(length), (xpos) , ypos-i, ypos - i, fill_color); //first
//				TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos, ypos+i,fill_color);     // second
//				TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos-length, ypos,fill_color);     //third
//				TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos, ypos+length, fill_color);   //four
			}


	}

void TFT_ST7789V_circle1(uint16_t xpos, uint16_t ypos, uint16_t radius, uint32_t boarder_color, uint32_t fill_color)	//11-01-2022
	{
		uint16_t i, length;
		i=0;
	//	TFT_ST7789V_BlockWrite(xpos,xpos+radius,ypos,ypos+radius);
	//	A0_DATA;
		for(i=1;i<radius;i++)
			{
				length = sqrt((radius+i)*(radius-i));
//				TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos-i, ypos - i, fill_color); //first
				TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos, ypos+i,fill_color);     // second
//				TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos-length, ypos,fill_color);     //third
//				TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos, ypos+length, fill_color);   //four
			}
	}

void TFT_ST7789V_circle2(uint16_t xpos, uint16_t ypos, uint16_t radius, uint32_t boarder_color, uint32_t fill_color)	//11-01-2022
	{
		uint16_t i, length;
		i=0;
	//	TFT_ST7789V_BlockWrite(xpos,xpos+radius,ypos,ypos+radius);
	//	A0_DATA;
		for(i=1;i<radius;i++)
			{
				length = sqrt((radius+i)*(radius-i));
//				TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos-i, ypos - i, fill_color); //first
//				TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos, ypos+i,fill_color);     // second
				TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos-length, ypos,fill_color);     //third
//				TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos, ypos+length, fill_color);   //four
			}
	}

void TFT_ST7789V_circle3(uint16_t xpos, uint16_t ypos, uint16_t radius, uint32_t boarder_color, uint32_t fill_color)	//11-01-2022
	{
		uint16_t i, length;
		i=0;
	//	TFT_ST7789V_BlockWrite(xpos,xpos+radius,ypos,ypos+radius);
	//	A0_DATA;
		for(i=1;i<radius;i++)
			{
				length = sqrt((radius+i)*(radius-i));
//				TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos-i, ypos - i, fill_color); //first
//				TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos, ypos+i,fill_color);     // second
//				TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos-length, ypos,fill_color);     //third
				TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos, ypos+length, fill_color);   //four
			}
	}


void TFT_ST7789V_lcd_drawcircle1(uint16_t xpos, uint16_t ypos, uint16_t radius, uint32_t boarder_color, uint32_t fill_color)	//11-01-2022
	{
		uint16_t i, length;
		i=0;
		TFT_ST7789V_BlockWrite(xpos,xpos+radius,ypos,ypos+radius);
		A0_DATA;
		for(i=1;i<radius;i++)
			{
				length = sqrt((radius+i)*(radius-i));
				TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos-i, ypos - i, fill_color); //first
				TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos, ypos+i,fill_color);     // second
				TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos-length, ypos,fill_color);     //third
				TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos, ypos+length, fill_color);   //four
			}
	}

void TFT_ST7789V_lcd_drawcircle1_Battery_Logo(uint16_t xpos, uint16_t ypos, uint16_t radius, uint32_t boarder_color, uint32_t fill_color)	//11-01-2022
	{
		uint16_t i, length;
		i=0;
		TFT_ST7789V_BlockWrite(xpos,xpos+radius,ypos,ypos+radius);
		A0_DATA;
		for(i=1;i<radius;i++)
		{
			length = sqrt((radius+i)*(radius-i));
			TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos-i, ypos - i,WHITE); //first
			TFT_ST7789V_lcd_shapes(xpos-length, xpos , ypos, ypos+i,RED);     // second
			TFT_ST7789V_lcd_shapes(xpos, xpos+i , ypos-length, ypos,WHITE);     //third
		}
	}

void TFT_ST7789V_lcd_shapes(int x1, int width, int y1, int height, uint32_t ch )
	{
		int x,y;

//		uint8_t red = 0;
//		uint8_t green = 0;
//		uint8_t blue = 0;

		uint16_t __16_bit_color = 0;


//		red = (ch&0xFF0000)>>16;
//		green =(ch&0x00FF00)>>8;
//		blue = (ch&0x0000FF);
//		__16_bit_color = (red<<11)|(green<<5)|(blue);
		__16_bit_color=__24_bit_to_16_bit_color(ch);
		TFT_ST7789V_BlockWrite(y1,height,x1,width);
		A0_DATA;
		for(x= x1 ; x<=width ; x++)
		{
			for(y=y1 ; y<=height ; y++)
				{
//					SPI_data(ch>>16);
//					SPI_data(ch>>8);
//					SPI_data(ch);
					SPI_transfer(__16_bit_color,_16_BIT);
				}
		}
	}
void draw_line(uint16_t start_xpos, uint16_t start_ypos, uint16_t end_xpos, uint16_t end_ypos,uint32_t colour)
{

	int x_length, y_length, new_y_length, new_x_length, new_xpos, new_ypos;
	uint16_t temp;

	if( (end_xpos >= start_xpos)&&(end_ypos >= start_ypos)){

		x_length = end_xpos - start_xpos;
		y_length = end_ypos - start_ypos;

		new_x_length = x_length;
		while(new_x_length--){

			new_y_length=(y_length*new_x_length)/x_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos + new_y_length;
			Rectangle(new_xpos ,new_ypos,1,1, colour);
			}

		new_y_length = y_length;
		while(new_y_length--){

			new_x_length=(x_length*new_y_length)/y_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos + new_y_length;
			Rectangle(new_xpos ,new_ypos,1,1, colour);
			}
		}

	else if( (end_xpos <= start_xpos)&&(end_ypos <= start_ypos)){

		temp = start_xpos;
		start_xpos = end_xpos;
		end_xpos = temp;

		temp = start_ypos;
		start_ypos = end_ypos;
		end_ypos = temp;

		x_length = end_xpos - start_xpos;
		y_length = end_ypos - start_ypos;

		new_x_length = x_length;
		while(new_x_length--){

			new_y_length=(y_length*new_x_length)/x_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos + new_y_length;
			Rectangle(new_xpos ,new_ypos,1,1, colour);
			}

		new_y_length = y_length;
		while(new_y_length--){

			new_x_length=(x_length*new_y_length)/y_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos + new_y_length;
			Rectangle(new_xpos ,new_ypos,1,1, colour);
			}
		}

	if( (end_xpos <= start_xpos)&&(end_ypos >= start_ypos)){

		x_length = start_xpos - end_xpos;
		y_length = end_ypos - start_ypos;

		new_x_length = x_length;
		while(new_x_length--){

			new_y_length=(y_length*new_x_length)/x_length;
			new_xpos= end_xpos + new_x_length;
			new_ypos= end_ypos - new_y_length;
			Rectangle(new_xpos ,new_ypos,1,1, colour);
			}

		new_y_length = y_length;
		while(new_y_length--){

			new_x_length=(x_length*new_y_length)/y_length;
			new_xpos= end_xpos + new_x_length;
			new_ypos= end_ypos - new_y_length;
			Rectangle(new_xpos ,new_ypos,1,1, colour);
			}
		}

	if( (end_xpos >= start_xpos)&&(end_ypos <= start_ypos)){

		x_length = end_xpos - start_xpos;
		y_length = start_ypos - end_ypos;

		new_x_length = x_length;
		while(new_x_length--){

			new_y_length=(y_length*new_x_length)/x_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos - new_y_length;
			Rectangle(new_xpos ,new_ypos,1,1, colour);
			}

		new_y_length = y_length;
		while(new_y_length--){

			new_x_length=(x_length*new_y_length)/y_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos - new_y_length;
			Rectangle(new_xpos ,new_ypos,1,1, colour);
			}
		}

		}

void footer_curve()
{
	 Rectangle(108,212,54,0,MAIN_FRAME_COL);
	 Rectangle(107,213,56,0,MAIN_FRAME_COL);
	 Rectangle(107,214,56,0,MAIN_FRAME_COL);
	 Rectangle(107,215,56,17,MAIN_FRAME_COL);
	 Rectangle(107,233,56,0,MAIN_FRAME_COL);
	 Rectangle(108,234,54,0,MAIN_FRAME_COL);
}

void lcd_drawline(uint16_t start_xpos, uint16_t start_ypos, uint16_t end_xpos, uint16_t end_ypos, uint32_t Color)
{
	uint16_t __16_bit_color=0;
	uint16_t x_length, y_length, new_y_length, new_x_length, new_xpos, new_ypos;
	uint16_t temp;

	__16_bit_color=__24_bit_to_16_bit_color(Color);

	if( (end_xpos >= start_xpos)&&(end_ypos >= start_ypos))
	{

		x_length = end_xpos - start_xpos;
		y_length = end_ypos - start_ypos;

		new_x_length = x_length;

		while(new_x_length--)
		{

			new_y_length=(y_length*new_x_length)/x_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos + new_y_length;
			//	lcd_color_pixel(new_xpos ,new_ypos, boarder_color);
			TFT_ST7789V_BlockWrite(new_ypos,new_ypos,new_xpos,new_xpos);

			A0_DATA;

			//		spi_write_data(Color>>16);
			//		spi_write_data(Color>>8);
			//		spi_write_data(Color);

			SPI_transfer(__16_bit_color,_16_BIT);
		}

		new_y_length = y_length;
		while(new_y_length--)
		{

			new_x_length=(x_length*new_y_length)/y_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos + new_y_length;
			//	lcd_color_pixel(new_xpos ,new_ypos, boarder_color);
			TFT_ST7789V_BlockWrite(new_ypos,new_ypos,new_xpos,new_xpos);
			A0_DATA;
			//		spi_write_data(Color>>16);
			//		spi_write_data(Color>>8);
			//		spi_write_data(Color);

			SPI_transfer(__16_bit_color,_16_BIT);
		}

	}


	else if( (end_xpos <= start_xpos)&&(end_ypos <= start_ypos))
	{

		temp = start_xpos;
		start_xpos = end_xpos;
		end_xpos = temp;

		temp = start_ypos;
		start_ypos = end_ypos;
		end_ypos = temp;

		x_length = end_xpos - start_xpos;
		y_length = end_ypos - start_ypos;

		new_x_length = x_length;
		while(new_x_length--)
		{

			new_y_length=(y_length*new_x_length)/x_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos + new_y_length;
			//	lcd_color_pixel(new_xpos ,new_ypos, boarder_color);
			TFT_ST7789V_BlockWrite(new_ypos,new_ypos,new_xpos,new_xpos);
			A0_DATA;
			//		spi_write_data(Color>>16);
			//		spi_write_data(Color>>8);
			//		spi_write_data(Color);

			SPI_transfer(__16_bit_color,_16_BIT);
		}

		new_y_length = y_length;
		while(new_y_length--)
		{

			new_x_length=(x_length*new_y_length)/y_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos + new_y_length;
			//	lcd_color_pixel(new_xpos ,new_ypos, boarder_color);
			TFT_ST7789V_BlockWrite(new_ypos,new_ypos,new_xpos,new_xpos);
			A0_DATA;
			//		spi_write_data(Color>>16);
			//		spi_write_data(Color>>8);
			//		spi_write_data(Color);

			SPI_transfer(__16_bit_color,_16_BIT);
		}

	}

	else if( (end_xpos <= start_xpos)&&(end_ypos >= start_ypos))
	{

		x_length = start_xpos - end_xpos;
		y_length = end_ypos - start_ypos;

		new_x_length = x_length;


		while(new_x_length--)
		{

			new_y_length=(y_length*new_x_length)/x_length;
			new_xpos= end_xpos + new_x_length;
			new_ypos= end_ypos - new_y_length;
			//	lcd_color_pixel(new_xpos ,new_ypos, boarder_color);

			TFT_ST7789V_BlockWrite(new_ypos,new_ypos,new_xpos,new_xpos);
			A0_DATA;
			//		spi_write_data(Color>>16);
			//		spi_write_data(Color>>8);
			//		spi_write_data(Color);

			SPI_transfer(__16_bit_color,_16_BIT);
		}

		new_y_length = y_length;
		while(new_y_length--)
		{

			new_x_length=(x_length*new_y_length)/y_length;
			new_xpos= end_xpos + new_x_length;
			new_ypos= end_ypos - new_y_length;
			//	lcd_color_pixel(new_xpos ,new_ypos, boarder_color);
			TFT_ST7789V_BlockWrite(new_ypos,new_ypos,new_xpos,new_xpos);
			A0_DATA;
			//		spi_write_data(Color>>16);
			//		spi_write_data(Color>>8);
			//		spi_write_data(Color);

			SPI_transfer(__16_bit_color,_16_BIT);
		}

	}

	else if( (end_xpos >= start_xpos)&&(end_ypos <= start_ypos))
	{

		x_length = end_xpos - start_xpos;
		y_length = start_ypos - end_ypos;

		new_x_length = x_length;
		while(new_x_length--)
		{

			new_y_length=(y_length*new_x_length)/x_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos - new_y_length;
			//	lcd_color_pixel(new_xpos ,new_ypos, boarder_color);
			TFT_ST7789V_BlockWrite(new_ypos,new_ypos,new_xpos,new_xpos);
			A0_DATA;
			//		spi_write_data(Color>>16);
			//		spi_write_data(Color>>8);
			//		spi_write_data(Color);

			SPI_transfer(__16_bit_color,_16_BIT);
		}

		new_y_length = y_length;
		while(new_y_length--)
		{

			new_x_length=(x_length*new_y_length)/y_length;
			new_xpos= start_xpos + new_x_length;
			new_ypos= start_ypos - new_y_length;
			//	lcd_color_pixel(new_xpos ,new_ypos, boarder_color);
			TFT_ST7789V_BlockWrite(new_ypos,new_ypos,new_xpos,new_xpos);
			A0_DATA;
			//		spi_write_data(Color>>16);
			//		spi_write_data(Color>>8);
			//		spi_write_data(Color);

			//SPI_transfer(__16_bit_color,_16_BIT);

			SPI_transfer(__16_bit_color,_16_BIT);
		}
	}

}
