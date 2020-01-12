/*!
 * @file       Extract_Edge.c
 * @brief      边界提取函数
 * @author
 * @version
 * @date       2019-05-21
 */

#include "include.h"
#include "Extract_Edge.h"


Str_Boundary  Boundary;  //边界结构体
Str_Inflection_point    Inflection_point;   //拐点结构体

/*临时变量*/


unsigned char EXTRACT_ROW;//当前查找行
unsigned char EXTRACT_COL;//当前查找列？
unsigned char W_num;
unsigned char W_num_row;
unsigned char W_num_col;
unsigned char counting_data;

//unsigned char L_cross_able=0;
//unsigned char R_cross_able=0;

unsigned char L_Link_num = 0;        //给连续左边沿个数赋初值  0
unsigned char R_Link_num = 0;        //给连续右边沿个数赋初值  0

unsigned char Allget_f = 0;
/***************************************************************
搜索边界

* 函数名称：extract_edge
* 功能说明：搜索边界
* 参数说明：
* 函数返回：void
* 修改时间： 7.4   00.35
* 备 注：
***************************************************************/
void extract_edge(Str_Boundary* str, unsigned char Row_START, unsigned char Row_END, unsigned char Col_START, unsigned char Col_END)       //此函数为边沿提取主函数
{
	memset(str->LeftEdge, 0, sizeof(str->LeftEdge));
	memset(str->RightEdge, 0, sizeof(str->RightEdge));
	memset(str->RightEdge, 0, sizeof(str->RowLose));

	//       memset(L_down_point,0,sizeof(L_down_point) );
	//       memset(L_up_point,  0,sizeof(L_up_point)   );
	//       memset(R_down_point,0,sizeof(R_down_point) );
	//       memset(R_up_cross,  0,sizeof(R_up_cross)   );

	Inflection_point.L_down_point.x = 0;
	Inflection_point.L_down_point.y = 0;
	Inflection_point.L_up_point.x = 0;
	Inflection_point.L_up_point.y = 0;
	Inflection_point.R_down_point.x = 0;
	Inflection_point.R_down_point.y = 0;
	Inflection_point.R_up_point.x = 0;
	Inflection_point.R_up_point.y = 0;

	///*以下变量可能需要用于外部函数*/
	str->OnlyLeftLose = 0;                    //左边丢失最大数
	str->OnlyRightLose = 0;
	str->AllLeftLose = 0;
	str->AllRightLose = 0;

	/*起始与结束*/
	str->L_EndLine = 0;                //左边沿搜索结束行数置  -1
	str->R_EndLine = 0;                //右边沿搜索结束行数置  -1

	str->L_StartLine = 0;
	str->R_StartLine = 0;


	str->Left_Edge_num = 0;
	str->Right_Edge_num = 0;



	unsigned char L_Search_end = 0;              //左边沿搜索结束标志置  0
	unsigned char R_Search_end = 0;              //右边沿搜索结束标志置  0

/*赛道搜索标志位*/
	unsigned char L_Trend_in = 0;        //左边沿内倾趋势判断标志置  0
	unsigned char R_Trend_in = 0;        //右边沿内倾趋势判断标志置  0

	unsigned char L_rowget_flag = 0;        //左边沿搜索得到标志置  0
	unsigned char R_rowget_flag = 0;        //右边沿搜索得到标志置  0

	unsigned char L_Lost_num = 1;        //左边沿连续丢失个数置  1（先定义为丢失边沿）
	unsigned char R_Lost_num = 1;        //左边沿连续丢失个数置  1（先定义为丢失边沿）

	unsigned char L_get_flag = 0;
	unsigned char R_get_flag = 0;
	//       L_down_end_row=0;           //左边沿向下搜索结束行数置  -1
	//       R_down_end_row=0;           //右边沿向下搜索结束行数置  -1


		  // END_LINE_FLAG=0;             //终点线标志置  0

	unsigned char L_TURN = 0;                    //左弯标志置  0
	unsigned char R_TURN = 0;                    //右弯标志置  0
//       curve_in_row=0;             //弯道入口行置  -1
//       curve_car_row=0;            //弯道前车行数  -1

//              S_FLAG=0;                    //S型 赛道标志置  0
//              s_curve_row=0;              //S型 赛道行数置  0

//       STRAIGHT_FLAG=0;             //直道标志置  0
//       little_s_flag=0;             //小S连续弯标志置  0
//       RAMP_FLAG=0;                 //坡道标志置  0


	   // /*以下变量不需要用于外部函数*/

	   //      unsigned char L_cross_able=1;
	   //      unsigned char R_cross_able=1;

	   //       end_bar_able=1;//终点线障碍物使能标志
	unsigned char L_edge_nearest;
	unsigned char R_edge_nearest;


	Allget_f = 0;

	L_Link_num = 0;
	R_Link_num = 0;
	/**********************************************检测开始***********************************************************/
	for (EXTRACT_ROW = Row_END; EXTRACT_ROW > Row_START; EXTRACT_ROW--)       //检测整幅图像的边沿
	{
		L_rowget_flag = 0;               //对左边沿搜索得到标志清0
		R_rowget_flag = 0;               //对右边沿搜索得到标志清0

		/*以下程序段为左边沿检测程序*/

		if (L_Search_end == 0)       //当左边沿搜索 未结束 时进入
		{
			if (L_Lost_num == 0)         //当左边沿没有 丢失时 进入处理
			{
				if (L_Link_num > 3)   //当左边沿连续数大于等于3，判断出边沿趋势时进入处理     //link_num很好的反映边线长度
				{
					if (L_Trend_in == 1)    //当左边沿内倾时进入处理
					{
						if (image[EXTRACT_ROW][L_edge_nearest] == Black)     //当上一行左边沿点的上方一点为黑
						{
							//修改此部分下面也要改
							for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL <= L_edge_nearest + 20 && EXTRACT_COL <= Col_END - 2; EXTRACT_COL++)      //左边沿内倾向内搜索
							{
								if (image[EXTRACT_ROW][EXTRACT_COL + 1] == White)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //找到黑白跳变点  白点
									{
										str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;    //储存本行左边沿列坐标

										if (str->RightEdge[EXTRACT_ROW + 1] < str->LeftEdge[EXTRACT_ROW])     //预判断  节省时间  判断左边搜到但是在右边界右边
										{
											if (str->RightEdge[EXTRACT_ROW + 1] != 0 && str->RightEdge[EXTRACT_ROW + 2] != 0 && str->RightEdge[EXTRACT_ROW + 2] < str->LeftEdge[EXTRACT_ROW]) //最终判断
											{
												L_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为左边沿搜索结束

												R_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为右边沿搜索结束

												str->L_EndLine = EXTRACT_ROW;//将上一行的行数值赋给左边沿结束行数值

												L_Link_num = 0;               //连续左边沿个数置  0

												L_Lost_num++;               //当没有搜索到左边沿点时，左边沿丢失数加一

											}
										}

										str->Left_Edge_num++;                        //左边沿个数加一

										L_Link_num++;                                 //连续左边沿个数加一

										L_rowget_flag = 1;                            //左边沿搜索得到标志置  1

										L_edge_nearest = EXTRACT_COL;                 //存储最近一个左边沿的列坐标

										break;                                       //本行左边沿搜索结束
									}
								}
							}
							if (L_rowget_flag == 0)        //当左边沿内倾向内未搜到边沿   很可能右弯
							{
								if (L_Link_num >= 7)
								{
									W_num = 0;
									for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 10 && W_num_row <= Row_END - 1; W_num_row++)      //zuo边沿内倾向内搜索
									{
										if (str->LeftEdge[W_num_row] < str->LeftEdge[W_num_row + 1])
										{
											W_num++;
										}
									}
									if (W_num <= 1 && str->AllRightLose > 5 && R_Link_num < 3) //右边丢线
									{
										R_TURN = 1;                   //右弯标志置1
									}
									else if (W_num <= 1 && R_Link_num >= 7)  //断路判断
									{
										if (EXTRACT_ROW > 17 && BlackEnd.BlackEndLMR < 65)
										{
											if (Break_Road.Break_Road_Flag == 0)
											{
												if (REN.EN_Break_RoadType == 1)   //
												{
													Break_Road.Break_Road_preFlag = 1;
												}
											}
										}
									}
								}

								L_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为左边沿搜索结束
								R_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为右边沿搜索结束
								str->L_EndLine = EXTRACT_ROW;//将上一行的行数值赋给左边沿结束行数值

								L_Link_num = 0;               //连续左边沿个数置  0

								L_Lost_num++;               //当没有搜索到左边沿点时，左边沿丢失数加一
							}
						}
						else      //当上一行左边沿点的上方一点为白
						{
							if (image[EXTRACT_ROW][L_edge_nearest + 1] != White || image[EXTRACT_ROW][L_edge_nearest + 2] != White)
							{
								for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL <= L_edge_nearest + 20 && EXTRACT_COL <= Col_END - 1; EXTRACT_COL++)      //左边沿内倾向内搜索
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White)      //找到黑白跳变点  白点
									{
										str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;    //储存本行左边沿列坐标

										str->Left_Edge_num++;                        //左边沿个数加一

										L_Link_num++;                                 //连续左边沿个数加一

										L_rowget_flag = 1;                            //左边沿搜索得到标志置  1

										L_edge_nearest = EXTRACT_COL;                 //存储最近一个左边沿的列坐标

										break;                                       //本行左边沿搜索结束
									}
								}
								if (L_rowget_flag == 0)        //当左边沿内倾向内未搜到边沿   很可能右弯
								{
									if (L_Link_num >= 10)
									{
										W_num = 0;
										for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 10 && W_num_row <= Row_END - 1; W_num_row++)      //zuo边沿内倾向内搜索
										{
											if (str->LeftEdge[W_num_row] < str->LeftEdge[W_num_row + 1])
											{
												W_num++;
											}
										}
										if (W_num <= 1 && str->AllRightLose > 5 && R_Link_num < 3) //右边丢线
										{
											R_TURN = 1;                   //右弯标志置1
										}
										else if (W_num <= 1 && R_Link_num >= 7)  //断路判断
										{
											if (EXTRACT_ROW > 17 && BlackEnd.BlackEndLMR < 65)
											{
												if (Break_Road.Break_Road_Flag == 0)
												{
													if (REN.EN_Break_RoadType == 1)   //
													{
														Break_Road.Break_Road_preFlag = 1;
													}
												}
											}
										}
									}

									L_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为左边沿搜索结束
									R_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为右边沿搜索结束
									str->L_EndLine = EXTRACT_ROW;//将上一行的行数值赋给左边沿结束行数值

									L_Link_num = 0;               //连续左边沿个数置  0

									L_Lost_num++;               //当没有搜索到左边沿点时，左边沿丢失数加一
								}
							}
							else
							{
								for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL >= L_edge_nearest - 20 && EXTRACT_COL > Col_START + 2; EXTRACT_COL--)      //左边沿内倾向nei搜索
								{
									if (image[EXTRACT_ROW][EXTRACT_COL - 1] == Black)
									{
										if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL - 1] == Black && image[EXTRACT_ROW][EXTRACT_COL - 2] == Black)
										{
											str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL - 1;          //储存本行左边沿列坐标

											str->Left_Edge_num++;                                //左边沿个数加一

											L_Link_num++;                                //连续左边沿个数加一

											L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

											L_edge_nearest = EXTRACT_COL - 1;                  //存储最近一个左边沿的列坐标

											L_Trend_in = 0;                                //将左边沿内倾标志置  0
											/////////////  内倾突然外张，判断为拐点，bug处可以忽略
											//  L_down_cross_judge();                        //
											if (Inflection_point.L_up_point.x == 0 && Inflection_point.L_down_point.x == 0 && str->LeftEdge[EXTRACT_ROW + 1] > 4 && EXTRACT_ROW > 10 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
											{
												Inflection_point.L_down_point.x = EXTRACT_ROW + 1;                           //存储左边沿十字下方直角拐点行坐标
												Inflection_point.L_down_point.y = str->LeftEdge[EXTRACT_ROW + 1];                //存储左边沿十字下方直角拐点列坐标
											}
											break;                                      //本行左边沿搜索结束
										}
									}
								}
								if (L_rowget_flag == 0)      //当左边沿内倾向外未搜到边沿
								{
									L_Link_num = 0;           //连续左边沿个数置  0

									L_Lost_num++;           //当没有搜索到左边沿点时，左边沿丢失数加一
									//////  内倾突然外张且无边界，判断为拐点，基本无问题，断点//
									if (Inflection_point.L_up_point.x == 0 && Inflection_point.L_down_point.x == 0 && str->LeftEdge[EXTRACT_ROW + 1] > 4 && EXTRACT_ROW > 10 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
									{
										Inflection_point.L_down_point.x = EXTRACT_ROW + 1;                           //存储左边沿十字下方直角拐点行坐标
										Inflection_point.L_down_point.y = str->LeftEdge[EXTRACT_ROW + 1];                //存储左边沿十字下方直角拐点列坐标
									}
								}
							}
						}
					}
					else  //当左边沿外倾时进入处理
					{
						if (image[EXTRACT_ROW][L_edge_nearest] == Black)     //当上一行左边沿点的上方一点为黑
						{
							for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL <= L_edge_nearest + 20 && EXTRACT_COL <= Col_END - 2; EXTRACT_COL++)      //左边沿外倾向内搜索
							{
								if (image[EXTRACT_ROW][EXTRACT_COL + 1] == White)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)  //找到黑白跳变点
									{
										str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;        //储存本行左边沿列坐标

										str->Left_Edge_num++;                                //左边沿个数加一

										L_Link_num++;                                //连续左边沿个数加一

										L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

										L_edge_nearest = EXTRACT_COL;                //存储最近一个左边沿的列坐标

										L_Trend_in = 1;                                //左边沿内倾趋势标志置  1
#if 0  //效果待观察啊
										Inflection_point.L_down_point.x = 0;                           //存储左边沿十字下方直角拐点行坐标
										Inflection_point.L_down_point.y = 0;

										if (str->LeftEdge[EXTRACT_ROW] >= str->LeftEdge[EXTRACT_ROW + 1])
										{
											for (W_num_row = EXTRACT_ROW; W_num_row < Row_END - 5; W_num_row++)
											{
												if (str->LeftEdge[W_num_row] < str->LeftEdge[W_num_row + 1])
													str->LeftEdge[W_num_row] = str->LeftEdge[W_num_row + 1];
												if (str->LeftEdge[W_num_row] > str->LeftEdge[W_num_row + 1])
													break;
											}
										}
#endif

										break;                                      //本行左边沿搜索结束
									}
								}
							}
							if (L_rowget_flag == 0)      //当左边沿外倾向右未搜到边沿
							{
								L_Search_end = 1;             //当左边沿外倾时，向内搜索不到白点，判断为左边沿搜索结束
								R_Search_end = 1;             //当左边沿外倾时，向内搜索不到白点，判断为右边沿搜索结束
								str->L_EndLine = EXTRACT_ROW;    //将上一行的行数值赋给左边沿结束行数值
								L_Link_num = 0;               //连续左边沿个数置  0
								L_Lost_num++;               //当没有搜索到左边沿点时，左边沿丢失数加一
							}
						}
						else          //当上一行左边沿点的上方一点为白
						{
							for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL >= L_edge_nearest - 20 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)     //开始向外搜索
							{
								if (image[EXTRACT_ROW][EXTRACT_COL - 1] == Black)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL - 1] == Black && image[EXTRACT_ROW][EXTRACT_COL - 2] == Black)
									{
										str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL - 1;          //储存本行左边沿列坐标

										str->Left_Edge_num++;                                //左边沿个数加一

										L_Link_num++;                                //连续左边沿个数加一

										L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

										L_edge_nearest = EXTRACT_COL - 1;                  //存储最近一个左边沿的列坐标

										break;                                      //本行左边沿搜索结束
									}
								}
							}
							if (L_rowget_flag == 0)
							{
								L_Link_num = 0;                                //连续左边沿个数置  0

								L_Lost_num++;                                //当左边沿左倾时，向左搜索不到白点，左边沿丢失数加一

								////// 外张且无边界，判断为拐点，基本无问题，断点//
								if (Inflection_point.L_up_point.x == 0 && Inflection_point.L_down_point.x == 0 && str->LeftEdge[EXTRACT_ROW + 1] > 4 && EXTRACT_ROW > 10 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
								{
									for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 5; W_num_row++)  //检测前8行内倾数
									{
										if (str->LeftEdge[W_num_row] - str->LeftEdge[W_num_row + 1] >= 0)
										{
											Inflection_point.L_down_point.x = W_num_row;               //存储十字拐点行坐标
											Inflection_point.L_down_point.y = str->LeftEdge[W_num_row];        //存储十字拐点列坐标
											break;
										}
									}
								}

							}

						}
					}
				}
				else                  //当左边沿没有判断出边沿趋势时进入处理
				{
					if (image[EXTRACT_ROW][L_edge_nearest] == Black)     //当上一行左边沿点的上方一点为黑
					{                                                                      //60
						for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL <= L_edge_nearest + 60 && EXTRACT_COL < Col_END - 2; EXTRACT_COL++)            //开始向右搜索
						{
							if (image[EXTRACT_ROW][EXTRACT_COL + 1] == White)
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)   //找到黑白跳变点
								{
									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行左边沿列坐标

									str->Left_Edge_num++;                                //左边沿个数加一

									L_Link_num++;                                //连续左边沿个数加一

									L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

									L_edge_nearest = EXTRACT_COL;                  //存储最近一个左边沿的列坐标

									break;                                      //本行左边沿搜索结束
								}


							}
						}

						if (L_rowget_flag == 0)
						{
							L_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为左边沿搜索结束
							R_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为右边沿搜索结束
							str->L_EndLine = EXTRACT_ROW;//将上一行的行数值赋给左边沿结束行数值
							L_Link_num = 0;               //连续左边沿个数置  0
							//有可能结束位
							L_Lost_num++;               //当没有搜索到左边沿点时，左边沿丢失数加一
						}
					}
					else     //当上一行左边沿点的上方一点为白
					{
						if (image[EXTRACT_ROW][L_edge_nearest + 1] == White && image[EXTRACT_ROW][L_edge_nearest + 2] == White)        //之后两点也为白
						{
							for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL >= L_edge_nearest - L_EDGE_OUT && EXTRACT_COL >= Col_START + 1; EXTRACT_COL--)           //开始向左搜索黑白跳变点
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL - 1] == Black && image[EXTRACT_ROW][EXTRACT_COL - 2] == Black)   //找到黑白跳变点
								{
									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL - 1;          //储存本行左边沿列坐标

									str->Left_Edge_num++;                                //左边沿个数加一

									L_Link_num++;                                //连续左边沿个数加一

									L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

									L_edge_nearest = EXTRACT_COL - 1;                  //存储最近一个左边沿的列坐标

									break;                                      //本行左边沿搜索结束
								}
							}
							if (L_rowget_flag == 0)
							{
								L_Link_num = 0;       //连续左边沿个数置  0
								//有可能
								L_Lost_num++;       //当没有搜索到左边沿点时，左边沿丢失数加一
							}
						}
						else           //之后两点有黑点,则判断为干扰
						{       /*有问题cl*/
							for (EXTRACT_COL = L_edge_nearest + 1; EXTRACT_COL <= L_edge_nearest + L_EDGE_IN; EXTRACT_COL++)            //开始向右搜索黑白跳变点
							{
								if (EXTRACT_COL > Col_END - 5)    //当搜索超过图像范围时，结束搜索
								{
									break;
								}
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 3] == White && image[EXTRACT_ROW][EXTRACT_COL + 5] == White)   //找到黑白跳变点
								{
									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行左边沿列坐标

									str->Left_Edge_num++;                                //左边沿个数加一

									L_Link_num++;                                //连续左边沿个数加一

									L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

									L_edge_nearest = EXTRACT_COL;                  //存储最近一个左边沿的列坐标

									break;                                      //本行左边沿搜索结束
								}
							}
							if (L_rowget_flag == 0)
							{
								L_Link_num = 0;               //连续左边沿个数置  0

								L_Lost_num++;               //当没有搜索到左边沿点时，左边沿丢失数加一
							}
						}
					}

					if (L_Link_num >= 3)  //当搜索完这一行后，连续边沿数大于等于3
					{
						if (str->LeftEdge[EXTRACT_ROW] >= str->LeftEdge[EXTRACT_ROW + 1] || str->LeftEdge[EXTRACT_ROW] >= str->LeftEdge[EXTRACT_ROW + 2])
						{
							L_Trend_in = 1;     //满足条件，左边沿内倾趋势置  1
						}
						else
						{
							L_Trend_in = 0;     //不满足条件，左边沿内倾趋势置  0
						}
					}
				}
			}
			else                        //当左边沿丢失时进入处理
			{
				if (Cross.Cross_flag == 1) //当左边沿十字标志为1时
				{
					/*
					 * //当检测到左边沿下方十字直角拐点
					 */
					if (Inflection_point.L_down_point.x != 0)    
					{
						if (Inflection_point.L_down_point.y - 10 > Col_START)
						{
							W_num_col = Inflection_point.L_down_point.y - 10;
						}
						else
						{
							W_num_col = Col_START + 1;
						}
						if (Inflection_point.L_down_point.x > 30)
						{
							for (EXTRACT_COL = W_num_col; EXTRACT_COL <= Inflection_point.L_down_point.y + 40 && EXTRACT_COL <= Col_END - 3; EXTRACT_COL++)    //开始搜索
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //当搜索到黑白跳变点
								{

									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行左边沿列坐标

									str->Left_Edge_num++;                                //左边沿个数加一

									L_Link_num++;                                //连续左边沿个数加一

									L_edge_nearest = EXTRACT_COL;                  //存储最近一个左边沿的列坐标

									//L_up_cross_error();                          //左边沿上方直角拐点错误纠正
									if (Inflection_point.L_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
									{
										W_num = 0; counting_data = EXTRACT_COL;
										for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //检测前8行内倾数
										{
											for (W_num_col = counting_data; W_num_col <= counting_data + 40 && W_num_col <= Col_END - 2; W_num_col++)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
											{
												if (image[W_num_row][W_num_col] == Black && image[W_num_row][W_num_col + 1] == White && image[W_num_row][W_num_col + 2] == White)
												{
													if (W_num_col - counting_data < 3)
														W_num++;
													counting_data = W_num_col;
													break;
												}
											}
											if (W_num > 0)
											{
												Inflection_point.L_up_point.x = W_num_row;               //存储十字拐点行坐标
												Inflection_point.L_up_point.y = counting_data;        //存储十字拐点列坐标
												break;
											}
										}
									}

									L_Lost_num = 0;                                //连续左边沿丢失数清0



									break;                                      //本行左边沿搜索结束

								}
							}
						}
						else
						{
							for (EXTRACT_COL = W_num_col; EXTRACT_COL <= Inflection_point.L_down_point.y + 40 && EXTRACT_COL <= Col_END - 3; EXTRACT_COL++)    //开始搜索
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White)      //当搜索到黑白跳变点
								{
									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行左边沿列坐标

									str->Left_Edge_num++;                                //左边沿个数加一

									L_Link_num++;                                //连续左边沿个数加一

									L_edge_nearest = EXTRACT_COL;                  //存储最近一个左边沿的列坐标

									if (Inflection_point.L_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
									{
										W_num = 0; counting_data = EXTRACT_COL;
										for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //检测前8行内倾数
										{
											for (W_num_col = counting_data; W_num_col <= counting_data + 40 && W_num_col <= Col_END - 2; W_num_col++)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
											{
												if (image[W_num_row][W_num_col] == Black && image[W_num_row][W_num_col + 1] == White && image[W_num_row][W_num_col + 2] == White)
												{
													if (W_num_col - counting_data < 3)
														W_num++;
													counting_data = W_num_col;
													break;
												}
											}
											if (W_num > 0)
											{
												Inflection_point.L_up_point.x = W_num_row;               //存储十字拐点行坐标
												Inflection_point.L_up_point.y = counting_data;        //存储十字拐点列坐标
												break;
											}
										}
									}

									L_Lost_num = 0;                                //连续左边沿丢失数清0

									break;                                      //本行左边沿搜索结束
								}
							}
						}

						if (L_rowget_flag == 0)
						{
							L_Lost_num++;                          //连续左边沿丢失数加1
						}
					}
					else                         //当未检测到左边沿下方十字直角拐点  //cl未懂 不需要感觉不太对
					{
						for (EXTRACT_COL = Col_START; EXTRACT_COL <= 60 && EXTRACT_COL <= Col_END - 3; EXTRACT_COL++)    //开始搜索
						{
							if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 3] == White)      //当搜索到黑白跳变点
							{

								str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行左边沿列坐标

								str->Left_Edge_num++;                                //左边沿个数加一

								L_Link_num++;                                //连续左边沿个数加一

								L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

								L_edge_nearest = EXTRACT_COL;                  //存储最近一个左边沿的列坐标

								L_Lost_num = 0;                                //连续左边沿丢失数清

								if (L_rowget_flag && L_get_flag == 0)   //左第一次获得线
								{
									L_get_flag = 1;
									str->L_StartLine = EXTRACT_ROW;
								}
								break;                                                  //本行左边沿搜索结束
							}
						}
						if (L_rowget_flag == 0)
						{
							L_Lost_num++;                          //连续左边沿丢失数加1

						}
					}
				}
				else                  //当左边沿十字标志为0时
				{
					if (image[EXTRACT_ROW][Col_START] == White && image[EXTRACT_ROW][Col_START + 1] == White && image[EXTRACT_ROW][Col_START + 2] == White)    //当前三个点为白
					{
						if (EXTRACT_ROW == Row_END)
						{
							for (EXTRACT_COL = Col_START + 2; EXTRACT_COL < 40; EXTRACT_COL++)    //开始搜索  刚开始底边丢线you必要搜 减少bug
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black)      //当搜索到黑白跳变点
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //当搜索到黑白跳变点
									{
										W_num = 0;            //清除白块计数值                 8
										for (W_num_col = EXTRACT_ROW - 1; W_num_col >= EXTRACT_ROW - 4 && W_num_col > Row_START; W_num_col--)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
										{
											if (image[W_num_col][EXTRACT_COL] == White)
											{
												W_num++;
											}
										}
										if (W_num < 2)
										{
											str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行左边沿列坐标

											str->Left_Edge_num++;                                //左边沿个数加一

											L_Link_num++;                                //连续左边沿个数加一

											L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

											L_edge_nearest = EXTRACT_COL;                  //存储最近一个左边沿的列坐标

											L_Lost_num = 0;                                //连续左边沿丢失数清

											if (L_rowget_flag && L_get_flag == 0)   //左第一次获得线
											{
												L_get_flag = 1;
												str->L_StartLine = EXTRACT_ROW;
											}
											break;                                      //本行左边沿搜索结束
										}
									}
								}
							}
							if (L_rowget_flag == 0)   //未搜到
							{
								L_Lost_num++;
							}

						}
						else
						{
							if (str->RightEdge[EXTRACT_ROW + 1] > Col_START + 2)
							{
								for (EXTRACT_COL = Col_START + 2; EXTRACT_COL < str->RightEdge[EXTRACT_ROW + 1]; EXTRACT_COL++)    //开始搜索  刚开始底边丢线you必要搜 减少bug
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //当搜索到黑白跳变点
									{
										W_num = 0;            //清除白块计数值                 8
										for (W_num_col = EXTRACT_ROW - 1; W_num_col >= EXTRACT_ROW - 4 && W_num_col > Row_START; W_num_col--)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
										{
											if (image[W_num_col][EXTRACT_COL] == White)
											{
												W_num++;
											}
										}
										if (W_num < 2)
										{
											str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行左边沿列坐标

											str->Left_Edge_num++;                                //左边沿个数加一

											L_Link_num++;                                //连续左边沿个数加一

											L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

											L_edge_nearest = EXTRACT_COL;                  //存储最近一个左边沿的列坐标

											L_Lost_num = 0;                                //连续左边沿丢失数清

											if (L_rowget_flag && L_get_flag == 0)   //左第一次获得线
											{
												L_get_flag = 1;
												str->L_StartLine = EXTRACT_ROW;
											}
											break;                                      //本行左边沿搜索结束
										}

									}

								}
							}
							else
							{
								for (EXTRACT_COL = Col_START + 2; EXTRACT_COL < 40; EXTRACT_COL++)    //开始搜索  刚开始底边丢线you必要搜 减少bug
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //当搜索到黑白跳变点
									{
										W_num = 0;            //清除白块计数值                 8
										for (W_num_col = EXTRACT_ROW - 1; W_num_col >= EXTRACT_ROW - 4 && W_num_col > Row_START; W_num_col--)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
										{
											if (image[W_num_col][EXTRACT_COL] == White)
											{
												W_num++;
											}
										}
										if (W_num < 2)
										{
											str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行左边沿列坐标

											str->Left_Edge_num++;                                //左边沿个数加一

											L_Link_num++;                                //连续左边沿个数加一

											L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

											L_edge_nearest = EXTRACT_COL;                  //存储最近一个左边沿的列坐标

											L_Lost_num = 0;                                //连续左边沿丢失数清

											if (L_rowget_flag && L_get_flag == 0)   //左第一次获得线
											{
												L_get_flag = 1;
												str->L_StartLine = EXTRACT_ROW;
											}
											break;                                      //本行左边沿搜索结束
										}

									}

								}
							}
						}
						if (L_rowget_flag == 0)  //当左边沿丢失时
						{
							L_Lost_num++;
							if (L_get_flag)
							{
								str->AllLeftLose++;
							}
						}

						//    出断路检测
   //					 if(Break_Road.Break_Road_Flag==1)
   //					 {
   //						if(EXTRACT_ROW==Row_END)
   //						{
   //						       Break_Road.Break_Road_Flag=0;
   //						}
   //					 }
					}
					else         //当前三个点不全为白
					{
						for (EXTRACT_COL = Col_START; EXTRACT_COL < Col_END - 2; EXTRACT_COL++)    //开始搜索
						{
							if (image[EXTRACT_ROW][EXTRACT_COL + 1] == White)
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //当搜索到黑白跳变点
								{
									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;        //储存本行左边沿列坐标

									str->Left_Edge_num++;                                //左边沿个数加一

									L_Link_num++;                                //连续左边沿个数加一

									L_rowget_flag = 1;                                //左边沿搜索得到标志置  1

									L_edge_nearest = EXTRACT_COL;                //存储最近一个左边沿的列坐标


									if (Inflection_point.L_down_point.x != 0 && (L_Lost_num > 8 || L_get_flag == 1) && EXTRACT_COL > 10)
									{
										//L_up_cross_error();
										if (Inflection_point.L_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
										{
											W_num = 0; counting_data = EXTRACT_COL;
											for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //检测前8行内倾数
											{
												for (W_num_col = counting_data; W_num_col <= counting_data + 40 && W_num_col <= Col_END - 2; W_num_col++)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
												{
													if (image[W_num_row][W_num_col] == Black && image[W_num_row][W_num_col + 1] == White && image[W_num_row][W_num_col + 2] == White)
													{
														if (W_num_col - counting_data < 3)
															W_num++;
														counting_data = W_num_col;
														break;
													}
												}
												if (W_num > 0)
												{
													Inflection_point.L_up_point.x = W_num_row;               //存储十字拐点行坐标
													Inflection_point.L_up_point.y = counting_data;        //存储十字拐点列坐标
													break;
												}
											}
										}
									}
									L_Lost_num = 0;                                //连续左边沿丢失数清0
									if (L_get_flag == 0)   //左第一次获得线
									{
										L_get_flag = 1;
										str->L_StartLine = EXTRACT_ROW - 1;

										if (Inflection_point.L_up_point.x == 0 && EXTRACT_COL > 10 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
										{
											W_num = 0; counting_data = EXTRACT_COL;
											for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //检测前8行内倾数
											{
												for (W_num_col = counting_data; W_num_col <= counting_data + 40 && W_num_col <= Col_END - 2; W_num_col++)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
												{
													if (image[W_num_row][W_num_col] == Black && image[W_num_row][W_num_col + 1] == White && image[W_num_row][W_num_col + 2] == White)
													{
														if (W_num_col - counting_data < 3)
															W_num++;
														counting_data = W_num_col;
														break;
													}
												}
												if (W_num > 0)
												{
													Inflection_point.L_up_point.x = W_num_row;               //存储十字拐点行坐标
													Inflection_point.L_up_point.y = W_num_col;        //存储十字拐点列坐标
													break;
												}
											}
										}
									}
									break;                                      //本行左边沿搜索结束

									//    出断路检测
  //							      if(Break_Road.Break_Road_Flag==1)
  //							      {
  //								     if(EXTRACT_ROW==Row_END)
  //								     {
  //									    Break_Road.Break_Road_Flag=0;
  //								     }
  //							      }
								}



							}
						}
						if (L_rowget_flag == 0)
						{
							L_Lost_num++;             //当左边沿丢失时，向右搜索不到黑白跳变点，判断为左边沿丢失


							if (EXTRACT_ROW == Row_END)       //在断路时
							{
								L_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为左边沿搜索结束
								R_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为右边沿搜索结束
								str->L_EndLine = EXTRACT_ROW;//将上一行的行数值赋给左边沿结束行数值
							}
						}
					}
				}
			}
		}
		else
		{
			if (L_TURN == 0 && R_TURN == 0)
			{
				if (image[EXTRACT_ROW][Col_START] == White && image[EXTRACT_ROW][Col_START + 2] == White && image[EXTRACT_ROW][Col_START + 4] == White)    //当前三个点为白
				{
					L_Lost_num++;             //判断为左边沿丢失
				}
			}
		}
