#include "include.h"
#include "Cross_Deal.h"


#define   Cross_Hang  7

/*ʮ��*/
//unsigned char L_up_cross_flag=0;         //ʮ���м�ֻ����ʮ�ֹս�
//unsigned char R_up_cross_flag=0;
//
//unsigned char L_cross_flag=0;              //������жϽ���ʮ�ֱ�־��  0
//unsigned char R_cross_flag=0;              //�ұ����жϽ���ʮ�ֱ�־��  0
//unsigned char Cross_flag=0;                //�жϽ���ʮ�ֱ�־��  0


unsigned char  Cross_row;       //
unsigned char  Cross_col;
unsigned char  Cross_num;
Str_Cross   Cross;
Enum_CrossType   CrossType;
Enum_CrossProcess  CrossProcess;

float  LeftCrossSlope;
float  RightCrossSlope;

void Cross_Detection();//ʮ�ּ��
void Cross_Process();//ʮ�ּ��
void Cross_Judge();//ʮ���ж�
/***************************************************************
ʮ�ִ���

* �������ƣ� Cross_Detection()//ʮ�ִ���
* ����˵���� ʮ�ִ����ⲿ����
* ����˵����
* �������أ� void
* �޸�ʱ�䣺
* �� ע��
*******************************************************************/
void Cross_Deal()//ʮ�ּ��
{
       if(REN.EN_Cross==1)
       {
	      if(!Cross.Cross_flag)
	      {
		     for(Cross_row=ROW_END;Cross_row>=ROW_START;Cross_row--)
		     {
			    Cross.Track_Width_forCross[Cross_row]=(int)(4.0+Cross_row*1.85); //����  Բ��·��
		     }
		     Cross_Detection();        //ʮ�ּ��
	      }
	      else
	      {
		    // Cross_Judge();//ʮ���ж�//����Ҫ��
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
ʮ�ּ��

* �������ƣ� Cross_Detection()//ʮ�ִ���
* ����˵���� ʮ�ּ��
* ����˵����
* �������أ� void
* �޸�ʱ�䣺
* �� ע��     ���һֱ����  ʮ�ּ��
 *******************************************************************/
void Cross_Detection()//ʮ�ּ��
{
       if(Inflection_point.L_down_point.x>0||Inflection_point.R_down_point.x>0)
       {
	      Cross_num=0;            //�������ֵ    �հ��������Լ���
	      for(Cross_row=Boundary.StartLine; Cross_row>ROW_START ; Cross_row-- )     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
	      {
		     if(Boundary.RowLose[Cross_row]==4)
		     {
			    for(Cross_col=Cross_row; Cross_col>ROW_START ; Cross_col-- )     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
			    {
				   if(Boundary.RowLose[Cross_col]==4)
				   {
					  Cross_num++;
				   }
				   else
				   {
					  if( Cross_num>=Cross_Hang)
					  {
						 Cross.Cross_flag=1;//ʮ��flag
						 CrossProcess=Find_Cross;
						 Cross_row=ROW_START;  //����ѭ��
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
ʮ���ж�

* �������ƣ� void Cross_Judge()//ʮ���ж���
* ����˵���� ʮ���ж�
* ����˵����
* �������أ� void
* �޸�ʱ�䣺
* �� ע��
 *******************************************************************/
void Cross_Judge()//ʮ���ж�
{
       //CrossType=Cro_else;

       //ʮ�ֽ���������
       if(Inflection_point.L_down_point.x>0||Inflection_point.R_down_point.x>0)
       {
	      if(Inflection_point.L_down_point.x>0&&Inflection_point.R_down_point.x==0)
	      {
		     if(Inflection_point.L_down_point.y>12)
		     {
			    if(Boundary.R_StartLine<20)
			    {
				   CrossType=Cro_RightLean;//ʮ������
			    }
		     }
	      }
	      else if(Inflection_point.L_down_point.x==0&&Inflection_point.R_down_point.x>0)
	      {
		     if(Inflection_point.R_down_point.y<68)
		     {
			    if(Boundary.L_StartLine<20)
			    {
				   CrossType=Cro_LeftLean; //ʮ������
			    }
		     }
	      }
	      else if(Inflection_point.L_down_point.x>0&&Inflection_point.R_down_point.x>0)
	      {
		     if(Inflection_point.L_up_point.x>0&&Inflection_point.R_up_point.x>0)
		     {
			    CrossType=Cro_ZHENG; //��ʮ��;
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
ʮ�ִ���

* �������ƣ�Cross_Process()//ʮ�ִ���
* ����˵���� ʮ�ִ���
* ����˵����
* �������أ� void
* �޸�ʱ�䣺
* �� ע��
*******************************************************************/
void Cross_Process()//ʮ�ִ���
{
       switch(CrossProcess)
       {
	    case Find_Cross:
	      {
		     //�жϽ�����һ�׶�
		      if(Boundary.StartLine<28)
		      {
			  CrossProcess=In_Cross;
		      }
		       Cross_num=0;            //�������ֵ    �հ��������Լ���
		      for(Cross_row=Boundary.StartLine; Cross_row>ROW_START ; Cross_row-- )     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
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
		     if(Inflection_point.L_down_point.x>0&&Inflection_point.R_down_point.x>0)  //��ʼ����    ���油��
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
				   if(Boundary.L_StartLine>Inflection_point.L_down_point.x+SC_MUN) //����10��
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


		     else if(Inflection_point.L_down_point.x>0&&Inflection_point.R_down_point.x==0)  //��ʼ����    ���油��
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
				   if(Boundary.L_StartLine>Inflection_point.L_down_point.x+SC_MUN) //����10��
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


		     if(Inflection_point.L_down_point.x==0&&Inflection_point.R_down_point.x>0)  //��ʼ����    ���油��
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
		     Cross.Cross_flag=0;//ʮ��flag
		     break;
	      }
	    default:
	      {
		     break;
	      }
       }

}
