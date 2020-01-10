#include "include.h"
#include "Midline_Process.h"



/***************************************************************
����

* �������ƣ�
* ����˵����  temp
* ����˵����
* �������أ�voiold
* �޸�ʱ�䣺
* �� ע��
**************************************************************/
uint8 track_Theoretical_width[ROW]={
6       ,  8    ,  10   ,  12   ,  14   ,
16	,  18	,  20	,  22	,  24	,
26	,  28	,  30	,  32	,  34	,
36	,  38	,  40	,  41	,  43	,
45	,  47	,  49	,  51	,  52	,
54	,  56	,  58	,  60	,  62	,
64	,  66	,  68	,  70	,  71	,
73	,  75	,  77	,  79	,  81	,
83	,  85	,  87	,  89	,  91	,
93	,  95	,  97	,  98	,  100	,
101	,  103	,  105	,  107	,  109	,
111	,  113	,  114	,  115	,  117	,
119	,  121	,  123	,  125	,  127	,
128	,  129	,  131	,  133	,  135	,
137	,  138  ,  139  ,  140	,  142  ,
144	,  146  ,  148  ,  150	,  152
};
float mid_k[ROW_END]={0};

uint8 CenterBlack[ROW/2]={0};
float Deviation=0;//ƫ��
uint8  Mline_end =0 ;

float Camera_CenterError ;

float slope_coefficient ;



#define  Width_P   1.9




