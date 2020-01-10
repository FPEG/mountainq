#include "include.h"
#include "Cross_Deal.h"


#define   Cross_Hang  7

/*十字*/
//unsigned char L_up_cross_flag=0;         //十字中间只有上十字拐角
//unsigned char R_up_cross_flag=0;
//
//unsigned char L_cross_flag=0;              //左边沿判断进入十字标志置  0
//unsigned char R_cross_flag=0;              //右边沿判断进入十字标志置  0
//unsigned char Cross_flag=0;                //判断进入十字标志置  0


unsigned char  Cross_row;       //
unsigned char  Cross_col;
unsigned char  Cross_num;
Str_Cross   Cross;
Enum_CrossType   CrossType;
Enum_CrossProcess  CrossProcess;

float  LeftCrossSlope;
float  RightCrossSlope;

void Cross_Detection();//十字检测
void Cross_Process();//十字检测
void Cross_Judge();//十字判断
/***************************************************************
十字处理

* 函数名称： Cross_Detection()//十字处理
* 功能说明： 十字处理外部调用
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
*******************************************************************/
void Cross_Deal()//十字检测
{
       if(REN.EN_Cross==1)
       {
	      if(!Cross.Cross_flag)
	      {
		     for(Cross_row=ROW_END;Cross_row>=ROW_START;Cross_row--)
		     {
			    Cross.Track_Width_forCross[Cross_row]=(int)(4.0+Cross_row*1.85); //计算  圆环路宽
		     }
		     Cross_Detection();        //十字检测
	      }
	      else
	      {
		    // Cross_Judge();//十字判断//不需要用
		     Cross_Process();
	      }
       }
//       TFT_showuint16(42,5,Cross.Cross_flag, RED,BLACK);
//       TFT_showuint16(42,6,CrossType, RED,BLACK);
//       TFT_showuint16(42,7,CrossProcess, RED,BLACK);
//      TFT_showuint16(42,8,Inflection_point.L_down_point.x, RED,BLACK);
//        TFT_showuint16(60,7,Boundary.L_StartLine, RED,BLACK);
//       TFT_showuint16(60,8,Boundary.R_StartLine, RED,BLACK);
//       TFT_showuint16(42,4,RoundProcess, RED,BLACK);
//       TFT_showuint16(42,5,RoundProcess, RED,BLACK);
}


/***************************************************************
十字检测

* 函数名称： Cross_Detection()//十字处理
* 功能说明： 十字检测
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：     检测一直开着  十字检测
 *******************************************************************/
void Cross_Detection()//十字检测
{
       if(Inflection_point.L_down_point.x>0||Inflection_point.R_down_point.x>0)
       {
	      Cross_num=0;            //清除计数值    空白行连续性计数
	      for(Cross_row=Boundary.StartLine; Cross_row>ROW_START ; Cross_row-- )     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
	      {
		     if(Boundary.RowLose[Cross_row]==4)
		     {
			    for(Cross_col=Cross_row; Cross_col>ROW_START ; Cross_col-- )     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
			    {
				   if(Boundary.RowLose[Cross_col]==4)
				   {
					  Cross_num++;
				   }
				   else
				   {
					  if( Cross_num>=Cross_Hang)
					  {
						 Cross.Cross_flag=1;//十字flag
						 CrossProcess=Find_Cross;
						 Cross_row=ROW_START;  //清外循环
						 break;
					  }
					  else
					  {
						 Cross_num=0;
					  }
				   }
			    }
		     }
	      }
       }
}

/***************************************************************
十字判断

* 函数名称： void Cross_Judge()//十字判断理
* 功能说明： 十字判断
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
 *******************************************************************/
void Cross_Judge()//十字判断
{
       //CrossType=Cro_else;

       //十字结束清类型
       if(Inflection_point.L_down_point.x>0||Inflection_point.R_down_point.x>0)
       {
	      if(Inflection_point.L_down_point.x>0&&Inflection_point.R_down_point.x==0)
	      {
		     if(Inflection_point.L_down_point.y>12)
		     {
			    if(Boundary.R_StartLine<20)
			    {
				   CrossType=Cro_RightLean;//十字右倾
			    }
		     }
	      }
	      else if(Inflection_point.L_down_point.x==0&&Inflection_point.R_down_point.x>0)
	      {
		     if(Inflection_point.R_down_point.y<68)
		     {
			    if(Boundary.L_StartLine<20)
			    {
				   CrossType=Cro_LeftLean; //十字左倾
			    }
		     }
	      }
	      else if(Inflection_point.L_down_point.x>0&&Inflection_point.R_down_point.x>0)
	      {
		     if(Inflection_point.L_up_point.x>0&&Inflection_point.R_up_point.x>0)
		     {
			    CrossType=Cro_ZHENG; //正十字;
		     }
		     //		     else
		     //		     {
		     //			   //
		     //		     }
	      }
       }
}


