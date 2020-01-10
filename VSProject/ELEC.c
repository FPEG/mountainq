#include "ELEC.h"

eRoundTypeDef            eRoundType=eNo_Round;
eRoundProcessTypeDef     eRoundProcess=eRound_Not_Found;
ElecTypeDef              ELEC;
//FuzzyPidTypeDef         eSteerPID;

float ELEC_err;
int Ring_state,count;

#define AD_CHANNEL1 ADC0_SE13//B3 R
#define AD_CHANNEL2 ADC1_SE11//B5 M
#define AD_CHANNEL3 ADC1_SE13//B7 L
#define AD_CHANNEL4 ADC1_SE10//B4 RV
#define AD_CHANNEL5 ADC1_SE12//B6 LV


uint16 limit_min[5];
uint16 limit_max[5];
uint32 elec_sum[5];
float fillter_s[5][30];

float sORn[4];
int type[4];
//float L,R,M,signS;
int   signO,sflag,s=0;
int   RF=0,LF=0,SE=0,RS=0,LS=0;
int   xflag,oflag;

//电磁初始化
void elec_init(void)
{
//  Fuzzy_Init(&eSteerPID,100,50,20,10,5,3);
  adc_init(AD_CHANNEL1);
  adc_init(AD_CHANNEL2);
  adc_init(AD_CHANNEL3);
  adc_init(AD_CHANNEL4);
  adc_init(AD_CHANNEL5);

  limit_min[0] = 255;
  limit_min[1] = 255;
  limit_min[2] = 255;
  limit_min[3] = 255;
  limit_min[4] = 255;
  limit_max[0] = 0;
  limit_max[1] = 0;
  limit_max[2] = 0;
  limit_max[3] = 0;
  limit_max[4] = 0;
  type[0]=1;
  type[1]=0;
  type[2]=1;
  type[3]=0;
  count=0;
}

//冒泡排序
void Sort_MAX_N(uint16 arr[], int size)
{
  int i = 0, k = 0;
  int temp = 0;
  for (k = 0; k<size-1; k++)
  {
    //降序排列！
    for (i = size - 1; i>k; i--)
    {
      if (arr[i]>arr[i - 1])
      {
        temp = arr[i - 1];
        arr[i - 1] = arr[i];
        arr[i] = temp;
      }
    }
  }
}

//采集电磁数据
void get_elec(ElecTypeDef *eStr)
{
  //数据采集
  for(uint8 i=0; i<FILTER_TIMES; i++)
  {
    eStr->Elec_Data[0][i]=  adc_once(AD_CHANNEL1,ADC_8bit);//采集电感的数据，R
    eStr->Elec_Data[1][i]=  adc_once(AD_CHANNEL2,ADC_8bit);//采集电感的数据，M
    eStr->Elec_Data[2][i]=  adc_once(AD_CHANNEL3,ADC_8bit);//采集电感的数据，L
    eStr->Elec_Data[3][i]=  adc_once(AD_CHANNEL4,ADC_8bit);//RV
    eStr->Elec_Data[4][i]=  adc_once(AD_CHANNEL5,ADC_8bit);//LV
  }
}

