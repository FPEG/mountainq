#include "include.h"
#include "Round_Deal.h"


//#define



Str_Round  Round = { 0 };
RoundTypeDef RoundType;
RoundSizeTypeDef RoundSize;
RoundProcessTypeDef RoundProcess;

//�ֲ������������ڱ��ļ�

unsigned char  Round_row = 0;//����ʱ����i����ʼ��Ϊ��������У���
unsigned char  Round_col = 0;//����ʱ����j
int  Round_num = 0;//ǰ�����ң����в�
unsigned char  LeftLoopTemp_R = 0;//��һ�ιյ�ǰ��ڰ�������row//str->EndLine
unsigned char LeftLoopTemp_C = 0;//��һ�ιյ��ұߺڰ�������col
unsigned char Inflection_Temp_x;
unsigned char Inflection_Temp_y;
float  LeftLoopSlope;
float  RightLoopSlope;


void Round_Process(Str_Boundary* str, Str_Round* round);
void Round_Detection(Str_Boundary* str, Str_Round* round);

/***************************************************************
Բ������

* �������ƣ�Round_Deal()
* ����˵���� Բ������
* ����˵����
* �������أ�void
* �޸�ʱ�䣺
* �� ע��
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
 * \brief Բ����⣬�ж�
 * \param[in] str �߽�ṹ��
 * \param round
 * \par
 * ���һֱ����
 * Բ�����\n
 * 1���ж�һ�����¹յ㣬һ��û���¹յ㣬�ҹյ�λ��������в�������5��\n
 * 2���յ�ඪ��\n
 * 3���ǹյ��ֱ��
 */
