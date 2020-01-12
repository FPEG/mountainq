
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


Str_Inflection_point    Inflection_point;   //�յ�ṹ��
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

