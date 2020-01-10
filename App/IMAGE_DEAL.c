/* @file       VCAN_OV7725_Eagle.h
 * @brief      鹰眼ov7725驱动代码
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-07
 */
#include "include.h"
#include "IMAGE_DEAL.h"



/*----------------------------------------------------------------------------------------------------------------
*  @brief  	图像减半函数  数据量减少3/4
*  @param  	*p 图像数组地址
*		*p1 转换图像地址
*		row图像行
*                  col图像列
*  @return	void
*  @since	v1.0
*  Sample usage:      halve_image(image_buff[0],image[0],ROW,COL);    //输出image[0]
//-----------------------------------------------------------------------------------------------------------------*/
void halve_image(unsigned char *p_in,unsigned char  *p_out,unsigned char row,unsigned char col) //图像减半
{
       int i, j;
       for (i = 0; i<row/2; i++)
       {
	      for (j = 0;j<col/2; j++)
	      {
		     *(p_out+i*col/2+j)=*(p_in+i*2*col+j*2);
	      }
       }

}




uint8 stop_car_flag;



/******************************************************************************
获取左中右线
******************************************************************************/
uint16 LeftBlack[ROW];
uint16 RightBlack[ROW];
uint16 BlackLineData[ROW];
uint8 LeftBlack_flag[ROW];
uint8 RightBlack_flag[ROW];

#if 0  //最古老搜线
uint8 stop_car_flag;
float center_line_err;


uint8 Line_White[ROW] ;

void find_L_R_M(void)
{
  int i=0,j=1  ;
  //int CountBlack = 0;
  int CountWhite = 0;

  unsigned char pLeft,pRight;//预变量
  uint8 jump_point=0;



  //中线扫描初始化
  for(i=ROW; i >0; i--)
  {
    LeftBlack_flag[i]=0;
    RightBlack_flag[i]=0;
  }


  for(i=ROW-1; i>ROW-2; i--)//前一行中线查找
  {
    pLeft=0,pRight=COL-1;
    for(j=1;j<COL-2;j++)
    {
      if(image_buff[i][j]!=image_buff[i][j+1])
      {
        jump_point++;
        if(image_buff[i][j]==Black&&image_buff[i][j+1]==White)
        {
          pLeft=j;
          LeftBlack_flag[i]=1;
        }
        else
        {
          pRight=j+1;
          RightBlack_flag[i]=1;
        }
      }
    }
    if(jump_point==1||jump_point==2)
      BlackLineData[i]=(pLeft+pRight)/2;
    else if(jump_point==0)
    {
      BlackLineData[i]=COL>>1;
    }
    else
    {
      //出界
    }
  }



  for(i=(ROW-2); i >0; i--) //寻找左右边线
  {
    CountWhite = 0;
    //指针初始化，从中间
    pLeft  = BlackLineData[i+1];//左边线指针
    pRight = BlackLineData[i+1];//右边线指针

    LeftBlack[i]  = pLeft; //初始化黑边
    RightBlack[i] = pRight;//初始化黑边

    for(j=COL/2;j>1||pLeft>1;j--) // 从中间向左边搜索，寻找黑点
    {
      if(image_buff[i][pLeft]==1)
      {
        CountWhite ++;
      }
      if(image_buff[i][pLeft]==Black&&image_buff[i][pLeft+1]==White)
      {
        //LeftBlack[i]=pLeft;
        break;
      }
      else
      {
        //LeftBlack[i]=1;  // 未找到左边黑点
      }

      pLeft--;

    }


    for(j=COL/2; j<COL-1||pRight<COL-1; j++)// 从中间向右边搜索，寻找黑点
    {
      if(image_buff[i][pRight]==1)
      {
        CountWhite ++;
      }
      if(image_buff[i][pRight-1]==White&&image_buff[i][pRight]==Black)
      {
        //RightBlack[i]=pRight;
        break;
      }
      else
      {
        //RightBlack[i]=COL-1;   //未找到右边黑点
      }

      pRight++;
    }
     Line_White[i]=CountWhite;
    //BlackLineData[i] = (RightBlack[i]+LeftBlack[i])/2;//正常两边黑线提取中心线
    if(image_buff[i-1][BlackLineData[i]]==Black)
      break;

  }//for结束--黑线提取结束

}//for结束-- 行扫描完