#if 1
		/*以下程序段为右边沿检测程序*/

		if (R_Search_end == 0)       //当右边沿搜索未结束时进入
		{
			if (R_Lost_num == 0)         //当右边沿没有丢失时进入处理
			{
				if (R_Link_num >= 3)   //当右边沿连续数大于等于3，判断出边沿趋势时进入处理
				{
					if (R_Trend_in == 1)       //当右边沿内倾时进入处理
					{
						if (image[EXTRACT_ROW][R_edge_nearest] == Black)     //当上一行右边沿点的上方一点为黑
						{
							for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL >= R_edge_nearest - 20 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)      //右边沿内倾向内搜索
							{
								if (image[EXTRACT_ROW][EXTRACT_COL - 1] == White)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)   //找到黑白跳变点
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;       //储存本行右边沿列坐标

										str->Right_Edge_num++;                                //右边沿个数加一

										R_Link_num++;                                //连续右边沿个数加一

										R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

										R_edge_nearest = EXTRACT_COL;               //存储最近一个右边沿的列坐标


										break;                                      //本行右边沿搜索结束
									}
								}
							}
							if (R_rowget_flag == 0)      //当右边沿内倾向内未搜到边沿
							{
								if (R_Link_num >= 10)
								{
									W_num = 0;
									for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 10 && W_num_row < Row_END; W_num_row++)      //右边沿内倾向内搜索
									{
										if (str->RightEdge[W_num_row] > str->RightEdge[W_num_row + 1])
										{
											W_num++;
										}
									}
									if (W_num <= 1 && str->AllLeftLose > 5 && L_Link_num < 3)
									{
										L_TURN = 1;                //左弯标志置  1
									}
									else   if (W_num <= 1 && L_Link_num >= 7)  //断路判断
									{
										if (EXTRACT_ROW > 17 && BlackEnd.BlackEndLMR < 65)
										{
											if (Break_Road.Break_Road_Flag == 0)
											{
												if (REN.EN_Break_RoadType == 1)   //
												{
													Break_Road.Break_Road_preFlag = 1;
												}
											}
										}
									}
								}
								str->R_EndLine = EXTRACT_ROW + 1; //将上一行的行数值赋给右边沿结束行数值

								str->LeftEdge[EXTRACT_ROW] = 0;
								R_Search_end = 1;             //当右边沿左倾时，向左搜索不到白点，判断为右边沿搜索结束
								L_Search_end = 1;             //当右边沿左倾时，向左搜索不到白点，判断为左边沿搜索结束

								R_Link_num = 0;               //连续右边沿个数置  0
								R_Lost_num++;               //当没有搜索到右边沿点时，右边沿丢失数加一
							}
						}
						else                                                   //当上一行右边沿点的上方一点为白
						{
							if (image[EXTRACT_ROW][R_edge_nearest - 1] != White || image[EXTRACT_ROW][R_edge_nearest - 2] != White)
							{
								for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL >= R_edge_nearest - 20 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)      //右边沿内倾向内搜索
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)   //找到黑白跳变点
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;       //储存本行右边沿列坐标

										str->Right_Edge_num++;                                //右边沿个数加一

										R_Link_num++;                                //连续右边沿个数加一

										R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

										R_edge_nearest = EXTRACT_COL;               //存储最近一个右边沿的列坐标

										break;                                      //本行右边沿搜索结束
									}
								}
								if (R_rowget_flag == 0)      //当右边沿内倾向内未搜到边沿
								{
									if (R_Link_num >= 10)
									{
										W_num = 0;
										for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 10 && W_num_row < Row_END; W_num_row++)      //右边沿内倾向内搜索
										{
											if (str->RightEdge[W_num_row] > str->RightEdge[W_num_row + 1])
											{
												W_num++;
											}
										}
										if (W_num <= 1 && str->AllLeftLose > 5 && L_Link_num < 3)
										{
											L_TURN = 1;                //左弯标志置  1
										}
										else   if (W_num <= 1 && L_Link_num >= 7)  //断路判断
										{
											if (EXTRACT_ROW > 17 && BlackEnd.BlackEndLMR < 65)
											{
												if (Break_Road.Break_Road_Flag == 0)
												{
													if (REN.EN_Break_RoadType == 1)   //
													{
														Break_Road.Break_Road_preFlag = 1;
													}
												}
											}
										}

									}
									str->R_EndLine = EXTRACT_ROW; //将上一行的行数值赋给右边沿结束行数值

									str->LeftEdge[EXTRACT_ROW] = 0;
									R_Search_end = 1;             //当右边沿左倾时，向左搜索不到白点，判断为右边沿搜索结束
									L_Search_end = 1;             //当右边沿左倾时，向左搜索不到白点，判断为左边沿搜索结束

									R_Link_num = 0;               //连续右边沿个数置  0
									R_Lost_num++;               //当没有搜索到右边沿点时，右边沿丢失数加一
								}

							}
							else
							{
								for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL <= R_edge_nearest + 20 && EXTRACT_COL <= Col_END - 2; EXTRACT_COL++)      //右边沿内倾向外搜索
								{
									if (image[EXTRACT_ROW][EXTRACT_COL + 1] == Black)
									{
										if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL + 1] == Black && image[EXTRACT_ROW][EXTRACT_COL + 2] == Black)   //找到黑白跳变点
										{
											str->RightEdge[EXTRACT_ROW] = EXTRACT_COL + 1;          //储存本行右边沿列坐标

											str->Right_Edge_num++;                                //右边沿个数加一

											R_Link_num++;                                //连续右边沿个数加一

											R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

											R_edge_nearest = EXTRACT_COL + 1;                  //存储最近一个右边沿的列坐标

											R_Trend_in = 0;                                //将右边沿内倾标志置  0

											//R_down_cross_judge();                        //判断是否为十字直角拐点
											/////////////  内倾突然外张，判断为拐点，bug处可以忽略
											if (Inflection_point.R_up_point.x == 0 && Inflection_point.R_down_point.x == 0 && str->RightEdge[EXTRACT_ROW + 1] < Col_END - 4 && EXTRACT_ROW>10 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
											{
												Inflection_point.R_down_point.x = EXTRACT_ROW + 1;                           //存储左边沿十字下方直角拐点行坐标
												Inflection_point.R_down_point.y = str->RightEdge[EXTRACT_ROW + 1];                //存储左边沿十字下方直角拐点列坐标
											}
											break;                                      //本行右边沿搜索结束
										}
									}
								}
								if (R_rowget_flag == 0)      //当右边沿内倾向外未搜到边沿
								{
									R_Link_num = 0;       //连续右边沿个数置  0

									R_Lost_num++;       //当没有搜索到右边沿点时，右边沿丢失数加一

									if (Inflection_point.R_up_point.x == 0 && Inflection_point.R_down_point.x == 0 && str->RightEdge[EXTRACT_ROW + 1] < Col_END - 4 && EXTRACT_ROW>10 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
									{
										Inflection_point.R_down_point.x = EXTRACT_ROW + 1;                           //存储左边沿十字下方直角拐点行坐标
										Inflection_point.R_down_point.y = str->RightEdge[EXTRACT_ROW + 1];                //存储左边沿十字下方直角拐点列坐标
									}
								}
							}
						}
					}
					else       //当右边沿外倾时进入处理
					{
						if (image[EXTRACT_ROW][R_edge_nearest] == Black)     //当上一行右边沿点的上方一点为黑
						{
							for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL >= R_edge_nearest - 20 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)      //右边沿外倾向内搜索
							{
								if (image[EXTRACT_ROW][EXTRACT_COL - 1] == White)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)   //找到黑白跳变点
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;        //储存本行右边沿列坐标

										str->Right_Edge_num++;                                //右边沿个数加一

										R_Link_num++;                                //连续右边沿个数加一

										R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

										R_edge_nearest = EXTRACT_COL;                //存储最近一个右边沿的列坐标

										R_Trend_in = 1;                                //右边沿内倾趋势标志置  1

										break;                                      //本行右边沿搜索结束
									}
								}

							}
							if (R_rowget_flag == 0)      //当右边沿外倾向内未搜到边沿
							{
								if (R_Link_num >= 10 && R_Link_num > L_Link_num)
								{
									str->R_EndLine = EXTRACT_ROW + 1; //将上一行的行数值赋给右边沿结束行数值

									R_Search_end = 1;             //当右边沿外倾时，向左搜索不到白点，判断为右边沿搜索结束
									L_Search_end = 1;             //当右边沿外倾时，向左搜索不到白点，判断为左边沿搜索结束
								}


								R_Link_num = 0;               //连续右边沿个数置  0

								R_Lost_num++;               //当没有搜索到右边沿点时，右边沿丢失数加一
							}
						}
						else          //当上一行右边沿点的上方一点为白
						{
							if (0)    //当右边沿下十字搜索得到
							{
								for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL <= R_edge_nearest + R_OUT_2_OUT && EXTRACT_COL <= Col_END; EXTRACT_COL++)     //开始向外搜索
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black)
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行右边沿列坐标

										str->Right_Edge_num++;                                //右边沿个数加一

										R_Link_num++;                                //连续右边沿个数加一

										R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

										R_edge_nearest = EXTRACT_COL;                  //存储最近一个右边沿的列坐标

										break;                                      //本行右边沿搜索结束
									}
								}
								if (R_rowget_flag == 0)
								{
									R_Link_num = 0;                                //连续右边沿个数置  0

									R_Lost_num++;                                //当右边沿右倾时，向外搜索不到白点，判断为右边沿丢失边沿
								}
							}
							else                    //当右边沿十字标志为0时
							{
								for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL <= R_edge_nearest + 20 && EXTRACT_COL <= Col_END - 2; EXTRACT_COL++)     //开始向外搜索
								{
									if (image[EXTRACT_ROW][EXTRACT_COL + 1] == Black)
									{
										if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL + 1] == Black && image[EXTRACT_ROW][EXTRACT_COL + 2] == Black)   //找到黑白跳变点
										{
											str->RightEdge[EXTRACT_ROW] = EXTRACT_COL + 1;          //储存本行右边沿列坐标

											str->Right_Edge_num++;                                //右边沿个数加一

											R_Link_num++;                                //连续右边沿个数加一

											R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

											R_edge_nearest = EXTRACT_COL + 1;                  //存储最近一个右边沿的列坐标

											break;                                      //本行右边沿搜索结束
										}
									}
								}
								if (R_rowget_flag == 0)
								{
									R_Link_num = 0;                                //连续右边沿个数置  0

									R_Lost_num++;                                //当右边沿右倾时，向外搜索不到白点，判断为右边沿丢失边沿
  //							      ////// 外张且无边界，判断为拐点，基本无问题，断点//
									if (Inflection_point.R_up_point.x == 0 && Inflection_point.R_down_point.x == 0 && str->RightEdge[EXTRACT_ROW + 1] < Col_END - 4 && EXTRACT_ROW>10 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
									{
										for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 5; W_num_row++)  //检测前5行内倾数
										{
											if (str->RightEdge[W_num_row] - str->RightEdge[W_num_row + 1] <= 0)
											{
												Inflection_point.R_down_point.x = W_num_row;               //存储十字拐点行坐标
												Inflection_point.R_down_point.y = str->RightEdge[W_num_row];        //存储十字拐点列坐标
												break;
											}
										}
									}
								}
							}
						}
					}
				}
				else                  //当右边沿没有判断出边沿趋势时进入处理
				{
					if (image[EXTRACT_ROW][R_edge_nearest] == Black)     //当上一行右边沿点的上方一点为黑
					{
						for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL >= R_edge_nearest - 60 && EXTRACT_COL >= Col_START + 3; EXTRACT_COL--)            //开始向内搜索
						{
							if (image[EXTRACT_ROW][EXTRACT_COL - 1] == White)
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White && image[EXTRACT_ROW][EXTRACT_COL - 3] == White)   //找到黑白跳变点
								{
									str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行右边沿列坐标

									str->Right_Edge_num++;                                //右边沿个数加一

									R_Link_num++;                                //连续右边沿个数加一

									R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

									R_edge_nearest = EXTRACT_COL;                  //存储最近一个右边沿的列坐标

	 //						         if(R_up_cross_flag)                        //下拐点找到
	 //						       {
	 //							      //R_up_cross_error();                    //如果右边沿十字标志任为1，进行十字直角拐点判断
	 //						       }

									break;                                      //本行右边沿搜索结束
								}
							}
						}
						if (R_rowget_flag == 0)
						{
							L_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为左边沿搜索结束
							R_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为右边沿搜索结束
							str->R_EndLine = EXTRACT_ROW + 1;//将上一行的行数值赋给左边沿结束行数值
							R_Link_num = 0;               //连续右边沿个数置  0

							R_Lost_num++;               //当没有搜索到右边沿点时，右边沿丢失数加一
						}
					}
					else     //当上一行右边沿点的上方一点为白
					{
						if (image[EXTRACT_ROW][R_edge_nearest - 1] == White && image[EXTRACT_ROW][R_edge_nearest - 2] == White)        //之后两点也为白
						{
							for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL <= R_edge_nearest + 20 && EXTRACT_COL <= Col_END - 2; EXTRACT_COL++)           //开始向外搜索黑白跳变点
							{
								if (image[EXTRACT_ROW][EXTRACT_COL + 1] == Black)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL + 1] == Black && image[EXTRACT_ROW][EXTRACT_COL + 2] == Black)   //找到黑白跳变点
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL + 1;          //储存本行右边沿列坐标

										str->Right_Edge_num++;                                //右边沿个数加一

										R_Link_num++;                                //连续右边沿个数加一

										R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

										R_edge_nearest = EXTRACT_COL + 1;                  //存储最近一个右边沿的列坐标

										break;                                      //本行右边沿搜索结束
									}
								}
							}
							if (R_rowget_flag == 0)
							{
								R_Link_num = 0;       //连续右边沿个数置  0

								R_Lost_num++;       //当没有搜索到右边沿点时，右边沿丢失数加一
							}
						}
						else           //之后两点有黑点,则判断为干扰
						{
							for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL >= R_edge_nearest - 20 && EXTRACT_COL >= Col_START + 5; EXTRACT_COL--)            //开始向外搜索黑白跳变点
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 3] == White && image[EXTRACT_ROW][EXTRACT_COL - 5] == White)   //找到黑白跳变点
								{
									str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行右边沿列坐标

									str->Right_Edge_num++;                                //右边沿个数加一

									R_Link_num++;                                //连续右边沿个数加一

									R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

									R_edge_nearest = EXTRACT_COL;                  //存储最近一个右边沿的列坐标

									break;                                      //本行左边沿搜索结束
								}
							}
							if (R_rowget_flag == 0)
							{
								R_Link_num = 0;               //连续右边沿个数置  0

								R_Lost_num++;               //当没有搜索到右边沿点时，右边沿丢失数加一
							}
						}
					}

					if (R_Link_num >= 3)  //当搜索完这一行后，连续边沿数大于等于3
					{
						if (str->RightEdge[EXTRACT_ROW] <= str->RightEdge[EXTRACT_ROW + 1] && str->RightEdge[EXTRACT_ROW + 1] <= str->RightEdge[EXTRACT_ROW + 2])
						{
							R_Trend_in = 1;     //满足条件，右边沿内倾趋势置  1
						}
						else
						{
							R_Trend_in = 0;     //不满足条件，右边沿内倾趋势置  0
						}
					}
				}
			}
			else                        //当右边沿丢失时进入处理
			{
				if (Cross.Cross_flag == 1) //当右边沿十字标志为1时
				{
					if (Inflection_point.R_down_point.x != 0)    //当检测到右边沿下方十字直角拐点
					{
						if (Inflection_point.R_down_point.y + 10 < Col_END)
						{
							W_num_col = Inflection_point.R_down_point.y + 10;
						}
						else
						{
							W_num_col = Col_END;
						}
						for (EXTRACT_COL = W_num_col; EXTRACT_COL >= Inflection_point.R_down_point.y - 40 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)    //开始搜索
						{
							if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //当搜索到黑白跳变点
							{

								str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行右边沿列坐标

								str->Right_Edge_num++;                                //右边沿个数加一

								R_Link_num++;                                //连续右边沿个数加一

								R_edge_nearest = EXTRACT_COL;                  //存储最近一个右边沿的列坐标

								//R_up_cross_error();                          //右边沿上方直角拐点错
								if (Inflection_point.R_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
								{
									W_num = 0; counting_data = EXTRACT_COL;
									for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //检测前8行内倾数
									{
										for (W_num_col = counting_data; W_num_col >= counting_data - 40 && W_num_col >= Col_START + 2; W_num_col--)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
										{
											if (image[W_num_row][W_num_col] == Black && image[W_num_row][W_num_col - 1] == White && image[W_num_row][W_num_col - 2] == White)
											{
												if (W_num_col - counting_data > -3)
													W_num++;
												counting_data = W_num_col;
												break;
											}
										}
										if (W_num > 0)
										{
											Inflection_point.R_up_point.x = W_num_row;               //存储十字拐点行坐标
											Inflection_point.R_up_point.y = counting_data;        //存储十字拐点列坐标
											break;
										}
									}
								}
								R_Lost_num = 0;                                //连续右边沿丢失数清0


								break;                                      //本行右边沿搜索结束

							}
						}

						if (R_rowget_flag == 0)
						{
							R_Lost_num++;                          //连续右边沿丢失数加1
						}
					}
					else                         //当检测未到右边沿下方十字直角拐点
					{
						for (EXTRACT_COL = Col_END - 1; EXTRACT_COL >= 30 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)    //开始搜索
						{
							if (image[EXTRACT_ROW][EXTRACT_COL + 1] == Black && image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //当搜索到黑白跳变点
							{
								str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行右边沿列坐标

								str->Right_Edge_num++;                                //右边沿个数加一

								R_Link_num++;                                //连续右边沿个数加一

								R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

								R_edge_nearest = EXTRACT_COL;                  //存储最近一个右边沿的列坐标


								R_Lost_num = 0;                                //连续右边沿丢失数清0

								if (R_get_flag == 0)   //左第一次获得线
								{
									R_get_flag = 1;
									str->R_StartLine = EXTRACT_ROW;
								}
								break;                                      //本行右边沿搜索结束
							}
						}
						if (R_rowget_flag == 0)
						{
							R_Lost_num++;                          //连续右边沿丢失数加1
						}
					}
				}
				else                  //当右边沿十字标志为0时
				{
					if (image[EXTRACT_ROW][Col_END] == White && image[EXTRACT_ROW][Col_END - 1] == White && image[EXTRACT_ROW][Col_END - 2] == White)    //当前三个点为白
					{
						if (EXTRACT_ROW == Row_END)
						{
							for (EXTRACT_COL = Col_END - 2; EXTRACT_COL > 40; EXTRACT_COL--)    //开始搜索//刚开始底边丢线没必要搜
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //当搜索到黑白跳变点
								{
									str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行右边沿列坐标

									str->Right_Edge_num++;                                //右边沿个数加一

									R_Link_num++;                                //连续右边沿个数加一

									R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

									R_edge_nearest = EXTRACT_COL;                  //存储最近一个右边沿的列坐标

									R_Lost_num = 0;                                //连续右边沿丢失数清0

									if (R_get_flag == 0)   //左第一次获得线
									{
										R_get_flag = 1;
										str->R_StartLine = EXTRACT_ROW;
									}
									break;                                      //本行右边沿搜索结束
								}
							}
						}
						else
						{
							if (str->LeftEdge[EXTRACT_ROW + 1] < Col_END - 2 && str->LeftEdge[EXTRACT_ROW + 1] != 0)
							{
								for (EXTRACT_COL = Col_END - 2; EXTRACT_COL > str->LeftEdge[EXTRACT_ROW + 1]; EXTRACT_COL--)    //开始搜索//刚开始底边丢线没必要搜
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //当搜索到黑白跳变点
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行右边沿列坐标

										str->Right_Edge_num++;                                //右边沿个数加一

										R_Link_num++;                                //连续右边沿个数加一

										R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

										R_edge_nearest = EXTRACT_COL;                  //存储最近一个右边沿的列坐标

										R_Lost_num = 0;                                //连续右边沿丢失数清0

										if (R_get_flag == 0)   //左第一次获得线
										{
											R_get_flag = 1;
											str->R_StartLine = EXTRACT_ROW;
										}
										break;                                      //本行右边沿搜索结束
									}
								}
							}
							else
							{
								for (EXTRACT_COL = Col_END - 2; EXTRACT_COL > 40; EXTRACT_COL--)    //开始搜索//刚开始底边丢线没必要搜
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //当搜索到黑白跳变点
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //储存本行右边沿列坐标

										str->Right_Edge_num++;                                //右边沿个数加一

										R_Link_num++;                                //连续右边沿个数加一

										R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

										R_edge_nearest = EXTRACT_COL;                  //存储最近一个右边沿的列坐标

										R_Lost_num = 0;                                //连续右边沿丢失数清0

										if (R_get_flag == 0)   //左第一次获得线
										{
											R_get_flag = 1;
											str->R_StartLine = EXTRACT_ROW;
										}
										break;                                      //本行右边沿搜索结束
									}
								}
							}
						}
						if (R_rowget_flag == 0)
						{
							R_Lost_num++;
							if (R_get_flag)
							{
								str->AllRightLose++;
							}
						}
					}
					else         //当前三个点不全为白
					{
						for (EXTRACT_COL = Col_END; EXTRACT_COL >= 30; EXTRACT_COL--)    //开始搜索
						{
							if (image[EXTRACT_ROW][EXTRACT_COL - 1] == White)
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //当搜索到黑白跳变点
								{
									str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;        //储存本行右边沿列坐标

									str->Right_Edge_num++;                                //右边沿个数加一

									R_Link_num++;                                //连续右边沿个数加一

									R_rowget_flag = 1;                                //右边沿搜索得到标志置  1

									R_edge_nearest = EXTRACT_COL;                //存储最近一个右边沿的列坐标

									if (Inflection_point.R_down_point.x != 0 && (R_Lost_num > 8 || R_get_flag == 1) && EXTRACT_COL < Col_END - 10)
									{
										if (Inflection_point.R_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //当要判断的点在范围内
										{
											W_num = 0; counting_data = EXTRACT_COL;
											for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //检测前8行内倾数
											{
												for (W_num_col = counting_data; W_num_col >= counting_data - 40 && W_num_col >= Col_START + 2; W_num_col--)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
												{
													if (image[W_num_row][W_num_col] == Black && image[W_num_row][W_num_col - 1] == White && image[W_num_row][W_num_col - 2] == White)
													{
														if (W_num_col - counting_data > -3)
															W_num++;
														counting_data = W_num_col;
														break;
													}
												}
												if (W_num > 0)
												{
													Inflection_point.R_up_point.x = W_num_row;               //存储十字拐点行坐标
													Inflection_point.R_up_point.y = counting_data;        //存储十字拐点列坐标
													break;
												}
											}
										}
									}
									R_Lost_num = 0;                                //连续右边沿丢失数清0

									if (R_rowget_flag && R_get_flag == 0)   //左第一次获得线
									{
										R_get_flag = 1;
										str->R_StartLine = EXTRACT_ROW;

										if (Inflection_point.R_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5 && EXTRACT_COL < Col_END - 10)      //当要判断的点在范围内
										{
											W_num = 0; counting_data = EXTRACT_COL;
											for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //检测前8行内倾数
											{
												for (W_num_col = counting_data; W_num_col >= counting_data - 40 && W_num_col >= Col_START + 2; W_num_col--)     //在搜索到的黑白跳变点后，向上搜索白点，判断是否为干扰点
												{
													if (image[W_num_row][W_num_col] == Black && image[W_num_row][W_num_col - 1] == White && image[W_num_row][W_num_col - 2] == White)
													{
														if (W_num_col - counting_data > -3)
															W_num++;
														counting_data = W_num_col;
														break;
													}
												}
												if (W_num > 0)
												{
													Inflection_point.R_up_point.x = W_num_row;               //存储十字拐点行坐标
													Inflection_point.R_up_point.y = counting_data;        //存储十字拐点列坐标
													break;
												}
											}
										}
									}
									break;                                      //本行右边沿搜索结束
								}
							}
						}
						if (R_rowget_flag == 0)
						{
							R_Lost_num++;             //当右边沿丢失时，向左搜索不到黑白跳变点，判断为右边沿丢失

							if (EXTRACT_ROW == Row_END)       //在断路时
							{
								L_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为左边沿搜索结束
								R_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为右边沿搜索结束
								str->L_EndLine = EXTRACT_ROW;//将上一行的行数值赋给左边沿结束行数值
							}
						}

					}
				}
			}
		}
		else
		{
			if (R_get_flag == 0)   //右拐时起始线会被屏蔽
			{
				R_get_flag = 1;
				str->R_StartLine = EXTRACT_ROW;
			}
			if (L_TURN == 0 && R_TURN == 0)
			{
				if (image[EXTRACT_ROW][Col_END] == White && image[EXTRACT_ROW][Col_END - 2] == White && image[EXTRACT_ROW][Col_END - 4] == White)    //当前三个点为白
				{
					R_Lost_num++;             //判断为左边沿丢失
				}
			}
		}
