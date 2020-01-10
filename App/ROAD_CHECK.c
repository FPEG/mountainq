#include "include.h"
#include "ROAD_CHECK.h"

//#define Size2 30

//uint8_t Head2 = 0,Rear2 = 0;

unsigned char StandardRoadType = 1;
int StraightToBendCount = 0;
signed char LastRoadType = 0;
//signed char RoadType = -1;
int HistoryRoadType[4] = {0};
//signed char RoadTypeData2[Size2] = {0};
//uint8_t ElementCount2 = 0;
int g_Head = 0,g_Rear=0;

int AllStraightCount = 0;
int AllSmallSCount = 0;
int AllBigSCount = 0;
int AllBendCount = 0;


#if 1
//赛道检测


enum_RoadType  RoadType=else_road;
Str_RoadEN   REN={0};

uint8 search_row;
uint8 search_count;


 int  max_speed=100;
 int  nor_speed=70;
 int  rou_speed=60;
 int  Break_speed=45;
 int  Bar_speed=50;

void RTRecognition()
{
       if(Barrier.Barrier_Flag==1)
       {
	      REN.EN_Break_RoadType=0;
	      REN.EN_Cross=0;
       }
       else if(Round.Round_flag)
       {
	      REN.EN_Break_RoadType=0;
	      REN.EN_Cross=0;
       }
       else
       {
	      //RoadType= else_road;
	      REN.EN_Break_RoadType=1;
	      REN.EN_Cross=1;
       }



       switch(RoadType)
       {
	    case else_road:
	      {
		     if(Boundary.EndLine<6&&Boundary.MiddleLine[Boundary.EndLine+1]>30&&Boundary.MiddleLine[Boundary.EndLine+1]<50)
		     {
			    search_count=0;
			    search_row=0;
			    for(uint8 i=20;i>Boundary.EndLine;i--)
			    {
				   if(Boundary.MiddleLine[i]-Boundary.MiddleLine[i-1]<=3 )
				   {
					  search_count++;
				   }
				   else
				   {
					  search_row=i;break;
				   }
			    }
			    if(search_count>13)
			    {
				   BuzzerRing;
				   Camera_CenterError=Getrouteoffset(25,7,39);
				   //OLED_PrintFloat(64,5,Camera_CenterError);
				   centerErr=Camera_CenterError+2.8;
				   servPram =3.7;    //舵机转角比例系数，调整方法：若舵机角度跟随慢增大该值，若舵机角度摆幅很大减小该值
				   servD    =   3.1;
				   Motor.SetSpeed=max_speed;
			    }
			    else
			    {
				   centerErr=Camera_CenterError;

				   servPram =  3.7;    //舵机转角比例系数，调整方法：若舵机角度跟随慢增大该值，若舵机角度摆幅很大减小该值
				   servD    =   2;
				   Motor.SetSpeed=nor_speed;
				   BuzzerQuiet;
			    }
		     }
		       else if (Boundary.EndLine==1&&Boundary.MiddleLine[Boundary.EndLine+1]>30&&Boundary.MiddleLine[Boundary.EndLine+1]<50)
		     {
			    search_count=0;
			    search_row=0;
			    for(uint8 i=30;i>Boundary.EndLine;i--)
			    {
				   if(Boundary.MiddleLine[i]-Boundary.MiddleLine[i-1]<=2 )
				   {
					  search_count++;
				   }
				   else
				   {
					  search_row=i;break;
				   }
			    }
			    if(search_count>7)
			    {
				   BuzzerRing;
				   Camera_CenterError=Getrouteoffset(25,7,39);
				   //OLED_PrintFloat(64,5,Camera_CenterError);
				   centerErr=Camera_CenterError+2.8;
				   servPram =3.7;    //舵机转角比例系数，调整方法：若舵机角度跟随慢增大该值，若舵机角度摆幅很大减小该值
				   servD    =3.1;
				   Motor.SetSpeed=90;
			    }
			    else
			    {
				   centerErr=Camera_CenterError;

				   servPram =  3.7;    //舵机转角比例系数，调整方法：若舵机角度跟随慢增大该值，若舵机角度摆幅很大减小该值
				   servD    =   2;
				   Motor.SetSpeed=nor_speed;
				   BuzzerQuiet;
			    }
		     }
		     else
		     {
			    centerErr=Camera_CenterError;
			    servPram =        3.7;
			    servD    =        2;
			    //	      servPram =        3.0;          //dahuan
			    //	      servD    =        6.0;
			    Motor.SetSpeed=nor_speed;
		     }
		     if(TFmini_Plus.Dist<150&&TFmini_Plus.Dist!=0)
		     {
			    Motor.SetSpeed=60;
		     }

		     break;
	      }
	    case Round_Road:    //yuanhuan
	      {
		     centerErr=Camera_CenterError;
		     servPram =        3.7;
		     servD    =        2;
		     //	      servPram =        3.0;          //dahuan
		     //	      servD    =        6.0;
		     Motor.SetSpeed=rou_speed;
		     break;
	      }
	    case Break_RoadType:  //断路
	      {
		     centerErr=ELEC_err/2.2;
		     servPram =        3.7;
		     servD    =        2;
		     Motor.SetSpeed=Break_speed;
		     break;
	      }
	    case Barrier_Road:    //路障
	      {
		     centerErr=Barrier.Bar_Err;
		     servPram =        3.7;
		     servD    =        2;
		     Motor.SetSpeed=Bar_speed;
		     break;
	      }

	    default:
	      {
		     centerErr=Camera_CenterError;
		     Motor.SetSpeed=50;
		     break;
	      }
       }
}
#endif