/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,野火科技
 *     All rights reserved.
 *     技术讨论：野火初学论坛 http://www.chuxue123.com
 *
 *     除注明出处外，以下所有内容版权均属野火科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留野火科技的版权声明。
 *
 * @file       MK60.c
 * @brief      野火K60 平台中断复位函数
 * @author     野火科技
 * @version    v5.0
 * @date       2013-06-26
 */
#include    "function library.h"
#include   "include.h"
uint8 ITEM1=0,ITEM2=0;
uint16 key_hold_count=0;
/*按键*/
uint8 key_flag=0;

void lcd_show_menu();
//buzzer_test();
void buzzer_test()
{
  gpio_set(buzzer_pin,0);
  DELAY_MS(200);
  gpio_turn(buzzer_pin);
  DELAY_MS(200);
  gpio_turn(buzzer_pin);
  DELAY_MS(200);
  gpio_set(buzzer_pin,1);
}


/*******************按键*******************/
uint8 erkey_get(PTXn_e ptxn)
{
  if(gpio_get(ptxn)==0)
  {
    lptmr_delay_ms(100);
    if(gpio_get(ptxn)==0)
    {
      return 1;
    }
    else
      return 0;
  }
  else
    return 0;
}


/*按键扫描程序*/
uint8 key_set_flag=0;
uint8 Key_Cancel,Key_Sure,Key_Down,Key_Up;
uint8 Key_Cancel_Last=0,Key_Sure_Last=0,Key_Down_Last=0,Key_Up_Last=0;
uint8 Menu_Page=1;
uint8 Menu_Bottom=4;
uint8 Arrow_Position=1;
uint8 edit_flag=0;


//计算x平方根的倒数
float InvSqrt (float x)
{
        float xhalf = 0.5f*x;
        int i = *(int*)&x;
        i = 0x5f3759df - (i >> 1);        // 计算第一个近似根
        x = *(float*)&i;
        x = x*(1.5f - xhalf*x*x);       // 牛顿迭代法
        return x;
}


void key_scan()
{
  //如果没有按键按下

    Key_Cancel=!gpio_get(KEY1);
    Key_Sure=!gpio_get(KEY2);
    Key_Down=!gpio_get(KEY3);
    Key_Up=!gpio_get(KEY4);

    if(Key_Cancel!=Key_Cancel_Last ||Key_Sure!=Key_Sure_Last||Key_Down!=Key_Down_Last||Key_Up!=Key_Up_Last)//如果按键有按下,退出扫描
    {
      DELAY_MS(200);
      OLED_Refresh_Gram();
      return;
    }

}