Site_xy midddd[ROW];
int ExtractCenterline(Str_Boundary  *str)
{
       //uint8 ii;
       uint8 i;

       for(i=ROW_END;i>ROW_START;i--)
       {
	      str->track_Width[i]=(int)(4.0+i*Width_P); //����  ·��
       }
       //       float k=-0.01,b=1;

       //       for(i=0;i<ROW_END;i++)
       //       {
       //	      mid_k[i]=k*i+b;
       //       }

         //����
       if(str->LeftEdge[ROW_END]!=0 && str->RightEdge[ROW_END]!=0)     //���Ҷ��б߽�
       {
	      str->MiddleLine[ROW_END] =(str->LeftEdge[ROW_END] + str->RightEdge[ROW_END])>>1;
	      str->MiddleLine[ROW_END+1]=str->MiddleLine[ROW_END] ;
       }
       else if(str->LeftEdge[ROW_END]!=0 && str->RightEdge[ROW_END]==0)//find left
       {
	      str->RightEdge[ROW_END]=COL_END;
	      str->MiddleLine[ROW_END]=(str->LeftEdge[ROW_END] + str->RightEdge[ROW_END])>>1;
	      str->MiddleLine[ROW_END+1] =str->MiddleLine[ROW_END] ;
       }
       else if( str->LeftEdge[ROW_END]==0 && str->RightEdge[ROW_END]!=0)//find right
       {
	      str->LeftEdge[ROW_END]=COL_START;
	      str->MiddleLine[ROW_END]=(str->LeftEdge[ROW_END] + str->RightEdge[ROW_END])>>1;
	      str->MiddleLine[ROW_END+1] =str->MiddleLine[ROW_END] ;
       }
       else if( str->LeftEdge[ROW_END]==0 &&str->RightEdge[ROW_END]==0)//���߶���   d������
       {
	      str->LeftEdge[ROW_END]=COL_START;
	      str->RightEdge[ROW_END]=COL_END;
	      str->MiddleLine[ROW_END]=str->MiddleLine[ROW_END+1]=(str->LeftEdge[ROW_END] + str->RightEdge[ROW_END])>>1;
       }

       //// �ӵڶ��п�ʼ����

       for(i=ROW_END-1;i>=ROW_START;i--)
       {

//	      if( (str->RightEdge[i]-str->LeftEdge[i])>=(str->RightEdge[i+1]-str->LeftEdge[i+1]+2) )//���������
//	      {
//		     str->MiddleLine[i] = str->MiddleLine[i+1];//����һ��
//	      }
//	      else
//	      {

	      if(str->LeftEdge[i]!=0 && str->RightEdge[i]!=0)     //���Ҷ��б߽�
	      {
		     str->MiddleLine[i]=(str->LeftEdge[i]+str->RightEdge[i])>>1;
	      }
	      else if(str->LeftEdge[i]!=0 && str->RightEdge[i]==0)//find left
	      {
		     if(str->LeftEdge[i+1]!=0) //�������
		     {
			   str->MiddleLine[i]=str->LeftEdge[i] + str->MiddleLine[i+1]-str->LeftEdge[i+1];
		     }
		     else
		     {
			    str->MiddleLine[i]=str->LeftEdge[i] + str->track_Width[i]/2;
		     }


		     //str->RightEdge[i]=COL_END;
		    // str->MiddleLine[i]=(str->LeftEdge[i] + str->RightEdge[i])>>1;
	      }
	      else if(str->LeftEdge[i]==0 && str->RightEdge[i]!=0)//find right
	      {

		     if(str->RightEdge[i+1]!=0) //�������
		     {
			   str->MiddleLine[i]=str->RightEdge[i] + str->MiddleLine[i+1]-str->RightEdge[i+1];
		     }
		     else
		     {
			    str->MiddleLine[i]  = str->RightEdge[i] - str->track_Width[i]/2;
		     }
		    // str->LeftEdge[i] =COL_START;
		    // str->MiddleLine[i]=(str->LeftEdge[i] + str->RightEdge[i])>>1;
	      }
	      else if( str->LeftEdge[i]==0&&str->RightEdge[i]==0)//���߶���   d������
	      {
		     //Ӧ��б�ʲ���  ��С���˷�
//		     str->LeftEdge[ROW_END]=COL_START;
//		     str->RightEdge[ROW_END]=COL_END;
		     str->MiddleLine[i]=str->MiddleLine[i+1] ;
	      }

#if 0//���ò���

		     //��б��ʮ�ֽ��о���
		     //�����Ǻ����õĳ���    ������Ҫ���ü� ���б����򵥺��õķ���

		     if(str->MiddleLine[i]-str->MiddleLine[i+1]>4&&i>=5)//��������ͻ��
		     {
			    ii = i;

			    while(1)
			    {
				   str->MiddleLine[ii-1] = str->MiddleLine[ii]-1;

				   ii++;

				   if( ii>=35 || ABS(str->MiddleLine[ii]-str->MiddleLine[ii+1])<=1 )
				   {
					  break;
				   }
			    }
		     }
		     if(str->MiddleLine[i+1]-str->MiddleLine[i]>4&& i>=5)
		     {
			    ii = i;
			    while(1)
			    {
				   str->MiddleLine[ii-1] =str-> MiddleLine[ii]+1;
				   //�����޷�
				   ii++;
				   if( ii>=35 || (str->MiddleLine[ii+1]-str->MiddleLine[ii]<=1) )
				   {
					  break;
				   }
			    }
		     }
		     //gu zhang
#endif
//	      }
	      if(image[i-1][str->MiddleLine[i]]==Black)
	      {
		     for(;i>ROW_START;i--)
		     {
			    str->MiddleLine[i] =str->MiddleLine[i+1];
		     }
	      }
       }





       //�������˲���
#if 0  //�ײ�û��Ҫ  ����  ����ܶ�

       if(str->StartLine>33)
       {
	      for(i=str->StartLine;i<=ROW_END;i++)   //�ײ�����
	      {
		     if(str->LeftEdge[i]!=0 && str->RightEdge[i]!=0)     //���Ҷ��б߽�
		     {
			    str->RowLose[i] = 1;
			    str->MiddleLine[i]=(str->LeftEdge[i]+str->RightEdge[i])>>1;
		     }
		     else if(str->LeftEdge[i]!=0 && str->RightEdge[i]==0)//find left
		     {
			    //str->OnlyRightLose++;
			    str->RowLose[i] =2;
			    str->RightEdge[i] =COL_END;
			    str->MiddleLine[i]=str->LeftEdge[i] + str->MiddleLine[i-1]-str->LeftEdge[i-1];
		     }
		     else if(str->LeftEdge[i]==0 && str-> RightEdge[i]!=0)//find right
		     {
			    //str->OnlyLeftLose++;
			    str->RowLose[i]= 3;
			    str->LeftEdge[i] =COL_START;
			    str->MiddleLine[i]=str->RightEdge[i]-str->RightEdge[i]+str->MiddleLine[i-1];
		     }
		     else if(str->LeftEdge[i]==0&&str->RightEdge[i]==0)//���߶���   d������
		     {
			    //str->AllLose++;
			    str->RowLose[i] = 4;
			    str->MiddleLine[i] =str->MiddleLine[i-1] ;
		     }
	      }
       }
#endif
       int aaaa=0;//,nnnn=0;
       int kkkk=0;

       for(i=ROW_END-1;i>str->EndLine;i--)  //��������
       {
	      aaaa+=str->MiddleLine[i]-str->MiddleLine[i+1];
	      //nnnn++;
       }
       aaaa=aaaa>>2;
       if(aaaa>=-5&&aaaa<=5)
       {
	      kkkk=0;
       }
       else if(aaaa<-5)
       {
	      kkkk=-1 ;
       }
       else if(aaaa>5)
       {
	      kkkk=1;
       }


       for(i=str->EndLine;i>=ROW_START;i--)
       {
	      str->MiddleLine[i]=str->MiddleLine[i+1]+kkkk;
       }


#if 0
       //�����޸�
       for(i=ROW_END-1;i>ROW_START+1;i--)    //  ROW_START  BUG��   ������
       {
	      if(str->MiddleLine[i]-str->MiddleLine[i-1]>4||str->MiddleLine[i]-str->MiddleLine[i-1]<-4)
	      {
		   for(int temp_row=i;temp_row<ROW_END;temp_row++)
		   {
			  str->MiddleLine[temp_row]=(str->MiddleLine[temp_row]+str->MiddleLine[temp_row-1])/2;
			  if(ABS(str->MiddleLine[temp_row]-str->MiddleLine[temp_row+1])<=4)
			  {
				 break;
			  }
		   }
	      }
       }

#endif





#if 0
       slope_coefficient=Least_Square_Method(str->MiddleLine,str->StartLine,str->EndLine+5,0.0,1);
#endif
       Camera_CenterError=Getrouteoffset(38,20,40);//*(Boundary.EndLine/20+0.9);    //����ƫ��
       OLED_PrintFloat(0,5,Camera_CenterError);
      // OLED_PrintFloat(0,3,Camera_CenterError);
      // Camera_CenterError=1.0*Camera_CenterError+slope_coefficient;

       for(i=ROW_END;i>ROW_START;i--)
       {
	      //str->RightEdge[i];
       }

       return  0;
}




float LineWeight[ROW_END+1]={
       0, 0, 0, 0, 0.4, 0.4, 0.4, 0.4, 0.5, 0.8,                       //0-9�У������ò���

       0.9, 0.9, 1.0, 1.0, 1.1,1.1, 1.2,1.3, 2,2,           //0-19�У������ò���

       2,   2,   2,   2,   2,   2,   2,   2, 2,  2,   //20-29��

       2,1.8,1.7, 1.6,1.5,1.4, 1.3,1.2,0.5, 0.1           //30-39��
};


