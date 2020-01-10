#include "DIRECTION.h"

#if 0
//获取中心线偏差量
float GetSteerError(unsigned char start,unsigned char end,float midpos)
{
  unsigned char i=0;
  unsigned char iCount=0;
  unsigned int  Black_Sum=0;
  float TemError = 0.0;
  for(i = start,iCount = 0; i < end; i++,iCount++)
  {
    Black_Sum += BlackLineData[i];
  }
  TemError = 	Black_Sum*1.0/iCount - midpos;
  return TemError;
}

int LineWeight[OV7725_EAGLE_H] = {0};

float GetSteerError2(unsigned char start,unsigned char end,float midpos)
{
  unsigned char i=0;
  //unsigned char iCount=0;
  unsigned int  Black_Sum=0;
  unsigned int weightSum = 0;
  float TemError = 0.0;
  MidLineExcursion = 0;
  for(i = 1; i < OV7725_EAGLE_H; i++)
  {
    Black_Sum += BlackLineData[i]*LineWeight[i];
    weightSum += LineWeight[i];
  }
  TemError = 	Black_Sum*1.0/weightSum - midpos;

  if(TemError > 40.0)
  {
    TemError = 40.0;
  }
  if(TemError < -40.0)
  {
    TemError = -40.0;
  }
  return TemError;
}
#endif

#if 0
volatile unsigned char IsStartLine = 0;
int CrossingBegin = 0;
int CrossingCount = 0;

int Foresight=15;//定义前瞻量
unsigned char StraightFS=45;
unsigned char SmallSFS=45;
unsigned char BigFS=40;
unsigned char BendFS=35;
unsigned char CommonFS=35;
unsigned char StraightToBendFS=35;

unsigned char StraightEnd=OV7725_EAGLE_H-8;
unsigned char SmallSEnd=OV7725_EAGLE_H-8;
unsigned char BigSEnd=OV7725_EAGLE_H-6;
unsigned char BendEnd=OV7725_EAGLE_H-10;
unsigned char CommonEnd=OV7725_EAGLE_H-8;
unsigned char StraightToBenEnd = OV7725_EAGLE_H-8;
unsigned char FarWeight = 0;

float StraightK = 0.3;
float SmallSK = 0.40;
float BigSK = 0.50;
float BendK = 0.65;
float CommonK = 0.55;
float StraightToBendK = 0.65;

float Error=0.0;
float LastError=0.0;
float k = 1.0;

extern float ke=0.0;