//中值滤波
void fillter_middle(ElecTypeDef *eStr)
{
  int i = 0, k = 0;
  float temp = 0;
  for (k = 0; k<FILTER_TIMES-1; k++)
  {
    //降序排列！
    for (i = FILTER_TIMES - 1; i>k; i--)
    {
      if (eStr->Elec_Data[0][i]>eStr->Elec_Data[0][i - 1])
      {
        temp = eStr->Elec_Data[0][i - 1];
        eStr->Elec_Data[0][i - 1] = eStr->Elec_Data[0][i];
        eStr->Elec_Data[0][i] = temp;
      }
      if (eStr->Elec_Data[1][i]>eStr->Elec_Data[1][i - 1])
      {
        temp = eStr->Elec_Data[1][i - 1];
        eStr->Elec_Data[1][i - 1] = eStr->Elec_Data[1][i];
        eStr->Elec_Data[1][i] = temp;
      }
      if (eStr->Elec_Data[2][i]>eStr->Elec_Data[2][i - 1])
      {
        temp = eStr->Elec_Data[2][i - 1];
        eStr->Elec_Data[2][i - 1] = eStr->Elec_Data[2][i];
        eStr->Elec_Data[2][i] = temp;
      }
      if (eStr->Elec_Data[3][i]>eStr->Elec_Data[3][i - 1])
      {
        temp = eStr->Elec_Data[3][i - 1];
        eStr->Elec_Data[3][i - 1] = eStr->Elec_Data[3][i];
        eStr->Elec_Data[3][i] = temp;
      }
      if (eStr->Elec_Data[4][i]>eStr->Elec_Data[4][i - 1])
      {
        temp = eStr->Elec_Data[4][i - 1];
        eStr->Elec_Data[4][i - 1] = eStr->Elec_Data[4][i];
        eStr->Elec_Data[4][i] = temp;
      }
    }
  }
  //归一化处理
  memset(eStr->Lvbojieguo,0,sizeof(eStr->Lvbojieguo));
  eStr->eSum=0;
  //中值滤波赋值结果
  eStr->Lvbojieguo[0]=ELEC_Plus_Right*1000.0f*(eStr->Elec_Data[0][(FILTER_TIMES/2)-1]+eStr->Elec_Data[0][(FILTER_TIMES/2)-2]+eStr->Elec_Data[0][FILTER_TIMES/2]+eStr->Elec_Data[0][(FILTER_TIMES/2)+1])/(4.0f*255);//R
  eStr->Lvbojieguo[1]=ELEC_Plus_Middle*1000.0f*(eStr->Elec_Data[1][(FILTER_TIMES/2)-1]+eStr->Elec_Data[1][(FILTER_TIMES/2)-2]+eStr->Elec_Data[1][FILTER_TIMES/2]+eStr->Elec_Data[1][(FILTER_TIMES/2)+1])/(4.0f*255);//M
  eStr->Lvbojieguo[2]=ELEC_Plus_Left*1000.0f*(eStr->Elec_Data[2][(FILTER_TIMES/2)-1]+eStr->Elec_Data[2][(FILTER_TIMES/2)-2]+eStr->Elec_Data[2][FILTER_TIMES/2]+eStr->Elec_Data[2][(FILTER_TIMES/2)+1])/(4.0f*255);//L
  eStr->Lvbojieguo[3]=ELEC_Plus_Right_Ver*1000.0f*(eStr->Elec_Data[3][(FILTER_TIMES/2)-1]+eStr->Elec_Data[3][(FILTER_TIMES/2)-2]+eStr->Elec_Data[3][FILTER_TIMES/2]+eStr->Elec_Data[3][(FILTER_TIMES/2)+1])/(4.0f*255);//RV
  eStr->Lvbojieguo[4]=ELEC_Plus_Left_Ver*1000.0f*(eStr->Elec_Data[4][(FILTER_TIMES/2)-1]+eStr->Elec_Data[4][(FILTER_TIMES/2)-2]+eStr->Elec_Data[4][FILTER_TIMES/2]+eStr->Elec_Data[4][(FILTER_TIMES/2)+1])/(4.0f*255);//LV
  eStr->eSum=eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2];
}


//电磁环岛识别
void eRound_Recognition(ElecTypeDef *eStr)
{
  //如果电磁和值超过一定值
  if(eStr->eSum>Round_SUM)
  {
    eRoundProcess=eFind_Round;
    //判断竖直电感值
    if(eStr->Lvbojieguo[4]>eStr->Lvbojieguo[3])//左边大于右边
    {
      eRoundType=eLeft_Round;
    }
    else
    {
      eRoundType=eRight_Round;
    }
  }
}