float Getrouteoffset(unsigned char start,unsigned char end,float midpos)
{
  unsigned char i=0;
  //unsigned char iCount=0;
  float  Black_Sum=0;
  float weightSum = 0;
  float TemError = 0.0;
 // MidLineExcursion = 0;


//  for(i=ROW_END;i>ROW_START;i--)
//  {
//	 LineWeight[i]=-0.0125*(i-20.0)*(i-20.0)+5.0; //����  Ȩ��
//	 if(LineWeight[i]<0)
//		LineWeight[i]=0;
//  }
  //  float k=-0.005,b=1;
  //����
//  for(i=0; i<ROW_END;i++)
//  {
//	 break;
//	 //LineWeight[i]=k*i+b;
//  }
  if(start<end)
  {
	return TemError;
  }
  else
  {

	 for(i=start; i>end;i--)
	 {
//		Black_Sum += (midpos-Boundary.MiddleLine[i])*LineWeight[i];
		//		weightSum += LineWeight[i];
		Black_Sum += (midpos-Boundary.MiddleLine[i]);
		weightSum += 1;
	 }
	 TemError =Black_Sum*1.4/weightSum;

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
}


float Getrouteoffset2(unsigned char start,unsigned char end,float midpos)
{
  unsigned char i=0;
  //unsigned char iCount=0;
  float  Black_Sum=0;
  float weightSum = 0;
  float TemError = 0.0;
 // MidLineExcursion = 0;

#if 0
  for(i=ROW_END;i>ROW_START;i--)
  {
	 LineWeight[i]=-0.0125*(i-20.0)*(i-20.0)+5.0; //����  Ȩ��
	 if(LineWeight[i]<0)
		LineWeight[i]=0;
  }
#endif
  //  float k=-0.005,b=1;
  //����
//  for(i=0; i<ROW_END;i++)
//  {
//	 break;
//	 //LineWeight[i]=k*i+b;
//  }
  if(start<end)
  {
	return TemError;
  }
  else
  {

	 for(i=start; i>end;i--)
	 {
//		Black_Sum += (midpos-Boundary.MiddleLine[i])*LineWeight[i];
		//		weightSum += LineWeight[i];
		Black_Sum += (midpos-Boundary.MiddleLine[i]);
		weightSum += 1;
	 }
	 TemError =Black_Sum/weightSum;

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
}





#if 0
#define rout_mid 40
/******************************************************************************
��ȡ������
******************************************************************************/
int NoValidCount = 0;
int NoValidMax=0;//�������߶��Ҳ�������
int StableNumbers = 0;//�ȶ��м���
int StableNumbers2 = 0;//ʮ���ȶ��м���
int LeftStableNumbers = 0;//���ȶ��м���
int RightStableNumbers = 0;//���ȶ��м���
int ValidLineCount1 = 0; //���Ҷ��ҵ�
int ValidLineCount2 = 0;//ֻҪһ���ҵ�
extern int8 num=0;
extern float ke;
int8 flag_cross=0,i_left_old=0,i_right_old=0;

unsigned char ValidLine[60]={0};//��Ч�м���

struct site
{
	int8 buff_save[60];
	int8 num;
}left_site={{0},0},mid_site={{0},0},right_site={{0},0};



/***********************************************

	name:                      black_deal()
	function:                  deal the condition
	                           when only oneline
	parameter:                 mid:line
							   imgb:picture
	return code:               none

***********************************************/

void black_deal(uint8 mid,uint8* imgb)
{
	int i=0,j=0,num_F=0;


	for(i=57-mid;i>0;i--)
		{
			for(j=BlackLineData[i+2]-5;j<=BlackLineData[i+2]+5;j++)
				{
					if(imgb[i*80+j]==0)
						break;
				}
			if(j>BlackLineData[i+2]+5)
				{
					break;
				}
			else
				{
					BlackLineData[i+1]=j;
                                        num_F++;
                                        LeftBlack[i+1]=j-25;
					RightBlack[i+1]=j+25;
				}
		}
	if(num_F>=3)
    {
        StableNumbers=num_F;
    }


}





#if 0

void GetLMR()
{
  int i=0,j=0;
  int temLeft = 0;
  int temRight = 0;
  unsigned char pLeft = 80/2,pRight = 80/2;
  unsigned char bFoundLeft = 0;
  unsigned char bFoundRight = 0;
  unsigned char TripLen = 4;
  unsigned char MidEnd = 0;
  unsigned char MidToBlackCount = 0;




int16 line1=0;
int8 jump[30]={0},jump_mid[29],jump_B[10]={0},j_save=0;
uint8 flag_wob=0;

  NoValidCount = 0;
  NoValidMax=0;
  StableNumbers=0;
  LeftStableNumbers = 0;
  RightStableNumbers = 0;
  ValidLineCount1=0;
  ValidLineCount2=0;



  for (i = 60-1;i >= 0 && !MidEnd;i--)//���һ�п�ʼ
  {
    // ��ʼ��ָ��

    if (i < 60 - 5)//5������
    {
      pLeft = BlackLineData[i+1];//������һ�б�Ե����Ѱ��
      pRight= BlackLineData[i+1];//������һ�б�Ե����Ѱ��
    }
    else
    {
      if(BlackEndL < 5 && BlackEndR > BlackEndM)//�����Ч����С��5��ͼ����ƫ
      {
        pLeft = 80*3/4-1;//ͼ�����ָ�����
        pRight = 80*3/4-1;
      }
      else if(BlackEndR < 5 && BlackEndL > BlackEndM)//�Ҳ���Ч����С��5��ͼ����ƫ
      {
        pLeft = 80/4-1;
        pRight = 80/4-1;
      }
      else
      {
        pLeft = 80/2-1;
        pRight = 80/2-1;
      }
    }


    // ��ʼ�����
    bFoundLeft = bFoundRight = 0;

    //���ߺ�����ȡ
    for (j = 0;j < 80;j++)
    {
      // ��������
      if (bFoundLeft == 0 && pLeft > 0)
      {
        //δ�ҵ����Ե��Ѱ��
        if (image[i][pLeft] == White && image[i][pLeft-1] == Black)
        {
          LeftBlack[i] = pLeft-1;
          if (LeftBlack[i] > 0)
          {
            bFoundLeft = 1;//�ҵ����Ե
          }
          if (bFoundRight)
          {
            break;//�ұ�ԵҲ�ҵ�
          }
        }
        else
        {
          pLeft--;
        }
      }// if����--������������Ե

      //��������
      if (bFoundRight == 0 && pRight < 80-1)
      {
        //δ�ҵ��ұ�Ե
        if (image[i][pRight] == White &&  image[i][pRight+1] == Black)
        {
          RightBlack[i] = pRight + 1;
          if (RightBlack[i] < 80-1)
          {
            bFoundRight = 1;//�ҵ��ұ�Ե
          }
          if (bFoundLeft)
          {
            break;// �ҵ����Ե
          }
        }
        else
        {
          pRight++;
        }
      }// if����
    }//for����


    if (i < 60-6 && !bFoundLeft&&!bFoundRight)//δ�ҵ���Ե
    {
      ValidLine[i] = 0;
      NoValidCount++;//��Ч�м���
      if (NoValidCount > NoValidMax)
      {
        NoValidMax = NoValidCount;//��Ч�м������ֵ����
      }
    }
    else
    {
      NoValidCount=0;
    }

    if(bFoundLeft && bFoundRight)
    {
      ValidLineCount1++;
      ValidLine[i] = 3;//�����ҵ�˫��Ե
    }
    else
    {
      if (bFoundLeft )
      {
        ValidLineCount2++;
          if(ValidLineCount2==8);
        ValidLine[i] = 1;
        if(g_Derict == Angle)
        {
          ValidLine[i] = 4;//��ֱ��

          //ke=35;
        }
      }
      if(bFoundRight)
      {
        ValidLineCount2++;
         if(ValidLineCount2==8);
        ValidLine[i] = 1;
        if(g_Derict == Angle)
        {
          ValidLine[i] = 5;//��ֱ��

          //ke=-35;
        }
      }
    }

    if (!bFoundLeft) //δ�ҵ����Ե
    {

      if (i < 35)
      {
        LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2]; //���в���
      }
      else
      {
        LeftBlack[i] = 0;
      }
    }
    else if (i < 35 && ABS(LeftBlack[i] - LeftBlack[i+1]) > TripLen  )
    {
      LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2];//���䲹��

    }

    if (!bFoundRight)//δ�ҵ��ұ�Ե
    {

      if (i < 35)
      {
        RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];//���в���
      }
      else
      {
        RightBlack[i] = 80-1;
      }
    }
    else if (i < 35 && ABS(RightBlack[i] - RightBlack[i+1]) > TripLen )
    {
      RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];//���䲹��

    }

    if (LeftBlack[i] > RightBlack[i])
    {
      temRight = temLeft = (LeftBlack[i] + RightBlack[i])/2;
      LeftBlack[i] = temLeft;
      RightBlack[i] = temRight;
    }

    LeftStableNumbers++;
    RightStableNumbers++;
    BlackLineData[i] = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;
    //������������

    //�������������ݴ���
    if (BlackLineData[i] < 4 || BlackLineData[i] > 80-4)
    {
      MidEnd = 1;
      break;
    }
    if (i < 60-20 && ABS(BlackLineData[i]-BlackLineData[i+1]) > TripLen)//�����䣬�в�ͼ��
    {
      BlackLineData[i] = BlackLineData[i+1] + BlackLineData[i+1] - BlackLineData[i+2];//����������ƽ��
    }
    if (image[i][BlackLineData[i]] == Black)//�м����
    {
      if(ABS(BlackLineData[i]-30)<=3)
      {
        ;//ʲôҲ����
      }
      else
      {
        MidToBlackCount++;
        if (MidToBlackCount >= 2)
        {
          MidEnd = 1;
        }
      }
    }
    else
    {
      MidToBlackCount = 0;
    }
    if (!MidEnd)
    {
      StableNumbers++;
    }
    if (ValidLineCount1 < 4 && i < 20)
    {
      MidEnd = 1;
    }
    if (ValidLineCount2 > 15)
    {
      ;
    }
  }//for����-- ��ɨ����

    /******************************************************************
  **********************************************************************
  *********************************************************************/
    flag_wob=image[57][0];
        j=1;
        jump[0]=0;
        num=1;
		for(i=0;i<80;i++)
		{
			if(image[57][i]!=flag_wob)
				{
					jump[j++]=i;
					flag_wob=image[57][i];
				}
		}
		jump[j]=79;
		i=j-1;
        if(i>=2)
        {
          j_save=j;
          for(;j_save>0;j_save--)
              {
                  jump_mid[j_save-1]=(jump[j_save]-jump[j_save-1])/2+jump[j_save-1];//�����е����߶�������jump_mid������
              }
          j_save=0;
          line1=i;
          //��ʱ����һ��
          for(line1--;line1>0;line1--)
              {
                    if(image[57][jump_mid[line1]]==0&&jump[line1+1]-jump[line1]<9)
                    {
                      j_save++;
                      jump_B[j_save]=jump_mid[line1];
                    }

              }
              i=0;
             while(j_save>0)
             {
               if(ABS(i-39)>ABS(jump_B[j_save]-39))
                 i=jump_B[j_save];
               j_save--;
             }

        }
        else
        {
          i=0;                // ��־λ
        }
        if(i)
        {
                 BlackLineData[59]=i;
                LeftBlack[59]=i-25;
		RightBlack[59]=i+25;
                black_deal(0,(void *)image);
        }



}

