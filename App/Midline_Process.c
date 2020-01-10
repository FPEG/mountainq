#include "include.h"
#include "Midline_Process.h"



/***************************************************************
中线

* 函数名称：
* 功能说明：  temp
* 参数说明：
* 函数返回：voiold
* 修改时间：
* 备 注：
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
float Deviation=0;//偏差
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
	      str->track_Width[i]=(int)(4.0+i*Width_P); //计算  路宽
       }
       //       float k=-0.01,b=1;

       //       for(i=0;i<ROW_END;i++)
       //       {
       //	      mid_k[i]=k*i+b;
       //       }

         //首行
       if(str->LeftEdge[ROW_END]!=0 && str->RightEdge[ROW_END]!=0)     //左右都有边界
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
       else if( str->LeftEdge[ROW_END]==0 &&str->RightEdge[ROW_END]==0)//两边丢线   d待处理
       {
	      str->LeftEdge[ROW_END]=COL_START;
	      str->RightEdge[ROW_END]=COL_END;
	      str->MiddleLine[ROW_END]=str->MiddleLine[ROW_END+1]=(str->LeftEdge[ROW_END] + str->RightEdge[ROW_END])>>1;
       }

       //// 从第二行开始搜索

       for(i=ROW_END-1;i>=ROW_START;i--)
       {

//	      if( (str->RightEdge[i]-str->LeftEdge[i])>=(str->RightEdge[i+1]-str->LeftEdge[i+1]+2) )//不满足畸变
//	      {
//		     str->MiddleLine[i] = str->MiddleLine[i+1];//用上一行
//	      }
//	      else
//	      {

	      if(str->LeftEdge[i]!=0 && str->RightEdge[i]!=0)     //左右都有边界
	      {
		     str->MiddleLine[i]=(str->LeftEdge[i]+str->RightEdge[i])>>1;
	      }
	      else if(str->LeftEdge[i]!=0 && str->RightEdge[i]==0)//find left
	      {
		     if(str->LeftEdge[i+1]!=0) //拟合中线
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

		     if(str->RightEdge[i+1]!=0) //拟合中线
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
	      else if( str->LeftEdge[i]==0&&str->RightEdge[i]==0)//两边丢线   d待处理
	      {
		     //应当斜率补线  最小二乘法
//		     str->LeftEdge[ROW_END]=COL_START;
//		     str->RightEdge[ROW_END]=COL_END;
		     str->MiddleLine[i]=str->MiddleLine[i+1] ;
	      }

#if 0//作用不大

		     //对斜出十字进行纠正
		     //下面是很有用的程序    ！！不要觉得简单 解决斜入最简单好用的方法

		     if(str->MiddleLine[i]-str->MiddleLine[i+1]>4&&i>=5)//中线向右突变
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
				   //中线限幅
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





       //中线两端补线
#if 0  //底部没必要  而且  问题很多

       if(str->StartLine>33)
       {
	      for(i=str->StartLine;i<=ROW_END;i++)   //底部补线
	      {
		     if(str->LeftEdge[i]!=0 && str->RightEdge[i]!=0)     //左右都有边界
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
		     else if(str->LeftEdge[i]==0&&str->RightEdge[i]==0)//两边丢线   d待处理
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

       for(i=ROW_END-1;i>str->EndLine;i--)  //顶端延伸
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
       //中线修复
       for(i=ROW_END-1;i>ROW_START+1;i--)    //  ROW_START  BUG点   不能用
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
       Camera_CenterError=Getrouteoffset(38,20,40);//*(Boundary.EndLine/20+0.9);    //计算偏差
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
       0, 0, 0, 0, 0.4, 0.4, 0.4, 0.4, 0.5, 0.8,                       //0-9行，基本用不到

       0.9, 0.9, 1.0, 1.0, 1.1,1.1, 1.2,1.3, 2,2,           //0-19行，基本用不到

       2,   2,   2,   2,   2,   2,   2,   2, 2,  2,   //20-29行

       2,1.8,1.7, 1.6,1.5,1.4, 1.3,1.2,0.5, 0.1           //30-39行
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
//	 LineWeight[i]=-0.0125*(i-20.0)*(i-20.0)+5.0; //计算  权重
//	 if(LineWeight[i]<0)
//		LineWeight[i]=0;
//  }
  //  float k=-0.005,b=1;
  //首行
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
	 LineWeight[i]=-0.0125*(i-20.0)*(i-20.0)+5.0; //计算  权重
	 if(LineWeight[i]<0)
		LineWeight[i]=0;
  }
#endif
  //  float k=-0.005,b=1;
  //首行
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
提取中心线
******************************************************************************/
int NoValidCount = 0;
int NoValidMax=0;//连续两边都找不到计数
int StableNumbers = 0;//稳定行计数
int StableNumbers2 = 0;//十字稳定行计数
int LeftStableNumbers = 0;//左稳定行计数
int RightStableNumbers = 0;//右稳定行计数
int ValidLineCount1 = 0; //左右都找到
int ValidLineCount2 = 0;//只要一边找到
extern int8 num=0;
extern float ke;
int8 flag_cross=0,i_left_old=0,i_right_old=0;