#endif

		//误判边线
		if (EXTRACT_ROW > 20 && Allget_f == 0)
		{
			if (L_get_flag == 1 && R_get_flag == 1)
			{
				Allget_f = 1;
				if (str->RightEdge[EXTRACT_ROW] <= str->LeftEdge[EXTRACT_ROW])
				{
					str->RightEdge[EXTRACT_ROW] = 0;
					str->LeftEdge[EXTRACT_ROW] = 0;
					L_Lost_num = 1;
					R_Lost_num = 1;
					L_get_flag = 0;
					R_get_flag = 0;
					Allget_f = 0;
				}

			}
		}
		//	     if(str->RightEdge[EXTRACT_ROW]!=0&&str->LeftEdge[EXTRACT_ROW]!=0&&EXTRACT_ROW<10)
		//	     {
		//		    if( str->RightEdge[EXTRACT_ROW]- str->LeftEdge[EXTRACT_ROW]<5 )
		//		    {
		//			   str->L_EndLine=EXTRACT_ROW;    //将上一行的行数值赋给左边沿结束行数值
		//			   str->R_EndLine=EXTRACT_ROW; //将上一行的行数值赋给右边沿结束行数值
		//
		//			   L_Search_end=1;             //当左边沿右倾时，向内搜索不到白点，判断为左边沿搜索结束
		//			   R_Search_end=1;             //当左边沿右倾时，向内搜索不到白点，判断为右边沿搜索结束
		//		    }
		//	     }

				 //结束行处理
		if (EXTRACT_ROW == ROW_START && (L_Search_end == 0 || R_Search_end == 0)) //最后一行
		{
			str->L_EndLine = EXTRACT_ROW;    //将上一行的行数值赋给左边沿结束行数值
			str->R_EndLine = EXTRACT_ROW; //将上一行的行数值赋给右边沿结束行数值

			L_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为左边沿搜索结束
			R_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为右边沿搜索结束
		}


		//行丢线判断