#endif


/******************************************************************************
//�˲�����
******************************************************************************/
int P0_X = 0;
int P0_Y = 0;
int P1_X = 0;
int P1_Y = 0;
int P2_X = 0;
int P2_Y = 0;
float Mid_K1 = 0.0;
float Mid_K2 = 0.0;

//���Ե�˲�
void LAverageFilter()
{
	unsigned char i = 0;
	unsigned char j = 0;
	int sum = 0;
	for (i = 60-1;i > 60-(LeftStableNumbers-5);i--)//��Ч�к�5��ȥ�������˲�
	{
		sum = 0;
		for (j = 0;j < 5;j++)
		{
			sum += LeftBlack[i-j];
		}
		LeftBlack[i] = sum/5;//����ƽ��
	}
	P1_X = LeftBlack[60-(LeftStableNumbers-6)];//���һ����Ч�������
	P1_Y = 60-(LeftStableNumbers-6);//���һ����Ч�������
}

//�ұ�Ե�˲�
void RAverageFilter()
{
	unsigned char i = 0;
	unsigned char j = 0;
	int sum = 0;
	for (i = 60-1;i > 60-(RightStableNumbers-5);i--)
	{
		sum = 0;
		for (j = 0;j < 5;j++)
		{
			sum += RightBlack[i-j];
		}
		RightBlack[i] = sum/5;
	}
	P2_X = RightBlack[60-(RightStableNumbers-6)];
	P2_Y = 60-(RightStableNumbers-6);

}