unsigned char ValidLine[60]={0};//有效行计数

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



  for (i = 60-1;i >= 0 && !MidEnd;i--)//最近一行开始
  {
    // 初始化指针

    if (i < 60 - 5)//5行以上
    {
      pLeft = BlackLineData[i+1];//利用下一行边缘继续寻线
      pRight= BlackLineData[i+1];//利用下一行边缘继续寻线
    }
    else
    {
      if(BlackEndL < 5 && BlackEndR > BlackEndM)//左侧有效点数小于5，图像右偏
      {
        pLeft = 80*3/4-1;//图像遍历指针给大
        pRight = 80*3/4-1;
      }
      else if(BlackEndR < 5 && BlackEndL > BlackEndM)//右侧有效点数小于5，图像左偏
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


    // 初始化标记
    bFoundLeft = bFoundRight = 0;

    //两边黑线提取
    for (j = 0;j < 80;j++)
    {
      // 往左搜索
      if (bFoundLeft == 0 && pLeft > 0)
      {
        //未找到左边缘则寻找
        if (image[i][pLeft] == White && image[i][pLeft-1] == Black)
        {
          LeftBlack[i] = pLeft-1;
          if (LeftBlack[i] > 0)
          {
            bFoundLeft = 1;//找到左边缘
          }
          if (bFoundRight)
          {
            break;//右边缘也找到
          }
        }
        else
        {
          pLeft--;
        }
      }// if结束--从左到右搜索边缘

      //往右搜索
      if (bFoundRight == 0 && pRight < 80-1)
      {
        //未找到右边缘
        if (image[i][pRight] == White &&  image[i][pRight+1] == Black)
        {
          RightBlack[i] = pRight + 1;
          if (RightBlack[i] < 80-1)
          {
            bFoundRight = 1;//找到右边缘
          }
          if (bFoundLeft)
          {
            break;// 找到左边缘
          }
        }
        else
        {
          pRight++;
        }
      }// if结束
    }//for结束


    if (i < 60-6 && !bFoundLeft&&!bFoundRight)//未找到边缘
    {
      ValidLine[i] = 0;
      NoValidCount++;//无效行计数
      if (NoValidCount > NoValidMax)
      {
        NoValidMax = NoValidCount;//无效行计数最大值更新
      }
    }
    else
    {
      NoValidCount=0;
    }

    if(bFoundLeft && bFoundRight)
    {
      ValidLineCount1++;
      ValidLine[i] = 3;//该行找到双边缘
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
          ValidLine[i] = 4;//右直角

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
          ValidLine[i] = 5;//左直角

          //ke=-35;
        }
      }
    }

    if (!bFoundLeft) //未找到左边缘
    {

      if (i < 35)
      {
        LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2]; //进行补线
      }
      else
      {
        LeftBlack[i] = 0;
      }
    }
    else if (i < 35 && ABS(LeftBlack[i] - LeftBlack[i+1]) > TripLen  )
    {
      LeftBlack[i] = LeftBlack[i+1] + LeftBlack[i+1] - LeftBlack[i+2];//跳变补线

    }

    if (!bFoundRight)//未找到右边缘
    {

      if (i < 35)
      {
        RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];//进行补线
      }
      else
      {
        RightBlack[i] = 80-1;
      }
    }
    else if (i < 35 && ABS(RightBlack[i] - RightBlack[i+1]) > TripLen )
    {
      RightBlack[i] = RightBlack[i+1] + RightBlack[i+1] - RightBlack[i+2];//跳变补线

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
    //中心搜索结束

    //中心线数组数据处理
    if (BlackLineData[i] < 4 || BlackLineData[i] > 80-4)
    {
      MidEnd = 1;
      break;
    }
    if (i < 60-20 && ABS(BlackLineData[i]-BlackLineData[i+1]) > TripLen)//有跳变，中部图像
    {
      BlackLineData[i] = BlackLineData[i+1] + BlackLineData[i+1] - BlackLineData[i+2];//进行中心线平均
    }
    if (image[i][BlackLineData[i]] == Black)//中间截至
    {
      if(ABS(BlackLineData[i]-30)<=3)
      {
        ;//什么也不做
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
  }//for结束-- 行扫描完

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
                  jump_mid[j_save-1]=(jump[j_save]-jump[j_save-1])/2+jump[j_save-1];//将所有的中线都保存在jump_mid数组中
              }
          j_save=0;
          line1=i;
          //暂时保存一下
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
          i=0;                // 标志位
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
//滤波函数
******************************************************************************/
int P0_X = 0;
int P0_Y = 0;
int P1_X = 0;
int P1_Y = 0;
int P2_X = 0;
int P2_Y = 0;
float Mid_K1 = 0.0;
float Mid_K2 = 0.0;

//左边缘滤波
void LAverageFilter()
{
	unsigned char i = 0;
	unsigned char j = 0;
	int sum = 0;
	for (i = 60-1;i > 60-(LeftStableNumbers-5);i--)//有效行后5行去掉进行滤波
	{
		sum = 0;
		for (j = 0;j < 5;j++)
		{
			sum += LeftBlack[i-j];
		}
		LeftBlack[i] = sum/5;//进行平均
	}
	P1_X = LeftBlack[60-(LeftStableNumbers-6)];//最后一个有效点的列数
	P1_Y = 60-(LeftStableNumbers-6);//最后一个有效点的行数
}

//右边缘滤波
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

//中心滤波
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

	Mid_K1 = ABS(P0_X-P1_X)*1.0/ABS(P0_Y-P1_Y);//斜率1
	Mid_K2 = ABS(P0_X-P2_X)*1.0/ABS(P0_Y-P2_Y);//斜率2

}
/******************************************************************************
//通过左右边缘来获得舵机控制中心线
******************************************************************************/
void GetFinalMidLine()
{
  int i = 0,MinStable = 0;
  MinStable = MIN(StableNumbers,LeftStableNumbers);
  MinStable = MIN(MinStable,RightStableNumbers);//最小有效行数

  for (i = 60-1;i > 60-(MinStable-5) ;i--)//有效行后5行去掉进行滤波
  {
    BlackLineData[i] = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;//中线数据

    if (BlackLineData[i] > 80-1)
    {
      BlackLineData[i] = 80-1;//限幅
    }
    else if (BlackLineData[i] <1)
    {
      BlackLineData[i]=1;//限幅
    }
  }

  if (StableNumbers > MinStable)//中心线数据有效
  {
    for (i = 60-(MinStable-5);i > 60 - (StableNumbers-5);i--)//滤掉的5行数据在处理
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



signed char TemMidLineData[60];//提取黑线值数据

void StoreMidLine()
{
  int i = 0;
  for(i = 0;i < 60;i++)
  {
    TemMidLineData[i] = BlackLineData[i];//中心线转存
  }
}

int TopE1=0;//中心线上部分偏差
int TopE2=0;//中心线下部分偏差
int TopLen1=0;//中心线上部分偏差
int TopLen2=0;//中心线上部分偏差
int SubBasePoint = 0;//下部分基准点最大偏差

void GetSectionParam()
{
  int TotalPoint = StableNumbers - 10;//可以修改，这个是分段算偏差的。
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
      TopE1 += (BlackLineData[i]-BlackLineData[i+1]);//上部分图像偏差类和
      TopLen1++;//标志位累加
    }
    else
    {
      TopE2 += (BlackLineData[i]-BlackLineData[i+1]);
      TopLen2++;
      if (ABS(BlackLineData[i]-BasePoint) > ABS(SubBasePoint))
      {
        SubBasePoint = BlackLineData[i]-BasePoint;//相邻中心点误差跟新
      }
    }
  }
}



