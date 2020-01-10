/*!
 * @file      Feature_Extration.c
 * @brief      ������ȡ��ȡ����
 * @author
 * @version
 * @date       2019-05-21
 */
#include "include.h"
#include "Feature_Extration.h"


Str_BlackEnd  BlackEnd;
Str_ZebraLine ZebraLine;

#if 0

////������ֱ�ߺ���
unsigned char JudgeStraightLineL( int End )
{
  int i;
  unsigned char StartTempL = 0;
  unsigned char NumTempL = 0;
  float SlopeL = 0.0;
  float Sum = 0.0;
  memset(LoopLeftEdge,0,sizeof(LoopLeftEdge));//��������
  for( i = 58; i > End; i-- )
  {
    if( LeftEdge[i] != 0 )
    {
      StartTempL = i; //��¼��߽粻���߿�ʼ��
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
    if( NumTempL < 2 ) //����
    {
      //����б��
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

//////////�ж�ֱ�ߺ���
unsigned char JudgeStraightLineR(int End )
{
    int i;
    unsigned char StartTempR = 0;
    unsigned char NumTempR = 0;
    float SlopeR = 0.0;
    float SumR = 0.0;

    memset(LoopRightEdge,0,sizeof(LoopRightEdge));//��������
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
	   if( NumTempR < 2 ) //����
	   {
		  //����б��
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
�����߼��
***********************************************************/
//uint32 total_time;//��ѭ������
void Mark_ZebraLine(unsigned int Z_TIME) //600
{
       if(ZebraLine.total_time<Z_TIME)
       {
	      ZebraLine.total_time++;      //��¼���ƵĴ���
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
				   if(image[i][j]!=image[i][j+1])//����
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
		     if(ZebraLine.many_bp_row>4)//�ڰ�������� 90
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


#if 0//ͼ���µ��ж�
#define Width_Factor 1.21//�������ϵ��
void RampJudge(uint16 ScanStart ,uint16 ScanEnd)
{
    uint32 i;
    int a;
    a=0;
    memset(RampRoadWidth,0,sizeof(RampRoadWidth));     //��������       &&!SecondRamdFlag
  //  RampFlag=FALSE;
    if(ABS(Error)<=10//&&LastLine>=4  RightLose<5&&LeftLose<5&&
       &&RampFlag==FALSE&&In_RightRound==0&&In_LeftRound==0&&ObsScan.ObstaclePosition==NO_OB)//ֱ������        ���Ǽ��벻������Զ�еĵ������
    {
        if(RampSlopeL<-200&&RampSlopeR>200)//б������
        {
            for(i=ScanStart; i>ScanEnd; i--)
            {
                if(RightEdge[i]<79&&LeftEdge[i]!=0&&RightEdge[i-1]<79&&LeftEdge[i-1]!=0&&RightEdge[i+1]<79&&LeftEdge[i+1]!=0)
                    //����������
                {
                    RampRoadWidth[i-1]=ABS(RightEdge[i-1]-LeftEdge[i-1]);
                    RampRoadWidth[i]=ABS(RightEdge[i]-LeftEdge[i]);
                    RampRoadWidth[i+1]=ABS(RightEdge[i+1]-LeftEdge[i+1]);

                         if(RampRoadWidth[i]>ObsWidthS[i]*Width_Factor
                            &&RampRoadWidth[i-1]>ObsWidthS[i-1]*Width_Factor
                            &&RampRoadWidth[i+1]>ObsWidthS[i+1]*Width_Factor//������ȱ������

                            &&RightEdge[i]-RightEdge[i-1]>=0&&RightEdge[i]-RightEdge[i-1]<3//����ƽ������
                            &&LeftEdge[i-1]-LeftEdge[i]>=0&&LeftEdge[i-1]-LeftEdge[i]<3)
                    {
                        a++;
                        if(a>=2)
                        {
                          a=0;
                          RampFlag=TRUE;//�µ���־����
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
void LR_BourndTrace(void)    //���ұ߽���Զ��׷��
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


 //��С���˷����б��

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
  if((end-begin)*x2sum-xsum*xsum) //�жϳ����Ƿ�Ϊ��
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
 *  @brief      ��ȡ���߽�����
 *  @param
 *  @return     VOID
 *  @note       ���ݵ�����ɫ��ȡ�ڽ�ֹ�кڰ׽�ֹ��
 *  @since
 *  time
 *  Sample usage:
 */
void GetBlackEndParam()//��ȡ���߽�����
{
       BlackEnd.LEndFlag = 0;//�������־
       BlackEnd.MEndFlag = 0;//�С�������־
       BlackEnd.REndFlag = 0;//�ҽ�����־
       BlackEnd.BlackEndL = 0;//���������
       BlackEnd.BlackEndM = 0;//�н�������
       BlackEnd.BlackEndR = 0;//�ҽ�������

       for (int i = ROW_END-1; i >0 ; i--)
       {
	      //�м�
	      if(image[i][40] == White && ! BlackEnd.MEndFlag )
	      {
		     BlackEnd.BlackEndM++;//�н�����������
	      }
	      else if(image[i-1][40] == Black)
	      {
		     BlackEnd.MEndFlag = 1;//������Ϊ�ڣ�������־��λ
	      }

	      //�ķ�֮һλ��
	      if(image[i][20] == White && ! BlackEnd.LEndFlag)
	      {
		     BlackEnd.BlackEndL++;//�������������
	      }
	      else if(image[i-1][20] == Black)
	      {
		     BlackEnd.LEndFlag=1;//������Ϊ�ڣ�������־��λ
	      }

	      //�ķ�֮��λ��
	      if(image[i][60] == White && ! BlackEnd.REndFlag )
	      {
		     BlackEnd.BlackEndR++;//�ҽ�����������
	      }
	      else if(image[i-1][60] == Black)
	      {
		     BlackEnd.REndFlag = 1;//������Ϊ�ڣ�������־��λ
	      }
       }

       //������Զ��
       BlackEnd.BlackEndMax = MAX(BlackEnd.BlackEndL,BlackEnd.BlackEndM);
       BlackEnd.BlackEndMax = MAX(BlackEnd.BlackEndMax,BlackEnd.BlackEndR);

       BlackEnd.BlackEndLMR = BlackEnd.BlackEndL + BlackEnd.BlackEndM + BlackEnd.BlackEndR;//��Զ��Ч�ڵ��ۺ�

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
//�������   CommonRectificate(&P_LeftBlack[0],startPos-1,endPos+1);
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
//�����߲���
******************************************************************************/

int CompensateCount = 0;
 int StableNumbers;
void MidLineCompensate()
{
  int i=0,icount=0,j=0;
  int CompensateData = 0;

  int sum = 0;
  float avg =0.0;
  int tem = 1;//����������־

  CompensateCount=0;

  for (i = 60-2,icount=0;i > 60-(StableNumbers-10);i--,icount++)
  {
    sum += (BlackLineData[i]-BlackLineData[i+1]);//����������������ƽ��ƫ���������
  }
  avg = sum*1.0 / icount;//ƽ��

  if (avg < 0)
  {
    tem = -1;
    avg = (-1)*avg;
  }
  if (avg > 1.0)
  {
    CompensateData = 3;//ƫ��̶ȴ󣬲�������
  }
  else if (avg > 0.35)
  {
    CompensateData = 2;//ƫ��̶��У���������
  }
  else if (avg > 0.25)
  {
    CompensateData = 1;//ƫ��̶�С��������С
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
//��ȡ������ƫ��
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

  MidLineExcursion = 0;//ƫ��������

  for(i=60-2,iCount=0; i>end; i--,iCount++)
  {
    Black_Sum += BlackLineData[i];//�ۺ�
    MidLineExcursion = MidLineExcursion + BlackLineData[i] - BlackLineData[i+1];//����ƫ�������
  }
  aver = Black_Sum*1.0 / iCount;//����ƫ�������ƽ��

  MidLineVariance = 0.0;//�����0

  for (i = 60-2;i > end;i--)
  {
    MidLineVariance+=(aver-BlackLineData[i])*(aver-BlackLineData[i]);//�������
  }
  MidLineVariance = MidLineVariance*1.0 / iCount;//ƽ������
}


 uint8 LoopRightEdge[40];



//////////�ж�ֱ�ߺ���
unsigned char JudgeStraightLine__(int End )
{
    int i;
    unsigned char StartTempR = 0;
    unsigned char NumTempR = 0;
    float SlopeR = 0.0;
    float SumR = 0.0;

    memset(LoopRightEdge,0,sizeof(LoopRightEdge));//��������
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
	   if( NumTempR < 2 ) //����
	   {
		  //����б��
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
 *  @brief      ��ȡƫ�����ߺ���  GetEPerCount()
 *  @param
 *  @return     VOID
 *  @note       ��ȡ��Чƫ������ƽ��ÿ��ƫ����  �߽�ƫ��      �������ߴ���������
 *  @since
 *  @time
 *  @Sample usage:
 */
float EPerCount=0.0;
int Excursion = 0;//ƫ����
int ValidLineCount = 0;//��Ч�м���
int ValidExcursionCount=0;//��Чƫ��������
signed char TripPointPos[ROW];//��������ľ�������д�������
signed char SubValue[ROW];//�������в����������

void GetEPerCount()
{
  unsigned char i=0,j=0;
  unsigned char TripPointCount = 0;//�����������
  unsigned char FilterNumber = 5;// ���������С�ڸ������˵�
  unsigned char TripLen = 5;//���䳤��

  Excursion = 0;//ƫ����
  ValidLineCount=0;//��Ч�м���
  ValidExcursionCount=0;//��Чƫ��������

  for (i = 0;i < ROW;i++)
  {
    TripPointPos[i] = 0;//��������ľ�������д�������
  }
  //����ֶ�
  for (i = 0;i < ROW;i++)
  {
    if (ABS(RightBlack_A[i] - LeftBlack_A[i]) < 3 )//g
    {
      BlackLineData_A[i] = LeftBlack_A[i];// ����Եֱ��ȡ��Ե
    }
    else
    {
      BlackLineData_A[i] = LeftBlack_A[i] + (RightBlack_A[i] - LeftBlack_A[i])/2;//�������ߺ�����ȡ������
    }
    //�������ĵ�����ҳ��������䴦�����зֶ�
    if (i > 0)
    {
      SubValue[i] = BlackLineData_A[i] - BlackLineData_A[i-1];//�������������
      //�����ж�
      if (ABS(SubValue[i]) > TripLen)
      {
        //��¼����λ��
        TripPointPos[TripPointCount] = i;//��¼��������
        TripPointCount++;//������������
      }
    }
  }//for����

  if (TripPointCount == 0)//û������
  {
    for (i = 0;i < ROW;i++)
    {
      ValidLineCount++;//��Ч�м���
      if(i > 25 && i < ROW)//ͼ���°벿//g
      {
              ValidExcursionCount++;//��Чƫ��������
              Excursion += (BlackLineData_A[i-1]-BlackLineData_A[i]);//�����������ƫ�����ۼ�
      }
    }
  }
  else //������
  {
    if (TripPointPos[0] > FilterNumber)//��һ������ʱ��������������8
    {
      for (i = 0;i < TripPointPos[0];i++)
      {
        ValidLineCount++;//��Ч������
        if(i > 25)//g
        {
                ValidExcursionCount++;//��Чƫ��������
                Excursion += (BlackLineData_A[i-1]-BlackLineData_A[i]);//�����������ƫ�����ۼ�
        }
      }
    }

    TripPointPos[TripPointCount] = ROW;//���һ�μ�������λ��ֱ�Ӹ����

    for (j = 0;j < TripPointCount;j++)
    {
      if (TripPointPos[j+1] - TripPointPos[j] > FilterNumber)//�����������5
      {
        for (i = TripPointPos[j];i < TripPointPos[j+1];i++)
        {
          ValidLineCount++;//��Ч������
          if(i > TripPointPos[j] && i > 25)
          {
                  ValidExcursionCount++;//��Чƫ��������
                  Excursion += (BlackLineData_A[i-1]-BlackLineData_A[i]);//ƫ�����ۼ�
          }
        }
      }//End if
    }//End for
  }//End else
  if(ValidExcursionCount > 1)//�б�����
  {
    EPerCount = ABS(Excursion)*1.0/ValidExcursionCount;//ƫ����ƽ��
  }
  else
  {
    EPerCount = 3;//ƫ����ƽ��Ϊ3
    Excursion=50;//��ƫ����Ϊ50
  }
}

#endif





#if 0
/******************************************************************************
//��ʮ�ִ���
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

  //���������
  unsigned char TripPointCount = 0;
  unsigned char TripLen = 3;
  //��ȡʮ�ֱ�Ե��
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
  //��������ֶ�
  for (i = 10;i < ROW;i++)
  {
    // ��������ҳ���������ֶ�
    if (i > 0)
    {
      SubValue[i] = P_LeftBlack[i] - P_LeftBlack[i-1];
      //�����ж�
      if (ABS(SubValue[i]) > TripLen || P_LeftBlack[i] < 2)
      {
        //��¼����λ��
        TripPointPos[TripPointCount] = i;
        TripPointCount++;
      }
    }
  }//for����
  TripPointPos[TripPointCount] = ROW;
  temCount = 0;
  countMax=0;
  temPos = TripPointPos[0]-1;
  startPos = temPos;
  endPos = 0;
  for (i = 1;i < TripPointCount;i++)
  {
    if (TripPointPos[i]-TripPointPos[i-1] < 3)//����������֮����к�֮��
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

  //�ұ�Ե����
  for (i = 0;i < ROW;i++)
  {
    TripPointPos[i] = 0;
  }
  TripPointCount=0;
  //��������ֶ�
  for (i = 10;i < ROW;i++)
  {
    // ��������ҳ���������ֶ�
    if (i > 0)
    {
      SubValue[i] = P_RightBlack[i] - P_RightBlack[i-1];
      //�����ж�
      if (ABS(SubValue[i]) > TripLen || P_RightBlack[i] > COL-3)
      {
        //��¼����λ��
        TripPointPos[TripPointCount] = i;
        TripPointCount++;
      }
    }
  }//for����


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
      //���
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
   //����
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
void  GetWhiteEdge_length()//��ȡ���߰ױ߳���
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
       Initial_line=MIN(LeftEdge_abort,RightEdge_abort);//��ȡ�����ʼ��
}


/*!
 *  @brief      ��ȡȫ��ȫ������ GetHangData1()
 *  @param
 *  @return     VOID
 *  @note       ��ȡȫ����ȫ�����жϵ�·����
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

unsigned char LineType[ROW] = {0};//������
///unsigned char RoadType=0;//��·����

void GetHangData1()
{
  int i=0,j=0;
  int CountBlack = 0;
  int CountWhite = 0;
  //ָ��

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
    CountBlack = 0;//ÿ�кڵ����
    CountWhite = 0;//ÿ�а׵����

    //���ߺ�����ȡ
    for (j = COL-2 ;j >0; j--)  ///youbianxian
    {
      //��->������
        if(image[i][j] == Black)
        {
          CountBlack++;//�ڵ����
        }
        else
        {
          CountWhite++;//�׵����
        }
        // δ�ҵ���Ե��Ѱ��

                if(image[i][j]&&!image[i][j+1])
                     {
                       RightBlack_A[i] = j+1;
                       Rblack_flag[i]=1;
                       break;
                     }

    }
    for (j =1;j<COL-2;j++)  ///youbianxian
    {
      //��->������
        if(image[i][j] == Black)
        {
          CountBlack++;//�ڵ����
        }
        else
        {
          CountWhite++;//�׵����
        }
        // δ�ҵ���Ե��Ѱ��

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
       LineType[i]=1;//ȫ��
     }
    else if(!Lblack_flag[i] &&Rblack_flag[i])
    {
       LWhiteRow++;
       LineType[i]=2;//����
    }
     else if(Lblack_flag[i] &&!Rblack_flag[i])
    {
       RWhiteRow++;
       LineType[i]=3;//�Ҷ���
    }
    else
    {
      asm("nop");
      LineType[i]=0;//˫��������
    }



  }//for����-- ��ɨ����
    if(WhiteRow>6)//��ʮ����û����
    {
      //RoadType=4;//ʮ��
    }
    else
    {
      //RoadType=0;//����
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
       LineType[i]=1;//ȫ��
     }
    else if(!Lblack_flag[i] &&Rblack_flag[i])
    {
       LWhiteRow++;
       LineType[i]=2;//����
    }
     else if(Lblack_flag[i] &&!Rblack_flag[i])
    {
       RWhiteRow++;
       LineType[i]=3;//�Ҷ���
    }
    else
    {
      asm("nop");
      LineType[i]=0;//˫��������
    }
  }

    if(WhiteRow>17)//��ʮ����û����
    {
      ///RoadType=4;//ʮ��
    }
    else
    {
      //RoadType=0;//����
    }
}