//�����˲�
void AverageFilter()
{
	unsigned char i = 0;
	unsigned char j = 0;
	int sum = 0;
	for (i = 60-1;i > 60-(StableNumbers-5-5);i--)
	{
		sum = 0;
		for (j = 0;j < 5;j++)
		{
			sum += BlackLineData[i-j];
		}
		BlackLineData[i] = sum/5;
	}
	P0_X = BlackLineData[60-1];
	P0_Y = 60-1;

	Mid_K1 = ABS(P0_X-P1_X)*1.0/ABS(P0_Y-P1_Y);//б��1
	Mid_K2 = ABS(P0_X-P2_X)*1.0/ABS(P0_Y-P2_Y);//б��2

}
/******************************************************************************
//ͨ�����ұ�Ե����ö������������
******************************************************************************/
void GetFinalMidLine()
{
  int i = 0,MinStable = 0;
  MinStable = MIN(StableNumbers,LeftStableNumbers);
  MinStable = MIN(MinStable,RightStableNumbers);//��С��Ч����

  for (i = 60-1;i > 60-(MinStable-5) ;i--)//��Ч�к�5��ȥ�������˲�
  {
    BlackLineData[i] = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;//��������

    if (BlackLineData[i] > 80-1)
    {
      BlackLineData[i] = 80-1;//�޷�
    }
    else if (BlackLineData[i] <1)
    {
      BlackLineData[i]=1;//�޷�
    }
  }

  if (StableNumbers > MinStable)//������������Ч
  {
    for (i = 60-(MinStable-5);i > 60 - (StableNumbers-5);i--)//�˵���5�������ڴ���
    {
      BlackLineData[i] = BlackLineData[i+1] + BlackLineData[i+1] - BlackLineData[i+2];
      if (BlackLineData[i] > 80-1)
      {
        BlackLineData[i] = 80-1;
      }
      else if (BlackLineData[i] <1)
      {
        BlackLineData[i]=1;
      }
    }
  }
}



signed char TemMidLineData[60];//��ȡ����ֵ����

void StoreMidLine()
{
  int i = 0;
  for(i = 0;i < 60;i++)
  {
    TemMidLineData[i] = BlackLineData[i];//������ת��
  }
}

int TopE1=0;//�������ϲ���ƫ��
int TopE2=0;//�������²���ƫ��
int TopLen1=0;//�������ϲ���ƫ��
int TopLen2=0;//�������ϲ���ƫ��
int SubBasePoint = 0;//�²��ֻ�׼�����ƫ��

void GetSectionParam()
{
  int TotalPoint = StableNumbers - 10;//�����޸ģ�����Ƿֶ���ƫ��ġ�
  int icount = 0;
  int i = 0;
  int BasePoint = BlackLineData[60-1];
  SubBasePoint = 0;
  TopE1=0;
  TopE2=0;
  TopLen1=0;
  TopLen2=0;

  for (i=60-(StableNumbers-10),icount=1;i < 60-2;i++,icount++)
  {
    if (icount < TotalPoint/2)
    {
      TopE1 += (BlackLineData[i]-BlackLineData[i+1]);//�ϲ���ͼ��ƫ�����
      TopLen1++;//��־λ�ۼ�
    }
    else
    {
      TopE2 += (BlackLineData[i]-BlackLineData[i+1]);
      TopLen2++;
      if (ABS(BlackLineData[i]-BasePoint) > ABS(SubBasePoint))
      {
        SubBasePoint = BlackLineData[i]-BasePoint;//�������ĵ�������
      }
    }
  }
}



/******************************************************************************
//��ȡ����������ƫ��
******************************************************************************/
void GetSpecialError()
{
  unsigned char i=0;
  int end = 60 - StableNumbers;

  MidLineExcursion = 0;
  for (i = 60-1;i > end ;i--)
  {
    BlackLineData[i] = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;
  }
  for(i=60-2; i>end; i--)
  {
    MidLineExcursion = MidLineExcursion + BlackLineData[i] - BlackLineData[i+1];
  }

  MidLineVariance = 300;

  if(MidLineExcursion > 0)
  {
    MidLineExcursion = 40;
  }
  else if(MidLineExcursion < 0)
  {
    MidLineExcursion = -40;
  }
}

/******************************************************************************
//��ȡʮ��������
******************************************************************************/
#if 0
unsigned char g_BasePos = 80/2-1;
unsigned char CrossingStable = 0;
unsigned char ValidLineR[60]={0}; //ʮ���ұ�����Ч��־����
unsigned char ValidLineL[60]={0};//ʮ���������Ч��־����
unsigned char NoValidLMax = 0;//ʮ�ֽ�������������߼���
unsigned char NoValidRMax = 0;//ʮ�ֽ����ұ��������߼���

void GetCrossingMidLine()
{
  unsigned char i = 0,j=0;
  //ָ��
  unsigned char pLeft = 80/2,pRight = 80/2;
  unsigned char bFoundLeft = 0;
  unsigned char bFoundRight = 0;
  unsigned char temLeft = 0,temRight = 0,temi=0;
  unsigned char temBasePos = 0;

  unsigned char EndFlag = 0;
  unsigned char LCount=0;
  unsigned char RCount=0;
  unsigned char bFoundFlag=0;

  CrossingStable=0;
  for (i = 0;i < 60;i++)
  {
    ValidLineR[i] = 0;
    ValidLineL[i] = 0;
  }

  //ʮ��������
  if (BlackEndMax == BlackEndL)
  {
    g_Derict = L_BlackEnd;//����
  }
  else if (BlackEndMax == BlackEndR)
  {
    g_Derict = R_BlackEnd;//����
  }
  else if (BlackEndMax == BlackEndM)
  {
    if (ABS(BlackEndM - BlackEndR)<=2 && ABS(BlackEndM - BlackEndL)<=2)
    {
      g_Derict = Angle;//ֱ��
    }
    else if (ABS(BlackEndL-BlackEndR) < 5)     ////////////
    {
      g_Derict = M_BlackEnd;//��ʮ��
    }
    else if (BlackEndL > BlackEndR)
    {
      g_Derict = L_BlackEnd;//��
    }
    else
    {
      g_Derict = R_BlackEnd;//��
    }
  }

  //����ʮ�ֱ���
  for (i = 60-1; i > 0;i--)
  {
    if (!EndFlag)
    {
      CrossingStable++;
    }
    //��ȥ15�д��м���������
    if (i > 60 - 15 )
    {
      //��ʼ��ָ��
      pLeft  = 80/2-1;
      pRight = 80/2-1;
    }
    else
    {
      //��ʼ��ָ��
      pLeft = g_BasePos;
      pRight = g_BasePos;
    }
    //��ʼ�����������
    bFoundLeft = bFoundRight = 0;

    for (j = 0;j < 80;j++)
    {
      //��������
      if (bFoundLeft == 0 && pLeft > 0)
      {
        if ((image[i][pLeft] == White && image[i][pLeft-1] == Black) || pLeft == 1)
        {
          //�ҵ����Ե
          LeftBlack[i] = pLeft-1;
          bFoundLeft = 1;

          if (LeftBlack[i] > 0)
          {
            ValidLineL[i]=1;
            LCount=0;
          }
          else
          {
            LCount++;
            if (LCount > NoValidLMax)
            {
              //���������������
              NoValidLMax=LCount;
            }
          }
          //�ұ�Ե�ҵ�
          if (bFoundRight)
          {
            break;
          }
        }
        else
        {
          pLeft--;
        }
      }// if����--��������������

      //��������
      if (bFoundRight == 0 && pRight < 80-1 )
      {
        //δ�ҵ��ұ�Ե��Ѱ��
        if ((image[i][pRight] == White && image[i][pRight+1] == Black) || pRight == 80-2)
        {
          // �ҵ��ұ�Ե
          RightBlack[i] = pRight + 1;
          bFoundRight = 1;
          if (RightBlack[i] < 80-1)
          {
            ValidLineR[i] = 1;
            RCount=0;
          }
          else
          {
            RCount++;
            if (RCount > NoValidRMax)
            {
              NoValidRMax=RCount;
            }
          }

          // ���ԵҲ�ҵ�
          if (bFoundLeft)
          {
            break;
          }
        }
        else
        {
          pRight++;
        }
      }// if����
    }//for����

    //���Ҳ�����Ե
    if (!bFoundLeft)
    {
      LeftBlack[i]=0;
    }
    //���Ҳ�����Ե
    if (!bFoundRight)
    {
      RightBlack[i]=80-1;
    }

    if ( i < 60 - 15)
    {
      //�����������������������
      if (g_Derict == L_BlackEnd)
      {
        //�ҵ���������λ��ƫ����Ϊ�µ���������
        if (LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2 < g_BasePos && ABS((LeftBlack[i] - LeftBlack[i+1])) < 3 && ABS((RightBlack[i] - RightBlack[i+1])) < 3)
        {
          temBasePos = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;
          if (ABS(temBasePos-g_BasePos)<20)
          {
            g_BasePos = temBasePos;
            if (g_BasePos < 2)
            {
              EndFlag = 1;
            }
          }
        }
        //�ҵ���������λ��ƫ�ң���Ϊ�µ���������
        else if (LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2 > g_BasePos || RightBlack[i] > RightBlack[i+1]+2)
        {
          temLeft = 0;
          temRight = 0;
          bFoundFlag=0;
        //ȷ���µ���������
          for (temi = 1;temi < 80-1;temi++)
          {
            if (image[i][temi] == White && image[i][temi+1] == White && temLeft == 0)
            {
              temLeft = temi;
            }
            if (temLeft != 0)
            {
              if (image[i][temi]== Black && image[i][temi+1] == Black && image[i][temi+5] == Black)
              {
                temRight = temi;
                bFoundFlag=1;
                break;
              }
            }
          }
          if(bFoundFlag && temLeft + (temRight-temLeft)/2 < g_BasePos)
          {
            temBasePos = temLeft + (temRight-temLeft)/2;
            g_BasePos = temBasePos;
            if (g_BasePos < 3)
            {
              //�ѵ��������������
              EndFlag = 1;
            }
          }
        }
      }

      //ʮ������
      if (g_Derict == R_BlackEnd )
      {
        //�ҵ���������λ��ƫ�ң���Ϊ�µ���������
        if (LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2 > g_BasePos && ABS((LeftBlack[i] - LeftBlack[i+1])) < 3 && ABS((RightBlack[i] - RightBlack[i+1])) < 3)
        {
          temBasePos = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;
          if (ABS(temBasePos-g_BasePos)<20)
          {
            g_BasePos = temBasePos;
            if (g_BasePos > 80-4)
            {
              EndFlag = 1;
            }
          }
        }
        //�ҵ���������λ��ƫ����Ϊ�µ���������
        else if (LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2 < g_BasePos || LeftBlack[i] < LeftBlack[i+1]-2)
        {
          temLeft = 0;
          temRight = 0;
          bFoundFlag = 0;
          //ȷ���µ���������
          for (temi = 80-1;temi > 1;temi--)
          {
            if (image[i][temi] == White && image[i][temi-1] == White && temRight == 0)
            {
            temRight = temi;
            }
            if (temRight != 0)
            {
              if (image[i][temi]== Black && image[i][temi-1]== Black && image[i][temi-5] == Black )
              {
                temLeft = temi;
                bFoundFlag=1;
                break;
              }
            }
          }
          if(bFoundFlag && temLeft + (temRight-temLeft)/2 > g_BasePos)
          {
            temBasePos = temLeft + (temRight-temLeft)/2;
            g_BasePos = temBasePos;
            if (g_BasePos > 80-3)
            {
              EndFlag = 1;
            }
          }
        }
      }
    }
  }
}
#endif