float Into_Round_Err;
int Into_Round_Distance;
//电磁环岛处理
void eRound_Process(ElecTypeDef *eStr)
{
  switch(eRoundType)
  {
  case eLeft_Round:
    {
      switch(eRoundProcess)
      {
      case eRound_Not_Found:
        {
          ELEC_err=150.0f*(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])/(eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2]);//正常取偏差
          break;
        }
      case eFind_Round:
        {
          ELEC_err=150.0f*(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])/(eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2]);//正常偏差
          //缩小左边偏差，放大右边偏差，因为入环的时候会往里打
          if(ELEC_err<0)
            ELEC_err=ELEC_err*1.5;
          else
            ELEC_err=ELEC_err*0.5;

          if(fabs(eStr->Lvbojieguo[4]-eStr->Lvbojieguo[3])<20 && (eStr->Lvbojieguo[3]<35 || eStr->Lvbojieguo[4]<35))
          {
            eRoundProcess=eInto_Round;
            if(eStr->Lvbojieguo[0]-eStr->Lvbojieguo[2]>200)//右减去左大于200，说明入环位置中间偏左
            {
              Into_Round_Err=eInto_Left_Round_Offset*0.8;
              Into_Round_Distance=650;
            }
            else if(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0]>200)//左减右大于200，说明中间偏右
            {
              Into_Round_Err=eInto_Left_Round_Offset*1.2;
              Into_Round_Distance=1000;
            }
            else//位置较为中间
            {
              Into_Round_Err=eInto_Left_Round_Offset;
              Into_Round_Distance=850;
            }
          }
          break;
        }
      case eInto_Round:
        {
          ELEC_err=Into_Round_Err;//左打死
          if(eStr->eSum<Round_SUM && distance_judge(Into_Round_Distance))//左中右电磁总和小于圆环和值，且跑过了1000
          {
            eRoundProcess = eIn_Round;
          }
          break;
        }
      case eIn_Round:
        {
          ELEC_err=150.0f*(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])/(eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2]);//正常偏差
          if(eStr->eSum>Round_SUM && fabs(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[1])<100)//左减中小于5（这种判断待定）
          {
            //if(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])//判断左右电感大小来决定出环舵机打角大小
            eRoundProcess = eOut_Round;
          }
          break;
        }
      case eOut_Round:
        {
          ELEC_err=150.0f*(eStr->Lvbojieguo[1]-eStr->Lvbojieguo[0])/(eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2]);
          if(fabs(eStr->Lvbojieguo[4]-eStr->Lvbojieguo[3])<20 && (eStr->Lvbojieguo[4]>100 || eStr->Lvbojieguo[3]>100)& distance_judge(700))
          {
            eRoundProcess=eLeave_Round;
          }
          break;
        }
      case eLeave_Round:
        {
          ELEC_err=150.0f*(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])/(eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2]);
          //缩小左边偏差，放大右边偏差，因为入环的时候会往里打
          if(ELEC_err<0)
            ELEC_err=ELEC_err*3;
          else
            ELEC_err=ELEC_err*0.25;
          if(eStr->eSum<Round_SUM)
          {
            eRoundProcess=eRound_Not_Found;
            eRoundType=eNo_Round;
          }
        }
      }
      break;
    }
  case eRight_Round:
    {
      switch(eRoundProcess)
      {
      case eRound_Not_Found:
        {
          ELEC_err=150.0f*(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])/(eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2]);//正常取偏差
          break;
        }
      case eFind_Round:
        {
          ELEC_err=150.0f*(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])/(eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2]);//正常偏差
          //缩小左边偏差，放大右边偏差，因为入环的时候会往里打
          if(ELEC_err>0)
            ELEC_err=ELEC_err*2;
          else
            ELEC_err=ELEC_err*0.5;

          if(fabs(eStr->Lvbojieguo[3]-eStr->Lvbojieguo[4])<20 && (eStr->Lvbojieguo[3]<35 || eStr->Lvbojieguo[4]<35))
          {
            eRoundProcess=eInto_Round;
            //根据不同入环位置确定舵机偏差
            /*此处细调*/
            if(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0]>200)//左减右大于200，说明中间偏右
            {
              Into_Round_Err=eInto_Right_Round_Offset*0.8;
              Into_Round_Distance=850;
            }
            else if(eStr->Lvbojieguo[0]-eStr->Lvbojieguo[2]>200)//右减去左大于200，说明入环位置中间偏左
            {
              Into_Round_Err=eInto_Right_Round_Offset*1.2;
              Into_Round_Distance=1200;
            }
            else//位置较为中间
            {
              Into_Round_Err=eInto_Right_Round_Offset;
              Into_Round_Distance=1000;
            }
          }
          break;
        }
      case eInto_Round:
        {
          ELEC_err=eInto_Right_Round_Offset;//右打死!
          if(eStr->eSum<Round_SUM && distance_judge(800))//左中右电磁总和小于圆环和值，且跑过了800
          {
            eRoundProcess = eIn_Round;
          }
          break;
        }
      case eIn_Round:
        {
          ELEC_err=150.0f*(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])/(eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2]);//正常偏差
          if(eStr->eSum>Round_SUM && fabs(eStr->Lvbojieguo[0]-eStr->Lvbojieguo[1])<100)//右减中小于100（这种判断待定）
          {
            /*此处细调*/
            //            if(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])//判断左右电感大小来决定出环舵机打角大小
            eRoundProcess = eOut_Round;
          }
          break;
        }
      case eOut_Round:
        {
          ELEC_err=eOut_Right_Round_Offset;//待修改
          if(fabs(eStr->Lvbojieguo[3]-eStr->Lvbojieguo[4])<20 && (eStr->Lvbojieguo[4]>100 || eStr->Lvbojieguo[3]>100) || distance_judge(500))
          {
            eRoundProcess=eLeave_Round;
          }
          break;
        }
      case eLeave_Round:
        {
          ELEC_err=150.0f*(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])/(eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2]);
          //缩小右边偏差，放大左边偏差，因为出环的时候会往里打
          if(ELEC_err>0)
            ELEC_err=ELEC_err*3;
          else
            ELEC_err=ELEC_err*0.25;
          if(eStr->eSum<Round_SUM)
          {
            eRoundProcess=eRound_Not_Found;
            eRoundType=eNo_Round;
          }
          break;
        }
      }
      break;
    }
  default:
    {
      ELEC_err=150.0f*(eStr->Lvbojieguo[2]-eStr->Lvbojieguo[0])/(eStr->Lvbojieguo[0]+eStr->Lvbojieguo[1]+eStr->Lvbojieguo[2]);
//      eRound_Recognition(eStr);
      break;
    }
  }
}

