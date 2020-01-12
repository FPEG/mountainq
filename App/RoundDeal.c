
#include "common.h"
#include "include.h"

#include "ROAD_CHECK.h"
#include "findtrack.h"
#include "RoundDeal.h"
#include "TrackType.h"

Str_Round  Round = { 0 };
Str_Boundary Boundary = {0};
RoundTypeDef RoundType;
RoundSizeTypeDef RoundSize;
RoundProcessTypeDef RoundProcess;


Str_Inflection_point    Inflection_point;   //拐点结构体
//局部变量，紧用于本文件

unsigned char  Round_row = 0;//行临时变量i，初始化为进环点的行？？
unsigned char  Round_col = 0;//列临时变量j
int  Round_num = 0;//前后左（右）线列差
unsigned char  LeftLoopTemp_R = 0;//上一次拐点前面黑白跳变点白row//str->EndLine
unsigned char LeftLoopTemp_C = 0;//上一次拐点右边黑白跳变点白col
unsigned char Inflection_Temp_x;
unsigned char Inflection_Temp_y;
float  LeftLoopSlope;
float  RightLoopSlope;

void Round_Process(Str_Boundary* str, Str_Round* round);
void Round_Detection(Str_Boundary* str, Str_Round* round);

void Round_Deal()
{
	if (Round.Round_flag == 0)
	{
		Round_Detection(&Boundary, &Round);
	}
	else
	{
		Round_Process(&Boundary, &Round);
	}
}
void Round_Process(Str_Boundary* str, Str_Round* round)
{
}
/**
 * \brief 圆环检测，判断
 * \param[in] str 边界结构体
 * \param round
 * \par
 * 检测一直开着
 * 圆环检测\n
 * 1、判断一边有下拐点，一边没有下拐点，且拐点位置离结束行不不少于5行\n
 * 2、拐点侧丢线\n
 * 3、非拐点侧直线
 */
void Round_Detection(Str_Boundary* str, Str_Round* round) //圆环
{
	//左环
	//检测到左下拐点，没检测到右下拐点，结束行比拐点远很多
	if (Inflection_point.L_down_point.x != 0 &&//检测到左下拐点
		Inflection_point.R_down_point.x == 0 &&//没检测到右下拐点
		str->EndLine < Inflection_point.L_down_point.x - 5//结束行比拐点远很多(5)
		)
	{
		//左下拐点行的上面三行左边丢线
		if (str->LeftEdge[Inflection_point.L_down_point.x - 1] == 0 &&
			str->LeftEdge[Inflection_point.L_down_point.x - 2] == 0 &&
			str->LeftEdge[Inflection_point.L_down_point.x - 3] == 0)
		{
			round->Entrance_count = 0;//圆环入口计数清零
			/*
			 * 初始化临时变量Round_row为左拐点下面5行
			 * Round_row=MAX(Inflection_point.L_down_point.x + 5,(38)ROW_END - 1)
			 */
			if (
				Inflection_point.L_down_point.x + 5 > //左拐点下面5行
				ROW_END - 1
				)
				Round_row = ROW_END - 1;
			else
				Round_row = Inflection_point.L_down_point.x + 5;
			/*
			 * //从左拐点下面5行到上面10行进行搜索（共15行，满足13行为圆环标记）
			 * 获取圆环入口计数++
			 * 条件：如果右边正常递减，且右边不丢线
			 */
			for (;
				(
					Round_row > //左拐点下面5行
					Inflection_point.L_down_point.x - 10 //左拐点上面10行
					) &&

				Round_row >= ROW_START;
				Round_row--)
			{
				/*
				 * 如果右边不丢线
				 */
				if (str->RightEdge[Round_row + 1] != 0 &&
					str->RightEdge[Round_row] != 0)
				{
					/*
					 * //如果右边正常递减(，且不丢线)     斜率符合要求
					 * 后-前=[0,2]
					 */
					if (str->RightEdge[Round_row + 1] - str->RightEdge[Round_row] >= 0 &&
						str->RightEdge[Round_row + 1] - str->RightEdge[Round_row] <= 2)
					{
						round->Entrance_count++; //圆环入口计数增加
					}
				}
			}
			/*
			 * //min修改，其中有13行满足条件即可判定为圆环
			 */
			if (round->Entrance_count >= 13)
			{
				round->Round_flag = 1;
				RoundType = Left_Round;
				RoundProcess = Find_Round;
				RoadType = Round_Road;
				//RoundSize=Large_Round;   //待定
			}
		}
	}

	//右环
	//检测到右下拐点，没检测到左下拐点，结束行比拐点远很多
	else if (Inflection_point.R_down_point.x != 0 && Inflection_point.L_down_point.x == 0 && str->EndLine < Inflection_point.R_down_point.x - 5)
	{
		//右下拐点行的上面三行左边丢线
		if (str->RightEdge[Inflection_point.R_down_point.x - 1] == 0 && str->RightEdge[Inflection_point.R_down_point.x - 2] == 0 && str->RightEdge[Inflection_point.R_down_point.x - 3] == 0)
		{
			round->Entrance_count = 0;//圆环入口计数清零
			if (Inflection_point.R_down_point.x + 5 > ROW_END - 1)
				Round_row = ROW_END - 1;
			else
				Round_row = Inflection_point.R_down_point.x + 5;
			for (; Round_row > Inflection_point.R_down_point.x - 10 && Round_row >= ROW_START; Round_row--)//从右拐点下面5行到上面10行进行搜索
			{
				if (str->LeftEdge[Round_row + 1] != 0 && str->LeftEdge[Round_row] != 0)
				{
					//如果左边正常递增，且不丢线
					if (str->LeftEdge[Round_row] - str->LeftEdge[Round_row + 1] >= 0 && str->LeftEdge[Round_row] - str->LeftEdge[Round_row + 1] <= 2)
					{
						round->Entrance_count++; //圆环入口计数增加
					}
				}
			}
			if (round->Entrance_count >= 13)//min修改，其中有13行满足条件即可判定为圆环
			{
				
				round->Round_flag = 1;
				RoundType = Right_Round;

				RoadType = Round_Road;
				//RoundSize=Large_Round;
				RoundProcess = Find_Round;
			}
		}
	}

}