void Round_Detection(Str_Boundary* str, Str_Round* round) //Բ��
{
	//��
	//��⵽���¹յ㣬û��⵽���¹յ㣬�����бȹյ�Զ�ܶ�
	if (Inflection_point.L_down_point.x != 0 &&//��⵽���¹յ�
		Inflection_point.R_down_point.x == 0 &&//û��⵽���¹յ�
		str->EndLine < Inflection_point.L_down_point.x - 5//�����бȹյ�Զ�ܶ�(5)
		)
	{
		//���¹յ��е�����������߶���
		if (str->LeftEdge[Inflection_point.L_down_point.x - 1] == 0 &&
			str->LeftEdge[Inflection_point.L_down_point.x - 2] == 0 &&
			str->LeftEdge[Inflection_point.L_down_point.x - 3] == 0)
		{
			round->Entrance_count = 0;//Բ����ڼ�������
			/*
			 * ��ʼ����ʱ����Round_rowΪ��յ�����5��	
			 * Round_row=MAX(Inflection_point.L_down_point.x + 5,(38)ROW_END - 1)
			 */
			if (
				Inflection_point.L_down_point.x + 5 > //��յ�����5��
				ROW_END - 1
				)
				Round_row = ROW_END - 1;
			else
				Round_row = Inflection_point.L_down_point.x + 5;
			/*
			 * //����յ�����5�е�����10�н�����������15�У�����13��ΪԲ����ǣ�
			 * ��ȡԲ����ڼ���++
			 * ����������ұ������ݼ������ұ߲�����
			 */
			for (; 
				(
					Round_row > //��յ�����5��
					Inflection_point.L_down_point.x - 10 //��յ�����10��
					) && 
				
				Round_row >= ROW_START;
				Round_row--)
			{
				/*
				 * ����ұ߲����� 
				 */
				if (str->RightEdge[Round_row + 1] != 0 &&
					str->RightEdge[Round_row] != 0)
				{
					/*
					 * //����ұ������ݼ�(���Ҳ�����)     б�ʷ���Ҫ��
					 * ��-ǰ=[0,2]
					 */
					if (str->RightEdge[Round_row + 1] - str->RightEdge[Round_row] >= 0 &&
						str->RightEdge[Round_row + 1] - str->RightEdge[Round_row] <= 2)
					{
						round->Entrance_count++; //Բ����ڼ�������
					}
				}
			}
			/*
			 * //min�޸ģ�������13���������������ж�ΪԲ��
			 */
			if (round->Entrance_count >= 13)
			{
				round->Round_flag = 1;
				RoundType = Left_Round;
				RoundProcess = Find_Round;
				RoadType = Round_Road;
				//RoundSize=Large_Round;   //����
			}
		}
	}

	//�һ�
	//��⵽���¹յ㣬û��⵽���¹յ㣬�����бȹյ�Զ�ܶ�
	else if (Inflection_point.R_down_point.x != 0 && Inflection_point.L_down_point.x == 0 && str->EndLine < Inflection_point.R_down_point.x - 5)
	{
		//���¹յ��е�����������߶���
		if (str->RightEdge[Inflection_point.R_down_point.x - 1] == 0 && str->RightEdge[Inflection_point.R_down_point.x - 2] == 0 && str->RightEdge[Inflection_point.R_down_point.x - 3] == 0)
		{
			round->Entrance_count = 0;//Բ����ڼ�������
			if (Inflection_point.R_down_point.x + 5 > ROW_END - 1)
				Round_row = ROW_END - 1;
			else
				Round_row = Inflection_point.R_down_point.x + 5;
			for (; Round_row > Inflection_point.R_down_point.x - 10 && Round_row >= ROW_START; Round_row--)//���ҹյ�����5�е�����10�н�������
			{
				if (str->LeftEdge[Round_row + 1] != 0 && str->LeftEdge[Round_row] != 0)
				{
					//�����������������Ҳ�����
					if (str->LeftEdge[Round_row] - str->LeftEdge[Round_row + 1] >= 0 && str->LeftEdge[Round_row] - str->LeftEdge[Round_row + 1] <= 2)
					{
						round->Entrance_count++; //Բ����ڼ�������
					}
				}
			}
			if (round->Entrance_count >= 13)//min�޸ģ�������13���������������ж�ΪԲ��
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
 * \brief Բ������
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
	case Left_Round:  //��
	{
		switch (RoundProcess)
		{
		case Find_Round://����Բ��
		{
			//*�����¸��׶ε��ж�*/
			if (str->L_StartLine < 30)   //da���Ľ�
			{
				Round_num = 0;
				/*
				 * //��ֹ����     ��⻻���ĵ�
				 * �ȴ�Find_Gap���
				 * �����߿�ʼ����ǰ��4���ۼ������в�ж��ۼӺ͵Ĵ�С
				 */
				for (
					Round_row = str->L_StartLine; 
					Round_row >= str->L_StartLine - 4 && 
					Round_row >= ROW_START + 1; 
					Round_row--)   
				{
					/*
					 * ���û����
					 */
					if (str->LeftEdge[Round_row] != 0)
					{
						Round_num += 
							str->LeftEdge[Round_row] - str->LeftEdge[Round_row + 1];//ǰ�������в�
					}
					/*
					 * ����
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
					RoundProcess = Find_Gap;//����Ϊ��ȱ��//����Բ������
				}
			}
			
			/*
			 * //*�뻷�Ĳ���
			 * �Ӻ��ѵ�ǰ
			 */
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				/*
				 * ����û����
				 */
				if (str->RightEdge[Round_row])
				{
					/*
					 * //��߽�����ұ߽�����
					 */
					str->LeftEdge[Round_row] = 
						str->RightEdge[Round_row] - //�ұ߽�
						round->Track_Width_forRound[Round_row];//���
				}
				/*
				 * ���߶���
				 */
				else
				{
					str->LeftEdge[Round_row] = 0;
				}
			}
			break;
		}
		case Find_Gap://����Բ������
		{
			/*��һ�׶ε��ж�*/
			//�ײ�������߽綼������
			if (str->LeftEdge[ROW_END - 3] &&
				str->LeftEdge[ROW_END - 1] &&
				str->LeftEdge[ROW_END - 2])
			{
				RoundProcess = Into_Round;
			}
			/*
			 * //����
			 * Ч��ͬ��
			 */
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				if (str->RightEdge[Round_row])
				{
					str->LeftEdge[Round_row] = str->RightEdge[Round_row] - round->Track_Width_forRound[Round_row];//��߽�����ұ߽�����
				}
				else
				{
					str->LeftEdge[Round_row] = 0;
				}
			}
			break;
		}
		case Into_Round://����    �ؼ� ������
		{
			/* //�����Ĳ��ߴ���
			 * �ײ�������߽綼������
			 */
			if (str->LeftEdge[ROW_END - 1] &&
				str->LeftEdge[ROW_END - 2])
				//&&str->LeftEdge[ROW-4])
			{
				//��ȡ���ߵ���Ϣ
				if (Inflection_point.L_down_point.x > 5) //��¼��һ�ιյ���ڵ���Ϣ
				{
					Inflection_Temp_x = Inflection_point.L_down_point.x;
					Inflection_Temp_y = Inflection_point.L_down_point.y;
				}

				/*
				 * //�õ���Ϣ�󣬿�ʼ����
				 * i=(��һ�ιյ�x,5]��ǰ����
				 * ����ֹ��
				 */
				for (uint8 i = Inflection_Temp_x; i > 5; i--)
				{
					/*
					 * ���к�ǰ�жԱ�
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
				 * i=[��һ�ιյ�y,70)
				 * �Һڰ�������col
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
				/*����б��*/
				LeftLoopSlope = 1.6; //*(str->RightEdge[aa1]-aa2)/(aa1-LeftLoopTemp_R);

				for (uint8 i = 0; i <= 35; i++)
				{
					/*
					 * str->RightEdge[LeftLoopTemp_R + i]����ǰ����λ��
					 */
					str->RightEdge[LeftLoopTemp_R + i] = (int)(i * LeftLoopSlope + LeftLoopTemp_C);
					/*
					 * �޷�
					 */
					str->RightEdge[LeftLoopTemp_R + i] = int16_range_protect(str->RightEdge[LeftLoopTemp_R + i], 1, 79);

					/*
					 * �����޷���
					 */
					if (
						str->RightEdge[LeftLoopTemp_R + i] > 78 ||
						LeftLoopTemp_R + i > ROW_END
					)
						break;
				}
			}
			/*
			 * �ײ�������߽綼����
			 */
			else
			{
				RoundProcess = Into_Round2;
			}

			/*
			 * ��ֹ���߱����߻���
			 * ɨ��i=[Endline,39),j=[1,75]
			 * 
			 */
			for (uint8 i = str->EndLine; i < ROW_END; i++)
			{
				//�װ��Һ�
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
		case Into_Round2://����    �ؼ� 4
		{
			/*�ж��Ƿ���뻷��*/
			if (distance_judge(1800))//&&str->L_StartLine==0&& RoundSize==Small_Round)      //������һ��
			{
				RoundProcess = In_Round;
				Inflection_Temp_x = 0;
				Inflection_Temp_y = 0;
			}

#if 1                                 //����
			/*
			 * ��ֹ���߱����߻���
			 * ɨ��i=[Endline,39),j=[1,75]
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


			/*����б��*/
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
					str->EndLine = i - 1;        //�ı�endline
					break;
				}
				if (str->LeftEdge[i] > 30)
				{
					str->LeftEdge[i] = 0;   //�޷�
				}
			}
#endif
			break;
		}
		case In_Round://�ڻ���    5
		{
			//��¼��һ�ιյ���ڵ���Ϣ
			if (Inflection_point.R_down_point.x &&
				image[Inflection_point.R_down_point.x][Inflection_point.R_down_point.y - 2] == White &&
				image[Inflection_point.R_down_point.x][Inflection_point.R_down_point.y - 3] == White &&
				image[Inflection_point.R_down_point.x - 1][Inflection_point.R_down_point.y] == White &&
				image[Inflection_point.R_down_point.x - 2][Inflection_point.R_down_point.y] == White)
			{
				Inflection_Temp_x = Inflection_point.R_down_point.x;
				Inflection_Temp_y = Inflection_point.R_down_point.y;
				//�ж���һ�׶�
				if (Inflection_point.R_down_point.x > 5)
				{
					uint8 hang_num = 0;            //����׿����ֵ                 8
					for (
						Round_row = Inflection_point.R_down_point.x - 1;
						Round_row >= Inflection_point.R_down_point.x - 6 &&
						Round_row > ROW_START;
						Round_row--)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
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
				//����
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
				//memset(str->LeftEdge, 0,sizeof(str->LeftEdge) );   //z�������
			}
			for (uint8 i = 39; i > str->EndLine; i--)    //��������������
			{
				if (str->LeftEdge[i] > 30)
				{
					str->LeftEdge[i] = 0;
				}
			}

			break;
		}
		case Out_Round://����   6
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

			if (distance_judge(1200))   //��һ�׶α�־
			{
				RoundProcess = Leave_Round;
			}
			for (uint8 i = 39; i > str->EndLine; i--)    //��������������
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
			/*��һ�׶ε��ж�*/

			break;
		}
		case Leave_Round://�뿪Բ��
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
				)  //30��Ҫ��  huo �ж�б��
			{
				RoundProcess = Leave_Round2;//�ݶ�
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

				for (uint8 i = 39; i > str->EndLine; i--)    //��������������
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
		case Leave_Round2://�뿪Բ�� 7
		{
			//����������Ұ
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				if (str->RightEdge[Round_row])
				{
					str->LeftEdge[Round_row] = str->RightEdge[Round_row] - round->Track_Width_forRound[Round_row];//��߽�����ұ߽�����
				}
				else
				{
					str->LeftEdge[Round_row] = 0;
				}
			}
			if (distance_judge(3000))
			{
				RoundProcess = Round_Not_Found;//�ݶ�
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
	//�һ�
	case Right_Round:
	{
		switch (RoundProcess)
		{
		case Find_Round://����Բ��
		{
			/*�����¸��׶ε��ж�*/
			//���������ʼ��֮�����5��ȱ�ڼ���Ϊ0����߽�ײ����ж���//��֤�ұ߽��б����һ����Χ���Է�б��Բ�����У���������
			if (str->R_StartLine < 30)   //da���Ľ�
			{
				Round_num = 0;
				for (Round_row = str->R_StartLine; Round_row >= str->R_StartLine - 4 && Round_row >= ROW_START + 1; Round_row--)   //��ֹ����     ��⻻���ĵ�
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
					RoundProcess = Find_Gap;//����Ϊ��ȱ��
				}
			}


			/*�뻷�Ĳ���*/
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				if (str->LeftEdge[Round_row])
				{
					str->RightEdge[Round_row] = str->LeftEdge[Round_row] + round->Track_Width_forRound[Round_row];//��߽�����ұ߽�����
				}
				else
				{
					str->RightEdge[Round_row] = 0;
				}
			}
			break;
		}
		case Find_Gap://����Բ������
		{
			/*��һ�׶ε��ж�*/
			//�ײ�������߽綼������
			if (str->RightEdge[ROW_END] && str->RightEdge[ROW_END - 1] && str->RightEdge[ROW_END - 2])//&&str->LeftEdge[ROW-4])
			{
				RoundProcess = Into_Round;
			}
			/*�Ҳ���*/
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				if (str->LeftEdge[Round_row])
				{
					str->RightEdge[Round_row] = str->LeftEdge[Round_row] + round->Track_Width_forRound[Round_row];//��߽�����ұ߽�����
				}
				else
				{
					str->RightEdge[Round_row] = 0;
				}
			}
			break;
		}
		case Into_Round://����  3
		{
			/*�����Ĳ��ߴ���*/
			//��ȡ���ߵ���Ϣ
			if (str->RightEdge[39] && str->RightEdge[38])//&&str->LeftEdge[ROW-4])
			{
				if (Inflection_point.R_down_point.x > 5) //��¼��һ�ιյ���ڵ���Ϣ
				{
					Inflection_Temp_x = Inflection_point.R_down_point.x;
					Inflection_Temp_y = Inflection_point.R_down_point.y;
				}
				//�õ���Ϣ�󣬿�ʼ����
				for (uint8 i = Inflection_Temp_x; i > 5; i--)//�յ����������׺����䣬��¼
				{
					if (image[i][Inflection_Temp_y] == White && image[i - 1][Inflection_Temp_y] == Black && image[i - 2][Inflection_Temp_y] == Black)
					{
						LeftLoopTemp_R = i - 1;
						str->EndLine = i - 1;
						break;
					}
				}
				for (uint8 i = Inflection_Temp_y; i > 10; i--)//�յ����������ڰ�����
				{
					if (image[LeftLoopTemp_R - 1][i - 2] == White && image[LeftLoopTemp_R - 1][i - 1] == White && image[LeftLoopTemp_R - 1][i] == Black)
					{
						LeftLoopTemp_C = i;
						break;
					}
				}
				/*����б��*/
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
		case Into_Round2://���� 4
		{
			/*�ж��Ƿ���뻷��*/
			if (distance_judge(1800))//&&str->L_StartLine==0&& RoundSize==Small_Round)      //������һ��
			{
				RoundProcess = In_Round;
				Inflection_Temp_x = 0;
				Inflection_Temp_y = 0;
			}
			/*�����Ĳ��ߴ���*/
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
			//����
			/*����б��*/
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
					str->RightEdge[i] = 0;   //�޷�
				}
			}
			break;
		}
		case In_Round://�ڻ���  5
		{
			if (Inflection_point.L_down_point.x && image[Inflection_point.L_down_point.x][Inflection_point.L_down_point.y + 2] == White && image[Inflection_point.L_down_point.x][Inflection_point.L_down_point.y + 3] == White)
			{
				Inflection_Temp_x = Inflection_point.L_down_point.x;
				Inflection_Temp_y = Inflection_point.L_down_point.y;
				if (Inflection_point.L_down_point.x > 5)
				{
					uint8 hang_num = 0;            //����׿����ֵ                 8
					for (Round_row = Inflection_point.L_down_point.x - 1; Round_row >= Inflection_point.L_down_point.x - 6 && Round_row > ROW_START; Round_row--)     //���������ĺڰ����������������׵㣬�ж��Ƿ�Ϊ���ŵ�
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
				//����
				LeftLoopSlope = 4.5;

				for (int i = Inflection_Temp_x + 1; i > ROW_START; i--)
				{
					str->LeftEdge[i] = str->LeftEdge[i + 1] + 5;
					//str->LeftEdge[i]=(int)((Inflection_Temp_x-i)*LeftLoopSlope+Inflection_Temp_y);    //���ʽ��you  bug
					if (str->LeftEdge[i] > COL_END)
					{
						str->EndLine = i;
						break;
					}
				}
				// str->StartLine=ROW_END;
			}
			for (uint8 i = 39; i > str->EndLine; i--)    //��������������
			{
				if (str->RightEdge[i] < 50)
				{
					str->RightEdge[i] = 0;
				}
			}

			break;
		}
		case Out_Round://����
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


			if (distance_judge(1200))   //��һ�׶α�־
			{
				RoundProcess = Leave_Round;
			}


			for (uint8 i = 39; i > str->EndLine; i--)    //��������������
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
		case Leave_Round://�뿪Բ��
		{
			Round_num = 0;
			for (Round_row = str->L_StartLine; Round_row > str->EndLine + 1 && Round_row > ROW_START + 10; Round_row--)
			{
				if (str->LeftEdge[Round_row] == 0)
				{
					Round_num++;
				}
			}
			if (Round_num < 3 && ((str->L_StartLine - str->EndLine) > 32 || str->LeftEdge[str->EndLine] < 65))  //30��Ҫ��
			{
				RoundProcess = Leave_Round2;//�ݶ�
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
				for (uint8 i = 39; i > str->EndLine; i--)    //��������������
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
		case Leave_Round2://�뿪Բ��
		{
			for (Round_row = ROW_END; Round_row >= ROW_START; Round_row--)
			{
				if (str->LeftEdge[Round_row])
				{
					str->RightEdge[Round_row] = str->LeftEdge[Round_row] + round->Track_Width_forRound[Round_row];//��߽�����ұ߽�����
				}
				else
				{
					str->RightEdge[Round_row] = 0;
				}
			}
			/*�뿪Բ���Ĳ��ߴ���*/
			if (distance_judge(3000))
			{
				RoundProcess = Round_Not_Found;//�ݶ�
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