void DirectionCtrol()
{
  float temK =1.0;
  int StartPos = OV7725_EAGLE_H - 15;
  int EndPos = OV7725_EAGLE_H-1;
  int MidPos = (int)(BlackLineData[OV7725_EAGLE_H-1] - LeftBlack[OV7725_EAGLE_H-1]*1.0/2 + (OV7725_EAGLE_W-RightBlack[OV7725_EAGLE_H-1])*1.0/2);
  unsigned char i = 0;
  unsigned char CtrlWeight = FarWeight;

  if(StableNumbers < 12)
  {
    if(MidLineExcursion > 0)
    {
      SERVO((int32)SERVO_STEER(35));
      ke = 35;
    }
    else if(MidLineExcursion < 0)
    {
      SERVO((int32)SERVO_STEER(-35));
      ke = -35;
    }
    return;
  }

  Error=0.0;

  switch(RoadType)
  {
    case 0://直
            Foresight = StraightFS;
            if(Foresight > StableNumbers-10)Foresight=StableNumbers-10;
            k = StraightK;
            StartPos = OV7725_EAGLE_H - Foresight+1;
            EndPos = StraightEnd;
            LastRoadType=RoadType;//
            break;

    case 1://s
            k = SmallSK;
            if(Foresight > StableNumbers-10)Foresight=StableNumbers-10;
            StartPos = OV7725_EAGLE_H - Foresight+1;
            EndPos =SmallSEnd;

            switch(LastRoadType)
            {
            case 2:
                    k = k + 0.2;
                    break;
            case 3:
                    k = k + 0.3;
                    break;
            default:
                    break;
            }
            LastRoadType=RoadType;//
            break;

    case 2://S
            Foresight=BigFS;
            if(Foresight > StableNumbers+CompensateCount-11)Foresight=StableNumbers+CompensateCount-11;
            k = BigSK;
            StartPos = OV7725_EAGLE_H - Foresight+1;
            EndPos = BigSEnd;
            LastRoadType=RoadType;//
            break;

    case 3://急弯
            Foresight=BendFS;//17
            if(Foresight > StableNumbers+CompensateCount-11)Foresight=StableNumbers+CompensateCount-11;
            k = BendK;
            StartPos = OV7725_EAGLE_H - Foresight+1;
            EndPos = BendEnd;
            MidPos = OV7725_EAGLE_W/2-1;
            LastRoadType=RoadType;//
            break;

    case 103://直到进弯
            Foresight=StraightToBendFS;
            if(Foresight > StableNumbers+CompensateCount-11)Foresight=StableNumbers+CompensateCount-11;
            StartPos = OV7725_EAGLE_H - Foresight+1;
            EndPos = StraightToBenEnd;
            LastRoadType=103;
            k = StraightToBendK;
            break;

    default:
            k = CommonK;
            Foresight=CommonFS;
            StartPos = OV7725_EAGLE_H - Foresight+1;
            EndPos = CommonEnd;
            break;

  }

  if(IsCrossing)
  {
    Foresight=20;
    if(Foresight > StableNumbers+CompensateCount-11)
      Foresight=StableNumbers+CompensateCount-11;
    StartPos = OV7725_EAGLE_H - Foresight+1;
    EndPos = OV7725_EAGLE_H-5;
  }

  if(StartPos >= EndPos)
  {
    StartPos = EndPos - 2;
    EndPos = EndPos + 2;
  }
  if(IsStartLine)
  {
    StartPos = OV7725_EAGLE_H-10;
    EndPos = OV7725_EAGLE_H-1;
    k = 1.0;
  }


  if( CrossingBegin)
  {
    CtrlWeight = 10;
  }

  for (i = OV7725_EAGLE_H-1;i > 0;i--)
  {
    if (i > OV7725_EAGLE_H - (StableNumbers+CompensateCount-11))
    {
      if (i >= StartPos && i <= EndPos)
      {
        if(ValidLine[i] == 0)
        {
                LineWeight[i]=3;
        }
        else{
                LineWeight[i]=10;
        }
      }
      else if(i > EndPos)
      {
        LineWeight[i]=1;
      }
      else if(i > 13)
      {
        LineWeight[i]=CtrlWeight;
      }
    }
    else
    {
      LineWeight[i]=0;
    }
  }

  if(IsCrossing && CrossingBegin == 0)//十字标志置位，但十字并未开始
  {
    CrossingBegin=1;//十字开始

    StartPos = OV7725_EAGLE_H-12;//其实行

    if(StartPos < OV7725_EAGLE_H-(StableNumbers2-5))
    {
      StartPos = OV7725_EAGLE_H-(StableNumbers2-5);
    }

    MidPos=OV7725_EAGLE_W/2-1;//改动过，按照我的理解应该是这个

    EndPos = OV7725_EAGLE_H-1;

    Error = GetSteerError(StartPos,EndPos,MidPos);

    SERVO((int32)SERVO_STEER(Error));

    LastError = Error;

    ke = Error;

    return;
  }

  if(CrossingBegin)
  {
    CrossingCount++;//过十字标志进行累加

    if(CrossingCount > 3)//计数大于3
    {
      CrossingBegin=0;//开始标志清零
      CrossingCount=0;//计数标志清零
      //十字走完，结束
    }
    else
    {
      SERVO((int32)SERVO_STEER(LastError));//执行上次舵机动作
    }
  }

  Error = GetSteerError2(StartPos,EndPos,MidPos)*k*temK;//计算误差

  SERVO((int32)SERVO_STEER(Error));//舵机作用//g

  ke = Error;//把偏差传给电机进行差速

  LastError = Error;//更新
}

#endif