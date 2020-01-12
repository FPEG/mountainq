/*!
 * @file       Extract_Edge.c
 * @brief      �߽���ȡ����
 * @author
 * @version
 * @date       2019-05-21
 */

#include "include.h"
#include "Extract_Edge.h"


Str_Boundary  Boundary;  //�߽�ṹ��
Str_Inflection_point    Inflection_point;   //�յ�ṹ��

/*��ʱ����*/


unsigned char EXTRACT_ROW;//��ǰ������
unsigned char EXTRACT_COL;//��ǰ�����У�
unsigned char W_num;
unsigned char W_num_row;
unsigned char W_num_col;
unsigned char counting_data;

//unsigned char L_cross_able=0;
//unsigned char R_cross_able=0;

unsigned char L_Link_num = 0;        //����������ظ�������ֵ  0
unsigned char R_Link_num = 0;        //�������ұ��ظ�������ֵ  0

unsigned char Allget_f = 0;
/***************************************************************
�����߽�

* �������ƣ�extract_edge
* ����˵���������߽�
* ����˵����
* �������أ�void
* �޸�ʱ�䣺 7.4   00.35
* �� ע��
***************************************************************/
void extract_edge(Str_Boundary* str, unsigned char Row_START, unsigned char Row_END, unsigned char Col_START, unsigned char Col_END)       //�˺���Ϊ������ȡ������
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

	///*���±���������Ҫ�����ⲿ����*/
	str->OnlyLeftLose = 0;                    //��߶�ʧ�����
	str->OnlyRightLose = 0;
	str->AllLeftLose = 0;
	str->AllRightLose = 0;

	/*��ʼ�����*/
	str->L_EndLine = 0;                //�������������������  -1
	str->R_EndLine = 0;                //�ұ�����������������  -1

	str->L_StartLine = 0;
	str->R_StartLine = 0;


	str->Left_Edge_num = 0;
	str->Right_Edge_num = 0;



	unsigned char L_Search_end = 0;              //���������������־��  0
	unsigned char R_Search_end = 0;              //�ұ�������������־��  0

/*����������־λ*/
	unsigned char L_Trend_in = 0;        //��������������жϱ�־��  0
	unsigned char R_Trend_in = 0;        //�ұ������������жϱ�־��  0

	unsigned char L_rowget_flag = 0;        //����������õ���־��  0
	unsigned char R_rowget_flag = 0;        //�ұ��������õ���־��  0

	unsigned char L_Lost_num = 1;        //�����������ʧ������  1���ȶ���Ϊ��ʧ���أ�
	unsigned char R_Lost_num = 1;        //�����������ʧ������  1���ȶ���Ϊ��ʧ���أ�

	unsigned char L_get_flag = 0;
	unsigned char R_get_flag = 0;
	//       L_down_end_row=0;           //�����������������������  -1
	//       R_down_end_row=0;           //�ұ���������������������  -1


		  // END_LINE_FLAG=0;             //�յ��߱�־��  0

	unsigned char L_TURN = 0;                    //�����־��  0
	unsigned char R_TURN = 0;                    //�����־��  0
//       curve_in_row=0;             //����������  -1
//       curve_car_row=0;            //���ǰ������  -1

//              S_FLAG=0;                    //S�� ������־��  0
//              s_curve_row=0;              //S�� ����������  0