//保存键值
void key_save()
{
  Key_Cancel_Last=Key_Cancel;
  Key_Sure_Last=Key_Sure;
  Key_Down_Last=Key_Down;
  Key_Up_Last=Key_Up;
//  Key_Cancel=0;
//  Key_Sure=0;
//  Key_Down=0;
//  Key_Up=0;
}
uint8 Quit_Flag=0;
/******* GUI*******/
void GUI()
{
  while(1)
  {
    //箭头位置限定
    if(Arrow_Position<1)
      Arrow_Position=Menu_Bottom;
    if(Arrow_Position>Menu_Bottom)
      Arrow_Position=1;
    //菜单页数限定
    if(Menu_Page<1)
      Menu_Page=1;
    if(Menu_Page>3)
      Menu_Page=3;
    //显示菜单
    lcd_show_menu();

    //获得一次按键值
    key_scan();
    //在不同菜单页下按键有不同
    switch(Menu_Page)
    {
    case 1:    //一级菜单
      {
        Menu_Bottom=4;
        //如果取消键被按下
        if(Key_Cancel==1)
        {
          Menu_Page--;
        }
        //如果确认键被按下
        else if(Key_Sure==1)
        {
          if(Arrow_Position==4)
          {
            Quit_Flag=1;
          }
          ITEM1=Arrow_Position-1;
          Arrow_Position=1;
          Menu_Page++;
        }
        //如果上键被按下
        else if(Key_Up==1)
        {
          Arrow_Position--;
        }//如果下键被按下
        else if(Key_Down==1)
        {
          Arrow_Position++;
        }
        break;
      }
    case 2:    //二级菜单
      {
        //二级菜单目录行数
        switch(ITEM1)
        {
        case 0://速度
          {
            Menu_Bottom=5;
            break;
          }
        case 1://障碍
          {
            Menu_Bottom=3;
            break;
          }
        case 2://断路
          {
            Menu_Bottom=1;
            break;
          }
        }
        //如果取消键被按下
        if(Key_Cancel==1)
        {
          Menu_Page--;
        }
        //如果确认键被按下
        else if(Key_Sure==1)
        {
          ITEM2=Arrow_Position-1;
          Arrow_Position=1;
          Menu_Page++;
        }
        //如果上键被按下
        else if(Key_Up==1)
        {
          Arrow_Position--;
        }//如果下键被按下
        else if(Key_Down==1)
        {
          Arrow_Position++;
        }
        break;
      }
    case 3:    //三级菜单
      {
        //如果取消键被按下
        if(Key_Cancel==1)
        {
          Menu_Page--;
          edit_flag=0;
        }
        //如果确认键被按下,表面开始修改参数
        else if(Key_Sure==1)
        {
          edit_flag=!edit_flag;
        }
        else if(Key_Up==1)
        {
          if(edit_flag)
          {
            if(ITEM1==0)//速度设置
            {
              if(ITEM2==0)//最大速度设置
              {
                max_speed++;
              }
              else if(ITEM2==1)//普通速度设置
              {
                nor_speed++;
              }
              else if(ITEM2==2)//圆环速度设置
              {
                rou_speed++;
              }
              else if(ITEM2==3)//断路速度设置
              {
                Bar_speed++;
              }
            }
            else if(ITEM1==1)//路障设置
            {
              if(ITEM2==0)//左打偏差
              {
                Bar_Left_para++;
              }
              else if(ITEM2==1)//右打偏差
              {
                Bar_Right_para++;
              }
              else if(ITEM2==2)//反应距离
              {
                BDet_distance++;
              }
            }
            else if(ITEM1==2)//断路设置
            {
              if(ITEM2==0)
              {
                G_yuzhi++;
              }
            }
          }
        }
        else if(Key_Down==1)
        {
          if(edit_flag)
          {
            if(ITEM1==0)//速度设置
            {
              if(ITEM2==0)//最大速度设置
              {
                max_speed--;
              }
              else if(ITEM2==1)//普通速度设置
              {
                nor_speed--;
              }
              else if(ITEM2==2)//圆环速度设置
              {
                rou_speed--;
              }
              else if(ITEM2==3)//断路速度设置
              {
                Bar_speed--;
              }
            }
            else if(ITEM1==1)//路障设置
            {
              if(ITEM2==0)//左打偏差
              {
                Bar_Left_para--;
              }
              else if(ITEM2==1)//右打偏差
              {
                Bar_Right_para--;
              }
              else if(ITEM2==2)//反应距离
              {
                BDet_distance--;
              }
            }
            else if(ITEM1==2)//断路设置
            {
              if(ITEM2==0)
              {
                G_yuzhi--;
              }
            }
          }
        }

      }
      break;
    }
    //保存一次键值并清零当前键值
    key_save();

    if(Quit_Flag==1)//退出
    {
      break;
    }
  }
}


 int   engine_key;
/******************设置界面显示1******************/
void lcd_show_setting()
{
  LCD_P6x8Str(5,1,"paodao");
  //LCD_PrintU16 (85,1,paodao);

  LCD_P6x8Str(5,2,"left");
  LCD_PrintU16(85,2,0);

  LCD_P6x8Str(5,3,"right");
  LCD_PrintU16(85,3,1);

  LCD_P6x8Str(5,4,"engine_phase2");
  LCD_PrintU16 (85,4,2);

  LCD_P6x8Str(5,5,"engine_phase3");
  LCD_PrintU16 (85,5,3);

  switch(engine_key)
  {
  case 0:LCD_P6x8Str(85,2,"      ");break;
  case 1:LCD_P6x8Str(85,3,"      ");break;
  case 2:LCD_P6x8Str(85,4,"      ");break;
  case 3:LCD_P6x8Str(85,5,"      ");break;
  }
}