#endif

 int Mline_deal(uint8 line_start,uint8 line_end)
{
  int i;
  float sum=0.0;
  int center_err;
  for(i=line_start;i<line_end;i++)
    sum+=NS_CenterBlack[i];
  center_err=(int)sum/(line_end-line_start);
  return center_err;

  /*
  for(uint8 i=70;i>40;i--)
  {
    //NS_CenterBlack[i]=(NS_RightBlack[i]+NS_LeftBlack[i])/2;
    NS_CenterBlack_sum+=NS_CenterBlack[i];
  }
  NS_Deviation=NS_CenterBlack_sum/30;
  center_line_err=NS_Deviation;*/
}










/***************************************************************
                   八邻域搜索法
****************************************************************/
/***************************************************************
* 函数名称：void Neighborhood_SearchLeft(uint8 x,uint8 y,uint8 k)
* 功能说明：左八邻域跟踪循线
* 参数说明： x   起始点x坐标
            y   起始点y坐标
            k   循环开始位置 判断角度用
* 函数返回：无
* 修改时间：2019年1月12日
* 备 注：
***************************************************************/
uint8 NS_LeftBlack[ROW]={0};
uint8 NS_LeftBlack_r[ROW]={0};


uint8 NS_finishFlag=0;
uint8 Tracking_J=1;

void Neighborhood_SearchLeft(uint8 x,uint8 y,uint8 k)//八邻域搜索法
{
  uint8 i=x;
  uint8 j=y;

  switch(k)
  {
  case 0://1
    if(j==COL-1)
    {
      //停止位
      NS_LeftBlack[i-1]=COL-1;
      break;
    }
    else
    {
      if(j+1==COL-1)
      {//停止位
        NS_LeftBlack[i]=COL-1;break;
      }
      else if(image_buff[i][j+1]==0)
      {
        for(uint8 t=j+2;t<COL;t++)
        {
          if(image_buff[i][t]==1)
          {
            NS_LeftBlack[i]=t-1;
            j=NS_LeftBlack[i];
            break;
          }
          else if(t==COL-1)
          {
            NS_LeftBlack[i]=t-1;
            j=NS_LeftBlack[i]-1;
            break;
          }
        }
      }
    }
  case 1://2
    if(j==COL-1)
    {//停止位
      NS_LeftBlack[i-1]=COL-1;   break;
    }
    else
    {
      if(j+1==COL-1)
      {//停止位
        NS_LeftBlack[i-1]=COL-1;   break;
      }
      else if(image_buff[i-1][j+1]==0)
      {
        NS_LeftBlack[i-1]=j+1;     break;
      }
    }
  case 2://3
    if(j==1)
    {
      NS_LeftBlack[i-1]=1;       break;
    }
    else if(image_buff[i-1][j]==0)
    {
      NS_LeftBlack[i-1]=j;
      break;
    }


  case 3://4

    if((j-1)==1)
    {
      NS_LeftBlack[i-1]=1;       break;
    }
    else if(image_buff[i-1][j-1]==0)
    {
      NS_LeftBlack[i-1]=j-1;    break;
    }
  case 4://5
    if(image_buff[i][j-1]==0)
    {//不赋值
      for(uint8 t=j-2;t>0;t--)
      {
        if(t==1)
        {
          NS_LeftBlack[i-1]=1;
          break;
        }
        else
        {
          if(image_buff[i][t]==0&&image_buff[i-1][t]==0)
          {
            NS_LeftBlack[i-1]=t;    break;
          }
          else if(image_buff[i][t]==1)
          {
            Tracking_J=NS_LeftBlack_r[i]=j-1;
            break;
            //反向跟踪；
          }
        }
      }
      break;
    }
  case 5://6
    if(image_buff[i+1][j-1]==0)
    {
      //Neighborhood_Search(i+1,j-1,3);
    }
    //default:
    //break;
  }
}


void Left_Tracking()
{
  NS_LeftBlack[39]=1;
  for(uint8 i=39;i>1;i--)
  {
    Neighborhood_SearchLeft(i,NS_LeftBlack[i],0);
  }
}