//       STRAIGHT_FLAG=0;             //ֱ����־��  0
//       little_s_flag=0;             //СS�������־��  0
//       RAMP_FLAG=0;                 //�µ���־��  0


	   // /*���±�������Ҫ�����ⲿ����*/

	   //      unsigned char L_cross_able=1;
	   //      unsigned char R_cross_able=1;

	   //       end_bar_able=1;//�յ����ϰ���ʹ�ܱ�־
	unsigned char L_edge_nearest;
	unsigned char R_edge_nearest;


	Allget_f = 0;

	L_Link_num = 0;
	R_Link_num = 0;
	/**********************************************��⿪ʼ***********************************************************/
	for (EXTRACT_ROW = Row_END; EXTRACT_ROW > Row_START; EXTRACT_ROW--)       //�������ͼ��ı���
	{
		L_rowget_flag = 0;               //������������õ���־��0
		R_rowget_flag = 0;               //���ұ��������õ���־��0

		/*���³����Ϊ����ؼ�����*/

		if (L_Search_end == 0)       //����������� δ���� ʱ����
		{
			if (L_Lost_num == 0)         //�������û�� ��ʧʱ ���봦��
			{
				if (L_Link_num > 3)   //����������������ڵ���3���жϳ���������ʱ���봦��     //link_num�ܺõķ�ӳ���߳���
				{
					if (L_Trend_in == 1)    //�����������ʱ���봦��
					{
						if (image[EXTRACT_ROW][L_edge_nearest] == Black)     //����һ������ص���Ϸ�һ��Ϊ��
						{
							//�޸Ĵ˲�������ҲҪ��
							for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL <= L_edge_nearest + 20 && EXTRACT_COL <= Col_END - 2; EXTRACT_COL++)      //�����������������
							{
								if (image[EXTRACT_ROW][EXTRACT_COL + 1] == White)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //�ҵ��ڰ������  �׵�
									{
										str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;    //���汾�������������

										if (str->RightEdge[EXTRACT_ROW + 1] < str->LeftEdge[EXTRACT_ROW])     //Ԥ�ж�  ��ʡʱ��  �ж�����ѵ��������ұ߽��ұ�
										{
											if (str->RightEdge[EXTRACT_ROW + 1] != 0 && str->RightEdge[EXTRACT_ROW + 2] != 0 && str->RightEdge[EXTRACT_ROW + 2] < str->LeftEdge[EXTRACT_ROW]) //�����ж�
											{
												L_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������

												R_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������

												str->L_EndLine = EXTRACT_ROW;//����һ�е�����ֵ��������ؽ�������ֵ

												L_Link_num = 0;               //��������ظ�����  0

												L_Lost_num++;               //��û������������ص�ʱ������ض�ʧ����һ

											}
										}

										str->Left_Edge_num++;                        //����ظ�����һ

										L_Link_num++;                                 //��������ظ�����һ

										L_rowget_flag = 1;                            //����������õ���־��  1

										L_edge_nearest = EXTRACT_COL;                 //�洢���һ������ص�������

										break;                                       //�����������������
									}
								}
							}
							if (L_rowget_flag == 0)        //���������������δ�ѵ�����   �ܿ�������
							{
								if (L_Link_num >= 7)
								{
									W_num = 0;
									for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 10 && W_num_row <= Row_END - 1; W_num_row++)      //zuo����������������
									{
										if (str->LeftEdge[W_num_row] < str->LeftEdge[W_num_row + 1])
										{
											W_num++;
										}
									}
									if (W_num <= 1 && str->AllRightLose > 5 && R_Link_num < 3) //�ұ߶���
									{
										R_TURN = 1;                   //�����־��1
									}
									else if (W_num <= 1 && R_Link_num >= 7)  //��·�ж�
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

								L_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������
								R_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
								str->L_EndLine = EXTRACT_ROW;//����һ�е�����ֵ��������ؽ�������ֵ

								L_Link_num = 0;               //��������ظ�����  0

								L_Lost_num++;               //��û������������ص�ʱ������ض�ʧ����һ
							}
						}
						else      //����һ������ص���Ϸ�һ��Ϊ��
						{
							if (image[EXTRACT_ROW][L_edge_nearest + 1] != White || image[EXTRACT_ROW][L_edge_nearest + 2] != White)
							{
								for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL <= L_edge_nearest + 20 && EXTRACT_COL <= Col_END - 1; EXTRACT_COL++)      //�����������������
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White)      //�ҵ��ڰ������  �׵�
									{
										str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;    //���汾�������������

										str->Left_Edge_num++;                        //����ظ�����һ

										L_Link_num++;                                 //��������ظ�����һ

										L_rowget_flag = 1;                            //����������õ���־��  1

										L_edge_nearest = EXTRACT_COL;                 //�洢���һ������ص�������

										break;                                       //�����������������
									}
								}
								if (L_rowget_flag == 0)        //���������������δ�ѵ�����   �ܿ�������
								{
									if (L_Link_num >= 10)
									{
										W_num = 0;
										for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 10 && W_num_row <= Row_END - 1; W_num_row++)      //zuo����������������
										{
											if (str->LeftEdge[W_num_row] < str->LeftEdge[W_num_row + 1])
											{
												W_num++;
											}
										}
										if (W_num <= 1 && str->AllRightLose > 5 && R_Link_num < 3) //�ұ߶���
										{
											R_TURN = 1;                   //�����־��1
										}
										else if (W_num <= 1 && R_Link_num >= 7)  //��·�ж�
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

									L_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������
									R_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
									str->L_EndLine = EXTRACT_ROW;//����һ�е�����ֵ��������ؽ�������ֵ

									L_Link_num = 0;               //��������ظ�����  0

									L_Lost_num++;               //��û������������ص�ʱ������ض�ʧ����һ
								}
							}
							else
							{
								for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL >= L_edge_nearest - 20 && EXTRACT_COL > Col_START + 2; EXTRACT_COL--)      //�����������nei����
								{
									if (image[EXTRACT_ROW][EXTRACT_COL - 1] == Black)
									{
										if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL - 1] == Black && image[EXTRACT_ROW][EXTRACT_COL - 2] == Black)
										{
											str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL - 1;          //���汾�������������

											str->Left_Edge_num++;                                //����ظ�����һ

											L_Link_num++;                                //��������ظ�����һ

											L_rowget_flag = 1;                                //����������õ���־��  1

											L_edge_nearest = EXTRACT_COL - 1;                  //�洢���һ������ص�������

											L_Trend_in = 0;                                //������������־��  0
											/////////////  ����ͻȻ���ţ��ж�Ϊ�յ㣬bug�����Ժ���
											//  L_down_cross_judge();                        //
											if (Inflection_point.L_up_point.x == 0 && Inflection_point.L_down_point.x == 0 && str->LeftEdge[EXTRACT_ROW + 1] > 4 && EXTRACT_ROW > 10 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
											{
												Inflection_point.L_down_point.x = EXTRACT_ROW + 1;                           //�洢�����ʮ���·�ֱ�ǹյ�������
												Inflection_point.L_down_point.y = str->LeftEdge[EXTRACT_ROW + 1];                //�洢�����ʮ���·�ֱ�ǹյ�������
											}
											break;                                      //�����������������
										}
									}
								}
								if (L_rowget_flag == 0)      //���������������δ�ѵ�����
								{
									L_Link_num = 0;           //��������ظ�����  0

									L_Lost_num++;           //��û������������ص�ʱ������ض�ʧ����һ
									//////  ����ͻȻ�������ޱ߽磬�ж�Ϊ�յ㣬���������⣬�ϵ�//
									if (Inflection_point.L_up_point.x == 0 && Inflection_point.L_down_point.x == 0 && str->LeftEdge[EXTRACT_ROW + 1] > 4 && EXTRACT_ROW > 10 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
									{
										Inflection_point.L_down_point.x = EXTRACT_ROW + 1;                           //�洢�����ʮ���·�ֱ�ǹյ�������
										Inflection_point.L_down_point.y = str->LeftEdge[EXTRACT_ROW + 1];                //�洢�����ʮ���·�ֱ�ǹյ�������
									}
								}
							}
						}
					}
					else  //�����������ʱ���봦��
					{
						if (image[EXTRACT_ROW][L_edge_nearest] == Black)     //����һ������ص���Ϸ�һ��Ϊ��
						{
							for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL <= L_edge_nearest + 20 && EXTRACT_COL <= Col_END - 2; EXTRACT_COL++)      //�����������������
							{
								if (image[EXTRACT_ROW][EXTRACT_COL + 1] == White)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)  //�ҵ��ڰ������
									{
										str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;        //���汾�������������

										str->Left_Edge_num++;                                //����ظ�����һ

										L_Link_num++;                                //��������ظ�����һ

										L_rowget_flag = 1;                                //����������õ���־��  1

										L_edge_nearest = EXTRACT_COL;                //�洢���һ������ص�������

										L_Trend_in = 1;                                //������������Ʊ�־��  1
#if 0  //Ч�����۲찡
										Inflection_point.L_down_point.x = 0;                           //�洢�����ʮ���·�ֱ�ǹյ�������
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

										break;                                      //�����������������
									}
								}
							}
							if (L_rowget_flag == 0)      //���������������δ�ѵ�����
							{
								L_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������
								R_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
								str->L_EndLine = EXTRACT_ROW;    //����һ�е�����ֵ��������ؽ�������ֵ
								L_Link_num = 0;               //��������ظ�����  0
								L_Lost_num++;               //��û������������ص�ʱ������ض�ʧ����һ
							}
						}
						else          //����һ������ص���Ϸ�һ��Ϊ��
						{
							for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL >= L_edge_nearest - 20 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)     //��ʼ��������
							{
								if (image[EXTRACT_ROW][EXTRACT_COL - 1] == Black)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL - 1] == Black && image[EXTRACT_ROW][EXTRACT_COL - 2] == Black)
									{
										str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL - 1;          //���汾�������������

										str->Left_Edge_num++;                                //����ظ�����һ

										L_Link_num++;                                //��������ظ�����һ

										L_rowget_flag = 1;                                //����������õ���־��  1

										L_edge_nearest = EXTRACT_COL - 1;                  //�洢���һ������ص�������

										break;                                      //�����������������
									}
								}
							}
							if (L_rowget_flag == 0)
							{
								L_Link_num = 0;                                //��������ظ�����  0

								L_Lost_num++;                                //�����������ʱ���������������׵㣬����ض�ʧ����һ

								////// �������ޱ߽磬�ж�Ϊ�յ㣬���������⣬�ϵ�//
								if (Inflection_point.L_up_point.x == 0 && Inflection_point.L_down_point.x == 0 && str->LeftEdge[EXTRACT_ROW + 1] > 4 && EXTRACT_ROW > 10 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
								{
									for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 5; W_num_row++)  //���ǰ8��������
									{
										if (str->LeftEdge[W_num_row] - str->LeftEdge[W_num_row + 1] >= 0)
										{
											Inflection_point.L_down_point.x = W_num_row;               //�洢ʮ�ֹյ�������
											Inflection_point.L_down_point.y = str->LeftEdge[W_num_row];        //�洢ʮ�ֹյ�������
											break;
										}
									}
								}

							}

						}
					}
				}
				else                  //�������û���жϳ���������ʱ���봦��
				{
					if (image[EXTRACT_ROW][L_edge_nearest] == Black)     //����һ������ص���Ϸ�һ��Ϊ��
					{                                                                      //60
						for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL <= L_edge_nearest + 60 && EXTRACT_COL < Col_END - 2; EXTRACT_COL++)            //��ʼ��������
						{
							if (image[EXTRACT_ROW][EXTRACT_COL + 1] == White)
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)   //�ҵ��ڰ������
								{
									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾�������������

									str->Left_Edge_num++;                                //����ظ�����һ

									L_Link_num++;                                //��������ظ�����һ

									L_rowget_flag = 1;                                //����������õ���־��  1

									L_edge_nearest = EXTRACT_COL;                  //�洢���һ������ص�������

									break;                                      //�����������������
								}


							}
						}

						if (L_rowget_flag == 0)
						{
							L_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������
							R_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
							str->L_EndLine = EXTRACT_ROW;//����һ�е�����ֵ��������ؽ�������ֵ
							L_Link_num = 0;               //��������ظ�����  0
							//�п��ܽ���λ
							L_Lost_num++;               //��û������������ص�ʱ������ض�ʧ����һ
						}
					}
					else     //����һ������ص���Ϸ�һ��Ϊ��
					{
						if (image[EXTRACT_ROW][L_edge_nearest + 1] == White && image[EXTRACT_ROW][L_edge_nearest + 2] == White)        //֮������ҲΪ��
						{
							for (EXTRACT_COL = L_edge_nearest; EXTRACT_COL >= L_edge_nearest - L_EDGE_OUT && EXTRACT_COL >= Col_START + 1; EXTRACT_COL--)           //��ʼ���������ڰ������
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL - 1] == Black && image[EXTRACT_ROW][EXTRACT_COL - 2] == Black)   //�ҵ��ڰ������
								{
									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL - 1;          //���汾�������������

									str->Left_Edge_num++;                                //����ظ�����һ

									L_Link_num++;                                //��������ظ�����һ

									L_rowget_flag = 1;                                //����������õ���־��  1

									L_edge_nearest = EXTRACT_COL - 1;                  //�洢���һ������ص�������

									break;                                      //�����������������
								}
							}
							if (L_rowget_flag == 0)
							{
								L_Link_num = 0;       //��������ظ�����  0
								//�п���
								L_Lost_num++;       //��û������������ص�ʱ������ض�ʧ����һ
							}
						}
						else           //֮�������кڵ�,���ж�Ϊ����
						{       /*������cl*/
							for (EXTRACT_COL = L_edge_nearest + 1; EXTRACT_COL <= L_edge_nearest + L_EDGE_IN; EXTRACT_COL++)            //��ʼ���������ڰ������
							{
								if (EXTRACT_COL > Col_END - 5)    //����������ͼ��Χʱ����������
								{
									break;
								}
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 3] == White && image[EXTRACT_ROW][EXTRACT_COL + 5] == White)   //�ҵ��ڰ������
								{
									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾�������������

									str->Left_Edge_num++;                                //����ظ�����һ

									L_Link_num++;                                //��������ظ�����һ

									L_rowget_flag = 1;                                //����������õ���־��  1

									L_edge_nearest = EXTRACT_COL;                  //�洢���һ������ص�������

									break;                                      //�����������������
								}
							}
							if (L_rowget_flag == 0)
							{
								L_Link_num = 0;               //��������ظ�����  0

								L_Lost_num++;               //��û������������ص�ʱ������ض�ʧ����һ
							}
						}
					}

					if (L_Link_num >= 3)  //����������һ�к��������������ڵ���3
					{
						if (str->LeftEdge[EXTRACT_ROW] >= str->LeftEdge[EXTRACT_ROW + 1] || str->LeftEdge[EXTRACT_ROW] >= str->LeftEdge[EXTRACT_ROW + 2])
						{
							L_Trend_in = 1;     //�������������������������  1
						}
						else
						{
							L_Trend_in = 0;     //���������������������������  0
						}
					}
				}
			}
			else                        //������ض�ʧʱ���봦��
			{
				if (Cross.Cross_flag == 1) //�������ʮ�ֱ�־Ϊ1ʱ
				{
					/*
					 * //����⵽������·�ʮ��ֱ�ǹյ�
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
							for (EXTRACT_COL = W_num_col; EXTRACT_COL <= Inflection_point.L_down_point.y + 40 && EXTRACT_COL <= Col_END - 3; EXTRACT_COL++)    //��ʼ����
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //���������ڰ������
								{

									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾�������������

									str->Left_Edge_num++;                                //����ظ�����һ

									L_Link_num++;                                //��������ظ�����һ

									L_edge_nearest = EXTRACT_COL;                  //�洢���һ������ص�������

									//L_up_cross_error();                          //������Ϸ�ֱ�ǹյ�������
									if (Inflection_point.L_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
									{
										W_num = 0; counting_data = EXTRACT_COL;
										for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //���ǰ8��������
										{
											for (W_num_col = counting_data; W_num_col <= counting_data + 40 && W_num_col <= Col_END - 2; W_num_col++)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
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
												Inflection_point.L_up_point.x = W_num_row;               //�洢ʮ�ֹյ�������
												Inflection_point.L_up_point.y = counting_data;        //�洢ʮ�ֹյ�������
												break;
											}
										}
									}

									L_Lost_num = 0;                                //��������ض�ʧ����0



									break;                                      //�����������������

								}
							}
						}
						else
						{
							for (EXTRACT_COL = W_num_col; EXTRACT_COL <= Inflection_point.L_down_point.y + 40 && EXTRACT_COL <= Col_END - 3; EXTRACT_COL++)    //��ʼ����
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White)      //���������ڰ������
								{
									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾�������������

									str->Left_Edge_num++;                                //����ظ�����һ

									L_Link_num++;                                //��������ظ�����һ

									L_edge_nearest = EXTRACT_COL;                  //�洢���һ������ص�������

									if (Inflection_point.L_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
									{
										W_num = 0; counting_data = EXTRACT_COL;
										for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //���ǰ8��������
										{
											for (W_num_col = counting_data; W_num_col <= counting_data + 40 && W_num_col <= Col_END - 2; W_num_col++)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
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
												Inflection_point.L_up_point.x = W_num_row;               //�洢ʮ�ֹյ�������
												Inflection_point.L_up_point.y = counting_data;        //�洢ʮ�ֹյ�������
												break;
											}
										}
									}

									L_Lost_num = 0;                                //��������ض�ʧ����0

									break;                                      //�����������������
								}
							}
						}

						if (L_rowget_flag == 0)
						{
							L_Lost_num++;                          //��������ض�ʧ����1
						}
					}
					else                         //��δ��⵽������·�ʮ��ֱ�ǹյ�  //clδ�� ����Ҫ�о���̫��
					{
						for (EXTRACT_COL = Col_START; EXTRACT_COL <= 60 && EXTRACT_COL <= Col_END - 3; EXTRACT_COL++)    //��ʼ����
						{
							if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 3] == White)      //���������ڰ������
							{

								str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾�������������

								str->Left_Edge_num++;                                //����ظ�����һ

								L_Link_num++;                                //��������ظ�����һ

								L_rowget_flag = 1;                                //����������õ���־��  1

								L_edge_nearest = EXTRACT_COL;                  //�洢���һ������ص�������

								L_Lost_num = 0;                                //��������ض�ʧ����

								if (L_rowget_flag && L_get_flag == 0)   //���һ�λ����
								{
									L_get_flag = 1;
									str->L_StartLine = EXTRACT_ROW;
								}
								break;                                                  //�����������������
							}
						}
						if (L_rowget_flag == 0)
						{
							L_Lost_num++;                          //��������ض�ʧ����1

						}
					}
				}
				else                  //�������ʮ�ֱ�־Ϊ0ʱ
				{
					if (image[EXTRACT_ROW][Col_START] == White && image[EXTRACT_ROW][Col_START + 1] == White && image[EXTRACT_ROW][Col_START + 2] == White)    //��ǰ������Ϊ��
					{
						if (EXTRACT_ROW == Row_END)
						{
							for (EXTRACT_COL = Col_START + 2; EXTRACT_COL < 40; EXTRACT_COL++)    //��ʼ����  �տ�ʼ�ױ߶���you��Ҫ�� ����bug
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black)      //���������ڰ������
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //���������ڰ������
									{
										W_num = 0;            //����׿����ֵ                 8
										for (W_num_col = EXTRACT_ROW - 1; W_num_col >= EXTRACT_ROW - 4 && W_num_col > Row_START; W_num_col--)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
										{
											if (image[W_num_col][EXTRACT_COL] == White)
											{
												W_num++;
											}
										}
										if (W_num < 2)
										{
											str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾�������������

											str->Left_Edge_num++;                                //����ظ�����һ

											L_Link_num++;                                //��������ظ�����һ

											L_rowget_flag = 1;                                //����������õ���־��  1

											L_edge_nearest = EXTRACT_COL;                  //�洢���һ������ص�������

											L_Lost_num = 0;                                //��������ض�ʧ����

											if (L_rowget_flag && L_get_flag == 0)   //���һ�λ����
											{
												L_get_flag = 1;
												str->L_StartLine = EXTRACT_ROW;
											}
											break;                                      //�����������������
										}
									}
								}
							}
							if (L_rowget_flag == 0)   //δ�ѵ�
							{
								L_Lost_num++;
							}

						}
						else
						{
							if (str->RightEdge[EXTRACT_ROW + 1] > Col_START + 2)
							{
								for (EXTRACT_COL = Col_START + 2; EXTRACT_COL < str->RightEdge[EXTRACT_ROW + 1]; EXTRACT_COL++)    //��ʼ����  �տ�ʼ�ױ߶���you��Ҫ�� ����bug
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //���������ڰ������
									{
										W_num = 0;            //����׿����ֵ                 8
										for (W_num_col = EXTRACT_ROW - 1; W_num_col >= EXTRACT_ROW - 4 && W_num_col > Row_START; W_num_col--)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
										{
											if (image[W_num_col][EXTRACT_COL] == White)
											{
												W_num++;
											}
										}
										if (W_num < 2)
										{
											str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾�������������

											str->Left_Edge_num++;                                //����ظ�����һ

											L_Link_num++;                                //��������ظ�����һ

											L_rowget_flag = 1;                                //����������õ���־��  1

											L_edge_nearest = EXTRACT_COL;                  //�洢���һ������ص�������

											L_Lost_num = 0;                                //��������ض�ʧ����

											if (L_rowget_flag && L_get_flag == 0)   //���һ�λ����
											{
												L_get_flag = 1;
												str->L_StartLine = EXTRACT_ROW;
											}
											break;                                      //�����������������
										}

									}

								}
							}
							else
							{
								for (EXTRACT_COL = Col_START + 2; EXTRACT_COL < 40; EXTRACT_COL++)    //��ʼ����  �տ�ʼ�ױ߶���you��Ҫ�� ����bug
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //���������ڰ������
									{
										W_num = 0;            //����׿����ֵ                 8
										for (W_num_col = EXTRACT_ROW - 1; W_num_col >= EXTRACT_ROW - 4 && W_num_col > Row_START; W_num_col--)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
										{
											if (image[W_num_col][EXTRACT_COL] == White)
											{
												W_num++;
											}
										}
										if (W_num < 2)
										{
											str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾�������������

											str->Left_Edge_num++;                                //����ظ�����һ

											L_Link_num++;                                //��������ظ�����һ

											L_rowget_flag = 1;                                //����������õ���־��  1

											L_edge_nearest = EXTRACT_COL;                  //�洢���һ������ص�������

											L_Lost_num = 0;                                //��������ض�ʧ����

											if (L_rowget_flag && L_get_flag == 0)   //���һ�λ����
											{
												L_get_flag = 1;
												str->L_StartLine = EXTRACT_ROW;
											}
											break;                                      //�����������������
										}

									}

								}
							}
						}
						if (L_rowget_flag == 0)  //������ض�ʧʱ
						{
							L_Lost_num++;
							if (L_get_flag)
							{
								str->AllLeftLose++;
							}
						}

						//    ����·���
   //					 if(Break_Road.Break_Road_Flag==1)
   //					 {
   //						if(EXTRACT_ROW==Row_END)
   //						{
   //						       Break_Road.Break_Road_Flag=0;
   //						}
   //					 }
					}
					else         //��ǰ�����㲻ȫΪ��
					{
						for (EXTRACT_COL = Col_START; EXTRACT_COL < Col_END - 2; EXTRACT_COL++)    //��ʼ����
						{
							if (image[EXTRACT_ROW][EXTRACT_COL + 1] == White)
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL + 1] == White && image[EXTRACT_ROW][EXTRACT_COL + 2] == White)      //���������ڰ������
								{
									str->LeftEdge[EXTRACT_ROW] = EXTRACT_COL;        //���汾�������������

									str->Left_Edge_num++;                                //����ظ�����һ

									L_Link_num++;                                //��������ظ�����һ

									L_rowget_flag = 1;                                //����������õ���־��  1

									L_edge_nearest = EXTRACT_COL;                //�洢���һ������ص�������


									if (Inflection_point.L_down_point.x != 0 && (L_Lost_num > 8 || L_get_flag == 1) && EXTRACT_COL > 10)
									{
										//L_up_cross_error();
										if (Inflection_point.L_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
										{
											W_num = 0; counting_data = EXTRACT_COL;
											for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //���ǰ8��������
											{
												for (W_num_col = counting_data; W_num_col <= counting_data + 40 && W_num_col <= Col_END - 2; W_num_col++)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
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
													Inflection_point.L_up_point.x = W_num_row;               //�洢ʮ�ֹյ�������
													Inflection_point.L_up_point.y = counting_data;        //�洢ʮ�ֹյ�������
													break;
												}
											}
										}
									}
									L_Lost_num = 0;                                //��������ض�ʧ����0
									if (L_get_flag == 0)   //���һ�λ����
									{
										L_get_flag = 1;
										str->L_StartLine = EXTRACT_ROW - 1;

										if (Inflection_point.L_up_point.x == 0 && EXTRACT_COL > 10 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
										{
											W_num = 0; counting_data = EXTRACT_COL;
											for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //���ǰ8��������
											{
												for (W_num_col = counting_data; W_num_col <= counting_data + 40 && W_num_col <= Col_END - 2; W_num_col++)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
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
													Inflection_point.L_up_point.x = W_num_row;               //�洢ʮ�ֹյ�������
													Inflection_point.L_up_point.y = W_num_col;        //�洢ʮ�ֹյ�������
													break;
												}
											}
										}
									}
									break;                                      //�����������������

									//    ����·���
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
							L_Lost_num++;             //������ض�ʧʱ���������������ڰ�����㣬�ж�Ϊ����ض�ʧ


							if (EXTRACT_ROW == Row_END)       //�ڶ�·ʱ
							{
								L_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������
								R_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
								str->L_EndLine = EXTRACT_ROW;//����һ�е�����ֵ��������ؽ�������ֵ
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
				if (image[EXTRACT_ROW][Col_START] == White && image[EXTRACT_ROW][Col_START + 2] == White && image[EXTRACT_ROW][Col_START + 4] == White)    //��ǰ������Ϊ��
				{
					L_Lost_num++;             //�ж�Ϊ����ض�ʧ
				}
			}
		}
#if 1
		/*���³����Ϊ�ұ��ؼ�����*/

		if (R_Search_end == 0)       //���ұ�������δ����ʱ����
		{
			if (R_Lost_num == 0)         //���ұ���û�ж�ʧʱ���봦��
			{
				if (R_Link_num >= 3)   //���ұ������������ڵ���3���жϳ���������ʱ���봦��
				{
					if (R_Trend_in == 1)       //���ұ�������ʱ���봦��
					{
						if (image[EXTRACT_ROW][R_edge_nearest] == Black)     //����һ���ұ��ص���Ϸ�һ��Ϊ��
						{
							for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL >= R_edge_nearest - 20 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)      //�ұ���������������
							{
								if (image[EXTRACT_ROW][EXTRACT_COL - 1] == White)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)   //�ҵ��ڰ������
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;       //���汾���ұ���������

										str->Right_Edge_num++;                                //�ұ��ظ�����һ

										R_Link_num++;                                //�����ұ��ظ�����һ

										R_rowget_flag = 1;                                //�ұ��������õ���־��  1

										R_edge_nearest = EXTRACT_COL;               //�洢���һ���ұ��ص�������


										break;                                      //�����ұ�����������
									}
								}
							}
							if (R_rowget_flag == 0)      //���ұ�����������δ�ѵ�����
							{
								if (R_Link_num >= 10)
								{
									W_num = 0;
									for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 10 && W_num_row < Row_END; W_num_row++)      //�ұ���������������
									{
										if (str->RightEdge[W_num_row] > str->RightEdge[W_num_row + 1])
										{
											W_num++;
										}
									}
									if (W_num <= 1 && str->AllLeftLose > 5 && L_Link_num < 3)
									{
										L_TURN = 1;                //�����־��  1
									}
									else   if (W_num <= 1 && L_Link_num >= 7)  //��·�ж�
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
								str->R_EndLine = EXTRACT_ROW + 1; //����һ�е�����ֵ�����ұ��ؽ�������ֵ

								str->LeftEdge[EXTRACT_ROW] = 0;
								R_Search_end = 1;             //���ұ�������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
								L_Search_end = 1;             //���ұ�������ʱ���������������׵㣬�ж�Ϊ�������������

								R_Link_num = 0;               //�����ұ��ظ�����  0
								R_Lost_num++;               //��û���������ұ��ص�ʱ���ұ��ض�ʧ����һ
							}
						}
						else                                                   //����һ���ұ��ص���Ϸ�һ��Ϊ��
						{
							if (image[EXTRACT_ROW][R_edge_nearest - 1] != White || image[EXTRACT_ROW][R_edge_nearest - 2] != White)
							{
								for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL >= R_edge_nearest - 20 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)      //�ұ���������������
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)   //�ҵ��ڰ������
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;       //���汾���ұ���������

										str->Right_Edge_num++;                                //�ұ��ظ�����һ

										R_Link_num++;                                //�����ұ��ظ�����һ

										R_rowget_flag = 1;                                //�ұ��������õ���־��  1

										R_edge_nearest = EXTRACT_COL;               //�洢���һ���ұ��ص�������

										break;                                      //�����ұ�����������
									}
								}
								if (R_rowget_flag == 0)      //���ұ�����������δ�ѵ�����
								{
									if (R_Link_num >= 10)
									{
										W_num = 0;
										for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 10 && W_num_row < Row_END; W_num_row++)      //�ұ���������������
										{
											if (str->RightEdge[W_num_row] > str->RightEdge[W_num_row + 1])
											{
												W_num++;
											}
										}
										if (W_num <= 1 && str->AllLeftLose > 5 && L_Link_num < 3)
										{
											L_TURN = 1;                //�����־��  1
										}
										else   if (W_num <= 1 && L_Link_num >= 7)  //��·�ж�
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
									str->R_EndLine = EXTRACT_ROW; //����һ�е�����ֵ�����ұ��ؽ�������ֵ

									str->LeftEdge[EXTRACT_ROW] = 0;
									R_Search_end = 1;             //���ұ�������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
									L_Search_end = 1;             //���ұ�������ʱ���������������׵㣬�ж�Ϊ�������������

									R_Link_num = 0;               //�����ұ��ظ�����  0
									R_Lost_num++;               //��û���������ұ��ص�ʱ���ұ��ض�ʧ����һ
								}

							}
							else
							{
								for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL <= R_edge_nearest + 20 && EXTRACT_COL <= Col_END - 2; EXTRACT_COL++)      //�ұ���������������
								{
									if (image[EXTRACT_ROW][EXTRACT_COL + 1] == Black)
									{
										if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL + 1] == Black && image[EXTRACT_ROW][EXTRACT_COL + 2] == Black)   //�ҵ��ڰ������
										{
											str->RightEdge[EXTRACT_ROW] = EXTRACT_COL + 1;          //���汾���ұ���������

											str->Right_Edge_num++;                                //�ұ��ظ�����һ

											R_Link_num++;                                //�����ұ��ظ�����һ

											R_rowget_flag = 1;                                //�ұ��������õ���־��  1

											R_edge_nearest = EXTRACT_COL + 1;                  //�洢���һ���ұ��ص�������

											R_Trend_in = 0;                                //���ұ��������־��  0

											//R_down_cross_judge();                        //�ж��Ƿ�Ϊʮ��ֱ�ǹյ�
											/////////////  ����ͻȻ���ţ��ж�Ϊ�յ㣬bug�����Ժ���
											if (Inflection_point.R_up_point.x == 0 && Inflection_point.R_down_point.x == 0 && str->RightEdge[EXTRACT_ROW + 1] < Col_END - 4 && EXTRACT_ROW>10 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
											{
												Inflection_point.R_down_point.x = EXTRACT_ROW + 1;                           //�洢�����ʮ���·�ֱ�ǹյ�������
												Inflection_point.R_down_point.y = str->RightEdge[EXTRACT_ROW + 1];                //�洢�����ʮ���·�ֱ�ǹյ�������
											}
											break;                                      //�����ұ�����������
										}
									}
								}
								if (R_rowget_flag == 0)      //���ұ�����������δ�ѵ�����
								{
									R_Link_num = 0;       //�����ұ��ظ�����  0

									R_Lost_num++;       //��û���������ұ��ص�ʱ���ұ��ض�ʧ����һ

									if (Inflection_point.R_up_point.x == 0 && Inflection_point.R_down_point.x == 0 && str->RightEdge[EXTRACT_ROW + 1] < Col_END - 4 && EXTRACT_ROW>10 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
									{
										Inflection_point.R_down_point.x = EXTRACT_ROW + 1;                           //�洢�����ʮ���·�ֱ�ǹյ�������
										Inflection_point.R_down_point.y = str->RightEdge[EXTRACT_ROW + 1];                //�洢�����ʮ���·�ֱ�ǹյ�������
									}
								}
							}
						}
					}
					else       //���ұ�������ʱ���봦��
					{
						if (image[EXTRACT_ROW][R_edge_nearest] == Black)     //����һ���ұ��ص���Ϸ�һ��Ϊ��
						{
							for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL >= R_edge_nearest - 20 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)      //�ұ���������������
							{
								if (image[EXTRACT_ROW][EXTRACT_COL - 1] == White)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)   //�ҵ��ڰ������
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;        //���汾���ұ���������

										str->Right_Edge_num++;                                //�ұ��ظ�����һ

										R_Link_num++;                                //�����ұ��ظ�����һ

										R_rowget_flag = 1;                                //�ұ��������õ���־��  1

										R_edge_nearest = EXTRACT_COL;                //�洢���һ���ұ��ص�������

										R_Trend_in = 1;                                //�ұ����������Ʊ�־��  1

										break;                                      //�����ұ�����������
									}
								}

							}
							if (R_rowget_flag == 0)      //���ұ�����������δ�ѵ�����
							{
								if (R_Link_num >= 10 && R_Link_num > L_Link_num)
								{
									str->R_EndLine = EXTRACT_ROW + 1; //����һ�е�����ֵ�����ұ��ؽ�������ֵ

									R_Search_end = 1;             //���ұ�������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
									L_Search_end = 1;             //���ұ�������ʱ���������������׵㣬�ж�Ϊ�������������
								}


								R_Link_num = 0;               //�����ұ��ظ�����  0

								R_Lost_num++;               //��û���������ұ��ص�ʱ���ұ��ض�ʧ����һ
							}
						}
						else          //����һ���ұ��ص���Ϸ�һ��Ϊ��
						{
							if (0)    //���ұ�����ʮ�������õ�
							{
								for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL <= R_edge_nearest + R_OUT_2_OUT && EXTRACT_COL <= Col_END; EXTRACT_COL++)     //��ʼ��������
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black)
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾���ұ���������

										str->Right_Edge_num++;                                //�ұ��ظ�����һ

										R_Link_num++;                                //�����ұ��ظ�����һ

										R_rowget_flag = 1;                                //�ұ��������õ���־��  1

										R_edge_nearest = EXTRACT_COL;                  //�洢���һ���ұ��ص�������

										break;                                      //�����ұ�����������
									}
								}
								if (R_rowget_flag == 0)
								{
									R_Link_num = 0;                                //�����ұ��ظ�����  0

									R_Lost_num++;                                //���ұ�������ʱ���������������׵㣬�ж�Ϊ�ұ��ض�ʧ����
								}
							}
							else                    //���ұ���ʮ�ֱ�־Ϊ0ʱ
							{
								for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL <= R_edge_nearest + 20 && EXTRACT_COL <= Col_END - 2; EXTRACT_COL++)     //��ʼ��������
								{
									if (image[EXTRACT_ROW][EXTRACT_COL + 1] == Black)
									{
										if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL + 1] == Black && image[EXTRACT_ROW][EXTRACT_COL + 2] == Black)   //�ҵ��ڰ������
										{
											str->RightEdge[EXTRACT_ROW] = EXTRACT_COL + 1;          //���汾���ұ���������

											str->Right_Edge_num++;                                //�ұ��ظ�����һ

											R_Link_num++;                                //�����ұ��ظ�����һ

											R_rowget_flag = 1;                                //�ұ��������õ���־��  1

											R_edge_nearest = EXTRACT_COL + 1;                  //�洢���һ���ұ��ص�������

											break;                                      //�����ұ�����������
										}
									}
								}
								if (R_rowget_flag == 0)
								{
									R_Link_num = 0;                                //�����ұ��ظ�����  0

									R_Lost_num++;                                //���ұ�������ʱ���������������׵㣬�ж�Ϊ�ұ��ض�ʧ����
  //							      ////// �������ޱ߽磬�ж�Ϊ�յ㣬���������⣬�ϵ�//
									if (Inflection_point.R_up_point.x == 0 && Inflection_point.R_down_point.x == 0 && str->RightEdge[EXTRACT_ROW + 1] < Col_END - 4 && EXTRACT_ROW>10 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
									{
										for (W_num_row = EXTRACT_ROW + 1; W_num_row <= EXTRACT_ROW + 5; W_num_row++)  //���ǰ5��������
										{
											if (str->RightEdge[W_num_row] - str->RightEdge[W_num_row + 1] <= 0)
											{
												Inflection_point.R_down_point.x = W_num_row;               //�洢ʮ�ֹյ�������
												Inflection_point.R_down_point.y = str->RightEdge[W_num_row];        //�洢ʮ�ֹյ�������
												break;
											}
										}
									}
								}
							}
						}
					}
				}
				else                  //���ұ���û���жϳ���������ʱ���봦��
				{
					if (image[EXTRACT_ROW][R_edge_nearest] == Black)     //����һ���ұ��ص���Ϸ�һ��Ϊ��
					{
						for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL >= R_edge_nearest - 60 && EXTRACT_COL >= Col_START + 3; EXTRACT_COL--)            //��ʼ��������
						{
							if (image[EXTRACT_ROW][EXTRACT_COL - 1] == White)
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White && image[EXTRACT_ROW][EXTRACT_COL - 3] == White)   //�ҵ��ڰ������
								{
									str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾���ұ���������

									str->Right_Edge_num++;                                //�ұ��ظ�����һ

									R_Link_num++;                                //�����ұ��ظ�����һ

									R_rowget_flag = 1;                                //�ұ��������õ���־��  1

									R_edge_nearest = EXTRACT_COL;                  //�洢���һ���ұ��ص�������

	 //						         if(R_up_cross_flag)                        //�¹յ��ҵ�
	 //						       {
	 //							      //R_up_cross_error();                    //����ұ���ʮ�ֱ�־��Ϊ1������ʮ��ֱ�ǹյ��ж�
	 //						       }

									break;                                      //�����ұ�����������
								}
							}
						}
						if (R_rowget_flag == 0)
						{
							L_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������
							R_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
							str->R_EndLine = EXTRACT_ROW + 1;//����һ�е�����ֵ��������ؽ�������ֵ
							R_Link_num = 0;               //�����ұ��ظ�����  0

							R_Lost_num++;               //��û���������ұ��ص�ʱ���ұ��ض�ʧ����һ
						}
					}
					else     //����һ���ұ��ص���Ϸ�һ��Ϊ��
					{
						if (image[EXTRACT_ROW][R_edge_nearest - 1] == White && image[EXTRACT_ROW][R_edge_nearest - 2] == White)        //֮������ҲΪ��
						{
							for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL <= R_edge_nearest + 20 && EXTRACT_COL <= Col_END - 2; EXTRACT_COL++)           //��ʼ���������ڰ������
							{
								if (image[EXTRACT_ROW][EXTRACT_COL + 1] == Black)
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == White && image[EXTRACT_ROW][EXTRACT_COL + 1] == Black && image[EXTRACT_ROW][EXTRACT_COL + 2] == Black)   //�ҵ��ڰ������
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL + 1;          //���汾���ұ���������

										str->Right_Edge_num++;                                //�ұ��ظ�����һ

										R_Link_num++;                                //�����ұ��ظ�����һ

										R_rowget_flag = 1;                                //�ұ��������õ���־��  1

										R_edge_nearest = EXTRACT_COL + 1;                  //�洢���һ���ұ��ص�������

										break;                                      //�����ұ�����������
									}
								}
							}
							if (R_rowget_flag == 0)
							{
								R_Link_num = 0;       //�����ұ��ظ�����  0

								R_Lost_num++;       //��û���������ұ��ص�ʱ���ұ��ض�ʧ����һ
							}
						}
						else           //֮�������кڵ�,���ж�Ϊ����
						{
							for (EXTRACT_COL = R_edge_nearest; EXTRACT_COL >= R_edge_nearest - 20 && EXTRACT_COL >= Col_START + 5; EXTRACT_COL--)            //��ʼ���������ڰ������
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 3] == White && image[EXTRACT_ROW][EXTRACT_COL - 5] == White)   //�ҵ��ڰ������
								{
									str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾���ұ���������

									str->Right_Edge_num++;                                //�ұ��ظ�����һ

									R_Link_num++;                                //�����ұ��ظ�����һ

									R_rowget_flag = 1;                                //�ұ��������õ���־��  1

									R_edge_nearest = EXTRACT_COL;                  //�洢���һ���ұ��ص�������

									break;                                      //�����������������
								}
							}
							if (R_rowget_flag == 0)
							{
								R_Link_num = 0;               //�����ұ��ظ�����  0

								R_Lost_num++;               //��û���������ұ��ص�ʱ���ұ��ض�ʧ����һ
							}
						}
					}

					if (R_Link_num >= 3)  //����������һ�к��������������ڵ���3
					{
						if (str->RightEdge[EXTRACT_ROW] <= str->RightEdge[EXTRACT_ROW + 1] && str->RightEdge[EXTRACT_ROW + 1] <= str->RightEdge[EXTRACT_ROW + 2])
						{
							R_Trend_in = 1;     //�����������ұ�������������  1
						}
						else
						{
							R_Trend_in = 0;     //�������������ұ�������������  0
						}
					}
				}
			}
			else                        //���ұ��ض�ʧʱ���봦��
			{
				if (Cross.Cross_flag == 1) //���ұ���ʮ�ֱ�־Ϊ1ʱ
				{
					if (Inflection_point.R_down_point.x != 0)    //����⵽�ұ����·�ʮ��ֱ�ǹյ�
					{
						if (Inflection_point.R_down_point.y + 10 < Col_END)
						{
							W_num_col = Inflection_point.R_down_point.y + 10;
						}
						else
						{
							W_num_col = Col_END;
						}
						for (EXTRACT_COL = W_num_col; EXTRACT_COL >= Inflection_point.R_down_point.y - 40 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)    //��ʼ����
						{
							if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //���������ڰ������
							{

								str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾���ұ���������

								str->Right_Edge_num++;                                //�ұ��ظ�����һ

								R_Link_num++;                                //�����ұ��ظ�����һ

								R_edge_nearest = EXTRACT_COL;                  //�洢���һ���ұ��ص�������

								//R_up_cross_error();                          //�ұ����Ϸ�ֱ�ǹյ��
								if (Inflection_point.R_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
								{
									W_num = 0; counting_data = EXTRACT_COL;
									for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //���ǰ8��������
									{
										for (W_num_col = counting_data; W_num_col >= counting_data - 40 && W_num_col >= Col_START + 2; W_num_col--)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
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
											Inflection_point.R_up_point.x = W_num_row;               //�洢ʮ�ֹյ�������
											Inflection_point.R_up_point.y = counting_data;        //�洢ʮ�ֹյ�������
											break;
										}
									}
								}
								R_Lost_num = 0;                                //�����ұ��ض�ʧ����0


								break;                                      //�����ұ�����������

							}
						}

						if (R_rowget_flag == 0)
						{
							R_Lost_num++;                          //�����ұ��ض�ʧ����1
						}
					}
					else                         //�����δ���ұ����·�ʮ��ֱ�ǹյ�
					{
						for (EXTRACT_COL = Col_END - 1; EXTRACT_COL >= 30 && EXTRACT_COL >= Col_START + 2; EXTRACT_COL--)    //��ʼ����
						{
							if (image[EXTRACT_ROW][EXTRACT_COL + 1] == Black && image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //���������ڰ������
							{
								str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾���ұ���������

								str->Right_Edge_num++;                                //�ұ��ظ�����һ

								R_Link_num++;                                //�����ұ��ظ�����һ

								R_rowget_flag = 1;                                //�ұ��������õ���־��  1

								R_edge_nearest = EXTRACT_COL;                  //�洢���һ���ұ��ص�������


								R_Lost_num = 0;                                //�����ұ��ض�ʧ����0

								if (R_get_flag == 0)   //���һ�λ����
								{
									R_get_flag = 1;
									str->R_StartLine = EXTRACT_ROW;
								}
								break;                                      //�����ұ�����������
							}
						}
						if (R_rowget_flag == 0)
						{
							R_Lost_num++;                          //�����ұ��ض�ʧ����1
						}
					}
				}
				else                  //���ұ���ʮ�ֱ�־Ϊ0ʱ
				{
					if (image[EXTRACT_ROW][Col_END] == White && image[EXTRACT_ROW][Col_END - 1] == White && image[EXTRACT_ROW][Col_END - 2] == White)    //��ǰ������Ϊ��
					{
						if (EXTRACT_ROW == Row_END)
						{
							for (EXTRACT_COL = Col_END - 2; EXTRACT_COL > 40; EXTRACT_COL--)    //��ʼ����//�տ�ʼ�ױ߶���û��Ҫ��
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //���������ڰ������
								{
									str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾���ұ���������

									str->Right_Edge_num++;                                //�ұ��ظ�����һ

									R_Link_num++;                                //�����ұ��ظ�����һ

									R_rowget_flag = 1;                                //�ұ��������õ���־��  1

									R_edge_nearest = EXTRACT_COL;                  //�洢���һ���ұ��ص�������

									R_Lost_num = 0;                                //�����ұ��ض�ʧ����0

									if (R_get_flag == 0)   //���һ�λ����
									{
										R_get_flag = 1;
										str->R_StartLine = EXTRACT_ROW;
									}
									break;                                      //�����ұ�����������
								}
							}
						}
						else
						{
							if (str->LeftEdge[EXTRACT_ROW + 1] < Col_END - 2 && str->LeftEdge[EXTRACT_ROW + 1] != 0)
							{
								for (EXTRACT_COL = Col_END - 2; EXTRACT_COL > str->LeftEdge[EXTRACT_ROW + 1]; EXTRACT_COL--)    //��ʼ����//�տ�ʼ�ױ߶���û��Ҫ��
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //���������ڰ������
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾���ұ���������

										str->Right_Edge_num++;                                //�ұ��ظ�����һ

										R_Link_num++;                                //�����ұ��ظ�����һ

										R_rowget_flag = 1;                                //�ұ��������õ���־��  1

										R_edge_nearest = EXTRACT_COL;                  //�洢���һ���ұ��ص�������

										R_Lost_num = 0;                                //�����ұ��ض�ʧ����0

										if (R_get_flag == 0)   //���һ�λ����
										{
											R_get_flag = 1;
											str->R_StartLine = EXTRACT_ROW;
										}
										break;                                      //�����ұ�����������
									}
								}
							}
							else
							{
								for (EXTRACT_COL = Col_END - 2; EXTRACT_COL > 40; EXTRACT_COL--)    //��ʼ����//�տ�ʼ�ױ߶���û��Ҫ��
								{
									if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //���������ڰ������
									{
										str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;          //���汾���ұ���������

										str->Right_Edge_num++;                                //�ұ��ظ�����һ

										R_Link_num++;                                //�����ұ��ظ�����һ

										R_rowget_flag = 1;                                //�ұ��������õ���־��  1

										R_edge_nearest = EXTRACT_COL;                  //�洢���һ���ұ��ص�������

										R_Lost_num = 0;                                //�����ұ��ض�ʧ����0

										if (R_get_flag == 0)   //���һ�λ����
										{
											R_get_flag = 1;
											str->R_StartLine = EXTRACT_ROW;
										}
										break;                                      //�����ұ�����������
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
					else         //��ǰ�����㲻ȫΪ��
					{
						for (EXTRACT_COL = Col_END; EXTRACT_COL >= 30; EXTRACT_COL--)    //��ʼ����
						{
							if (image[EXTRACT_ROW][EXTRACT_COL - 1] == White)
							{
								if (image[EXTRACT_ROW][EXTRACT_COL] == Black && image[EXTRACT_ROW][EXTRACT_COL - 1] == White && image[EXTRACT_ROW][EXTRACT_COL - 2] == White)      //���������ڰ������
								{
									str->RightEdge[EXTRACT_ROW] = EXTRACT_COL;        //���汾���ұ���������

									str->Right_Edge_num++;                                //�ұ��ظ�����һ

									R_Link_num++;                                //�����ұ��ظ�����һ

									R_rowget_flag = 1;                                //�ұ��������õ���־��  1

									R_edge_nearest = EXTRACT_COL;                //�洢���һ���ұ��ص�������

									if (Inflection_point.R_down_point.x != 0 && (R_Lost_num > 8 || R_get_flag == 1) && EXTRACT_COL < Col_END - 10)
									{
										if (Inflection_point.R_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5)      //��Ҫ�жϵĵ��ڷ�Χ��
										{
											W_num = 0; counting_data = EXTRACT_COL;
											for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //���ǰ8��������
											{
												for (W_num_col = counting_data; W_num_col >= counting_data - 40 && W_num_col >= Col_START + 2; W_num_col--)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
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
													Inflection_point.R_up_point.x = W_num_row;               //�洢ʮ�ֹյ�������
													Inflection_point.R_up_point.y = counting_data;        //�洢ʮ�ֹյ�������
													break;
												}
											}
										}
									}
									R_Lost_num = 0;                                //�����ұ��ض�ʧ����0

									if (R_rowget_flag && R_get_flag == 0)   //���һ�λ����
									{
										R_get_flag = 1;
										str->R_StartLine = EXTRACT_ROW;

										if (Inflection_point.R_up_point.x == 0 && EXTRACT_ROW > 5 && EXTRACT_ROW <= Row_END - 5 && EXTRACT_COL < Col_END - 10)      //��Ҫ�жϵĵ��ڷ�Χ��
										{
											W_num = 0; counting_data = EXTRACT_COL;
											for (W_num_row = EXTRACT_ROW - 1; W_num_row > EXTRACT_ROW - 5; W_num_row--)  //���ǰ8��������
											{
												for (W_num_col = counting_data; W_num_col >= counting_data - 40 && W_num_col >= Col_START + 2; W_num_col--)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
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
													Inflection_point.R_up_point.x = W_num_row;               //�洢ʮ�ֹյ�������
													Inflection_point.R_up_point.y = counting_data;        //�洢ʮ�ֹյ�������
													break;
												}
											}
										}
									}
									break;                                      //�����ұ�����������
								}
							}
						}
						if (R_rowget_flag == 0)
						{
							R_Lost_num++;             //���ұ��ض�ʧʱ���������������ڰ�����㣬�ж�Ϊ�ұ��ض�ʧ

							if (EXTRACT_ROW == Row_END)       //�ڶ�·ʱ
							{
								L_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������
								R_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
								str->L_EndLine = EXTRACT_ROW;//����һ�е�����ֵ��������ؽ�������ֵ
							}
						}

					}
				}
			}
		}
		else
		{
			if (R_get_flag == 0)   //�ҹ�ʱ��ʼ�߻ᱻ����
			{
				R_get_flag = 1;
				str->R_StartLine = EXTRACT_ROW;
			}
			if (L_TURN == 0 && R_TURN == 0)
			{
				if (image[EXTRACT_ROW][Col_END] == White && image[EXTRACT_ROW][Col_END - 2] == White && image[EXTRACT_ROW][Col_END - 4] == White)    //��ǰ������Ϊ��
				{
					R_Lost_num++;             //�ж�Ϊ����ض�ʧ
				}
			}
		}