#define  SC_MUN  8
/***************************************************************
十字处理

* 函数名称：Cross_Process()//十字处理
* 功能说明： 十字处理
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
*******************************************************************/
void Cross_Process()//十字处理
{
       switch(CrossProcess)
       {
	    case Find_Cross:
	      {
		     //判断进入下一阶段
		      if(Boundary.StartLine<28)
		      {
			  CrossProcess=In_Cross;
		      }
		       Cross_num=0;            //清除计数值    空白行连续性计数
		      for(Cross_row=Boundary.StartLine; Cross_row>ROW_START ; Cross_row-- )     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
		      {
			     if(Boundary.RowLose[Cross_row]==4)
			     {
				    Cross_num++;
				    if( Cross_num>Cross_Hang)
				    {
					   break;
				    }
			     }
		      }
		      if( Cross_num<Cross_Hang-1)
		      {
			     CrossProcess=In_Cross;
		      }

#if 1
		     if(Inflection_point.L_down_point.x>0&&Inflection_point.R_down_point.x>0)  //开始补线    常规补线
		     {
			    if(Inflection_point.L_up_point.x!=0&&Inflection_point.R_up_point.x!=0)
			    {
				   CommonRectificate(&Boundary.LeftEdge[0],Inflection_point.L_up_point.x,Inflection_point.L_down_point.x+1);
				   CommonRectificate(&Boundary.RightEdge[0],Inflection_point.R_up_point.x,Inflection_point.R_down_point.x+1);
			    }
			    else if(Inflection_point.L_up_point.x!=0&&Inflection_point.R_up_point.x==0)
			    {
				   if(Inflection_point.L_up_point.y>Inflection_point.L_down_point.y)
				   {
					  CommonRectificate(&Boundary.LeftEdge[0],Inflection_point.L_up_point.x-1,Inflection_point.L_down_point.x+1);
					  for(Cross_row=Inflection_point.L_up_point.x-1;Cross_row<=Inflection_point.L_down_point.x+1;Cross_row++)
					  {
						 Boundary.RightEdge[Cross_row] = Boundary.LeftEdge[Cross_row]+ Cross.Track_Width_forCross[Cross_row] ;
					  }
				   }
			    }
			    else if(Inflection_point.L_up_point.x==0&&Inflection_point.R_up_point.x!=0)
			    {
				   if(Inflection_point.R_up_point.y<Inflection_point.R_down_point.y)
				   {
					  CommonRectificate(&Boundary.RightEdge[0],Inflection_point.R_up_point.x-1,Inflection_point.R_down_point.x+1);
					  for(Cross_row=Inflection_point.R_up_point.x-1;Cross_row<=Inflection_point.R_down_point.x+1;Cross_row++)
					  {
						 Boundary.LeftEdge[Cross_row] = Boundary.RightEdge[Cross_row]- Cross.Track_Width_forCross[Cross_row] ;
					  }
				   }
			    }
			    else
			    {
				   if(Boundary.L_StartLine>Inflection_point.L_down_point.x+SC_MUN) //至少10个
				   {
					  // Cross.addL_flag=1;
					  LeftCrossSlope= Slope_Calculate(Inflection_point.L_down_point.x+1,Boundary.L_StartLine,&Boundary.LeftEdge[0]) ;
					  for(Cross_row=Inflection_point.L_down_point.x;Cross_row>ROW_START;Cross_row--)
					  {
						 Boundary.LeftEdge[Cross_row]=(int)((Cross_row-Inflection_point.L_down_point.x)*LeftCrossSlope+Inflection_point.L_down_point.y);
						 Boundary.RightEdge[Cross_row] = Boundary.LeftEdge[Cross_row]+ Cross.Track_Width_forCross[Cross_row] ;
					  }
				   }
				   else if(Boundary.R_StartLine>Inflection_point.R_down_point.x+SC_MUN)
				   {
					  // Cross.addR_flag=1;
					  RightCrossSlope=Slope_Calculate(Inflection_point.R_down_point.x+1,Boundary.R_StartLine,&Boundary.RightEdge[0]) ;
					  for(Cross_row=Inflection_point.R_down_point.x;Cross_row>ROW_START;Cross_row--)
					  {
						 Boundary.RightEdge[Cross_row]=(int)((Cross_row-Inflection_point.R_down_point.x)*RightCrossSlope+Inflection_point.R_down_point.y);
						 Boundary.LeftEdge[Cross_row] = Boundary.RightEdge[Cross_row]- Cross.Track_Width_forCross[Cross_row] ;
					  }
				   }

			    }
		     }


		     else if(Inflection_point.L_down_point.x>0&&Inflection_point.R_down_point.x==0)  //开始补线    常规补线
		     {
			    if(Inflection_point.L_up_point.x!=0)
			    {
				   CommonRectificate(&Boundary.LeftEdge[0],Inflection_point.L_up_point.x-1,Inflection_point.L_down_point.x+1);
				   for(Cross_row=Inflection_point.L_up_point.x-1;Cross_row<=Inflection_point.L_down_point.x+1;Cross_row++)
				   {
					  Boundary.RightEdge[Cross_row] = Boundary.LeftEdge[Cross_row]+ Cross.Track_Width_forCross[Cross_row] ;
				   }
			    }
			    else
			    {
				   if(Boundary.L_StartLine>Inflection_point.L_down_point.x+SC_MUN) //至少10个
				   {
					  // Cross.addL_flag=1;
					  LeftCrossSlope= Slope_Calculate(Inflection_point.L_down_point.x+1,Boundary.L_StartLine,&Boundary.LeftEdge[0]) ;
					  for(Cross_row=Inflection_point.L_down_point.x;Cross_row>ROW_START;Cross_row--)
					  {
						 Boundary.LeftEdge[Cross_row]=(int)((Cross_row-Inflection_point.L_down_point.x)*LeftCrossSlope+Inflection_point.L_down_point.y);
						 Boundary.RightEdge[Cross_row] = Boundary.LeftEdge[Cross_row]+ Cross.Track_Width_forCross[Cross_row] ;
					  }
				   }
			    }
		     }


		     if(Inflection_point.L_down_point.x==0&&Inflection_point.R_down_point.x>0)  //开始补线    常规补线
		     {
			    if(Inflection_point.R_up_point.x!=0)
			    {
				   CommonRectificate(&Boundary.RightEdge[0],Inflection_point.R_up_point.x-1,Inflection_point.R_down_point.x+1);
				   for(Cross_row=Inflection_point.R_up_point.x-1;Cross_row<=Inflection_point.R_down_point.x+1;Cross_row++)
				   {
					  Boundary.LeftEdge[Cross_row] = Boundary.RightEdge[Cross_row]- Cross.Track_Width_forCross[Cross_row] ;
				   }
			    }
			    else
			    {
				   if(Boundary.R_StartLine>Inflection_point.R_down_point.x+SC_MUN)
				   {
					  // Cross.addR_flag=1;
					  RightCrossSlope=Slope_Calculate(Inflection_point.R_down_point.x+1,Boundary.R_StartLine,&Boundary.RightEdge[0]) ;
					  for(Cross_row=Inflection_point.R_down_point.x;Cross_row>ROW_START;Cross_row--)
					  {
						 Boundary.RightEdge[Cross_row]=(int)((Cross_row-Inflection_point.R_down_point.x)*RightCrossSlope+Inflection_point.R_down_point.y);
						 Boundary.LeftEdge[Cross_row] = Boundary.RightEdge[Cross_row]- Cross.Track_Width_forCross[Cross_row] ;
					  }
				   }

			    }
		     }
#endif
		     break;
	      }
	    case In_Cross:
	      {
		      if(Boundary.StartLine>32)
		      {
			  CrossProcess=Out_Cross;
		      }
		     break;
	      }

	    case  Out_Cross :
	      {
		     Cross.Cross_flag=0;//十字flag
		     break;
	      }
	    default:
	      {
		     break;
	      }
       }

}