/******************************************************************************
//ʮ�ִ���
******************************************************************************/
#if 0
int LCrossingTripPos = 0;
int RCrossingTripPos = 0;
unsigned char bFoundTripPoint = 0;//�ҵ�ʮ�������


void ProcessCrossing()
{
  unsigned char i = 0,iStart= 60 - StableNumbers + 6,iEnd = 60-1;
  unsigned char iCount=0;
  unsigned char TripPos = 0,pos = 0;
  unsigned char Count1 = 0,Count2=0;
  unsigned char tem0 = 0,tem1=0;
  int startPos=0,endPos=0;
  unsigned char ProcessFlag = 0;

  LCrossingTripPos = 0;
  RCrossingTripPos = 0;

  bFoundTripPoint = 0;
  if (iStart < 5)
  {
    iStart = 5;
  }
  for (i = iStart;i < iEnd;i++)
  {
    tem0 = RightBlack[i]>80-1 ? 80-1:RightBlack[i];
    tem1 = LeftBlack[i] < 1 ? 0:LeftBlack[i];
    if (tem0 - tem1 > 70 )//�ж�ʮ�ֿհ�����ֵ
    {
      iCount++;
    }
    else
    {
      break;
    }
  }

  if (iCount > 30)//�հ��м����ﵽ7
  {
    IsCrossing = 1;//��־λ��λ
  }
  else
  {

    if (NoValidMax > 30 )//��Ч�м����ﵽ8
    {
      IsCrossing = 1;//��־λ��λ

      if (ValidLine[60-7] == 0)//�ж�57�����ͣ��Ƿ���û���ҵ�˫��
      {
        SCProcessing();//��ʮ�ִ���
        return;
      }
      else
      {
        SCProcessing();//��ʮ�ִ���
        return;
      }
    }
    else
    {
      IsCrossing = 0;//��־λ��0
      return;
    }
  }
  //��ʮ��
  if(iCount>30 && g_Derict==M_BlackEnd )
  {
    SCProcessing();
    return;
  }

  //���������Ե����
    if (iCount > 30 && g_Derict == R_BlackEnd)
    {
      i = iStart;
      while (i < iEnd && LeftBlack[i] - LeftBlack[i+1] == 0)
      {
        i++;
      }
      Count1=0;
      Count2=0;
      if (LeftBlack[i]-LeftBlack[i+1] > 0)
      {
        Count1++;
        i++;
        for (;i < iEnd;i++)
        {
          if (LeftBlack[i]-LeftBlack[i+1] >= 0)
          {
            Count1++;
            if (Count2 != 0)
            {
              Count1 = 1;
              Count2 = 0;
            }
          }
          else if (LeftBlack[i]-LeftBlack[i+1] < 0)
          {
            if (Count1 > 2 && TripPos == 0)
            {
              TripPos = i;
            }
            Count2++;
          }
        }
        if (Count1 >2 && Count2 > 2)
        {
          bFoundTripPoint = 1;
          LCrossingTripPos = TripPos;//���Ե�����ҵ�
        }
        else
        {
          Count1++;
          i++;
          for (;i < iEnd;i++)
          {
            if (LeftBlack[i]-LeftBlack[i+1] < 0)
            {
              Count1++;
              if (Count2 != 0)
              {
                Count1 = 1;
                Count2 = 0;
              }
            }
            else if (LeftBlack[i]-LeftBlack[i+1] > 0)
            {
              if (Count1 >2 && TripPos == 0)
              {
                TripPos = i;
              }
              Count2++;
            }
          }
          if (Count1 >2 && Count2 > 2)
          {
            bFoundTripPoint = 1;
            LCrossingTripPos = TripPos;//���Ե�����ҵ�
          }
        }

    //�������ұ�Ե����
    if (iCount > 30 && g_Derict == L_BlackEnd)
    {
      i = iStart;
      Count1=0;
      Count2=0;
      while (i < iEnd && RightBlack[i] - RightBlack[i+1] == 0)
      {
        i++;
      }
      if (RightBlack[i]-RightBlack[i+1] > 0)
      {
        Count1++;
        i++;
        for (;i < iEnd;i++)
        {

          if (RightBlack[i]-RightBlack[i+1] > 0)
          {
            Count1++;
            if (Count2 != 0)
            {
              Count1 = 1;
              Count2 = 0;
            }
          }
          else if (RightBlack[i]-RightBlack[i+1] < 0)
          {
            if (Count1 >2 && TripPos == 0)
            {
              TripPos = i;
            }
            Count2++;
          }
        }
        if (Count1 > 2 && Count2 > 2)
        {
          bFoundTripPoint = 1;
          RCrossingTripPos = TripPos;//�ұ�Ե�����ҵ�
        }
      }
      else
      {
        Count1++;
        i++;
        for (;i < iEnd;i++)
        {
          if (RightBlack[i]-RightBlack[i+1] < 0)
          {
            Count1++;
            if (Count2 != 0)
            {
              Count1 = 1;
              Count2 = 0;
            }
        }
        else if (RightBlack[i]-RightBlack[i+1] > 0)
        {
          if (Count1 > 2 && TripPos == 0)
          {

          }
          Count2++;
          }
        }
        if (Count1 > 2 && Count2 > 2)
        {
          bFoundTripPoint = 1;
          RCrossingTripPos = TripPos;//�ұ�Ե�����ҵ�
        }
        }
      }
    }
    if (bFoundTripPoint)
    {
      GetCrossingMidLine();//ȡʮ�ֱ�Ե��
      if (g_Derict == L_BlackEnd)//����
      {
        pos = TripPos-8;
        while(pos > 0 && (ValidLineR[pos] == 0 || RightBlack[pos] > RightBlack[TripPos]))
        {
          pos--;
        }
        if (RightBlack[pos-2] < RightBlack[TripPos])//���
        {
        //�ұ�Ե���
          CommonRectificate(&RightBlack_A[0],pos-2,TripPos);
          ProcessFlag = 1;
        }
        else
        {
          //��һ�����
          for (i = 0;i < 80;i++)
          {
            if (image[pos-2][i] == White && image[pos-2][i+1] == Black)
            {
              RightBlack[pos-2]=i;
              break;
            }
          }
          if (RightBlack[pos-2] < RightBlack[TripPos])//���
          {
           //�ұ�Ե���
            CommonRectificate(&RightBlack_A[0],pos-2,TripPos);
            ProcessFlag = 1;
          }
          else if (NoValidLMax > 20)
          {
            RightBlack[pos-2]=2;
            CommonRectificate(&RightBlack_A[0],pos-2,TripPos);
            ProcessFlag = 1;
          }
        }
      }
      else if (g_Derict == R_BlackEnd)//����
      {
        pos = TripPos-8;
        while(pos > 0 && ValidLineL[pos] == 0 || LeftBlack[pos] < LeftBlack[TripPos])
        {
          pos--;
        }

        if (LeftBlack[pos-2] > LeftBlack[TripPos])
        {
          CommonRectificate(&LeftBlack_A[0],pos-2,TripPos);
          ProcessFlag = 1;
        }
        else
        {
          for (i = 80-1;i > 0;i--)
          {
            if (image[pos-2][i] == White && image[pos-2][i-1] == Black)
            {
            LeftBlack[pos-2]=i;
            break;
            }
          }
          if (LeftBlack[pos-2] > LeftBlack[TripPos] )
          {
            CommonRectificate(&LeftBlack_A[0],pos-2,TripPos);
            ProcessFlag = 1;
          }
          else if (NoValidRMax > 25)
          {
            LeftBlack[pos-2]=80-2;
            CommonRectificate(&LeftBlack_A[0],pos-2,TripPos);
            ProcessFlag = 1;
          }
        }
      }
    }
    else//�Ҳ�������
    {
      GetCrossingMidLine();//ȡ��Ե��
      if (g_Derict == L_BlackEnd)//����
      {
        pos=20;
        while (ValidLineR[pos]==0)
        {
        pos++;
        }
        while(ValidLineR[pos]==1)
        {
          pos++;
        }
        startPos = pos-2;
        pos += 8;
        while (pos < 60-1 && (ValidLineR[pos] == 0 || RightBlack[pos] > 80-3 ))
        {
          pos++;
        }
        endPos = pos+4;
        {
          CommonRectificate(&RightBlack_A[0],startPos,endPos);
          ProcessFlag = 1;
        }
      }
      else if (g_Derict == R_BlackEnd)
      {
        pos=20;
        while (ValidLineL[pos]==0)
        {
          pos++;
        }
        while(ValidLineL[pos]==1)
        {
          pos++;
        }
        startPos = pos-2;
        pos += 8;
        while (pos < 60-1 && (ValidLineL[pos] == 0 || LeftBlack[pos] < 3 ))
        {
          pos++;
        }
        endPos = pos+4;
        {
          CommonRectificate(&LeftBlack_A[0],startPos,endPos);
          ProcessFlag = 1;
        }
      }
    }

    if (ProcessFlag == 0)
    {
      IsCrossing = 0;
      return;
    }
  }
}