#endif

		//���б���
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
		//			   str->L_EndLine=EXTRACT_ROW;    //����һ�е�����ֵ��������ؽ�������ֵ
		//			   str->R_EndLine=EXTRACT_ROW; //����һ�е�����ֵ�����ұ��ؽ�������ֵ
		//
		//			   L_Search_end=1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������
		//			   R_Search_end=1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
		//		    }
		//	     }

				 //�����д���
		if (EXTRACT_ROW == ROW_START && (L_Search_end == 0 || R_Search_end == 0)) //���һ��
		{
			str->L_EndLine = EXTRACT_ROW;    //����һ�е�����ֵ��������ؽ�������ֵ
			str->R_EndLine = EXTRACT_ROW; //����һ�е�����ֵ�����ұ��ؽ�������ֵ

			L_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������
			R_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
		}


		//�ж����ж�
#if 1
		if (str->LeftEdge[EXTRACT_ROW] != 0 && str->RightEdge[EXTRACT_ROW] != 0)     //���Ҷ��б߽�
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
		else if (str->LeftEdge[EXTRACT_ROW] == 0 && str->RightEdge[EXTRACT_ROW] == 0)//���߶���   d������
		{
			str->RowLose[EXTRACT_ROW] = 4;
		}
#endif

		//�������  �����ұ߽���   ȥ�������һ���ѵ���
		if (R_Search_end == 1 && str->LeftEdge[EXTRACT_ROW] != 0)
			str->LeftEdge[EXTRACT_ROW] = 0;