#if 1
		if (str->LeftEdge[EXTRACT_ROW] != 0 && str->RightEdge[EXTRACT_ROW] != 0)     //左右都有边界
		{
			str->RowLose[EXTRACT_ROW] = 1;
		}
		else if (str->LeftEdge[EXTRACT_ROW] != 0 && str->RightEdge[EXTRACT_ROW] == 0)//find left
		{
			str->RowLose[EXTRACT_ROW] = 2;
		}
		else if (str->LeftEdge[EXTRACT_ROW] == 0 && str->RightEdge[EXTRACT_ROW] != 0)//find right
		{
			str->RowLose[EXTRACT_ROW] = 3;
		}
		else if (str->LeftEdge[EXTRACT_ROW] == 0 && str->RightEdge[EXTRACT_ROW] == 0)//两边丢线   d待处理
		{
			str->RowLose[EXTRACT_ROW] = 4;
		}
#endif

		//左边先搜  可能右边结束   去除左边这一行搜到的
		if (R_Search_end == 1 && str->LeftEdge[EXTRACT_ROW] != 0)
			str->LeftEdge[EXTRACT_ROW] = 0;


#if 0
		if (L_TURN == 1 || R_TURN == 1)
		{
			L_cross_flag = 0;      //当符合条件时，判断为边沿突变，左边沿十字标志置  1
			R_cross_flag = 0;      //当符合条件时，判断为边沿突变，左边沿十字标志置  1

			//强制为弯道 取消十字与圆岛处理

			str->L_EndLine = EXTRACT_ROW;    //将上一行的行数值赋给左边沿结束行数值
			str->R_EndLine = EXTRACT_ROW; //将上一行的行数值赋给右边沿结束行数值

			L_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为左边沿搜索结束
			R_Search_end = 1;             //当左边沿右倾时，向内搜索不到白点，判断为右边沿搜索结束
		}

#endif


	}//整幅图像每行扫描完成

	if (R_Search_end == 1 && L_Search_end == 1)
	{
		if (str->L_EndLine != 0)
			str->EndLine = str->L_EndLine;//将上一行的行数值赋给左边沿结束行数值
		else if (str->R_EndLine != 0)
			str->EndLine = str->R_EndLine;//将上一行的行数值赋给左边沿结束行数值


	}

	if (ABS(str->LastEndLine - str->EndLine) > 3)
	{
		str->EndLine = (str->LastEndLine + str->EndLine) / 2;
	}
	str->LastEndLine = str->EndLine;


	//开始行处理
	str->StartLine = MAX(str->L_StartLine, str->R_StartLine);


#if 0
	cross_edge_add();     //十字边沿补充（修正十字错误边沿）

		  //   edge_error_process(); //边沿错误处理（修正普通赛道错误边沿，修正左右拐，识别S弯）
	   //
	road_judge();         //判断赛道类型（识别直道，坡道，小S，出弯口）
//
//
//
//      road_num();           //赛道类型计数（十字，障碍物个数）
//

#endif
}