/***************************************************************
* 函数名称：void Neighborhood_SearchRight(uint8 x,uint8 y,uint8 k)
* 功能说明：右八邻域跟踪循线
* 参数说明： x   起始点x坐标
            y   起始点y坐标
            k   循环开始位置 判断角度用
* 函数返回：无
* 修改时间：2019年1月12日
* 备 注：
***************************************************************/
uint8 NS_RightBlack[ROW]={0};
uint8 NS_RightBlack_r[ROW]={0};
void Neighborhood_SearchRight(uint8 x,uint8 y,uint8 k)//八邻域搜索法
{
  uint8 i=x;
  uint8 j=y;
  switch(k)
  {
  case 0://5
    if(j==1)
    {//停止位
      NS_RightBlack[i]=1;break;
    }
    else
    {
      if(j-1==1)
      {//停止位
        NS_RightBlack[i]=1;break;
      }
      else if(image_buff[i][j-1]==0)
      {
        for(uint8 t=j-2;t>0;t--)
        {
          if(image_buff[i][t]==1)
          {
            NS_RightBlack[i]=t+1;
            j=NS_RightBlack[i];
            break;
          }
          else if(t==1)
          {
            NS_RightBlack[i]=t+1;
            j=NS_RightBlack[i]-1;
            break;
          }
        }

      }
    }


  case 1://4
    if(j==1)
    {//停止位
       NS_RightBlack[i-1]=1;   break;
    }
    else
    {
      if(j-1==1)
      {//停止位
        NS_RightBlack[i-1]=1;   break;
      }
      else if(image_buff[i-1][j-1]==0)
      {
        NS_RightBlack[i-1]=j-1;
        break;
      }
    }


  case 2://3
    if(j==COL-1)//出边
    {
      NS_RightBlack[i-1]=COL-1;      break;
    }
    else if(image_buff[i-1][j]==0)
    {
      NS_RightBlack[i-1]=j;         break;
    }


  case 3://2
    if((j+1)==COL-1)
    {
      NS_RightBlack[i-1]=COL-1;         break;
    }
    else if(image_buff[i-1][j+1]==0)
    {
      NS_RightBlack[i-1]=j+1;       break;
    }


  case 4://1
    if(image_buff[i][j+1]==0)
    {//不赋值
      for(uint8 t=j+2;t<COL;t++)
      {
        if(t==COL-1)
        {
          NS_RightBlack[i-1]=COL-1;     break;
        }
        else
        {
          if(image_buff[i][t]==0&&image_buff[i-1][t]==0)
          {
            NS_RightBlack[i-1]=t;   break;
          }
          else if(image_buff[i][t]==1)
          {
            NS_RightBlack_r[i]=j-1;
            break;
            //反向跟踪；
          }
        }
      }
      break;
    }


  case 5://8
    if(image_buff[i+1][j+1]==0)
    {

    }

  }
}


void Right_Tracking()
{
  NS_RightBlack[39]=COL-1;
  for(uint8 i=39;i>1;i--)
  {
    Neighborhood_SearchRight(i,NS_RightBlack[i],0);
  }
}




/*************************************************************
提取中线主函数
**************************************************************/
uint8 NS_CenterBlack[ROW]={0};
float NS_Deviation=0;//偏差
int NS_ExtractCenterline()
{
  //int NS_CenterBlack_sum=0;
  for(uint8 i=99;i>0;i--)
  {
    NS_CenterBlack[i]=(NS_RightBlack[i]+NS_LeftBlack[i])/2;
    //NS_CenterBlack_sum+=NS_CenterBlack[i];
  }
//  if( RoadType==4)
//     CommonRectificate(&NS_CenterBlack[0],Crossingpoint_start,Crossingpoint_end);

/*
  for(uint8 i=70;i>40;i--)
  {
    //NS_CenterBlack[i]=(NS_RightBlack[i]+NS_LeftBlack[i])/2;
    NS_CenterBlack_sum+=NS_CenterBlack[i];
  }
  NS_Deviation=NS_CenterBlack_sum/30;
  center_line_err=NS_Deviation;*/
  center_line_err=Mline_deal(40,70);
  return 0;
}


/*************************************************************
数据初始化
**************************************************************/
void Data_reset()
{
  for(uint8 i=0;i<100;i++)
  {
    NS_CenterBlack[i]=0;
    NS_RightBlack[i]=COL-1;
    NS_LeftBlack[i]=1;
  }
}


