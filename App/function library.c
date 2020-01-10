/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       MK60.c
 * @brief      Ұ��K60 ƽ̨�жϸ�λ����
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */
#include    "function library.h"
#include   "include.h"
uint8 ITEM1=0,ITEM2=0;
uint16 key_hold_count=0;
/*����*/
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


/*******************����*******************/
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


/*����ɨ�����*/
uint8 key_set_flag=0;
uint8 Key_Cancel,Key_Sure,Key_Down,Key_Up;
uint8 Key_Cancel_Last=0,Key_Sure_Last=0,Key_Down_Last=0,Key_Up_Last=0;
uint8 Menu_Page=1;
uint8 Menu_Bottom=4;
uint8 Arrow_Position=1;
uint8 edit_flag=0;


//����xƽ�����ĵ���
float InvSqrt (float x)
{
        float xhalf = 0.5f*x;
        int i = *(int*)&x;
        i = 0x5f3759df - (i >> 1);        // �����һ�����Ƹ�
        x = *(float*)&i;
        x = x*(1.5f - xhalf*x*x);       // ţ�ٵ�����
        return x;
}


void key_scan()
{
  //���û�а�������

    Key_Cancel=!gpio_get(KEY1);
    Key_Sure=!gpio_get(KEY2);
    Key_Down=!gpio_get(KEY3);
    Key_Up=!gpio_get(KEY4);

    if(Key_Cancel!=Key_Cancel_Last ||Key_Sure!=Key_Sure_Last||Key_Down!=Key_Down_Last||Key_Up!=Key_Up_Last)//��������а���,�˳�ɨ��
    {
      DELAY_MS(200);
      OLED_Refresh_Gram();
      return;
    }

}