void lcd_show_menu()
{

  //判断拨码开关的状态

      LCD_P6x8Str(30,0,"Camera_Menu");
      //一级菜单
      if(Menu_Page==1)
      {
        LCD_P6x8Str(20,1,"1.Speed_Set");
        LCD_P6x8Str(20,2,"2.Berrier_Set");
        LCD_P6x8Str(20,3,"3.Break_Set");
        LCD_P6x8Str(20,4,"4.Quit");
      }
      //二级菜单
      else if(Menu_Page==2)
      {
        switch(ITEM1)
        {
        case 0://速度设置
          {
            LCD_P6x8Str(20,1,"1.Max_Speed");//最大速度
            LCD_P6x8Str(20,2,"2.Normal_Speed");//普通速度
            LCD_P6x8Str(20,3,"3.Round_Speed");//环岛速度
            LCD_P6x8Str(20,4,"4.Break_Speed");//断路速度
            LCD_P6x8Str(20,5,"5.Barrier_Speed");//障碍速度
            break;
          }
        case 1://障碍设置
          {
            LCD_P6x8Str(20,1,"1.Bar_Left_para");//左转系数
            LCD_P6x8Str(20,2,"2.Bar_Right_para");//右转系数
            LCD_P6x8Str(20,3,"3.Bar_distance");//左转距离
            break;
          }
        case 2://断路设置
          {
            LCD_P6x8Str(20,1,"1.Break_Threshold");//断路阈值
            break;
          }
        }
      }
      //三级菜单
      else if(Menu_Page==3)
      {
        switch(ITEM1)
        {
        case 0://速度设置
          {
            switch(ITEM2)
            {
            case 0://直线速度
              {
                LCD_P6x8Str(0,1," Max_Speed:");
                LCD_Print16(45,2,max_speed);
                break;
              }
            case 1://弯道速度
              {
                LCD_P6x8Str(0,1," Normal_Speed:");
                LCD_Print16(45,2,nor_speed);
                break;
              }
            case 2://圆环速度
              {
               LCD_P6x8Str(0,1," Round_Speed:");
                LCD_Print16(45,2,rou_speed);
                break;
              }
            case 3://断路速度
              {
                LCD_P6x8Str(0,1," Break_Speed:");
                LCD_Print16(45,2,Break_speed);
                break;
              }
            case 4://过障碍速度
              {
                LCD_P6x8Str(0,1," Barrier_Speed:");
                LCD_Print16(45,2,Bar_speed);
                break;
              }
            }
            break;
          }
        case 1://路障设置
          {
            switch(ITEM2)
            {
            case 0://左偏差
              {
                LCD_P6x8Str(20,1," Bar_Left_offset:");
                LCD_Print16(45,2,Bar_Left_para);
                break;
              }
            case 1://右偏差
              {
                LCD_P6x8Str(20,1," Bar_Right_offset:");
                LCD_Print16(45,2,Bar_Right_para);
                break;
              }
            case 2:
              {
                LCD_P6x8Str(20,1," Bar_Distance:");
                LCD_PrintU16(45,2,BDet_distance);
                break;
              }
            }
            break;
          }
        case 2://断路设置
          {
            LCD_P6x8Str(20,1," Break_Threshold:");
            LCD_PrintU16(45,2,G_yuzhi);
            break;
          }
        }
      }
  if(Menu_Page<3)
    LCD_P6x8Str(5,Arrow_Position,"->");

}




float MidLine_Curvature;
//计算曲率函数
void Curvature_Calc(Str_Boundary *Str,uint8 Row_Start,uint8 Row_End)
{
  float S_triangle;
  float l1,l2,l3;
  S_triangle=((Str->MiddleLine[Row_Start]-Str->MiddleLine[(Row_End+Row_Start)>>1])*(Row_Start-Row_End)-(Str->MiddleLine[Row_End]-Str->MiddleLine[Row_Start])*(Row_Start-(Row_Start+Row_End)>>1))/2.0;
  l1=InvSqrt(Row_Start-(Row_End+Row_Start)>>1)*(Row_Start-(Row_End+Row_Start)>>1)+(Str->MiddleLine[Row_Start]-Str->MiddleLine[(Row_End+Row_Start)>>1])*(Str->MiddleLine[Row_Start]-Str->MiddleLine[(Row_End+Row_Start)>>1]);
  l2=InvSqrt(Row_End-(Row_End+Row_Start)>>1)*(Row_End-(Row_End+Row_Start)>>1)+(Str->MiddleLine[Row_End]-Str->MiddleLine[(Row_End+Row_Start)>>1])*(Str->MiddleLine[Row_End]-Str->MiddleLine[(Row_End+Row_Start)>>1]);
  l3=InvSqrt(Row_End-Row_Start)*(Row_End-Row_Start)+(Str->MiddleLine[Row_End]-Str->MiddleLine[Row_Start])*(Str->MiddleLine[Row_End]-Str->MiddleLine[Row_Start]);
  MidLine_Curvature=4*S_triangle*l1*l2*l3;
}