#if 0
float kk=1;
/*************************************************************
图像处理主函数
**************************************************************/
uint8 Forane_WendingRow[ROW];
void image_deal1()
{
  Data_reset();
  binarization_processing1();
  //GetWhiteEdge_length();
  GetBlackEndParam();

  Left_Tracking();
  Right_Tracking();
  NS_ExtractCenterline();
  //GetHangData();
  mark_stop();
  if(BlackEndM>96)
  {
    int countk=0;
    for(int i=7;i<40;i++)
    {
      if(NS_RightBlack[i]-NS_LeftBlack[i]<50&&ABS(NS_CenterBlack[i]-NS_CenterBlack[i+1])<5)
      {
        Forane_WendingRow[countk]=i;countk++;
      }
    }
    if(countk>5)
    {
      int temCount = 0;
      int countMax=0;

      int endPos = 0;
      int temPos =  Forane_WendingRow[0]-1;
      int startPos = temPos;
      for(int j=0;j<countk-2;j++)
      {
        if(ABS(Forane_WendingRow[j+1]-Forane_WendingRow[j])<3)
        {
          temCount++;
          if (temCount > countMax)
          {
            countMax = temCount;
            startPos = temPos;
            endPos =  Forane_WendingRow[j]+1;
          }
        }
        else
        {
          temPos =  Forane_WendingRow[j]-1;
          temCount=0;
        }
      }
      center_line_err=Mline_deal(startPos,endPos);
    }
    NS_Speed=100;

    servPram =         2 ;    //舵机转角比例系数，调整方法：若舵机角度跟随慢增大该值，若舵机角度摆幅很大减小该值
    servD    =         0.5   ;
    if(BlackEndM>98)
    {NS_Speed=105;
    }
  }
  else if(BlackEndM<71)
  {
    NS_Speed=53;
    servPram =         5.5 ;    //舵机转角比例系数，调整方法：若舵机角度跟随慢增大该值，若舵机角度摆幅很大减小该值
    servD    =         3   ;
  }
  else
  {
    NS_Speed=62;
    servPram =         5.5 ;    //舵机转角比例系数，调整方法：若舵机角度跟随慢增大该值，若舵机角度摆幅很大减小该值
    servD    =         3   ;
  }
//  if( RoadType==4)
//  {
//    SCProcessing();
//  }

  if(stop_car_flag==1)//停车条件成立
  {
    NS_Speed=0;
  }
  /*if(paodao==0)
  {
    if(tanshe_flag==0)
    {
      if(NS_Speed>60)
        NS_Speed=60;
    }
  }*/
}

#endif

int sswr(float fdx)//四舍五入
{
  float T_fdx;
  int res;
  if(fdx>=0)
  {
    T_fdx=fdx-(int)(fdx);
    if((T_fdx-0.5)>=0)
    {res=(int)(fdx)+1;}
    else
      res=(int)(fdx);
  }
  else
  {
    fdx=-fdx;
    T_fdx=fdx-(int)(fdx);
    if((T_fdx-0.5)>=0)
    {res=(int)(fdx)+1;}
    else
      res=(int)(fdx);
    res=-res;
  }
  return res;
}


#if 0//最小二乘法搜边线
int zxexf(int x1,int x2,int x3,int x4,int x5)//最小二值化优化版本
{
  float Y,E;
  int Result;
  int y1=x1-x2,y2=x2-x3,y3=x3-x4,y4=x4-x5;//一阶导
  int e1=y1-y2,e2=y2-y3,e3=y3-y4;//二阶导
  E=(e1+e2+e3)/3.0;
  Y=((y1+y2+y3+y4)>>2)+E;
  Result=sswr(Y);//四舍五入
  return Result;
}



/*
第一行处理

*/

void Deal_TheFirst_Row()
{
  uint8 First_LeftBlack_flag=0;
  uint8 First_RightBlack_flag=0;
  uint16 j;

  if(!image_buff[Border_bottom][mid_point])//分别取中间，左边点和右边点，如果是黑点，代表不可以是起始点
  {
    if(!image_buff[Border_bottom][Border_left])//左边点是黑色
    {
      if(!image_buff[Border_bottom][Border_right])//右边点是黑色
      {
        //停车  冲出赛道
      }
      else//右边点是白色
      {
        for(j=mid_point;j<Border_right;j++)
        {
          if(image_buff[Border_bottom][j]!=image_buff[Border_bottom][j+1]&&image_buff[Border_bottom][j]==Black&&image_buff[Border_bottom][j+1]==White)
          {
            First_LeftBlack_flag=1;
            LeftBlack[Border_bottom]=j;break;
          }
        }
      }
    }
    else//左边点是bai色
    {
      for(j=mid_point;j>Border_left;j--)
      {
        if(image_buff[Border_bottom][j]!=image_buff[Border_bottom][j-1]&&image_buff[Border_bottom][j]==Black&&image_buff[Border_bottom][j-1]==White)
        {
          First_RightBlack_flag=1;
          RightBlack[Border_bottom]=j; break;
        }
      }
    }
  }
  else//中点是白色，可以直接搜索
  {
    for(j=mid_point;j>Border_left;j--)//从中间到左边搜索左边线
    {
      if(image_buff[Border_bottom][j]!=image_buff[Border_bottom][j-1]&&image_buff[Border_bottom][j-1]==Black&&image_buff[Border_bottom][j]==White)
      {
        First_LeftBlack_flag=1;
        LeftBlack[Border_bottom]=j;break;
      }
    }
    for(j=mid_point;j<Border_right;j++)//从中间到右边搜索右边线
    {
      if(image_buff[Border_bottom][j]!=image_buff[Border_bottom][j+1]&&image_buff[Border_bottom][j+1]==Black&&image_buff[Border_bottom][j]==White)
      {
        First_RightBlack_flag=1;
        RightBlack[Border_bottom]=j; break;
      }
    }
  }

  if(First_LeftBlack_flag)
    LeftBlack_flag[Border_bottom]=1;
  else
  {
    LeftBlack_flag[Border_bottom]=0;
    LeftBlack[Border_bottom]=1;
  }
  if(First_RightBlack_flag)
    RightBlack_flag[Border_bottom]=1;
  else
  {
    RightBlack_flag[Border_bottom]=0;
    RightBlack[Border_bottom]=Border_right;
  }
}