#if 0
		if (L_TURN == 1 || R_TURN == 1)
		{
			L_cross_flag = 0;      //����������ʱ���ж�Ϊ����ͻ�䣬�����ʮ�ֱ�־��  1
			R_cross_flag = 0;      //����������ʱ���ж�Ϊ����ͻ�䣬�����ʮ�ֱ�־��  1

			//ǿ��Ϊ��� ȡ��ʮ����Բ������

			str->L_EndLine = EXTRACT_ROW;    //����һ�е�����ֵ��������ؽ�������ֵ
			str->R_EndLine = EXTRACT_ROW; //����һ�е�����ֵ�����ұ��ؽ�������ֵ

			L_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�������������
			R_Search_end = 1;             //�����������ʱ���������������׵㣬�ж�Ϊ�ұ�����������
		}

#endif


	}//����ͼ��ÿ��ɨ�����

	if (R_Search_end == 1 && L_Search_end == 1)
	{
		if (str->L_EndLine != 0)
			str->EndLine = str->L_EndLine;//����һ�е�����ֵ��������ؽ�������ֵ
		else if (str->R_EndLine != 0)
			str->EndLine = str->R_EndLine;//����һ�е�����ֵ��������ؽ�������ֵ


	}

	if (ABS(str->LastEndLine - str->EndLine) > 3)
	{
		str->EndLine = (str->LastEndLine + str->EndLine) / 2;
	}
	str->LastEndLine = str->EndLine;


	//��ʼ�д���
	str->StartLine = MAX(str->L_StartLine, str->R_StartLine);


#if 0
	cross_edge_add();     //ʮ�ֱ��ز��䣨����ʮ�ִ�����أ�

		  //   edge_error_process(); //���ش�����������ͨ����������أ��������ҹգ�ʶ��S�䣩
	   //
	road_judge();         //�ж��������ͣ�ʶ��ֱ�����µ���СS������ڣ�
//
//
//
//      road_num();           //�������ͼ�����ʮ�֣��ϰ��������
//

#endif
}