/******************************************************************************
//获取特殊中心线偏差
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
//获取十字中心线
******************************************************************************/
#if 0
unsigned char g_BasePos = 80/2-1;
unsigned char CrossingStable = 0;
unsigned char ValidLineR[60]={0}; //十字右边行有效标志数组
unsigned char ValidLineL[60]={0};//十字左边行有效标志数组
unsigned char NoValidLMax = 0;//十字交叉左边连续丢线计数
unsigned char NoValidRMax = 0;//十字交叉右边连续丢线计数

void GetCrossingMidLine()
{
  unsigned char i = 0,j=0;
  //指针
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

  //十字左右倾
  if (BlackEndMax == BlackEndL)
  {
    g_Derict = L_BlackEnd;//左倾
  }
  else if (BlackEndMax == BlackEndR)
  {
    g_Derict = R_BlackEnd;//右倾
  }
  else if (BlackEndMax == BlackEndM)
  {
    if (ABS(BlackEndM - BlackEndR)<=2 && ABS(BlackEndM - BlackEndL)<=2)
    {
      g_Derict = Angle;//直角
    }
    else if (ABS(BlackEndL-BlackEndR) < 5)     ////////////
    {
      g_Derict = M_BlackEnd;//正十字
    }
    else if (BlackEndL > BlackEndR)
    {
      g_Derict = L_BlackEnd;//左
    }
    else
    {
      g_Derict = R_BlackEnd;//右
    }
  }

  //搜索十字边线
  for (i = 60-1; i > 0;i--)
  {
    if (!EndFlag)
    {
      CrossingStable++;
    }
    //除去15行从中间往两边搜
    if (i > 60 - 15 )
    {
      //初始化指针
      pLeft  = 80/2-1;
      pRight = 80/2-1;
    }
    else
    {
      //初始化指针
      pLeft = g_BasePos;
      pRight = g_BasePos;
    }
    //初始化搜索到标记
    bFoundLeft = bFoundRight = 0;

    for (j = 0;j < 80;j++)
    {
      //往左搜索
      if (bFoundLeft == 0 && pLeft > 0)
      {
        if ((image[i][pLeft] == White && image[i][pLeft-1] == Black) || pLeft == 1)
        {
          //找到左边缘
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
              //左边连续丢线总数
              NoValidLMax=LCount;
            }
          }
          //右边缘找到
          if (bFoundRight)
          {
            break;
          }
        }
        else
        {
          pLeft--;
        }
      }// if结束--从左到右搜索结束

      //往右搜索
      if (bFoundRight == 0 && pRight < 80-1 )
      {
        //未找到右边缘则寻找
        if ((image[i][pRight] == White && image[i][pRight+1] == Black) || pRight == 80-2)
        {
          // 找到右边缘
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

          // 左边缘也找到
          if (bFoundLeft)
          {
            break;
          }
        }
        else
        {
          pRight++;
        }
      }// if结束
    }//for结束

    //左找不到边缘
    if (!bFoundLeft)
    {
      LeftBlack[i]=0;
    }
    //右找不到边缘
    if (!bFoundRight)
    {
      RightBlack[i]=80-1;
    }

    if ( i < 60 - 15)
    {
      //左倾情况搜索，中心线往左
      if (g_Derict == L_BlackEnd)
      {
        //找到的中心线位置偏左，作为新的搜索基点
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
        //找到的中心线位置偏右，作为新的搜索基点
        else if (LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2 > g_BasePos || RightBlack[i] > RightBlack[i+1]+2)
        {
          temLeft = 0;
          temRight = 0;
          bFoundFlag=0;
        //确定新的搜索基点
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
              //已到最左边无需搜素
              EndFlag = 1;
            }
          }
        }
      }

      //十字右倾
      if (g_Derict == R_BlackEnd )
      {
        //找到的中心线位置偏右，作为新的搜索基点
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
        //找到的中心线位置偏左，作为新的搜索基点
        else if (LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2 < g_BasePos || LeftBlack[i] < LeftBlack[i+1]-2)
        {
          temLeft = 0;
          temRight = 0;
          bFoundFlag = 0;
          //确定新的搜索基点
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
//十字处理
******************************************************************************/
#if 0
int LCrossingTripPos = 0;
int RCrossingTripPos = 0;
unsigned char bFoundTripPoint = 0;//找到十字跳变点


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
    if (tem0 - tem1 > 70 )//判定十字空白行阈值
    {
      iCount++;
    }
    else
    {
      break;
    }
  }

  if (iCount > 30)//空白行计数达到7
  {
    IsCrossing = 1;//标志位置位
  }
  else
  {

    if (NoValidMax > 30 )//无效行计数达到8
    {
      IsCrossing = 1;//标志位置位

      if (ValidLine[60-7] == 0)//判断57行线型，是否是没有找到双边
      {
        SCProcessing();//正十字处理
        return;
      }
      else
      {
        SCProcessing();//正十字处理
        return;
      }
    }
    else
    {
      IsCrossing = 0;//标志位清0
      return;
    }
  }
  //正十字
  if(iCount>30 && g_Derict==M_BlackEnd )
  {
    SCProcessing();
    return;
  }

  //右倾找左边缘跳变
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
          LCrossingTripPos = TripPos;//左边缘跳变找到
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
            LCrossingTripPos = TripPos;//左边缘跳变找到
          }
        }

    //左倾找右边缘跳变
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
          RCrossingTripPos = TripPos;//右边缘跳变找到
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
          RCrossingTripPos = TripPos;//右边缘跳变找到
        }
        }
      }
    }
    if (bFoundTripPoint)
    {
      GetCrossingMidLine();//取十字边缘线
      if (g_Derict == L_BlackEnd)//左倾
      {
        pos = TripPos-8;
        while(pos > 0 && (ValidLineR[pos] == 0 || RightBlack[pos] > RightBlack[TripPos]))
        {
          pos--;
        }
        if (RightBlack[pos-2] < RightBlack[TripPos])//拟合
        {
        //右边缘拟合
          CommonRectificate(&RightBlack_A[0],pos-2,TripPos);
          ProcessFlag = 1;
        }
        else
        {
          //另一种拟合
          for (i = 0;i < 80;i++)
          {
            if (image[pos-2][i] == White && image[pos-2][i+1] == Black)
            {
              RightBlack[pos-2]=i;
              break;
            }
          }
          if (RightBlack[pos-2] < RightBlack[TripPos])//拟合
          {
           //右边缘拟合
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
      else if (g_Derict == R_BlackEnd)//右倾
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
    else//找不到跳变
    {
      GetCrossingMidLine();//取边缘线
      if (g_Derict == L_BlackEnd)//左倾
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
    BlackLineData[i] = TemMidLineData[i];//中心线数据转储
  }
}


//新提取十字进行滤波
void CrossingMidFilter()
{
  signed char i = 0,j=0,MidEnd = 0;
  unsigned char MidToBlackCount = 0;
  int sum = 0;
  StableNumbers2 = 0;

  for (i = 60-1;i >= 0 && !MidEnd;i--)
  {
    BlackLineData[i] = LeftBlack[i] + (RightBlack[i]-LeftBlack[i] )/2;

    StableNumbers2++;//十字中心线稳定行计数增加

    if (BlackLineData[i] < 4 || BlackLineData[i] > 80-4)
    {
      MidEnd = 1;//中心线搜索停止
      break;
    }
    if (i < 60-5 && ABS(BlackLineData[i]-BlackLineData[i+1]) > 3)
    {
      BlackLineData[i] = BlackLineData[i+1] + BlackLineData[i+1] - BlackLineData[i+2];//中心线平均
    }
    if (image[i][BlackLineData[i]] == Black)
    {
      ////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////
      //这里需要修改
      MidToBlackCount++;
      if (MidToBlackCount >= 3)
      {
              //碰到黑点
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
//中心线处理
******************************************************************************/
int StoreFlag = 0;

void MidLineProcess()
{
	StoreFlag = 0;
	GetLMR();//提取左边缘，右边缘，中心线

	//正常情况提取中心线
	if(StableNumbers > 12)
	{
          LAverageFilter();//左边缘滤波
          RAverageFilter();//右边缘滤波
          GetFinalMidLine();//获取舵机中心控制线
          AverageFilter();//滤波处理
          MidLineCompensate();//中心线补偿
          StoreFlag=1;
          StoreMidLine();//存储中心线数据
          GetSectionParam();//分段提取偏差
          GetMidLineVariance();//提取方差
	}
	else
	{
          GetSpecialError();//获取特殊偏差
	}
        //处理十字
	ProcessCrossing();

	if (!IsCrossing)
	{
          if(StoreFlag)
          {
            UseTemMidLine();//储存零时中心线
          }
	}
	else
	{
          CrossingMidFilter();//十字中线提取滤波顺滑
          GetSectionParam();//分段提取偏差
	}
}

#endif