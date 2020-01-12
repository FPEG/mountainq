/*!
 * @file      Display.c
 * @brief      显示函数
 * @author
 * @version
 * @date       2019-05-21
 */
#include "include.h"
#include "Display.h"

uint8 En_Display=0;

void Tft_Display()
{
   #if  1
       LCD_P6x8Str(0,0,"Threshold");


       LCD_P6x8Str(0,1,"Round");
       LCD_Show_Number3(60,1,Round.Round_flag);
       //LCD_Show_Number3(100,1,RoundProcess);

       LCD_P6x8Str(0,2,"break");
       LCD_Show_Number3(60,2,Break_Road.Break_Road_Flag);
       LCD_Show_Number3(100,2,Break_Road.Break_Road_preFlag);

       LCD_P6x8Str(0,3,"cross");
       LCD_Show_Number3(60,3,Cross.Cross_flag);
       LCD_Show_Number3(100,3,CrossProcess);

       LCD_P6x8Str(0,4,"Barrier");
       LCD_Show_Number3(60,4,Barrier.Barrier_Flag);
       LCD_Show_Number3(100,4,Bar_Process);

       LCD_P6x8Str(0,6,"distance");
       LCD_Show_Number3(64,6,TFmini_Plus.Dist);

       LCD_Print16(0,7,encoder_left_speed);
       LCD_Print16(64,7,encoder_right_speed);        //速度显示


		     displayimage_binaryzation(0, 0,image[0],40,80,BLUE,BLACK);
		     for(int i=0;i<80;i++)//100
		     {
			    TFT_DrawPoint(i,160,BLACK);
		     }



		     uint8 r;
		     for(int i=0;i<ROW/2;i++)
		     {      r=(uint8)Boundary.MiddleLine[i];     //中线显示
		     if(r>0)
			    TFT_DrawPoint(i+1,80-r,RED);
		     }



		     // JudgeStraightLineR() ;
		     for(int i=0;i<ROW/2;i++)//100
		     {      r=(uint8)Boundary.RightEdge[i];
		     //		       TFT_DrawPoint(invROW[i][NS_RightBlack[i]],160-invCOL[i][NS_RightBlack[i]],GREEN);
		     //		       TFT_DrawPoint(invROW[i][NS_LeftBlack[i]],160-invCOL[i][NS_LeftBlack[i]],RED);
		     if(r>0)
			    TFT_DrawPoint(i+1,80-r,RED);
		     // TFT_DrawPoint(i,160-invCOL[i][LeftBlack[i]],RED);
		     }



		     if(Inflection_point.R_down_point.x)
		     {
			    for(int i=Inflection_point.R_down_point.x-3;i<Inflection_point.R_down_point.x+4;i++)
			    {
				   TFT_DrawPoint(i,80-Inflection_point.R_down_point.y,GREEN);
			    }
		     }
		     if(Inflection_point.R_up_point.x)
		     {
			    for(int i=Inflection_point.R_up_point.x-3;i<Inflection_point.R_up_point.x+4;i++)
			    {
				   TFT_DrawPoint(i,80-Inflection_point.R_up_point.y,GREEN);
			    }
		     }
		     for(int i=0;i<COL/2;i++)
		     {
			    TFT_DrawPoint(Boundary.R_StartLine,i,GREEN);
		     }

		     for(int i=0;i<ROW/2;i++)//100
		     {      r=(uint8)Boundary.LeftEdge[i];
		     if(r>0)
			    TFT_DrawPoint(i+1,80-r,RED);
		     }





		     if(Inflection_point.L_down_point.x)
		     {
			    for(int i=Inflection_point.L_down_point.x-4;i<Inflection_point.L_down_point.x+4;i++)
			    {
				   TFT_DrawPoint(i+1,80-Inflection_point.L_down_point.y,GREEN);
			    }
		     }
		     if(Inflection_point.L_up_point.x)
		     {
			    for(int i=Inflection_point.L_up_point.x-2;i<Inflection_point.L_up_point.x+3;i++)
			    {
				   TFT_DrawPoint(i+1,80-Inflection_point.L_up_point.y,GREEN);
			    }
		     }

		     for(int i=0;i<COL/2;i++)
		     {
			    TFT_DrawPoint(Boundary.L_StartLine,i,RED);
		     }


#endif

#if 0
		     LCD_PrintU16(50,1,TFmini_Plus.Dist);
		     LCD_PrintU16(50,3,TFmini_Plus.Strength);
		     LCD_PrintU16(50,5,TFmini_Plus.temperature/8-256);
		     LCD_PrintU16(50,7,TFmini_Plus.CheckSum);
#endif
#if 0
		     gpio_turn (PTC0);     // PTC0管脚 输出 反转
		     gpio_turn (PTD15);    // PTD15管脚 输出 反转
		     gpio_turn (PTE26);    // PTE26管脚 输出 反转
#endif
#if 0//OLED 屏幕
		     if(erkey_get(PTB18))
		     {
			    width_row--;
			    if(width_row<=0)
				   width_row=0;

		     }
		     w= Boundary.RightEdge[width_row]-Boundary.LeftEdge[width_row] ;
		     LCD_Show_Number3(10,2,width_row);
		     LCD_Show_Number3(80,2,w);

		     LCD_show_ZZF_image();
		     LCD_Print16(80,1,(int)center_line_err);
#endif
#if 0//tft 屏幕
		     //seekfree_sendimg_032(40,80);
		     //displayimage032(image[0]);
#endif
#if 0
		     pit_time_start(PIT1);
		     //displayimage032(image[0]);
		     uint32 time = pit_time_get_ms(PIT1);
		     LCD_Print16(20,7,time);
#endif
}