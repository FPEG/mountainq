/*!
 * @file      Feature_Extration.c
 * @brief      特征提取提取函数
 * @author
 * @version
 * @date       2019-05-21
 */
#include "include.h"
#include "Feature_Extration.h"


Str_BlackEnd  BlackEnd;
Str_ZebraLine ZebraLine;

#if 0

////两点求直线函数
unsigned char JudgeStraightLineL( int End )
{
  int i;
  unsigned char StartTempL = 0;
  unsigned char NumTempL = 0;
  float SlopeL = 0.0;
  float Sum = 0.0;
  memset(LoopLeftEdge,0,sizeof(LoopLeftEdge));//数组清零
  for( i = 58; i > End; i-- )
  {
    if( LeftEdge[i] != 0 )
    {
      StartTempL = i; //记录左边界不丢线开始行
      break;
    }
  }
  if( StartTempL > End ) //
  {
    for( i = StartTempL; i > End; i-- )
    {
      if( LeftEdge[i] == 0 )
      {
        NumTempL++;
      }
    }
    if( NumTempL < 2 ) //连续
    {
      //计算斜率
      SlopeL = 1.0 * ( LeftEdge[End + 1] - LeftEdge[StartTempL] ) / ( StartTempL - End + 1 );
      for( i = 0; i <= StartTempL - End + 1; i++ )
      {
        LoopLeftEdge[StartTempL - i] = ( unsigned char )( ( i * SlopeL ) + LeftEdge[StartTempL] );
        Sum = Sum + ABS( ( LoopLeftEdge[StartTempL - i] - LeftEdge[StartTempL - i] ) );
      }
      Sum = Sum / ( StartTempL - End + 1 );
    }
    //Sum=2.1;
    if( Sum != 0 )
    {
      if( Sum <= 2.85 )
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
  }
  return 0;
}

//////////判断直线函数
unsigned char JudgeStraightLineR(int End )
{
    int i;
    unsigned char StartTempR = 0;
    unsigned char NumTempR = 0;
    float SlopeR = 0.0;
    float SumR = 0.0;

    memset(LoopRightEdge,0,sizeof(LoopRightEdge));//数组清零
    for( i = 58; i > End; i-- )
    {
	   if( RightEdge[i] < ColumnMax - 1 )
	   {
		  StartTempR = i;
		  break;
	   }
    }
    if( StartTempR > End )
    {
	   for( i = StartTempR; i > End; i-- )
	   {
		  if( RightEdge[i] >= ColumnMax - 1 )
		  {
			 NumTempR++;
		  }
	   }
	   if( NumTempR < 2 ) //连续
	   {
		  //计算斜率
		  SlopeR = 1.0 * ( RightEdge[StartTempR] - RightEdge[End + 1] ) / ( StartTempR - End + 1 );
		  for( i = 0; i <= StartTempR - End + 1; i++ )
		  {
			 LoopRightEdge[StartTempR - i] = 79 - ( unsigned char )( ( ( i * SlopeR ) ) + ( 79 - ( RightEdge[StartTempR] ) ) );
			 SumR = SumR + ABS( ( LoopRightEdge[StartTempR - i] - RightEdge[StartTempR - i] ) );
		  }
		  SumR = SumR / ( StartTempR - End + 1 );
	   }
	   //Sum=2.1;
	   if( SumR != 0 )
	   {
		  if( SumR <= 2.85 )
		  {
			 return 1;
		  }
		  else
		  {
			 return 0;
		  }
	   }
    }
    return 0;
}
#endif


/**********************************************************
起跑线检测
***********************************************************/
//uint32 total_time;//主循环计数
void Mark_ZebraLine(unsigned int Z_TIME) //600
{
       if(ZebraLine.total_time<Z_TIME)
       {
	      ZebraLine.total_time++;      //记录控制的次数
       }
       if(ZebraLine.total_time>=Z_TIME)
       {
	      for(uint8 i=38;i>15;i--)
	      {
		     ZebraLine.break_point_num=0;
		     if( Boundary.RightEdge[i]&&Boundary.LeftEdge[i])
		     {
			    for(uint8 j=Boundary.LeftEdge[i];j<Boundary.RightEdge[i];j++)
			    {
				   if(image[i][j]!=image[i][j+1])//跳变
				   {
					  ZebraLine.break_point_num++;
					  if(ZebraLine.break_point_num>10)
					  {
						 ZebraLine.many_bp_row++;
						 break;
					  }
				   }
			    }
		     }
		     if(ZebraLine.many_bp_row>4)//黑白跳变点数 90
		     {
			    stop_car_flag=1;
			    ZebraLine.ZebraLine_Flag=1;
			    ZebraLine.ZebraLine_Pos=i;
			    break;
		     }
	      }
       }
//       LCD_Show_Number3(30,4,qipao_lne);
//       LCD_Show_Number3(30,5,stop_car_flag) ;
}


#if 0//图像坡道判断
#define Width_Factor 1.21//赛道变宽系数
void RampJudge(uint16 ScanStart ,uint16 ScanEnd)
{
    uint32 i;
    int a;
    a=0;
    memset(RampRoadWidth,0,sizeof(RampRoadWidth));     //数组清零       &&!SecondRamdFlag
  //  RampFlag=FALSE;
    if(ABS(Error)<=10//&&LastLine>=4  RightLose<5&&LeftLose<5&&
       &&RampFlag==FALSE&&In_RightRound==0&&In_LeftRound==0&&ObsScan.ObstaclePosition==NO_OB)//直道条件        考虑加入不丢边最远行的点的坐标
    {
        if(RampSlopeL<-200&&RampSlopeR>200)//斜率条件
        {
            for(i=ScanStart; i>ScanEnd; i--)
            {
                if(RightEdge[i]<79&&LeftEdge[i]!=0&&RightEdge[i-1]<79&&LeftEdge[i-1]!=0&&RightEdge[i+1]<79&&LeftEdge[i+1]!=0)
                    //不丢线条件
                {
                    RampRoadWidth[i-1]=ABS(RightEdge[i-1]-LeftEdge[i-1]);
                    RampRoadWidth[i]=ABS(RightEdge[i]-LeftEdge[i]);
                    RampRoadWidth[i+1]=ABS(RightEdge[i+1]-LeftEdge[i+1]);

                         if(RampRoadWidth[i]>ObsWidthS[i]*Width_Factor
                            &&RampRoadWidth[i-1]>ObsWidthS[i-1]*Width_Factor
                            &&RampRoadWidth[i+1]>ObsWidthS[i+1]*Width_Factor//赛道宽度变宽条件

                            &&RightEdge[i]-RightEdge[i-1]>=0&&RightEdge[i]-RightEdge[i-1]<3//赛道平滑条件
                            &&LeftEdge[i-1]-LeftEdge[i]>=0&&LeftEdge[i-1]-LeftEdge[i]<3)
                    {
                        a++;
                        if(a>=2)
                        {
                          a=0;
                          RampFlag=TRUE;//坡道标志挂起
                          break;
                        }
                    }
                }
            }
        }
    }

}
#endif


#if 0
void LR_BourndTrace(void)    //左右边界最远点追踪
{
  int i;
  Left_Turn_End=0,Right_Turn_End=0;
  Left_Turn_End_Hang=0,Right_Turn_End_Hang=0;
  for(i=50;i>LastLine+2;i--)//
  {
    if(LeftEdge[i]==0&&LeftEdge[i-1]==0&&i<=25||i<=LastLine)
    {
      break;
    }

    else if(LeftEdge[i]<=LeftEdge[i-1])
    {
      Left_Turn_End=LeftEdge[i-1];
      Left_Turn_End_Hang=i-1;

    }
  }

    for(i=50;i>LastLine+2;i--)
  {
    if(RightEdge[i]>=79&&RightEdge[i-1]>=79&&i<=25||i<=LastLine)
    {
      break;
    }

    else if(RightEdge[i]>=RightEdge[i-1])
    {
      Right_Turn_End=RightEdge[i-1];
      Right_Turn_End_Hang=i-1;

    }
  }

}
#endif


 //最小二乘法拟合斜率

float Slope_Calculate(uint8 begin,uint8 end,int *p)
{
  float xsum=0,ysum=0,xysum=0,x2sum=0;
   uint8 i=0;
   float result=0;
   static float resultlast;
   p=p+begin;
   for(i=begin;i<end;i++)
   {
	   xsum+=i;
	   ysum+=*p;
	   xysum+=i*(*p);
	   x2sum+=i*i;
	   p=p+1;
   }
  if((end-begin)*x2sum-xsum*xsum) //判断除数是否为零
  {
    result=((end-begin)*xysum-xsum*ysum)/((end-begin)*x2sum-xsum*xsum);
    resultlast=result;
  }
  else
  {
   result=resultlast;
  }
  return result;
}



/*!
 *  @brief      获取黑线截至行
 *  @param
 *  @return     VOID
 *  @note       根据底行颜色提取黑截止行黑白截止行
 *  @since
 *  time
 *  Sample usage:
 */
void GetBlackEndParam()//获取黑线截至行
{
       BlackEnd.LEndFlag = 0;//左截至标志
       BlackEnd.MEndFlag = 0;//中·截至标志
       BlackEnd.REndFlag = 0;//右截至标志
       BlackEnd.BlackEndL = 0;//左截至计数
       BlackEnd.BlackEndM = 0;//中截至计数
       BlackEnd.BlackEndR = 0;//右截至计数

       for (int i = ROW_END-1; i >0 ; i--)
       {
	      //中间
	      if(image[i][40] == White && ! BlackEnd.MEndFlag )
	      {
		     BlackEnd.BlackEndM++;//中截至计数增加
	      }
	      else if(image[i-1][40] == Black)
	      {
		     BlackEnd.MEndFlag = 1;//上两行为黑，截至标志置位
	      }

	      //四分之一位置
	      if(image[i][20] == White && ! BlackEnd.LEndFlag)
	      {
		     BlackEnd.BlackEndL++;//左截至计数增加
	      }
	      else if(image[i-1][20] == Black)
	      {
		     BlackEnd.LEndFlag=1;//上两行为黑，截至标志置位
	      }

	      //四分之三位置
	      if(image[i][60] == White && ! BlackEnd.REndFlag )
	      {
		     BlackEnd.BlackEndR++;//右截至计数增加
	      }
	      else if(image[i-1][60] == Black)
	      {
		     BlackEnd.REndFlag = 1;//上两行为黑，截至标志置位
	      }
       }

       //计算最远行
       BlackEnd.BlackEndMax = MAX(BlackEnd.BlackEndL,BlackEnd.BlackEndM);
       BlackEnd.BlackEndMax = MAX(BlackEnd.BlackEndMax,BlackEnd.BlackEndR);

       BlackEnd.BlackEndLMR = BlackEnd.BlackEndL + BlackEnd.BlackEndM + BlackEnd.BlackEndR;//最远有效黑点累和

       if( BlackEnd.BlackEndMax<2)
       {
	      //stop_car_flag=1;
       }
//       LCD_PrintU16(50,1, BlackEnd.BlackEndMax);
//       LCD_PrintU16(50,3,BlackEnd.BlackEndL);
//       LCD_PrintU16(50,5,BlackEnd.BlackEndM);
//       LCD_PrintU16(50,7,BlackEnd.BlackEndR);
//       LCD_PrintU16(50,2,BlackEnd.BlackEndLMR);

}



/******************************************************************************
//曲线拟合   CommonRectificate(&P_LeftBlack[0],startPos-1,endPos+1);
******************************************************************************/
void CommonRectificate(int data[],unsigned char begin,unsigned char end)
{
       unsigned char MidPos = 0;
       if (end > 60-1)
       {
	      end = 60-1;
       }
       if (begin == end)
       {
	      data[begin] = (data[begin-1]+data[begin+1])/2;
	      BlackLineData[begin] =  LeftBlack[begin] + (RightBlack[begin]-LeftBlack[begin])/2;
       }
       else if(begin < end)
       {
	      MidPos = (begin+end)/2;
	      data[MidPos] = (data[begin]+data[end])/2;
	      BlackLineData[MidPos] =  LeftBlack[MidPos] + (RightBlack[MidPos]-LeftBlack[MidPos])/2;
	      if (begin+1 < MidPos)
	      {
		     CommonRectificate(data,begin,MidPos);
	      }
	      if (MidPos+1 < end)
	      {
		     CommonRectificate(data,MidPos,end);
	      }
       }
}
/******************************************************************************
//中心线补偿
******************************************************************************/

int CompensateCount = 0;
 int StableNumbers;
void MidLineCompensate()
{
  int i=0,icount=0,j=0;
  int CompensateData = 0;

  int sum = 0;
  float avg =0.0;
  int tem = 1;//正负补偿标志

  CompensateCount=0;

  for (i = 60-2,icount=0;i > 60-(StableNumbers-10);i--,icount++)
  {
    sum += (BlackLineData[i]-BlackLineData[i+1]);//进行所有中线数据平均偏离数据类和
  }
  avg = sum*1.0 / icount;//平均

  if (avg < 0)
  {
    tem = -1;
    avg = (-1)*avg;
  }
  if (avg > 1.0)
  {
    CompensateData = 3;//偏离程度大，补偿量大
  }
  else if (avg > 0.35)
  {
    CompensateData = 2;//偏离程度中，补偿量中
  }
  else if (avg > 0.25)
  {
    CompensateData = 1;//偏离程度小，补偿量小
  }
  else
  {
    CompensateData = 0;
  }

  CompensateData = CompensateData*tem;

  for (i = 60 - (StableNumbers-10);i > 0;i--)
  {
    BlackLineData[i] = BlackLineData[i+1] + CompensateData;

    CompensateCount++;

    if (image[i][BlackLineData[i]] == Black || BlackLineData[i] < 2 || BlackLineData[i] > 80 -2)
    {
      break;
    }
    sum = 0;
    for (j = 60-2,icount=0;j > i;j--,icount++)
    {
      sum += (BlackLineData[j]-BlackLineData[j+1]);
    }
    avg = sum*1.0 / icount;
    if (avg < 0)
    {
      tem = -1;
      avg = (-1)*avg;
    }
    if (avg > 1.0)
    {
      CompensateData = 3;
    }
    else if (avg > 0.55)
    {
      CompensateData = 2;
    }
    else if (avg > 0.25)
    {
      CompensateData = 1;
    }
    else
    {
      CompensateData = 0;
    }
    CompensateData = CompensateData*tem;
  }
}
/******************************************************************************
//获取中心线偏差
******************************************************************************/


double MidLineVariance = 0.0;
int MidLineExcursion = 0;

void GetMidLineVariance()
{
  unsigned char i=0;
  unsigned char iCount=0;
  unsigned int  Black_Sum=0;
  float aver=0.0;
  int end = 60 - (StableNumbers - 5);

  MidLineExcursion = 0;//偏移量置零

  for(i=60-2,iCount=0; i>end; i--,iCount++)
  {
    Black_Sum += BlackLineData[i];//累和
    MidLineExcursion = MidLineExcursion + BlackLineData[i] - BlackLineData[i+1];//中线偏移量类和
  }
  aver = Black_Sum*1.0 / iCount;//中线偏移量类和平均

  MidLineVariance = 0.0;//方差给0

  for (i = 60-2;i > end;i--)
  {
    MidLineVariance+=(aver-BlackLineData[i])*(aver-BlackLineData[i]);//方差类和
  }
  MidLineVariance = MidLineVariance*1.0 / iCount;//平均方差
}


 uint8 LoopRightEdge[40];



//////////判断直线函数
unsigned char JudgeStraightLine__(int End )
{
    int i;
    unsigned char StartTempR = 0;
    unsigned char NumTempR = 0;
    float SlopeR = 0.0;
    float SumR = 0.0;

    memset(LoopRightEdge,0,sizeof(LoopRightEdge));//数组清零
    for( i = 58; i > End; i-- )
    {
	   if( Boundary.RightEdge[i] < COL_END - 1 )
	   {
		  StartTempR = i;
		  break;
	   }
    }
    if( StartTempR > End )
    {
	   for( i = StartTempR; i > End; i-- )
	   {
		  if( Boundary.RightEdge[i] >= COL_END - 1 )
		  {
			 NumTempR++;
		  }
	   }
	   if( NumTempR < 2 ) //连续
	   {
		  //计算斜率
		  SlopeR = 1.0 * ( Boundary.RightEdge[StartTempR] - Boundary.RightEdge[End + 1] ) / ( StartTempR - End + 1 );
		  for( i = 0; i <= StartTempR - End + 1; i++ )
		  {
			 LoopRightEdge[StartTempR - i] = 79 - ( unsigned char )( ( ( i * SlopeR ) ) + ( 79 - ( Boundary.RightEdge[StartTempR] ) ) );
			 SumR = SumR + ABS( ( LoopRightEdge[StartTempR - i] - Boundary.RightEdge[StartTempR - i] ) );
		  }
		  SumR = SumR / ( StartTempR - End + 1 );
	   }
	   //Sum=2.1;
	   if( SumR != 0 )
	   {
		  if( SumR <= 2.85 )
		  {
			 return 1;
		  }
		  else
		  {
			 return 0;
		  }
	   }
    }
    return 0;
}



#if 0
/*!
 *  @brief      获取偏移量线函数  GetEPerCount()
 *  @param
 *  @return     VOID
 *  @note       获取有效偏移量，平均每列偏移量  边界偏移      后面中线处理完再用
 *  @since
 *  @time
 *  @Sample usage:
 */
float EPerCount=0.0;
int Excursion = 0;//偏移量
int ValidLineCount = 0;//有效行计数
int ValidExcursionCount=0;//有效偏移量计数
signed char TripPointPos[ROW];//产生跳变的具体行数写入该数组
signed char SubValue[ROW];//上下两行产生的跳变差

void GetEPerCount()
{
  unsigned char i=0,j=0;
  unsigned char TripPointCount = 0;//中心跳变计数
  unsigned char FilterNumber = 5;// 连续跳变点小于该数则滤掉
  unsigned char TripLen = 5;//跳变长度

  Excursion = 0;//偏移量
  ValidLineCount=0;//有效行计数
  ValidExcursionCount=0;//有效偏移量计数

  for (i = 0;i < ROW;i++)
  {
    TripPointPos[i] = 0;//产生跳变的具体行数写入该数组
  }
  //跳变分段
  for (i = 0;i < ROW;i++)
  {
    if (ABS(RightBlack_A[i] - LeftBlack_A[i]) < 3 )//g
    {
      BlackLineData_A[i] = LeftBlack_A[i];// 单边缘直接取边缘
    }
    else
    {
      BlackLineData_A[i] = LeftBlack_A[i] + (RightBlack_A[i] - LeftBlack_A[i])/2;//正常两边黑线提取中心线
    }
    //相邻中心点做差，找出中心跳变处，经行分段
    if (i > 0)
    {
      SubValue[i] = BlackLineData_A[i] - BlackLineData_A[i-1];//上下两行跳变差
      //跳变判断
      if (ABS(SubValue[i]) > TripLen)
      {
        //记录跳变位置
        TripPointPos[TripPointCount] = i;//记录具体行数
        TripPointCount++;//跳变行数增加
      }
    }
  }//for结束

  if (TripPointCount == 0)//没有跳变
  {
    for (i = 0;i < ROW;i++)
    {
      ValidLineCount++;//有效行计数
      if(i > 25 && i < ROW)//图像下半部//g
      {
              ValidExcursionCount++;//有效偏移量增加
              Excursion += (BlackLineData_A[i-1]-BlackLineData_A[i]);//上下两行相对偏移量累计
      }
    }
  }
  else //有跳变
  {
    if (TripPointPos[0] > FilterNumber)//第一次跳变时，跳变行数大于8
    {
      for (i = 0;i < TripPointPos[0];i++)
      {
        ValidLineCount++;//有效行增加
        if(i > 25)//g
        {
                ValidExcursionCount++;//有效偏移量增加
                Excursion += (BlackLineData_A[i-1]-BlackLineData_A[i]);//上下两行相对偏移量累计
        }
      }
    }

    TripPointPos[TripPointCount] = ROW;//最后一次假想跳变位置直接给最大

    for (j = 0;j < TripPointCount;j++)
    {
      if (TripPointPos[j+1] - TripPointPos[j] > FilterNumber)//相邻跳变大于5
      {
        for (i = TripPointPos[j];i < TripPointPos[j+1];i++)
        {
          ValidLineCount++;//有效行增加
          if(i > TripPointPos[j] && i > 25)
          {
                  ValidExcursionCount++;//有效偏移量增加
                  Excursion += (BlackLineData_A[i-1]-BlackLineData_A[i]);//偏移量累计
          }
        }
      }//End if
    }//End for
  }//End else
  if(ValidExcursionCount > 1)//有便宜量
  {
    EPerCount = ABS(Excursion)*1.0/ValidExcursionCount;//偏移量平均
  }
  else
  {
    EPerCount = 3;//偏移量平均为3
    Excursion=50;//总偏移量为50
  }
}

#endif





#if 0
/******************************************************************************
//正十字处理
******************************************************************************/
unsigned char IsCrossing=0;
unsigned char P_LeftBlack[ROW]={0};
unsigned char P_RightBlack[ROW]={0};
unsigned char Crossingpoint_start;
unsigned char Crossingpoint_end;
void SCProcessing()
{
  unsigned char i = 0;
  int startPos=0,endPos=0,temCount=0,countMax=0,temPos=0;
  int ProcessFlag=0;

  //ì跳变计数
  unsigned char TripPointCount = 0;
  unsigned char TripLen = 3;
  //获取十字边缘线
 // GetCrossingMidLine();
   for (i = 0;i < ROW;i++)
  {
    P_LeftBlack[i]=NS_LeftBlack[i];
    P_RightBlack[i]=NS_RightBlack[i];
  }
  for (i = 0;i < ROW;i++)
  {
    TripPointPos[i] = 0;
  }
  TripPointCount=0;
  //根据跳变分段
  for (i = 10;i < ROW;i++)
  {
    // 中心做差，找出跳变出，分段
    if (i > 0)
    {
      SubValue[i] = P_LeftBlack[i] - P_LeftBlack[i-1];
      //跳变判断
      if (ABS(SubValue[i]) > TripLen || P_LeftBlack[i] < 2)
      {
        //记录跳变位置
        TripPointPos[TripPointCount] = i;
        TripPointCount++;
      }
    }
  }//for结束
  TripPointPos[TripPointCount] = ROW;
  temCount = 0;
  countMax=0;
  temPos = TripPointPos[0]-1;
  startPos = temPos;
  endPos = 0;
  for (i = 1;i < TripPointCount;i++)
  {
    if (TripPointPos[i]-TripPointPos[i-1] < 3)//两次跳变行之间的行号之差
    {
      temCount++;
      if (temCount > countMax)
      {
        countMax = temCount;
        startPos = temPos;
        endPos   = TripPointPos[i]+1;
      }
    }
    else
    {
      temPos = TripPointPos[i]-1;
      temCount=0;
    }
  }//End for

  if (startPos > 10)
  {
    CommonRectificate(&P_LeftBlack[0],startPos-1,endPos+1);
     Crossingpoint_start=startPos-1;
    Crossingpoint_end=endPos+1;
    ProcessFlag=1;
  }

  //右边缘补线
  for (i = 0;i < ROW;i++)
  {
    TripPointPos[i] = 0;
  }
  TripPointCount=0;
  //根据跳变分段
  for (i = 10;i < ROW;i++)
  {
    // 中心做差，找出跳变出，分段
    if (i > 0)
    {
      SubValue[i] = P_RightBlack[i] - P_RightBlack[i-1];
      //跳变判断
      if (ABS(SubValue[i]) > TripLen || P_RightBlack[i] > COL-3)
      {
        //记录跳变位置
        TripPointPos[TripPointCount] = i;
        TripPointCount++;
      }
    }
  }//for结束


    temCount = 0;
    countMax=0;

    endPos = 0;
    temPos = TripPointPos[0]-1;
    startPos = temPos;
    for (i = 0;i < TripPointCount;i++)
    {
      if (TripPointPos[i]-TripPointPos[i-1] < 3)
      {
        temCount++;
        if (temCount > countMax)
        {
          countMax = temCount;
          startPos = temPos;
          endPos = TripPointPos[i]+1;
        }
      }
      else
      {
        temPos = TripPointPos[i]-1;
        temCount=0;
      }
    }//End for

    if (startPos > 12)
    {
      //拟合
      CommonRectificate(&P_RightBlack[0],startPos-1,endPos+1);
      if(Crossingpoint_start>startPos-1)
      Crossingpoint_start=startPos-1;
      if(Crossingpoint_end<endPos+1)
      Crossingpoint_end=endPos+1;
      ProcessFlag=1;

    }
    if(!ProcessFlag)
    {
      IsCrossing = 0;
    }
}

#endif
void EdgeTracking()
{
   //定义
  unsigned char leftpoint_I;
  unsigned char leftpoint_J;
  unsigned char i;
  unsigned char j;

  //qianjihang chuli
  for(;;)
  {
    i=leftpoint_I;
    j=leftpoint_J;
    switch(0){
  case 0:
     if(image[i][j-1]==0)
     {}
     else
     {
       leftpoint_J--;
       break;}
  case 1:
     if(image[i+1][j]==0)
     {}
     else
     {
       leftpoint_I--;
       break;}
  case 2:
    if(image[i][j+1]==0)
     {}
    // else
   //  {break;}
    }
  }
}


uint8  Initial_line=0;
void  GetWhiteEdge_length()//获取两边白边长度
{
       uint8  LeftEdge_length_C=0;
       uint8  LeftEdge_abort=0;
       uint8  RightEdge_length_C=0;
       uint8  RightEdge_abort=0;



       for(int i=ROW-1;i>0;i--)
       {
	      if(image[i][1]==White)
	      {
		     LeftEdge_length_C++;
	      }
	      else
	      {
		     LeftEdge_abort=i;
		     break;
	      }
       }
       for(int i=ROW-1;i>0;i--)
       {
	      if(image[i][COL_END-1]==White)
	      {
		     RightEdge_length_C++;
	      }
	      else
	      {
		     RightEdge_abort=i;
		     break;
	      }
       }
       Initial_line=MIN(LeftEdge_abort,RightEdge_abort);//获取最低起始行
}


/*!
 *  @brief      获取全白全黑至行 GetHangData1()
 *  @param
 *  @return     VOID
 *  @note       获取全白行全黑行判断道路类型
 *  @since
 *  time
 *  Sample usage:
 */
unsigned char LeftBlack_A[ROW];
unsigned char RightBlack_A[ROW];
unsigned char BlackLineData_A[ROW];


unsigned char BlackRow=0;
unsigned char WhiteRow=0;
unsigned char RWhiteRow=0;
unsigned char LWhiteRow=0;

unsigned char Lblack_flag[ROW]={0};
unsigned char Rblack_flag[ROW]={0};

unsigned char LineType[ROW] = {0};//行类型
///unsigned char RoadType=0;//道路类型

void GetHangData1()
{
  int i=0,j=0;
  int CountBlack = 0;
  int CountWhite = 0;
  //指针

  BlackRow=0;
  WhiteRow=0;
  RWhiteRow=0;
  LWhiteRow=0;
  for (i=ROW-1 ;i >0 ;i--)
  {
     Lblack_flag[i]=0;
     Rblack_flag[i]=0;
  }

  for (i=ROW-1 ;i >0 ;i--)
  {
    CountBlack = 0;//每行黑点计数
    CountWhite = 0;//每行白点计数

    //两边黑线提取
    for (j = COL-2 ;j >0; j--)  ///youbianxian
    {
      //中->左搜索
        if(image[i][j] == Black)
        {
          CountBlack++;//黑点计数
        }
        else
        {
          CountWhite++;//白点计数
        }
        // 未找到边缘则寻找

                if(image[i][j]&&!image[i][j+1])
                     {
                       RightBlack_A[i] = j+1;
                       Rblack_flag[i]=1;
                       break;
                     }

    }
    for (j =1;j<COL-2;j++)  ///youbianxian
    {
      //中->左搜索
        if(image[i][j] == Black)
        {
          CountBlack++;//黑点计数
        }
        else
        {
          CountWhite++;//白点计数
        }
        // 未找到边缘则寻找

                if(!image[i][j]&&image[i][j+1])
                     {
                       LeftBlack_A[i] = j+1;
                       Lblack_flag[i]=1;
                       break;
                     }

    }

     if( !Lblack_flag[i] && !Rblack_flag[i])//&& i<Initial_line)
     {
       WhiteRow++;
       LineType[i]=1;//全白
     }
    else if(!Lblack_flag[i] &&Rblack_flag[i])
    {
       LWhiteRow++;
       LineType[i]=2;//左丢线
    }
     else if(Lblack_flag[i] &&!Rblack_flag[i])
    {
       RWhiteRow++;
       LineType[i]=3;//右丢线
    }
    else
    {
      asm("nop");
      LineType[i]=0;//双边线正常
    }



  }//for结束-- 行扫描完
    if(WhiteRow>6)//在十字中没处理
    {
      //RoadType=4;//十字
    }
    else
    {
      //RoadType=0;//常规
    }
}


void GetHangData()
{
  int i=0;


  BlackRow=0;
  WhiteRow=0;
  RWhiteRow=0;
  LWhiteRow=0;


  for (i=ROW-20 ;i >0 ;i--)
  {
    Lblack_flag[i]=0;

    Rblack_flag[i]=0;
    if(NS_LeftBlack[i]>2)
                     {
                       Rblack_flag[i]=1;
                     }
    if(NS_RightBlack[i]<COL-2)
                     {
                       Lblack_flag[i]=1;
                     }




     if( !Lblack_flag[i] && !Rblack_flag[i])
     {
       WhiteRow++;
       LineType[i]=1;//全白
     }
    else if(!Lblack_flag[i] &&Rblack_flag[i])
    {
       LWhiteRow++;
       LineType[i]=2;//左丢线
    }
     else if(Lblack_flag[i] &&!Rblack_flag[i])
    {
       RWhiteRow++;
       LineType[i]=3;//右丢线
    }
    else
    {
      asm("nop");
      LineType[i]=0;//双边线正常
    }
  }

    if(WhiteRow>17)//在十字中没处理
    {
      ///RoadType=4;//十字
    }
    else
    {
      //RoadType=0;//常规
    }
}