#include  "include.h"
#include  "BreakRoad.h"

Str_Break_Road  Break_Road;
Str_Barrier  Barrier;  //障碍结构体
Enum_Bar_Process  Bar_Process; //障碍路段过程计数
Enum_BarType      BarType;

uint8 Bar_row;
uint8 Bar_count;



int16  Bar_Left_para=25;//左转向参数
int16  Bar_Right_para=-33;//右转向参数



void  Barrier_Detection();
void  Barrier_Process();


/***************************************************************
障碍处理

* 函数名称： Barrier_Deal()//障碍处理
* 功能说明： 障碍处理外部调用
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
*******************************************************************/
void Barrier_Deal()//障碍处理
{
       if(!Barrier.Barrier_Flag)
       {
	      Barrier_Detection();        //障碍检测
       }
       else
       {
	     Barrier_Process();
       }
//       TFT_showuint16(42,5,Barrier.Barrier_Flag, RED,BLACK);
//      TFT_showuint16(42,6,Bar_Process, RED,BLACK);
//      TFT_showuint16(42,7,BarType, RED,BLACK);
//       TFT_showint16(42,8,Barrier.Bar_Err, RED,BLACK);
//       TFT_showuint16(60,7,Boundary.L_StartLine, RED,BLACK);
//       TFT_showuint16(60,8,Boundary.R_StartLine, RED,BLACK);
//       TFT_showuint16(42,4,RoundProcess, RED,BLACK);
//       TFT_showuint16(42,5,RoundProcess, RED,BLACK);
}

/***************************************************************
断路处理

* 函数名称： Break_Road_Deal()//断路处理
* 功能说明： 断路处理外部调用
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
*******************************************************************/
void Break_Road_Deal()//断路处理
{
       if(Break_Road.Break_Road_Flag==1)
       {
	      //检测出断路
	      if(distance_judge(1400))
	      {
		     if(BlackEnd.BlackEndL>5&&BlackEnd.BlackEndM>5&&BlackEnd.BlackEndR>5)
		     {
			    RoadType=else_road;
			    Break_Road.Break_Road_Flag=0;
			    Break_Road.Break_Road_preFlag =0;
			   // Motor.SetSpeed=50;
		     }
	      }
       }

//       TFT_showuint16(42,5,Barrier.Barrier_Flag, RED,BLACK);
//      TFT_showuint16(42,6,Bar_Process, RED,BLACK);
//      TFT_showuint16(42,7,BarType, RED,BLACK);
//       TFT_showint16(42,8,Barrier.Bar_Err, RED,BLACK);
//       TFT_showuint16(60,7,Boundary.L_StartLine, RED,BLACK);
//       TFT_showuint16(60,8,Boundary.R_StartLine, RED,BLACK);
//       TFT_showuint16(42,4,RoundProcess, RED,BLACK);
//       TFT_showuint16(42,5,RoundProcess, RED,BLACK);
}



int BDet_distance=90;
/***************************************************************
障碍检测

* 函数名称： Barrier_Deal()//障碍处理
* 功能说明： 障碍处理外部调用
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
*******************************************************************/
void  Barrier_Detection() //障碍检测
{
       if(TFmini_Plus.Dist<BDet_distance&&TFmini_Plus.Dist!=0)   //一定看看周围有无误判
       {
	      if(BlackEnd.BlackEndM<=28&&Boundary.EndLine>=8)  //&&q全内倾
	      {
		     Bar_count=0;
		     if (Boundary.StartLine>30)
		     {
			    for(Bar_row=Boundary.StartLine-1;Bar_row>Boundary.EndLine+2;Bar_row--)
			    {
				   if(Boundary.RightEdge[Bar_row+1]!=0 )
				   {
					  if(Boundary.RightEdge[Bar_row+1]-Boundary.RightEdge[Bar_row]<0||Boundary.RightEdge[Bar_row+1]- Boundary.RightEdge[Bar_row]>=2)  //右边界在左点附近是直线  待修改
					  {
						 Bar_count++;
					  }
				   }
				   else
				   {
					  Bar_count++;
				   }
				   if(Boundary.LeftEdge[Bar_row+1]!=0 )
				   {
					  if(Boundary.LeftEdge[Bar_row+1]-Boundary.LeftEdge[Bar_row]>0||Boundary.LeftEdge[Bar_row+1]- Boundary.LeftEdge[Bar_row]<=-2)  //右边界在左点附近是直线  待修改
					  {
						 Bar_count++;
					  }
				   }
				   else
				   {
					  Bar_count++;
				   }
			    }
			    if(Bar_count<3)   //排除弯道
			    {
				   RoadType=Barrier_Road;
				   Barrier.Barrier_Flag=1;
				   Bar_Process=Find_Bar;
			    }
		     }
		     // TFT_showuint16(40,2, Bar_count, BLUE,BLACK);
	      }
	      else if (BlackEnd.BlackEndM>=37&&Boundary.EndLine<=3) //&&全内倾
	      {
		     //RoadType=Ramp;  //坡道
	      }
       }
       else
       {
	      if(Break_Road.Break_Road_Flag==0)
	      {
		     if(TFmini_Plus.Dist>150||TFmini_Plus.Dist==0)
		     {
			    if( Break_Road.Break_Road_preFlag ==1)
			    {
				   // for(Bar_row=Boundary.StartLine-1;Bar_row>Boundary.EndLine+2;Bar_row--)
				   //{
				   if(BlackEnd.BlackEndL<10&&BlackEnd.BlackEndM<10&&BlackEnd.BlackEndR<10)
				   {
					  //如果中线截止行数小于10，左右起始行都存在
					  //		     if(BlackEnd.BlackEndM<25&&Boundary.R_StartLine>30&&Boundary.L_StartLine>30)
					  //		     {
					  RoadType=Break_RoadType;
					  Break_Road.Break_Road_Flag=1;
					  Break_Road.Break_Road_preFlag =0;
					  //Motor.SetSpeed=35;
					  //

					  if(BlackEnd.BlackEndM>28)
					  {
						 Break_Road.Break_Road_preFlag =0;

					  }
				   }
			    }
		     }
	      }

       }
}