#endif
void UseTemMidLine()
{
  int i = 0;
  for(i = 0;i < 60;i++)
  {
    BlackLineData[i] = TemMidLineData[i];//����������ת��
  }
}


//����ȡʮ�ֽ����˲�
void CrossingMidFilter()
{
  signed char i = 0,j=0,MidEnd = 0;
  unsigned char MidToBlackCount = 0;
  int sum = 0;
  StableNumbers2 = 0;

  for (i = 60-1;i >= 0 && !MidEnd;i--)
  {
    BlackLineData[i] = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;

    StableNumbers2++;//ʮ���������ȶ��м�������

    if (BlackLineData[i] < 4 || BlackLineData[i] > 80-4)
    {
      MidEnd = 1;//����������ֹͣ
      break;
    }
    if (i < 60-5 && ABS(BlackLineData[i]-BlackLineData[i+1]) > 3)
    {
      BlackLineData[i] = BlackLineData[i+1] + BlackLineData[i+1] - BlackLineData[i+2];//������ƽ��
    }
    if (image[i][BlackLineData[i]] == Black)
    {
      ////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
      //������Ҫ�޸�
      MidToBlackCount++;
      if (MidToBlackCount >= 3)
      {
              //�����ڵ�
              MidEnd = 1;
      }
    }
  }

  for (i = 60-1;i > 60-(StableNumbers2-5);i--)
  {
    sum = 0;
    for (j = 0;j < 5;j++)
    {
      sum += BlackLineData[i-j];
    }
    BlackLineData[i] = sum/5;
  }
}

/******************************************************************************
//�����ߴ���
******************************************************************************/
int StoreFlag = 0;

void MidLineProcess()
{
	StoreFlag = 0;
	GetLMR();//��ȡ���Ե���ұ�Ե��������

	//���������ȡ������
	if(StableNumbers > 12)
	{
          LAverageFilter();//���Ե�˲�
          RAverageFilter();//�ұ�Ե�˲�
          GetFinalMidLine();//��ȡ������Ŀ�����
          AverageFilter();//�˲�����
          MidLineCompensate();//�����߲���
          StoreFlag=1;
          StoreMidLine();//�洢����������
          GetSectionParam();//�ֶ���ȡƫ��
          GetMidLineVariance();//��ȡ����
	}
	else
	{
          GetSpecialError();//��ȡ����ƫ��
	}
        //����ʮ��
	ProcessCrossing();

	if (!IsCrossing)
	{
          if(StoreFlag)
          {
            UseTemMidLine();//������ʱ������
          }
	}
	else
	{
          CrossingMidFilter();//ʮ��������ȡ�˲�˳��
          GetSectionParam();//�ֶ���ȡƫ��
	}
}

#endif