void ELEC_Display(void)
{
  LCD_P6x8Str(0,0," Right:");
  LCD_P6x8Str(0,1," Middle:");
  LCD_P6x8Str(0,2," Left:");
  LCD_P6x8Str(0,3," RV:");
  LCD_P6x8Str(0,4," Lv:");
  LCD_P6x8Str(0,5," Sum:");
  LCD_P6x8Str(0,6," eError:");
  LCD_P6x8Str(0,7,"eRoundProcess");

  OLED_PrintFloat(70,0,ELEC.Lvbojieguo[0]);
  OLED_PrintFloat(70,1,ELEC.Lvbojieguo[1]);
  OLED_PrintFloat(70,2,ELEC.Lvbojieguo[2]);
  OLED_PrintFloat(70,3,ELEC.Lvbojieguo[3]);
  OLED_PrintFloat(70,4,ELEC.Lvbojieguo[4]);
  OLED_PrintFloat(70,5,ELEC.eSum);
  OLED_PrintFloat(70,6,ELEC_err);
  LCD_Show_Number3(80,7,eRoundProcess+100*eRoundType);
}

void ELEC_Display_ADC()
{
  LCD_P6x8Str(0,0," RightAD:");
  LCD_P6x8Str(0,1," MiddleAD:");
  LCD_P6x8Str(0,2," LeftAD:");
  LCD_P6x8Str(0,3," RVAD:");
  LCD_P6x8Str(0,4," LvAD:");

  LCD_Show_Number3(70,0,(uint8)ELEC.Elec_Data[0][0]);
  LCD_Show_Number3(70,1,(uint8)ELEC.Elec_Data[1][0]);
  LCD_Show_Number3(70,2,(uint8)ELEC.Elec_Data[2][0]);
  LCD_Show_Number3(70,3,(uint8)ELEC.Elec_Data[3][0]);
  LCD_Show_Number3(70,4,(uint8)ELEC.Elec_Data[4][0]);
}

void Push_And_Pull(float *buff,int len,float newdata)
{
 int i;
 for(i=len-1;i>0;i--)
 {
   *(buff+i)=*(buff+i-1);
 }
   *buff=newdata;
}

void elec(void)
{
  get_elec(&ELEC);//采集
  fillter_middle(&ELEC); //中值滤波
  eRound_Process(&ELEC);
  Elec_Lose_Protect(&ELEC);
}

void s_fillter(void)
{
  int i=0,s_count;

  fillter_s[0][i++]=adc_once(AD_CHANNEL1,ADC_8bit);
  fillter_s[1][i++]=adc_once(AD_CHANNEL2,ADC_8bit);
  fillter_s[2][i++]=adc_once(AD_CHANNEL3,ADC_8bit);
  fillter_s[3][i++]=adc_once(AD_CHANNEL4,ADC_8bit);
  fillter_s[4][i++]=adc_once(AD_CHANNEL5,ADC_8bit);

  if(i==30)
  {
    i=0;
  }
  for(s_count=0;s_count<30;s_count++)
  {
    elec_sum[0]+=fillter_s[0][s_count];
    elec_sum[1]+=fillter_s[1][s_count];
    elec_sum[2]+=fillter_s[2][s_count];
    elec_sum[3]+=fillter_s[3][s_count];
    elec_sum[4]+=fillter_s[4][s_count];
  }
  lvbojieguo[0]=elec_sum[0]/30.0f;
  lvbojieguo[1]=elec_sum[1]/30.0f;
  lvbojieguo[2]=elec_sum[2]/30.0f;
  lvbojieguo[3]=elec_sum[3]/30.0f;
  lvbojieguo[4]=elec_sum[4]/30.0f;
}

#define Elec_Lose_Sum 777
void Elec_Lose_Protect(ElecTypeDef *eStr)
{
  //如果和值小于丢线值，且左电感大于右电感，左>中>右
  if(eStr->eSum<Elec_Lose_Sum&& eStr->Lvbojieguo[0]<eStr->Lvbojieguo[1]&&eStr->Lvbojieguo[1]<eStr->Lvbojieguo[2])
  {
    ELEC_err=120;
  }
  //如果和值小于丢线值，且右电感大于左电感，右>中>左
  else if(eStr->eSum<Elec_Lose_Sum&& eStr->Lvbojieguo[2] < eStr->Lvbojieguo[1] && eStr->Lvbojieguo[1] < eStr->Lvbojieguo[0])
  {
    ELEC_err=-120;
  }
}