/***************************************************************
障碍处理

* 函数名称： Barrier_Deal()//障碍处理
* 功能说明： 障碍处理内部调用
* 参数说明：
* 函数返回： void
* 修改时间：
* 备 注：
******************************************************************/
int  Bar_NUM=1;

float eeeeee=0;
float last_eeeeee=0;
float int_eeeeee=0;
void  Barrier_Process()
{
      switch(Bar_NUM)
       {
	    case 1:
	      {
		     BarType=Left_Run;
		     switch(BarType)
		     {
			  case Left_Run:
			    {
				   //gpio_set(PTA9,1);
				   switch(Bar_Process)
				   {
					case Find_Bar: //1
					  {
						 Barrier.Bar_Err=Bar_Left_para ;  //chu
						 //速度
						 if(distance_judge(1500))
						 {
							Bar_Process=In_Bar;
						 }
						 break;
					  }
					case In_Bar:
					  {
						 Barrier.Bar_Err=Bar_Right_para;
						 if(distance_judge(1200))
						 {
							Bar_Process=Out_Bar;
						 }
					  }

					  break;
					case Out_Bar:
					  //检测赛道
					  {

						 if(Boundary.L_StartLine>30 &&Boundary.R_StartLine>15)
						 {         //数据清除
							RoadType=else_road;
						 }

						 if(distance_judge(2000))
						 {
							Bar_NUM++;
							Bar_Process=No_Bar;
							Barrier.Barrier_Flag=0;;
						 }
						 break;
					  }
					default:
					  {break;}
				   }
				   break;
			    }
			  case Right_Run:
			    {
				   switch(Bar_Process)
				   {
					case Find_Bar:
					  {
						 Barrier.Bar_Err=-22;  //chu
						 //速度
						 if(distance_judge(1500))
						 {
							Bar_Process=In_Bar;
						 }
						 break;
					  }
					case In_Bar:
					  {
						 Barrier.Bar_Err=34;
						 if(distance_judge(1000))
						 {
							Bar_Process=Out_Bar;
						 }
					  }

					  break;
					case Out_Bar:
					  //检测赛道
					  {
						 if(Boundary.L_StartLine>15 &&Boundary.R_StartLine>30)
						 {         //数据清除

							RoadType=else_road;

						 }
						  if(distance_judge(2000))
						 {
							Bar_NUM++;
							Bar_Process=No_Bar;
							Barrier.Barrier_Flag=0;;
						 }
						 break;
					  }
					default:
					  {break;}
				   }
				   break;
			    }
			  case Back_Run:
			    {
				   switch(Bar_Process)
				   {
					case Find_Bar:
					  {
						 eeeeee=TFmini_Plus.Dist-110;

						 int_eeeeee+= eeeeee ;
						 eeeeee=0.1*eeeeee+0.001*int_eeeeee+0.3*(eeeeee-last_eeeeee);

						 //Motor.SetSpeed= eeeeee;

						 last_eeeeee=eeeeee;
						// Barrier.Bar_Err=15;  //chu
						 //速度
						 if(eeeeee<3)
						 {
							Bar_Process=In_Bar;
							//Motor.SetSpeed=50;
						 }
						 break;
					  }
					case In_Bar:
					  {
						 Barrier.Bar_Err=-22;  //chu
						 //速度
						 if(distance_judge(1500))
						 {
							Bar_Process=Out_Bar;
						 }
						break;
					  }
					case Out_Bar:
					  //检测赛道
					  {
						 Barrier.Bar_Err=34;
						 if(distance_judge(1000))
						 {
							//Bar_Process=Out_Bar;
							 Barrier.Barrier_Flag=0;
							Bar_Process=No_Bar;
							RoadType=else_road;
							Bar_NUM++;
						 }
						 if(Boundary.L_StartLine>15 &&Boundary.R_StartLine>30)
						 {         //数据清除
							Barrier.Barrier_Flag=0;
							Bar_Process=No_Bar;
							RoadType=else_road;
							Bar_NUM++;
						 }

						 break;
					  }
					default:
					  {break;}
				   }
				   break;
			    }
			  default:
			    {break;}

		     }
		     break;
	      }
#if 1

	  case 2:
	      {
		     BarType=Left_Run;
		     switch(BarType)
		     {
			  case Left_Run:
			    {
				   //gpio_set(PTA9,1);
				   switch(Bar_Process)
				   {
					case Find_Bar: //1
					  {
						 Barrier.Bar_Err=Bar_Left_para ;  //chu
						 //速度
						 if(distance_judge(1400))
						 {
							Bar_Process=In_Bar;
						 }
						 break;
					  }
					case In_Bar:
					  {
						 Barrier.Bar_Err=Bar_Right_para;
						 if(distance_judge(1000))
						 {
							Bar_Process=Out_Bar;
						 }
					  }

					  break;
					case Out_Bar:
					  //检测赛道
					  {
						 if(Boundary.L_StartLine>30 &&Boundary.R_StartLine>15)
						 {         //数据清除
//							Barrier.Barrier_Flag=0;
//							Bar_Process=No_Bar;
							RoadType=else_road;
							//Bar_NUM++;

						 }
						  if(distance_judge(2000))
						 {
							//Bar_NUM++;
							Bar_Process=No_Bar;
							Barrier.Barrier_Flag=0;;
						 }
						 break;
					  }
					default:
					  {break;}
				   }
				   break;
			    }
			  case Right_Run:
			    {
				   switch(Bar_Process)
				   {
					case Find_Bar:
					  {
						 Barrier.Bar_Err=-22;  //chu
						 //速度
						 if(distance_judge(1500))
						 {
							Bar_Process=In_Bar;
						 }
						 break;
					  }
					case In_Bar:
					  {
						 Barrier.Bar_Err=34;
						 if(distance_judge(1000))
						 {
							Bar_Process=Out_Bar;
						 }
					  }

					  break;
					case Out_Bar:
					  //检测赛道
					  {
						 if(Boundary.L_StartLine>15 &&Boundary.R_StartLine>30)
						 {         //数据清除
//							Barrier.Barrier_Flag=0;
//							Bar_Process=No_Bar;
							RoadType=else_road;
							//Bar_NUM++;
						 }
						  if(distance_judge(2000))
						 {
							//Bar_NUM++;
							Bar_Process=No_Bar;
							Barrier.Barrier_Flag=0;;
						 }
						 break;
					  }
					default:
					  {break;}
				   }
				   break;
			    }
			  case Back_Run:
			    {
				   switch(Bar_Process)
				   {
					case Find_Bar:
					  {
						 eeeeee=TFmini_Plus.Dist-110;

						 int_eeeeee+= eeeeee ;
						 eeeeee=0.1*eeeeee+0.001*int_eeeeee+0.3*(eeeeee-last_eeeeee);

						 //Motor.SetSpeed= eeeeee;

						 last_eeeeee=eeeeee;
						// Barrier.Bar_Err=15;  //chu
						 //速度
						 if(eeeeee<3)
						 {
							Bar_Process=In_Bar;
							//Motor.SetSpeed=50;
						 }
						 break;
					  }
					case In_Bar:
					  {
						 Barrier.Bar_Err=-22;  //chu
						 //速度
						 if(distance_judge(1500))
						 {
							Bar_Process=Out_Bar;
						 }
						break;
					  }
					case Out_Bar:
					  //检测赛道
					  {
						 Barrier.Bar_Err=34;
						 if(distance_judge(1000))
						 {
							//Bar_Process=Out_Bar;
							 Barrier.Barrier_Flag=0;
							Bar_Process=No_Bar;
							RoadType=else_road;
							Bar_NUM++;
						 }
						 if(Boundary.L_StartLine>15 &&Boundary.R_StartLine>30)
						 {         //数据清除
							Barrier.Barrier_Flag=0;
							Bar_Process=No_Bar;
							RoadType=else_road;
							//Bar_NUM++;
						 }

						 break;
					  }
					default:
					  {break;}
				   }
				   break;
			    }
			  default:
			    {break;}

		     }
		     break;
	      }
#endif
	    default:
	      {break;}
       }
}