//�����ֵ
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
    //��ͷλ���޶�
    if(Arrow_Position<1)
      Arrow_Position=Menu_Bottom;
    if(Arrow_Position>Menu_Bottom)
      Arrow_Position=1;
    //�˵�ҳ���޶�
    if(Menu_Page<1)
      Menu_Page=1;
    if(Menu_Page>3)
      Menu_Page=3;
    //��ʾ�˵�
    lcd_show_menu();

    //���һ�ΰ���ֵ
    key_scan();
    //�ڲ�ͬ�˵�ҳ�°����в�ͬ
    switch(Menu_Page)
    {
    case 1:    //һ���˵�
      {
        Menu_Bottom=4;
        //���ȡ����������
        if(Key_Cancel==1)
        {
          Menu_Page--;
        }
        //���ȷ�ϼ�������
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
        //����ϼ�������
        else if(Key_Up==1)
        {
          Arrow_Position--;
        }//����¼�������
        else if(Key_Down==1)
        {
          Arrow_Position++;
        }
        break;
      }
    case 2:    //�����˵�
      {
        //�����˵�Ŀ¼����
        switch(ITEM1)
        {
        case 0://�ٶ�
          {
            Menu_Bottom=5;
            break;
          }
        case 1://�ϰ�
          {
            Menu_Bottom=3;
            break;
          }
        case 2://��·
          {
            Menu_Bottom=1;
            break;
          }
        }
        //���ȡ����������
        if(Key_Cancel==1)
        {
          Menu_Page--;
        }
        //���ȷ�ϼ�������
        else if(Key_Sure==1)
        {
          ITEM2=Arrow_Position-1;
          Arrow_Position=1;
          Menu_Page++;
        }
        //����ϼ�������
        else if(Key_Up==1)
        {
          Arrow_Position--;
        }//����¼�������
        else if(Key_Down==1)
        {
          Arrow_Position++;
        }
        break;
      }
    case 3:    //�����˵�
      {
        //���ȡ����������
        if(Key_Cancel==1)
        {
          Menu_Page--;
          edit_flag=0;
        }
        //���ȷ�ϼ�������,���濪ʼ�޸Ĳ���
        else if(Key_Sure==1)
        {
          edit_flag=!edit_flag;
        }
        else if(Key_Up==1)
        {
          if(edit_flag)
          {
            if(ITEM1==0)//�ٶ�����
            {
              if(ITEM2==0)//����ٶ�����
              {
                max_speed++;
              }
              else if(ITEM2==1)//��ͨ�ٶ�����
              {
                nor_speed++;
              }
              else if(ITEM2==2)//Բ���ٶ�����
              {
                rou_speed++;
              }
              else if(ITEM2==3)//��·�ٶ�����
              {
                Bar_speed++;
              }
            }
            else if(ITEM1==1)//·������
            {
              if(ITEM2==0)//���ƫ��
              {
                Bar_Left_para++;
              }
              else if(ITEM2==1)//�Ҵ�ƫ��
              {
                Bar_Right_para++;
              }
              else if(ITEM2==2)//��Ӧ����
              {
                BDet_distance++;
              }
            }
            else if(ITEM1==2)//��·����
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
            if(ITEM1==0)//�ٶ�����
            {
              if(ITEM2==0)//����ٶ�����
              {
                max_speed--;
              }
              else if(ITEM2==1)//��ͨ�ٶ�����
              {
                nor_speed--;
              }
              else if(ITEM2==2)//Բ���ٶ�����
              {
                rou_speed--;
              }
              else if(ITEM2==3)//��·�ٶ�����
              {
                Bar_speed--;
              }
            }
            else if(ITEM1==1)//·������
            {
              if(ITEM2==0)//���ƫ��
              {
                Bar_Left_para--;
              }
              else if(ITEM2==1)//�Ҵ�ƫ��
              {
                Bar_Right_para--;
              }
              else if(ITEM2==2)//��Ӧ����
              {
                BDet_distance--;
              }
            }
            else if(ITEM1==2)//��·����
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
    //����һ�μ�ֵ�����㵱ǰ��ֵ
    key_save();

    if(Quit_Flag==1)//�˳�
    {
      break;
    }
  }
}


 int   engine_key;
/******************���ý�����ʾ1******************/
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

  //�жϲ��뿪�ص�״̬

      LCD_P6x8Str(30,0,"Camera_Menu");
      //һ���˵�
      if(Menu_Page==1)
      {
        LCD_P6x8Str(20,1,"1.Speed_Set");
        LCD_P6x8Str(20,2,"2.Berrier_Set");
        LCD_P6x8Str(20,3,"3.Break_Set");
        LCD_P6x8Str(20,4,"4.Quit");
      }
      //�����˵�
      else if(Menu_Page==2)
      {
        switch(ITEM1)
        {
        case 0://�ٶ�����
          {
            LCD_P6x8Str(20,1,"1.Max_Speed");//����ٶ�
            LCD_P6x8Str(20,2,"2.Normal_Speed");//��ͨ�ٶ�
            LCD_P6x8Str(20,3,"3.Round_Speed");//�����ٶ�
            LCD_P6x8Str(20,4,"4.Break_Speed");//��·�ٶ�
            LCD_P6x8Str(20,5,"5.Barrier_Speed");//�ϰ��ٶ�
            break;
          }
        case 1://�ϰ�����
          {
            LCD_P6x8Str(20,1,"1.Bar_Left_para");//��תϵ��
            LCD_P6x8Str(20,2,"2.Bar_Right_para");//��תϵ��
            LCD_P6x8Str(20,3,"3.Bar_distance");//��ת����
            break;
          }
        case 2://��·����
          {
            LCD_P6x8Str(20,1,"1.Break_Threshold");//��·��ֵ
            break;
          }
        }
      }
      //�����˵�
      else if(Menu_Page==3)
      {
        switch(ITEM1)
        {
        case 0://�ٶ�����
          {
            switch(ITEM2)
            {
            case 0://ֱ���ٶ�
              {
                LCD_P6x8Str(0,1," Max_Speed:");
                LCD_Print16(45,2,max_speed);
                break;
              }
            case 1://����ٶ�
              {
                LCD_P6x8Str(0,1," Normal_Speed:");
                LCD_Print16(45,2,nor_speed);
                break;
              }
            case 2://Բ���ٶ�
              {
               LCD_P6x8Str(0,1," Round_Speed:");
                LCD_Print16(45,2,rou_speed);
                break;
              }
            case 3://��·�ٶ�
              {
                LCD_P6x8Str(0,1," Break_Speed:");
                LCD_Print16(45,2,Break_speed);
                break;
              }
            case 4://���ϰ��ٶ�
              {
                LCD_P6x8Str(0,1," Barrier_Speed:");
                LCD_Print16(45,2,Bar_speed);
                break;
              }
            }
            break;
          }
        case 1://·������
          {
            switch(ITEM2)
            {
            case 0://��ƫ��
              {
                LCD_P6x8Str(20,1," Bar_Left_offset:");
                LCD_Print16(45,2,Bar_Left_para);
                break;
              }
            case 1://��ƫ��
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
        case 2://��·����
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
//�������ʺ���
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