/*
其余行处理
*/

void Deal_ROW(uint8 row,uint16 lastR_Mpoint)
{
  uint8 row_LeftBlack_flag=0;
  uint8 row_RightBlack_flag=0;
  uint16 j;

  if(!image_buff[row][lastR_Mpoint])//判断本行起始点黑白  黑则终止
  {

  }
  else//本行中点是白色，可以直接搜索
  {
    for(j=lastR_Mpoint;j>Border_left;j--)//从中间到左边搜索左边线
    {
      if(image_buff[row][j]!=image_buff[row][j-1]&&image_buff[row][j-1]==Black&&image_buff[row][j]==White)
      {
        row_LeftBlack_flag=1;
        LeftBlack[row]=j;break;
      }
    }
    for(j=lastR_Mpoint;j<Border_right;j++)//从中间到右边搜索右边线
    {
      if(image_buff[row][j]!=image_buff[row][j+1]&&image_buff[row][j+1]==Black&&image_buff[row][j]==White)
      {
        row_RightBlack_flag=1;
        RightBlack[row]=j; break;
      }
    }
  }

  if(row_LeftBlack_flag)
    LeftBlack_flag[row]=1;
  else
  {
    LeftBlack_flag[row]=0;
    LeftBlack[row]=1;
  }
  if(row_RightBlack_flag)
    RightBlack_flag[row]=1;
  else
  {
    RightBlack_flag[row]=0;
    RightBlack[row]=Border_right;
  }
  BlackLineData[row]=(RightBlack[row]+LeftBlack[row])>>1;
}



void TrackingFollow_MidToEdge()
{
  Deal_TheFirst_Row();
  for(uint8 i=Border_bottom-1;i>Border_bottom-20;i--)
  {
    Deal_ROW(i,BlackLineData[i+1]);
  }
}

void yvce_sousuo()
{

}

void k()
{
  uint16 Pre_LeftBlack[ROW],Pre_RightBlack[ROW];
  TrackingFollow_MidToEdge();
  for(uint8 i=Border_bottom-20;i>Border_top;i--)
  {
    uint8 row_LeftBlack_flag=0;
  uint8 row_RightBlack_flag=0;
  //左预测找线
    Pre_LeftBlack[i]=LeftBlack[i+1]+zxexf(LeftBlack[i+1],LeftBlack[i+2],LeftBlack[i+3],LeftBlack[i+4],LeftBlack[i+5]);
    for(uint16 j=Pre_LeftBlack[i]+6;j>Pre_LeftBlack[i]-4&&j>Border_left;j--)
    {
      if(image_buff[i][j]!=image_buff[i][j-1]&&image_buff[i][j-1]==Black&&image_buff[i][j]==White)
      {
        LeftBlack[i]=j;
        row_LeftBlack_flag=1;break;
      }
    }
    if(row_LeftBlack_flag)
    {
      LeftBlack_flag[i]=1;
    }
    else
    {
      LeftBlack_flag[i]=0;
      LeftBlack[i]=Pre_LeftBlack[i];
    }

    //右预测找线
    Pre_RightBlack[i]=RightBlack[i+1]+zxexf(RightBlack[i+1],RightBlack[i+2],RightBlack[i+3],RightBlack[i+4],RightBlack[i+5]);
    for(uint16 j=Pre_RightBlack[i]-6;j<Pre_RightBlack[i]+4&&j<Border_right;j++)
    {
      if(image_buff[i][j]!=image_buff[i][j+1]&&image_buff[i][j+1]==Black&&image_buff[i][j]==White)
      {
        RightBlack[i]=j;
        row_RightBlack_flag=1;break;
      }
    }
    if(row_RightBlack_flag)
    {
      RightBlack_flag[i]=1;
    }
    else
    {
      RightBlack_flag[i]=0;
      RightBlack[i]=Pre_RightBlack[i];
    }
  }
}



#endif

