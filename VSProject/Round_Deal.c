#include "include.h"
#include "Round_Deal.h"


//#define



Str_Round  Round = { 0 };
RoundTypeDef RoundType;
RoundSizeTypeDef RoundSize;
RoundProcessTypeDef RoundProcess;

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

/***************************************************************
圆环处理

* 函数名称：Round_Deal()
* 功能说明： 圆环处理
* 参数说明：
* 函数返回：void
* 修改时间：
* 备 注：
**************************************************************/
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
	//TFT_showuint16(42,2, Round.Round_flag, RED,BLACK)  ;
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

/**
 * \brief 圆环处理
 * \param str 
 * \param round
 * 
 */
void Round_Process(Str_Boundary* str, Str_Round* round)
{
	//  LCD_P6x8Str(0,7,"Rou_Pro");
	//  LCD_Show_Number3(80,7,RoundProcess);
	//  LCD_Show_Number3(80,2,RoundType);
	//  LCD_Show_Number3(80,3,RoundSize);
	//       TFT_showuint16(42,1,round->Round_flag, RED,BLACK);
	//       TFT_showuint16(42,2,RoundProcess, RED,BLACK);
	//
	//       TFT_showuint16(42,4,RoundType, RED,BLACK);
	switch (RoundType)
	{
	case Left_Round:  //左环
	{
		switch (RoundProcess)
		{
		case Find_Round://发现圆环
		{
			//*进入下个阶段的判定*/
			if (str->L_StartLine < 30)   //da待改进
			{
				Round_num = 0;
				/*
				 * //防止误判     检测换中心岛
				 * 等待Find_Gap标记
				 * 从左线开始点向前搜4格，累加左线列差，判断累加和的大小
				 */
				for (
					Round_row = str->L_StartLine; 
					Round_row >= str->L_StartLine - 4 && 
					Round_row >= ROW_START + 1; 
					Round_row--)   
				{
					/*
					 * 左边没丢线
					 */
					if (str->LeftEdge[Round_row] != 0)
					{
						Round_num += 
							str->LeftEdge[Round_row] - str->LeftEdge[Round_row + 1];//前后左线列差
					}
					/*
					 * 丢线
					 */
					else
					{
						Round_num = 0;
					}
				}
				//LCD_Show_Number3(0,4,Round_num);
				
				if (Round_num > 7)
				{
					Round_num = 0;
					RoundProcess = Find_Gap;//就认为有缺口//发现圆环中心
				}
			}
			
			/*
			 * //*入环的补线
			 * 从后搜到前
			 */
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				/*
				 * 右线没丢线
				 */
				if (str->RightEdge[Round_row])
				{
					/*
					 * //左边界等于右边界减半宽
					 */
					str->LeftEdge[Round_row] = 
						str->RightEdge[Round_row] - //右边界
						round->Track_Width_forRound[Round_row];//半宽
				}
				/*
				 * 右线丢线
				 */
				else
				{
					str->LeftEdge[Round_row] = 0;
				}
			}
			break;
		}
		case Find_Gap://发现圆环中心
		{
			/*下一阶段的判定*/
			//底部三行左边界都不丢线
			if (str->LeftEdge[ROW_END - 3] &&
				str->LeftEdge[ROW_END - 1] &&
				str->LeftEdge[ROW_END - 2])
			{
				RoundProcess = Into_Round;
			}
			/*
			 * //左补线
			 * 效果同上
			 */
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				if (str->RightEdge[Round_row])
				{
					str->LeftEdge[Round_row] = str->RightEdge[Round_row] - round->Track_Width_forRound[Round_row];//左边界等于右边界减半宽
				}
				else
				{
					str->LeftEdge[Round_row] = 0;
				}
			}
			break;
		}
		case Into_Round://进环    关键 分两步
		{
			/* //进环的补线处理
			 * 底部两行左边界都不丢线
			 */
			if (str->LeftEdge[ROW_END - 1] &&
				str->LeftEdge[ROW_END - 2])
				//&&str->LeftEdge[ROW-4])
			{
				//获取补线点信息
				if (Inflection_point.L_down_point.x > 5) //记录上一次拐点存在的信息
				{
					Inflection_Temp_x = Inflection_point.L_down_point.x;
					Inflection_Temp_y = Inflection_point.L_down_point.y;
				}

				/*
				 * //得到信息后，开始补线
				 * i=(上一次拐点x,5]向前遍历
				 * 找中止行
				 */
				for (uint8 i = Inflection_Temp_x; i > 5; i--)
				{
					/*
					 * 此行和前行对比
					 */
					if (image[i][Inflection_Temp_y] == White &&
						image[i - 1][Inflection_Temp_y] == Black)
					{
						LeftLoopTemp_R = i - 1;
						str->EndLine = i - 1;
						break;
					}
				}
				/*
				 * i=[上一次拐点y,70)
				 * 找黑白跳变点白col
				 */
				for (uint8 i = Inflection_Temp_y; i < 70; i++)
				{
					if (image[LeftLoopTemp_R - 1][i + 1] == White &&
						image[LeftLoopTemp_R - 1][i] == Black)
					{
						LeftLoopTemp_C = i;
						break;
					}
				}
				/*进环斜率*/
				LeftLoopSlope = 1.6; //*(str->RightEdge[aa1]-aa2)/(aa1-LeftLoopTemp_R);

				for (uint8 i = 0; i <= 35; i++)
				{
					/*
					 * str->RightEdge[LeftLoopTemp_R + i]：当前补线位置
					 */
					str->RightEdge[LeftLoopTemp_R + i] = (int)(i * LeftLoopSlope + LeftLoopTemp_C);
					/*
					 * 限幅
					 */
					str->RightEdge[LeftLoopTemp_R + i] = int16_range_protect(str->RightEdge[LeftLoopTemp_R + i], 1, 79);

					/*
					 * 补线限幅？
					 */
					if (
						str->RightEdge[LeftLoopTemp_R + i] > 78 ||
						LeftLoopTemp_R + i > ROW_END
					)
						break;
				}
			}
			/*
			 * 底部两行左边界都丢线
			 */
			else
			{
				RoundProcess = Into_Round2;
			}

			/*
			 * 防止补线比右线还右
			 * 扫描i=[Endline,39),j=[1,75]
			 * 
			 */
			for (uint8 i = str->EndLine; i < ROW_END; i++)
			{
				//白白右黑
				for (uint8 j = 1; j <= 75; j++)
				{
					if (image[i][j] == White &&
						image[i][j + 1] == Black)
					{
						if (str->RightEdge[i] > j + 1)
							str->RightEdge[i] = j + 1;
						break;
					}
				}
			}
			break;
		}
		case Into_Round2://进环    关键 4
		{
			/*判断是否进入环岛*/
			if (distance_judge(1800))//&&str->L_StartLine==0&& RoundSize==Small_Round)      //定距离一段
			{
				RoundProcess = In_Round;
				Inflection_Temp_x = 0;
				Inflection_Temp_y = 0;
			}

#if 1                                 //补线
			/*
			 * 防止补线比右线还右
			 * 扫描i=[Endline,39),j=[1,75]
			 *
			 */
			for (uint8 i = str->EndLine; i <= ROW_END; i++)
			{
				for (uint8 j = 1; j <= 75; j++)
				{
					if (image[i][j] == White &&
						image[i][j + 1] == Black &&
						image[i][j + 2] == Black &&
						image[i][j + 3] == Black)
					{
						if (str->RightEdge[i] > j + 1)
							str->RightEdge[i] = j + 1;
						break;
					}
				}
			}


			/*进环斜率*/
			LeftLoopSlope = 2.0;
			for (uint8 i = ROW_END; i > ROW_START; i--)
			{
				if (str->RightEdge[i] != 0)
					str->RightEdge[i] = str->RightEdge[i] < 
					(int)(LeftLoopSlope * i + 5) ? str->RightEdge[i] :
				(int)(LeftLoopSlope * i + 5);
				else
					str->RightEdge[i] = (int)(LeftLoopSlope * i + 5);
				if (str->RightEdge[i] > 79)
					str->RightEdge[i] = 0;
			}
			for (uint8 i = 39; i > str->EndLine && i > ROW_START + 2; i--)
			{
				if (image[i][10] == White &&
					image[i - 1][10] == Black &&
					image[i - 2][10] == Black)
				{
					for (uint8 j = i; 
						j > str->EndLine &&
						j > ROW_START; 
						j--)
					{
						str->LeftEdge[j] = 0;
					}
					str->EndLine = i - 1;        //改变endline
					break;
				}
				if (str->LeftEdge[i] > 30)
				{
					str->LeftEdge[i] = 0;   //限幅
				}
			}
#endif
			break;
		}
		case In_Round://在环内    5
		{
			//记录上一次拐点存在的信息
			if (Inflection_point.R_down_point.x &&
				image[Inflection_point.R_down_point.x][Inflection_point.R_down_point.y - 2] == White &&
				image[Inflection_point.R_down_point.x][Inflection_point.R_down_point.y - 3] == White &&
				image[Inflection_point.R_down_point.x - 1][Inflection_point.R_down_point.y] == White &&
				image[Inflection_point.R_down_point.x - 2][Inflection_point.R_down_point.y] == White)
			{
				Inflection_Temp_x = Inflection_point.R_down_point.x;
				Inflection_Temp_y = Inflection_point.R_down_point.y;
				//判断下一阶段
				if (Inflection_point.R_down_point.x > 5)
				{
					uint8 hang_num = 0;            //清除白块计数值                 8
					for (
						Round_row = Inflection_point.R_down_point.x - 1;
						Round_row >= Inflection_point.R_down_point.x - 6 &&
						Round_row > ROW_START;
						Round_row--)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
					{
						if (str->RowLose[Round_row] == 4)
						{
							hang_num++;
						}
					}
					if (
						hang_num > 1 ||
						Inflection_point.R_down_point.x > 25 ||
						Inflection_point.R_down_point.y > 50
						)
					{
						RoundProcess = Out_Round;
					}
				}
			}


			if (Inflection_Temp_x > 0)
			{
				//补线
				LeftLoopSlope = 4.5;

				for (uint8 i = Inflection_Temp_x + 1; i > ROW_START; i--)
				{
					str->RightEdge[i] = (int)((i - Inflection_Temp_x) * LeftLoopSlope + Inflection_Temp_y);    //fuhao
					if (str->RightEdge[i] < COL_START)
					{
						str->EndLine = i;
						break;
					}
				}
				//str->StartLine=ROW_END;
				//memset(str->LeftEdge, 0,sizeof(str->LeftEdge) );   //z左线清空
			}
			for (uint8 i = 39; i > str->EndLine; i--)    //消除左线误搜索
			{
				if (str->LeftEdge[i] > 30)
				{
					str->LeftEdge[i] = 0;
				}
			}

			break;
		}
		case Out_Round://出环   6
		{
			LeftLoopSlope = 1.7;
			for (uint8 i = 39; i > 5; i--)
			{
				if (str->RightEdge[i] != 0)
					str->RightEdge[i] = str->RightEdge[i] < (unsigned char)(LeftLoopSlope * i + 7) ? str->RightEdge[i] : (unsigned char)(LeftLoopSlope * i + 7);
				else
					str->RightEdge[i] = (unsigned char)(LeftLoopSlope * i + 7);
				if (str->RightEdge[i] > 79)
					str->RightEdge[i] = 0;
			}
			//				   str->StartLine=ROW_END;
			//				   memset(str->LeftEdge, 0,sizeof(str->LeftEdge) );

			if (distance_judge(1200))   //下一阶段标志
			{
				RoundProcess = Leave_Round;
			}
			for (uint8 i = 39; i > str->EndLine; i--)    //消除左线误搜索
			{
				if (str->LeftEdge[i] > 40)
				{
					str->LeftEdge[i] = 0;
				}
				if (i<39 && i>ROW_START)
				{
					if (str->LeftEdge[i] != 0 && str->LeftEdge[i - 1] == 0 && str->LeftEdge[i + 1] == 0)
						str->LeftEdge[i] = 0;
				}
			}
			/*下一阶段的判定*/

			break;
		}
		case Leave_Round://离开圆环
		{
			Round_num = 0;
			for (
				Round_row = str->R_StartLine;
				Round_row > str->EndLine + 1 &&
				Round_row > ROW_START + 10;
				Round_row--)
			{
				if (str->RightEdge[Round_row] == 0)
				{
					Round_num++;
				}
			}
			if (Round_num < 3 &&
				(
					(str->R_StartLine - str->EndLine) > 32 ||
					str->RightEdge[str->EndLine] > 15)
				)  //30需要调  huo 判断斜率
			{
				RoundProcess = Leave_Round2;//暂定
			}
			else
			{
				LeftLoopSlope = 1.7;
				for (uint8 i = 39; i > 5; i--)
				{
					if (str->RightEdge[i] != 0)
						str->RightEdge[i] = str->RightEdge[i] < (unsigned char)(LeftLoopSlope * i + 5) ? str->RightEdge[i] : (unsigned char)(LeftLoopSlope * i + 5);
					else
						str->RightEdge[i] = (unsigned char)(LeftLoopSlope * i + 5);
					if (str->RightEdge[i] > 79)
						str->RightEdge[i] = 0;
				}

				for (uint8 i = 39; i > str->EndLine; i--)    //消除左线误搜索
				{
					if (str->LeftEdge[i] > 40)
					{
						str->LeftEdge[i] = 0;
					}
					if (i<39 && i>ROW_START)
					{
						if (str->LeftEdge[i] != 0 && str->LeftEdge[i - 1] == 0 && str->LeftEdge[i + 1] == 0)
							str->LeftEdge[i] = 0;
					}
				}
			}
			break;
		}
		case Leave_Round2://离开圆环 7
		{
			//可以拉长视野
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				if (str->RightEdge[Round_row])
				{
					str->LeftEdge[Round_row] = str->RightEdge[Round_row] - round->Track_Width_forRound[Round_row];//左边界等于右边界减半宽
				}
				else
				{
					str->LeftEdge[Round_row] = 0;
				}
			}
			if (distance_judge(3000))
			{
				RoundProcess = Round_Not_Found;//暂定
				round->Round_flag = 0;
				RoadType = else_road;
			}
			break;
		}
		default:
		{
			break;
		}
		}
		break;
	}
	//右环
	case Right_Round:
	{
		switch (RoundProcess)
		{
		case Find_Round://发现圆环
		{
			/*进入下个阶段的判定*/
			//如果左右起始行之差大于5，缺口计数为0，左边界底部四行丢线//保证右边界的斜率在一定范围，以防斜入圆环误判，参数待定
			if (str->R_StartLine < 30)   //da待改进
			{
				Round_num = 0;
				for (Round_row = str->R_StartLine; Round_row >= str->R_StartLine - 4 && Round_row >= ROW_START + 1; Round_row--)   //防止误判     检测换中心岛
				{
					if (str->RightEdge[Round_row] != 0 && str->RightEdge[Round_row - 1] != 0)
					{
						Round_num += str->RightEdge[Round_row] - str->RightEdge[Round_row - 1];
					}
					else
					{
						Round_num = 0;
					}
				}
				if (Round_num > 7)
				{
					Round_num = 0;
					RoundProcess = Find_Gap;//就认为有缺口
				}
			}


			/*入环的补线*/
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				if (str->LeftEdge[Round_row])
				{
					str->RightEdge[Round_row] = str->LeftEdge[Round_row] + round->Track_Width_forRound[Round_row];//左边界等于右边界减半宽
				}
				else
				{
					str->RightEdge[Round_row] = 0;
				}
			}
			break;
		}
		case Find_Gap://发现圆环中心
		{
			/*下一阶段的判定*/
			//底部三行左边界都不丢线
			if (str->RightEdge[ROW_END] && str->RightEdge[ROW_END - 1] && str->RightEdge[ROW_END - 2])//&&str->LeftEdge[ROW-4])
			{
				RoundProcess = Into_Round;
			}
			/*右补线*/
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				if (str->LeftEdge[Round_row])
				{
					str->RightEdge[Round_row] = str->LeftEdge[Round_row] + round->Track_Width_forRound[Round_row];//左边界等于右边界减半宽
				}
				else
				{
					str->RightEdge[Round_row] = 0;
				}
			}
			break;
		}
		case Into_Round://进环  3
		{
			/*进环的补线处理*/
			//获取补线点信息
			if (str->RightEdge[39] && str->RightEdge[38])//&&str->LeftEdge[ROW-4])
			{
				if (Inflection_point.R_down_point.x > 5) //记录上一次拐点存在的信息
				{
					Inflection_Temp_x = Inflection_point.R_down_point.x;
					Inflection_Temp_y = Inflection_point.R_down_point.y;
				}
				//得到信息后，开始补线
				for (uint8 i = Inflection_Temp_x; i > 5; i--)//拐点向上搜索白黑跳变，记录
				{
					if (image[i][Inflection_Temp_y] == White && image[i - 1][Inflection_Temp_y] == Black && image[i - 2][Inflection_Temp_y] == Black)
					{
						LeftLoopTemp_R = i - 1;
						str->EndLine = i - 1;
						break;
					}
				}
				for (uint8 i = Inflection_Temp_y; i > 10; i--)//拐点向左搜索黑白跳变
				{
					if (image[LeftLoopTemp_R - 1][i - 2] == White && image[LeftLoopTemp_R - 1][i - 1] == White && image[LeftLoopTemp_R - 1][i] == Black)
					{
						LeftLoopTemp_C = i;
						break;
					}
				}
				/*进环斜率*/
				LeftLoopSlope = -1.8;

				for (uint8 i = 0; i <= 35; i++)
				{
					str->LeftEdge[LeftLoopTemp_R + i] = (int)(LeftLoopTemp_C + i * LeftLoopSlope);
					str->LeftEdge[LeftLoopTemp_R + i] = int16_range_protect(str->LeftEdge[LeftLoopTemp_R + i], 1, 79);
					if (str->LeftEdge[LeftLoopTemp_R + i]<2 || LeftLoopTemp_R + i>ROW_END)
						break;
				}
			}
			else
			{
				RoundProcess = Into_Round2;
			}

			for (uint8 i = str->EndLine; i <= 39; i++)
			{
				for (uint8 j = COL_END; j >= COL_START + 4; j--)
				{
					if (image[i][j] == Black && image[i][j + 1] == White && image[i][j - 1] == Black)
					{
						if (str->LeftEdge[i] < j)
						{
							str->LeftEdge[i] = j;
						}
						break;
					}
				}
			}
			break;
		}
		case Into_Round2://进环 4
		{
			/*判断是否进入环岛*/
			if (distance_judge(1800))//&&str->L_StartLine==0&& RoundSize==Small_Round)      //定距离一段
			{
				RoundProcess = In_Round;
				Inflection_Temp_x = 0;
				Inflection_Temp_y = 0;
			}
			/*进环的补线处理*/
			for (uint8 i = str->EndLine; i <= 39; i++)
			{
				for (uint8 j = COL_END; j >= COL_START + 4; j--)
				{
					if (image[i][j] == Black && image[i][j + 1] == White && image[i][j - 1] == Black)
					{
						if (str->LeftEdge[i] < j)
						{
							str->LeftEdge[i] = j;
						}
						break;
					}
				}
			}
			//补线
			/*进环斜率*/
			LeftLoopSlope = -2.0;
			for (uint8 i = 39; i > 5; i--)
			{
				if (str->LeftEdge[i] != 0)
					str->LeftEdge[i] = str->LeftEdge[i] > (int)(LeftLoopSlope * i + 75) ? str->LeftEdge[i] : (int)(LeftLoopSlope * i + 75);
				else
					str->LeftEdge[i] = (int)(LeftLoopSlope * i + 75);
				if (str->LeftEdge[i] < COL_START)
					str->LeftEdge[i] = 0;
			}
			for (uint8 i = 35; i > str->EndLine && i > ROW_START + 2; i--)
			{
				if (image[i][COL_END - 10] == White && image[i - 1][COL_END - 10] == Black && image[i - 2][COL_END - 10] == Black)
				{
					for (uint8 j = i; j > str->EndLine - 1; j--)
					{
						str->RightEdge[j] = 0;
					}
					str->EndLine = i - 1;
					break;
				}
				if (str->RightEdge[i] < 50)
				{
					str->RightEdge[i] = 0;   //限幅
				}
			}
			break;
		}
		case In_Round://在环内  5
		{
			if (Inflection_point.L_down_point.x && image[Inflection_point.L_down_point.x][Inflection_point.L_down_point.y + 2] == White && image[Inflection_point.L_down_point.x][Inflection_point.L_down_point.y + 3] == White)
			{
				Inflection_Temp_x = Inflection_point.L_down_point.x;
				Inflection_Temp_y = Inflection_point.L_down_point.y;
				if (Inflection_point.L_down_point.x > 5)
				{
					uint8 hang_num = 0;            //清除白块计数值                 8
					for (Round_row = Inflection_point.L_down_point.x - 1; Round_row >= Inflection_point.L_down_point.x - 6 && Round_row > ROW_START; Round_row--)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
					{
						if (str->RowLose[Round_row] == 4)
						{
							hang_num++;
						}
					}
					if (hang_num > 1 || Inflection_point.L_down_point.x > 30 || (Inflection_point.L_down_point.y < 30 && Inflection_point.L_down_point.y != 0))
					{
						RoundProcess = Out_Round;
					}
				}
			}


			if (Inflection_Temp_x > 0)
			{
				//补线
				LeftLoopSlope = 4.5;

				for (int i = Inflection_Temp_x + 1; i > ROW_START; i--)
				{
					str->LeftEdge[i] = str->LeftEdge[i + 1] + 5;
					//str->LeftEdge[i]=(int)((Inflection_Temp_x-i)*LeftLoopSlope+Inflection_Temp_y);    //这个式子you  bug
					if (str->LeftEdge[i] > COL_END)
					{
						str->EndLine = i;
						break;
					}
				}
				// str->StartLine=ROW_END;
			}
			for (uint8 i = 39; i > str->EndLine; i--)    //消除右线误搜索
			{
				if (str->RightEdge[i] < 50)
				{
					str->RightEdge[i] = 0;
				}
			}

			break;
		}
		case Out_Round://出环
		{
			LeftLoopSlope = -1.7;
			for (int i = 39; i > 5; i--)
			{
				if (str->LeftEdge[i])
					str->LeftEdge[i] = str->LeftEdge[i] > (int)(LeftLoopSlope * i + 75) ? str->LeftEdge[i] : (int)(LeftLoopSlope * i + 75);
				else
					str->LeftEdge[i] = (int)(LeftLoopSlope * i + 75);
				if (str->LeftEdge[i] < COL_START)
					str->LeftEdge[i] = 0;
			}
			//str->StartLine=ROW_END;


			if (distance_judge(1200))   //下一阶段标志
			{
				RoundProcess = Leave_Round;
			}


			for (uint8 i = 39; i > str->EndLine; i--)    //消除右线误搜索
			{
				if (str->RightEdge[i] < 50)
				{
					str->RightEdge[i] = 0;
				}
				if (i<39 && i>ROW_START)
				{
					if (str->RightEdge[i] != 0 && str->RightEdge[i - 1] == 0 && str->RightEdge[i + 1] == 0)
						str->RightEdge[i] = 0;
				}
			}
			break;
		}
		case Leave_Round://离开圆环
		{
			Round_num = 0;
			for (Round_row = str->L_StartLine; Round_row > str->EndLine + 1 && Round_row > ROW_START + 10; Round_row--)
			{
				if (str->LeftEdge[Round_row] == 0)
				{
					Round_num++;
				}
			}
			if (Round_num < 3 && ((str->L_StartLine - str->EndLine) > 32 || str->LeftEdge[str->EndLine] < 65))  //30需要调
			{
				RoundProcess = Leave_Round2;//暂定
			}
			else
			{
				LeftLoopSlope = -1.7;
				for (int i = 39; i > 5; i--)
				{
					if (str->LeftEdge[i])
						str->LeftEdge[i] = str->LeftEdge[i] > (int)(LeftLoopSlope * i + 75) ? str->LeftEdge[i] : (int)(LeftLoopSlope * i + 75);
					else
						str->LeftEdge[i] = (int)(LeftLoopSlope * i + 75);
					if (str->LeftEdge[i] < COL_START)
						str->LeftEdge[i] = 0;
				}
				for (uint8 i = 39; i > str->EndLine; i--)    //消除右线误搜索
				{
					if (str->RightEdge[i] < 50)
					{
						str->RightEdge[i] = 0;
					}
					if (i<39 && i>ROW_START)
					{
						if (str->RightEdge[i] != 0 && str->RightEdge[i - 1] == 0 && str->RightEdge[i + 1] == 0)
							str->RightEdge[i] = 0;
					}
				}
			}
			break;
		}
		case Leave_Round2://离开圆环
		{
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				if (str->LeftEdge[Round_row])
				{
					str->RightEdge[Round_row] = str->LeftEdge[Round_row] + round->Track_Width_forRound[Round_row];//左边界等于右边界减半宽
				}
				else
				{
					str->RightEdge[Round_row] = 0;
				}
			}
			/*离开圆环的补线处理*/
			if (distance_judge(3000))
			{
				RoundProcess = Round_Not_Found;//暂定
				round->Round_flag = 0;
				RoadType = else_road;
			}
			break;
		}
		default:
		{
			break;
		}
		}
		break;
	}
	default:
	{
		break;
	}
	}
}