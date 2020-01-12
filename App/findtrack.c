
#include  "common.h"
#include "include.h"
#include "findtrack.h"


//*********define for SearchCenterBlackline**********/


int   MiddleLine[RowMax + 1];//�������飬����Խ��Խ������
int   RightEdge[RowMax + 1];//�ұ������飬����Խ��Խ������
int   LeftEdge[RowMax + 1];//��������飬����Խ��Խ������
//int   Width[RowMax + 1] = { 2,3,3,3,4,4,5,5,6,6,
//					   8,8,10,10,12,13,14,14,16,17,
//					   18,18,20,20,22,22,24,24,26,26,
//					   28,30,31,32,32,34,36,36,38,39,
//					   41,41,43,43,45,45,47,47,49,50,
//					   50,51,52,54,55,56,57,58,59,60,61 };//�������
int   Width[RowMax + 1] = {
	1,	3,	3,	5,	5,	7,	7,	8,	10,	11,
	11,	13,	14,	15,	16,	17,	18,	19,	20,	21,
	21,	23,	23,	25,	25,	26,	27,	28,	29,	30,
	31,	32,	33,	34,	36,	36,	38,	38,	40,	40,
	42,	42,	44,	44,	46,	46,	48,	48,	50,	50,
	52,	52,	54,	54,	55,	56,	57,	58,	58,	60,	61 };


int   MidPri = 40;//��ǰ���е�������ʼ��,ȡ40,10,70
int   LastLine = 0;//���һ�У���̬ǰհ
float AvaliableLines = 0;//��Ч����
int   LeftLose = 0;//ֻ�����߶�������
int   RightLose = 0;//ֻ�����߶�������
int   AllLose = 0;//�����߶���������

int   LeftLoseStart = 0;//��¼��߶��ߵĿ�ʼ��
int   RightLoseStart = 0;//��¼�ұ߱߶��ߵĿ�ʼ��
int   WhiteStart = 0;


//define for GetBlackEndParam**********/

int BlackEndMR = 0;//������50
int BlackEndML = 0;//������30
int BlackEndLL = 0;//������10
int BlackEndRR = 0;//������70
int BlackEndL = 0;//������20
int BlackEndM = 0;//������40
int BlackEndR = 0;//������60
int BlackEndMaxMax = 0;
int BlackEndMax = 0;
int DropRow = 0;//�ⶥ������,��ǰ��������Ӧ���Ǻڸ�

/*********define for FindInflectionPoint()**********/

int RightInflectionPointRow = 0;
int RightInflectionPointCol = 0;
int LeftInflectionPointRow = 0;
int LeftInflectionPointCol = 0;
unsigned char RightInflectionPointFlag = 0;
unsigned char LeftInflectionPointFlag = 0;
unsigned char LeftInflectionPointSecondFlag = 0;
unsigned char RightInflectionPointSecondFlag = 0;
unsigned char LoopFlag = 0;
unsigned char LoopRightOkFlag = 0;
unsigned char LoopLeftOkFlag = 0;
unsigned int StartRow = 0;
unsigned int StartCol = 0;
unsigned char MilldleBlack = 0;
unsigned int LoopTop = 0;
unsigned int LoopRightBorderLose = 0;
unsigned int LoopLeftBorderLose = 0;
int LoopBorttomFlag = 0;
int LoopBorttomRow = 0;
int LoopMilldleRow = 0;
unsigned int LoopMilldleFlag = 0;
unsigned int LoopTopRow = 0;
unsigned int LoopLeft = 0;
unsigned int MilldlePonit = 0;
unsigned int LoopRight = 0;
unsigned int LoopRightR = 0;
unsigned int LoopLeftL = 0;
int BigLoopLeftUp[60];
int BigLoopRightUp[60];
int BigLooptUp[80];
unsigned int LeftUpExitFlag = 0;
unsigned int RightUpExitFlag = 0;
unsigned int  ExitFlag = 0;
unsigned int  LeftExitFlag = 0;
unsigned int  RightExitFlag = 0;;
unsigned char ClearLoopControlFlag = 0;
unsigned char LoopRightControlFlag = 0;
unsigned char LoopLeftControlFlag = 0;
int MotivateLoopDlayFlagR = 0;
int MotivateLoopDlayFlagL = 0;
unsigned char InLoopLeftInflectionPointFlag = 0;
unsigned int InLoopLeftInflectionPointRow = 0;
unsigned char MotivateLoopDlayFlagLOK = 0;
unsigned char MotivateLoopDlayFlagROK = 0;
unsigned char OpenLoopExitRepairFlagL = 0;
unsigned char OpenLoopExitRepairFlagR = 0;
unsigned char InLoopRightInflectionPointFlag = 0;
unsigned int  InLoopRightInflectionPointRow = 0;
unsigned char BreakEndL = 0;
unsigned char BreakEndLFlag = 0;
unsigned char BreakEndR = 0;
unsigned char BreakEndRFlag = 0;
unsigned char BreakStartL = 0;
unsigned char BreakStartLFlag = 0;
unsigned char BreakStartR = 0;
unsigned char BreakStartRFlag = 0;

/**
 * \brief �趨��ֵ
 * \par �ֶ�
 * ��������ÿһ�ж�Ӧ��������ȣ����ļ��ڲ�����
 * \par ��ֵ
 * ���ߣ�0	\n
 * ���ߣ�ColumnMax / 2 = 40	\n
 * ���ߣ�ColumnMax = 80
 */
void SetInitVal()
{
	int i;

	for (i = 0; i < RowMax; i++)//����ֵ             
	{
		RightEdge[i] = ColumnMax;
		LeftEdge[i] = 0;
		MiddleLine[i] = ColumnMax / 2;

	}
    float a[5]={0,0,0,0,0}; 
    float *p;
    arm_var_f32(a,5,p);

}

/**
 * \brief ȫ��ɨ��ͱ�Ե�����ȡ����������
 * \par �ֶ�
 * ǰʮ��ȫ��ɨ��\n
 * ����50�У�������һ�е�Ѱ�������������ǰ�е�Ѱ�߷�ʽ�����
 * \par ������ʽ
 * �ⲿ����
 * \param[out]	MiddleLine[61]
 * \param[out]	LeftEdge[61]
 * \param[out]	RightEdge[61]
 *
 */
void SearchCenterBlackline(void)
{

	int16 i = 0;//��ǰ������Խ��Խ������
	int16 j = 0;//����ɨ�����,��ǰɨ��λ��
	uint8 jj = 0;//��ɨ��λ�ã�������ʣ���к�j�Ա�
	uint8 WhiteNum = 0;

	LeftLose = 0;//��������
	RightLose = 0;
	AllLose = 0;///ʮ��·�� ȫ��
	WhiteNum = 0;

	MiddleLine[RowMax] = ColumnMax / 2;//��60��
	LeftEdge[RowMax] = 0;
	RightEdge[RowMax] = ColumnMax;
	Width[RowMax] = ColumnMax;

	SetInitVal();

	//������ǰʮ�У�ȫ��ɨ��
	for (i = RowMax - 1; i >= 50; i--)
	{
#pragma region Ѱ�ұ߽�

#pragma region Ѱ����߽�
		if (i == RowMax - 1)//���о���ͼ��������Ϊɨ�����
		{
			j = MidPri;//40 Ϊ ColumnMax/2
		}
		else
		{
			j = MiddleLine[i + 1];//���������һ���е��λ����Ϊ����ɨ�����
		}
		/*
		 * �е㲻��̫��
		 */
		if (j <= 2) j = 2;
		/*
		 * ���е���������
		 */

		int left_j_first_flag = 0;
		int left_j_count = 0;

		while (j >= 3)//j>=3��Ч��Χ�ڽ�����Ѱ 
		{
			//���������ҵ��װ׺����� 
			if (img[i][j] == White_Point &&
				img[i][j - 1] == Black_Point &&
				img[i][j - 2] == Black_Point
				)
			{
				/*
				 * ��¼����׵�
				 */
				if(left_j_first_flag==0)
				{
					left_j_first_flag = 1;
					LeftEdge[i] = j;//�ҵ���ֵ �Ҳ�������ԭֵ0
				}
				left_j_count++ ;
				
				//break;//��������Ѱ��
			}
			j--;//���������ƶ�
		}
		if(left_j_count>2)
		{
			LeftEdge[i] = j;//�ҵ���ֵ �Ҳ�������ԭֵ0
		}
#pragma endregion Ѱ����߽�
#pragma region Ѱ���ұ߽�

		if (i == RowMax - 1) //�����ұ߽�
		{
			j = MidPri;//������У���ͼ�����Ŀ�ʼ��Ѱ
		}
		else
		{
			j = MiddleLine[i + 1];//�������һ������λ�ÿ�ʼ��Ѱ
		}
		if (j >= ColumnMax - 3)//j >=ColumnMax-2��Ч��Χ����Ѱ����
		{
			j = ColumnMax - 3;
		}
		while (j <= ColumnMax - 3)
		{
			//���������ҵ��װ׺������
			if (img[i][j] == White_Point && 
				img[i][j + 1] == Black_Point &&
				img[i][j + 2] == Black_Point
				)
			{
				RightEdge[i] = j;//�ҵ���ֵ   �Ҳ�������ԭֵ
				break;//��������Ѱ��
			}
			j=j+1;//���������ƶ�
		}
#pragma endregion Ѱ���ұ߽�
#pragma endregion Ѱ�ұ߽�
#pragma region �ж��е�
		/*
		 * // �����жϣ�û�ж���
		 * �ж������Ƿ���
		 */
		if (
			LeftEdge[i] != 0 &&
			RightEdge[i] != ColumnMax
			)
		{
			/*
			 * ȡ�����е�
			 */
			MiddleLine[i] = (LeftEdge[i] + RightEdge[i]) / 2;
		}


		/*
		 * //��������
		 * �ж������Ƿ���
		 */
		else if (
			LeftEdge[i] != 0 && //����û��
			RightEdge[i] == ColumnMax //��������
			)
		{
			RightLose++;//��¼ֻ�����߶�������

			/*
			 * ������ʮ�ֵ���������ͻ��
			 */
			 //ͻ��
			if (
				(RightEdge[i] - LeftEdge[i]) >=  /*��ǰ�п�*/
				(RightEdge[i + 1] - LeftEdge[i + 1] + 1)/*ǰһ�п�*/
				)
			{
				MiddleLine[i] = MiddleLine[i + 1];//����һ�е��е�    
			}
			/*
			 *�������������������û��ͻ��
			 */
			else
			{
				MiddleLine[i] = LeftEdge[i] + Width[i] / 2;//�����Ļ����ð��
			}
		}
		///�Գƶ���///
		/*
		 * �ж������Ƿ���
		 */
		else if (
			LeftEdge[i] == 0 && //��������
			RightEdge[i] != ColumnMax //����û��
			)
		{
			LeftLose++;
			if ((RightEdge[i] - LeftEdge[i]) >= (RightEdge[i + 1] - LeftEdge[i + 1] + 1))//ͻ��      
			{
				MiddleLine[i] = MiddleLine[i + 1];//����һ��   
			}
			else
			{
				MiddleLine[i] = RightEdge[i] - Width[i] / 2;//�߿�
			}
		}

		/*
		 * �ж������Ƿ񶼶���
		 */
		else if (LeftEdge[i] == 0 && RightEdge[i] == ColumnMax)//���߶����˵Ļ�  
		{
			AllLose++;
			if (i == RowMax - 1)//��������о���ͼ��������Ϊ�е�
			{
				MiddleLine[i] = MidPri;
			}
			else
			{
				MiddleLine[i] = MiddleLine[i + 1];//����������о�����һ�е�������Ϊ�����е�
			}
		}


#pragma endregion �ж��е�
#pragma region ����MidPri
		/*
		 * ���߲���̫��
		 */
		if (MiddleLine[RowMax - 1] >= 70)
		{
			MidPri = 70;
		}
		/*
		 * ���߲���̫��
		 */
		else if (MiddleLine[RowMax - 1] <= 10)
		{
			MidPri = 10;
		}
		/*
		 * ��¼MidPri
		 */
		else
		{
			MidPri = MiddleLine[RowMax - 1];//��¼��֡ͼ���59�е�����ֵ����Ϊ��һ��ͼ���59��ɨ����ʼ��
		}
#pragma endregion ����MidPri
	}

	/*
	 * //����ʣ����
	 */
	for (i = 49; i > 0; i--)
	{
#pragma region ����ɨ��

		/*
		 *�����߽粿��
		 *#1100
		 */
		 /*
		  * ��һ���ҵ�����
		  */
		if (
			LeftEdge[i + 1] != 0 &&
			RightEdge[i + 1] != ColumnMax
			)
		{
			/*
			 * j = min(LeftEdge[i + 1] + 10,ColumnMax - 2)
			 *
			 */
			j = (
				(LeftEdge[i + 1] + 10) >=//ǰһ���������Ͻ�10��
				ColumnMax - 2//������һ��
				) ?
				ColumnMax - 2 :
				(LeftEdge[i + 1] + 10);//������߽�
			/*
			 * j = max((LeftEdge[i + 1] - 5),1)
			 */
			jj = ((LeftEdge[i + 1] - 5) <= 1) ? 1 : (LeftEdge[i + 1] - 5);
			//j�ƽ�jj���ҵ��׺�����
			while (j >= jj)
			{
				if (img[i][j] == White_Point && img[i][j - 1] == Black_Point)
				{
					LeftEdge[i] = j;
					break;
				}
				j--;
			}
			///�Գ��ұ߽�///
			j = ((RightEdge[i + 1] - 10) <= 1) ? 1 : (RightEdge[i + 1] - 10); //�����ұ߽�   
			jj = ((RightEdge[i + 1] + 5) >= ColumnMax - 2) ? ColumnMax - 2 : (RightEdge[i + 1] + 5);
			while (j <= jj)
			{
				if (img[i][j] == White_Point && img[i][j + 1] == Black_Point)
				{
					RightEdge[i] = j;
					break;
				}
				j++;
			}


		}

		/*
		 * ��һ��ֻ�ҵ����
		 * #1102
		 */
		else if (LeftEdge[i + 1] != 0 && RightEdge[i + 1] == ColumnMax)//��һ��ֻ�ҵ���߽�   
		{
			j = ((LeftEdge[i + 1] + 10) >= ColumnMax - 2) ? ColumnMax - 2 : (LeftEdge[i + 1] + 10);//��߽��ñ���ɨ��   
			jj = ((LeftEdge[i + 1] - 5) <= 1) ? 1 : (LeftEdge[i + 1] - 5);
			while (j >= jj)
			{
				if (img[i][j] == White_Point && img[i][j - 1] == Black_Point)
				{
					LeftEdge[i] = j;
					break;
				}
				j--;
			}


			j = MiddleLine[i + 1];//��һ�ж����ұ߽���ȫ��ɨ�� 
			///�����Ƿϻ�
			if (j >= 78)//ColumnMax	80   ����
			{
				j = 78;
			}

			while (j <= ColumnMax - 2)
			{
				if (img[i][j] == White_Point && img[i][j + 1] == Black_Point)
				{
					RightEdge[i] = j;
					break;
				}
				j++;
			}

		}
		/*
		  * ��һ��ֻ�ҵ��ұ�
		  */
		else if (LeftEdge[i + 1] == 0 && RightEdge[i + 1] != ColumnMax) //��һ��ֻ�ҵ��ұ߽�      
		{
			j = ((RightEdge[i + 1] - 10) <= 1) ? 1 : (RightEdge[i + 1] - 10);//��Ե׷�����ұ߽� 
			jj = ((RightEdge[i + 1] + 5) >= ColumnMax - 2) ? ColumnMax - 2 : (RightEdge[i + 1] + 5);
			while (j <= jj)
			{
				if (img[i][j] == White_Point && img[i][j + 1] == Black_Point)
				{
					RightEdge[i] = j;
					break;
				}
				j++;
			}
			j = MiddleLine[i + 1]; //ȫ��ɨ������߽�
			if (j < 2)
			{
				j = 2;
			}
			while (j >= 1)
			{
				if (img[i][j] == White_Point && img[i][j - 1] == Black_Point)
				{
					LeftEdge[i] = j;
					break;
				}
				j--;
			}
		}

		/*
		 * //��һ��û�ҵ��߽磬������ʮ�ֻ��߻���
		 * ȫ��Ѱ�ң�
		 * û����
		 */
		else if (LeftEdge[i + 1] == 0 && RightEdge[i + 1] == ColumnMax)
		{
			j = MiddleLine[i + 1];   //��ȫ������߽�
			while (j >= 1)
			{
				if (img[i][j] == White_Point && img[i][j - 1] == Black_Point)
				{
					LeftEdge[i] = j;
					break;
				}
				j--;
			}

			j = MiddleLine[i + 1];   //ȫ�����ұ߽�   
			while (j <= ColumnMax - 2)
			{
				if (img[i][j] == White_Point && img[i][j + 1] == Black_Point)
				{
					RightEdge[i] = j;
					break;
				}
				j++;

			}
		}
#pragma endregion ����ɨ��
#pragma region ������
		//todo ����Ķ������д����

		/*
		 * ��������䣬��һ�б���һ�п�
		 * ����һ�е�����
		 *
		 */
		if ((RightEdge[i] - LeftEdge[i]) >= (RightEdge[i + 1] - LeftEdge[i + 1] + 1))//��������� 
		{
			MiddleLine[i] = MiddleLine[i + 1];//����һ��
		}
		/*
		 * ������䣬��һ�б���һ��խ
		 * �����ж�
		 */
		else
		{
			/*
			 * ���߶�û����
			 * ����������
			 */
			if (LeftEdge[i] != 0 && RightEdge[i] != ColumnMax)
			{
				MiddleLine[i] = (LeftEdge[i] + RightEdge[i]) / 2;

				//��б��ʮ�ֽ��о���


				/*
				 * ��������ͻ��
				 */
				if (MiddleLine[i] - MiddleLine[i + 1] > 8 && //������ƫ����8��
					((
						ABS(LeftEdge[i] - LeftEdge[i + 1] > 3) ||//����߸ı����3��
						ABS(RightEdge[i] - RightEdge[i + 1] > 3)//�ұ��߸ı����3��
						)) &&
					i >= 30)//���복С��30��
				{
					uint8 ii = i;

					/*
					 * ��k=1��б��/����֮ǰ�����������복10��
					 */
					while (1)
					{
						MiddleLine[ii + 1] = MiddleLine[ii] - 1;
						ii++;


						if (ii >= 50 || //һֱ�����복10��
							(MiddleLine[ii] - MiddleLine[ii + 1] <= 1)//������ǰ��������������С��1
							)
						{
							break;
						}
					}
				}

				/*
				 * ��������ͻ��
				 */
				if ((MiddleLine[i + 1] - MiddleLine[i] > 8) && ((ABS(LeftEdge[i] - LeftEdge[i + 1] > 3) || ABS(RightEdge[i] - RightEdge[i + 1] > 3))) && i >= 30)///����ͻ��
				{
					uint8 ii = i;

					while (1)
					{
						MiddleLine[ii + 1] = MiddleLine[ii] + 1;
						ii++;

						if (ii >= 50 || (MiddleLine[ii + 1] - MiddleLine[ii] <= 1))
						{
							break;
						}
					}
				}


			}
			/*
			 * �ұ��߶�ʧ
			 */
			else if (LeftEdge[i] != 0 && RightEdge[i] == ColumnMax)//find left
			{
				RightLose++;

				if (LeftEdge[i + 1] != 0)
				{
					MiddleLine[i] = MiddleLine[i + 1] + LeftEdge[i] - LeftEdge[i + 1];
				}
				else
				{
					MiddleLine[i] = LeftEdge[i] + Width[i] / 2;
				}
			}
			/*
			 * ����߶�ʧ
			 */
			else if (LeftEdge[i] == 0 && RightEdge[i] != ColumnMax)//find right
			{
				LeftLose++;


				if (RightEdge[i + 1] != ColumnMax)
				{

					MiddleLine[i] = MiddleLine[i + 1] + RightEdge[i] - RightEdge[i + 1];
				}
				else
				{

					MiddleLine[i] = RightEdge[i] - Width[i] / 2;
				}
			}
			/*
			 * �����߶���
			 */
			else if (LeftEdge[i] == 0 && RightEdge[i] == ColumnMax)//���߶���    
			{
				WhiteNum++;
				AllLose++;

				if (WhiteNum == 1)
				{
					WhiteStart = i;
				}
				MiddleLine[i] = MiddleLine[i + 1];
			}

		}
#pragma endregion ������
#pragma region ����Ч������
		/*
		 * �����������һ�о�ִֹͣ�к�������
		 */
		if (i == 0)
		{
			AvaliableLines = 60;
			LastLine = 0;
			break;
		}

		uint16 m = MiddleLine[i];//���ߵ�[5,75]��ֵ
		if (m < 5)
		{
			m = 5;
		}
		if (m > 75)
		{
			m = 75;
		}
		//���һ��   
		if (
			(
				LeftEdge[i] != 0 && //�������[65,80]
				LeftEdge[i] >= 65
				) ||
				(
					RightEdge[i] != ColumnMax &&
					RightEdge[i] < 15 //�ұ�����[0,15]
					) ||
					(i >= 1) && //������Զһ��
			(img[i - 1][m] == Black_Point)//��һ�е�����[5,75]��ֵ�Ǻ�
			)
		{
			LastLine = i;//���һ�У���̬ǰհ
			AvaliableLines = 60 - i;//��Ч����
			break;
		}
#pragma endregion ����Ч������
	}
}

/**
 * \brief //��ȡͼ�������
 * \par
 * //ѡȡ���У���ͼ��ײ�����ɨ��
 * \par
 * ��ʱû������ʲô��
 * \param[out] DropRow �ⶥ����
 */
void GetBlackEndParam()//��ȡ���߽�ֹ��
{
	/*
	 * ǰ�����������֮���־��1
	 */
	unsigned char LEndFlag = 0;//20ֹͣɨ���־
	unsigned char MEndFlag = 0;//40ֹͣɨ���־
	unsigned char REndFlag = 0;//60ֹͣɨ���־
	unsigned char MREndFlag = 0;//50ֹͣɨ���־
	unsigned char MLEndFlag = 0;//30ֹͣɨ���־
	unsigned char LLEndFlag = 0;//10ֹͣɨ���־
	unsigned char RREndFlag = 0;//70ֹͣɨ���־

	int i = 0;

	BlackEndMR = 0;//������50
	BlackEndML = 0;//������30
	BlackEndLL = 0;//������10
	BlackEndRR = 0;//������70
	BlackEndL = 0;//������20
	BlackEndM = 0;//������40
	BlackEndR = 0;//������60


	/*
	 * ɨ��������[3,60]
	 */
	for (i = RowMax - 1; i >= 3; i--)
	{
		/*
		 * ���к��߽�����40
		 */
		if (img[i][ColumnMax / 2] == White_Point && !MEndFlag)//!MEndFlag=1 //40
		{
			BlackEndM++;//�к��߽�����
		}
		else if (
			i > 1 &&
			img[i - 1][ColumnMax / 2] == Black_Point && //���������Ǻ�ɫ
			img[i - 2][ColumnMax / 2] == Black_Point//���������Ǻ�ɫ
			)
		{
			MEndFlag = 1;
		}
		/*
		 * ������߽�����20
		 */
		if (img[i][ColumnMax / 4] == White_Point && !LEndFlag)//20
		{
			BlackEndL++;//����߽�����
		}
		else if (
			i > 1 &&
			img[i - 1][ColumnMax / 4] == Black_Point &&
			img[i - 2][ColumnMax / 4] == Black_Point)
		{
			LEndFlag = 1;
		}
		/*
		 * ���Һ��߽�����60
		 */
		if (img[i][ColumnMax * 3 / 4] == White_Point && !REndFlag)//60
		{
			BlackEndR++;//�Һ��߽�����
		}
		else if (
			i > 1 &&
			img[i - 1][ColumnMax * 3 / 4] == Black_Point &&
			img[i - 2][ColumnMax * 3 / 4] == Black_Point
			)
		{
			REndFlag = 1;
		}
		/*
		 * ��������߽�����30
		 */
		if (img[i][30] == White_Point && !MLEndFlag)
		{
			BlackEndML++;
		}
		else if (i > 1 && img[i - 1][30] == Black_Point && img[i - 2][30] == Black_Point)
		{
			MLEndFlag = 1;
		}
		/*
		 * �����Һ��߽�����50
		 */
		if (img[i][50] == White_Point && !MREndFlag)
		{
			BlackEndMR++;
		}
		else if (i > 1 && img[i - 1][50] == Black_Point && img[i - 2][50] == Black_Point)
		{
			MREndFlag = 1;
		}
		/*
		 * ��������߽�����10
		 */
		if (img[i][10] == White_Point && !LLEndFlag)
		{
			BlackEndLL++;
		}
		else if (i > 1 && img[i - 1][10] == Black_Point && img[i - 2][10] == Black_Point)
		{
			LLEndFlag = 1;
		}
		/*
		 * �����Һ��߽�����70
		 */
		if (img[i][70] == White_Point && !RREndFlag)
		{
			BlackEndRR++;
		}
		else if (i > 1 && img[i - 1][70] == Black_Point && img[i - 2][70] == Black_Point)
		{
			RREndFlag = 1;
		}
	}

	/*
	 * ȡ�߸������е����ֵ
	 * ͨ��һϵ��max��������
	 */
	BlackEndMax = MAX(BlackEndL, BlackEndM);//ȡ��ֵ
	BlackEndMax = MAX(BlackEndMax, BlackEndR);
	BlackEndMaxMax = MAX(BlackEndMax, BlackEndMR);
	BlackEndMaxMax = MAX(BlackEndMax, BlackEndML);
	BlackEndMaxMax = MAX(BlackEndMax, BlackEndLL);
	BlackEndMaxMax = MAX(BlackEndMax, BlackEndRR);
	/*
	 * �������ֵ[0,56]
	 */
	if (BlackEndMaxMax >= 60)
	{
		BlackEndMaxMax = 58;
	}

	DropRow = 60 - BlackEndMaxMax;//�ⶥ������      
}



#if 1

//�����ҹյ㣬ʶ��
//һֱ��Ҫ����

/**0
 * \brief //Ѱ�ҹյ�
 */
void FindInflectionPoint()
{

	char i = 0;
	char i_i = 0;
	char i_i_i = 0;

	//��������

	RightInflectionPointRow = 0;//�ұߵĹյ��������
	RightInflectionPointCol = 0;//�ұߵĹյ��������
	LeftInflectionPointRow = 0;//��ߵĹյ��������
	LeftInflectionPointCol = 0;//��ߵĹյ��������
	RightInflectionPointFlag = 0;//�ҵ���յ�ı�־λ
	LeftInflectionPointFlag = 0;//�ҵ��ҹյ�ı�־λ

	StartRow = 0;//�ӵײ�����ɨ��Բ���м�ĺ�Բ����ʼ��
	StartCol = 0;//�ӵײ�����ɨ��Բ���м�ĺ�Բ����ʼ��

	LoopBorttomFlag = 0;//�ҵ�Բ���м�ĺ�Բ�ĵײ��ı�־λ
	LoopBorttomRow = 0;//Բ���м�ĺ�Բ�ĵײ���������
	LoopMilldleRow = 0;//Բ���м�ĺ�Բ�Ķ�����������
	LoopMilldleFlag = 0;//�ҵ�Բ���м�ĺ�Բ�Ķ����ı�־λ
	LoopTopRow = 0;//�ҵ��⻷�Ķ���
	MilldleBlack = 0;//Բ���м�ĺ�Բ�Ķ�����Բ���м�ĺ�Բ�ĵײ����������ƽ��ֵ
	LoopFlag = 0;//�ҵ������������͵ı�־

	 //�ҹյ�

	for (i = 59; i >= 25; i--)//����ɨ��̫Զ�����������
	{
		if (RightEdge[i] != ColumnMax && RightEdge[i - 1] != ColumnMax && RightEdge[i + 1] != ColumnMax) //�������в�����
		{

			if ((RightEdge[i] - RightEdge[i + 1] <= 0) && (RightEdge[i - 1] - RightEdge[i]) > 0)//�ҵ��ұ����йյ�
			{

				RightInflectionPointRow = i;//��¼�յ����
				RightInflectionPointCol = RightEdge[i];//��¼�յ����
				RightInflectionPointFlag = 1;//����ҵ��ҹյ�
				break;//�˳�for
			}

		}
	}

	//��յ�
	for (i = 59; i > 25; i--)
	{
		if (LeftEdge[i] != 0 && LeftEdge[i - 1] != 0 && LeftEdge[i + 1] != 0)
		{

			if ((LeftEdge[i] - LeftEdge[i + 1] >= 0) && (LeftEdge[i - 1] - LeftEdge[i] < 0))//�ҵ��յ�
			{
				LeftInflectionPointRow = i;//��¼�ùյ����

				LeftInflectionPointCol = LeftEdge[i];//��¼�ùյ����

				LeftInflectionPointFlag = 1;//����ҵ���յ�

				break;//�ҵ��˳�
			}
		}
	}

	//����ͬʱ�ҵ������յ㣬��ʼʶ��·(������Կ���һ�£����ֻ�ҵ�һ���յ��������������ܸ�����ʶ�𵽻�·)

	if (LeftInflectionPointFlag && RightInflectionPointFlag)//ͬʱ�ҵ������յ�
	{
		StartCol = (unsigned int)((LeftInflectionPointCol + RightInflectionPointCol) / 2);// ȡ���ҹյ��������ƽ��ֵ

		StartRow = (unsigned int)((LeftInflectionPointRow + RightInflectionPointRow) / 2);//ȡ���ҹյ���������ƽ��ֵ

		for (i = StartRow; i >= 8; i--)//�̶�һ�У�StartCol���ӿ�ʼ��������ɨ�裬Ѱ�һ�·�м��Բ���������ҵ�һ���׵��ڵ����䣬Ȼ����кڵ�Ȼ�󵽰ף�
		{
			if (img[i][StartCol] == White_Point && img[i - 1][StartCol] == Black_Point)
			{

				LoopBorttomRow = i - 1;//��¼��һ���������

				LoopBorttomFlag = 1;//��λ��־

				break;
			}

		}

		if (LoopBorttomFlag)//����ҵ���һ������
		{
			i_i = i - 2;//����һ��������п�ʼ��������Ѱ�Һ�->������

			while (i_i >= 8)
			{
				if (img[i_i][StartCol] == Black_Point && img[i_i - 1][StartCol] == White_Point)
				{


					LoopMilldleRow = i_i - 1;//��¼�ڶ��������������
					LoopMilldleFlag = 1;//��־λ��λ
					break; //�˳�while   

				}

				i_i--;

			}
		}


		if (LoopMilldleFlag)//�ҵ�Բ���м�ĺ�Բ�Ķ���
		{
			i_i_i = i_i - 1;

			while (i_i_i > 8)
			{
				if (img[i_i_i][StartCol] == White_Point && img[i_i_i - 1][StartCol] == Black_Point)//�ɰ׵������䣬�ҵ�
				{
					LoopTopRow = i_i_i - 1;

					if (LoopBorttomRow - LoopMilldleRow >= 4 && AllLose < 1 && (LoopMilldleRow - LoopTopRow) <= 5)//�ж�һ��Բ�ĺڵ���
					{


						if (ABS(LeftEdge[StartRow - 2] - RightEdge[StartRow - 2]) - Width[StartRow - 2] >= 6 && ABS(LeftEdge[StartRow - 3] - RightEdge[StartRow - 3]) - Width[StartRow - 3] >= 10 && ABS(LeftEdge[StartRow - 4] - RightEdge[StartRow - 4]) - Width[StartRow - 4] >= 12)//�������ͻ��
						{
							LoopFlag = 1;//����������־λ��
							break;
						}
					}

				}

				i_i_i--;
			}
		}
	}
}
//���¼��ж��ߣ�����һ�ź�Բ


#endif



#if 1

//Ѱ�һ��γ���
//�ҵ�Բ��4���е�
//���м�ĺڿ���г�����  

void FindLoopExit()

{
	int i, j;
	unsigned char i_i;
	unsigned char RightScanFlag;
	unsigned char LeftScanFlag;

	LoopLeft = 0;
	MilldlePonit = 0;
	LoopRight = 0;
	LoopRightR = 0;
	LoopLeftL = 0;
	LeftUpExitFlag = 0;
	RightUpExitFlag = 0;
	ExitFlag = 0;
	RightExitFlag = 0;
	LeftExitFlag = 0;
	RightScanFlag = 0;
	LeftScanFlag = 0;


	if (LoopFlag)
	{

		//�Ӻ�Բ���м�������Ѱ������

		MilldlePonit = LoopBorttomRow - ((LoopBorttomRow - LoopMilldleRow) / 2);

		for (j = StartCol; j < 76; j++)
		{
			if (img[MilldlePonit][j] == Black_Point && img[MilldlePonit][j + 1] == Black_Point && img[MilldlePonit][j + 2] == White_Point)
			{

				LoopRight = j + 1;
				//break;
			}

			unsigned char j_j = j + 2;

			if (LoopRight)
			{

				while (j_j < 76)
				{
					if (img[MilldlePonit][j_j] == White_Point && img[MilldlePonit][j_j + 1] == White_Point && img[MilldlePonit][j_j + 2] == Black_Point)
					{
						LoopRightR = j_j + 2;

						break;
					}

					j_j++;
				}

			}

			if (LoopRightR)
			{
				break;

			}
		}

		for (j = StartCol; j > 3; j--)
		{
			if (img[MilldlePonit][j] == Black_Point && img[MilldlePonit][j - 1] == Black_Point && img[MilldlePonit][j - 2] == White_Point)
			{

				LoopLeft = j - 1;
				//break;
			}

			unsigned char jj = j - 2;

			if (LoopLeft)
			{

				while (jj > 3)
				{
					if (img[MilldlePonit][jj] == White_Point && img[MilldlePonit][jj - 1] == White_Point && img[MilldlePonit][jj - 2] == Black_Point)
					{
						LoopLeftL = jj - 2;
						break;
					}

					jj--;
				}

			}

			if (LoopLeftL)
			{
				break;
			}
		}

		//�ȴ������ҳ���


		for (i = 0; i < 80; i++)
		{

			BigLooptUp[i] = 0;
		}

		for (j = LoopLeft - 10; j < LoopRight + 10; j++)
		{
			for (i = MilldlePonit; i > 5; i--)
			{
				if (img[i][j] == White_Point && img[i - 1][j] == Black_Point)
				{
					BigLooptUp[j] = i - 1;
					break;
				}

			}

		}

		for (j = 2; j < 75; j++)
		{

			if (BigLooptUp[j] != 0 && BigLooptUp[j + 1] != 0 && BigLooptUp[j - 1] != 0)
			{
				if (ABS(BigLooptUp[j + 1] - BigLooptUp[j]) >= 2 && ABS(BigLooptUp[j + 1] - BigLooptUp[j - 1]) >= 2)
				{

					ExitFlag = j;
					break;

				}
				else
				{
					ExitFlag = 0;
				}

			}

		}

		if (ExitFlag != 0)
		{

			if (ExitFlag >= StartCol)
			{
				RightUpExitFlag = 1;

			}
			else if (ExitFlag < StartCol)
			{

				LeftUpExitFlag = 1;
			}

		}

		//�������û���ҵ����ڣ��Ϳ�����û�ж�һ�� 


		for (i = 0; i < 60; i++)
		{

			BigLoopLeftUp[i] = 0;
			BigLoopRightUp[i] = 0;
		}

		if ((!LeftUpExitFlag) && (!RightUpExitFlag) && LoopFlag && LoopLeftControlFlag == 0 && LoopRightControlFlag == 0)//����Ĺ�
		{

			if (StartCol >= 39)
			{
				LeftScanFlag = 1;//��־������ҳ���

				for (i = StartRow; i > LoopMilldleRow - 5; i--)
				{

					for (j = LoopLeft - 2; j > 1; j--)
					{

						if (img[i][j + 1] == White_Point)//���ж�ǰһ���ǲ��ǰ׵�
						{

							if (img[i][j] == White_Point && img[i][j - 1] == Black_Point)//�ɰ׵�������
							{

								BigLoopLeftUp[i] = j - 1;
								break;
							}

						}

						else

						{
							break;

						}
					}


				}

				for (i = StartRow - 1; i > LoopMilldleRow - 5; i--)
				{

					if (BigLoopLeftUp[i] != 0 && BigLoopLeftUp[i - 1] == 0)
					{

						BreakStartL = BigLoopLeftUp[i];
						BreakStartLFlag = 1;
						break;
					}
					else
					{
						BreakStartL = 0;
						BreakStartLFlag = 0;
					}
				}

				if (BreakStartLFlag)
				{

					i_i = i - 1;

					while (i_i > LoopMilldleRow - 5)

					{
						if (BigLoopLeftUp[i_i] == 0 && BigLoopLeftUp[i_i - 1] != 0)
						{

							BreakEndL = BigLoopLeftUp[i - 1];
							BreakEndLFlag = 1;
							break;
						}

						else
						{

							BreakEndL = 0;
							BreakEndLFlag = 0;

						}

						i_i--;
					}

				}

				if (BreakEndLFlag && BreakStartLFlag)
				{
					if (ABS(BreakStartL - BreakEndL >= 6))
					{
						LeftExitFlag = 1;
						//break;
					}

				}
			}


			else if (StartCol < 39)
			{
				RightScanFlag = 1;//��־���ұ��ҳ���

				for (i = StartRow; i > LoopMilldleRow - 5; i--)
				{

					for (j = LoopRight + 2; j < 79; j++)
					{

						if (img[i][j - 1] == White_Point)
						{

							if (img[i][j] == White_Point && img[i][j + 1] == Black_Point)
							{

								BigLoopRightUp[i] = j - 1;
								break;
							}
						}
						else
						{
							break;

						}

					}
				}

				for (i = StartRow - 1; i > LoopMilldleRow - 5; i--)
				{

					if (BigLoopRightUp[i] != 0 && BigLoopRightUp[i - 1] == 0)
					{

						BreakStartR = BigLoopRightUp[i];
						BreakStartRFlag = 1;
						break;
					}
					else
					{
						BreakStartR = 0;
						BreakStartRFlag = 0;
					}
				}

				if (BreakStartRFlag)
				{

					i_i = i - 1;

					while (i_i > LoopMilldleRow - 5)

					{
						if (BigLoopRightUp[i_i] == 0 && BigLoopRightUp[i_i - 1] != 0)
						{

							BreakEndR = BigLoopLeftUp[i - 1];
							BreakEndRFlag = 1;
							break;
						}

						else
						{

							BreakEndR = 0;
							BreakEndRFlag = 0;

						}

						i_i--;
					}

				}

				if (BreakEndRFlag && BreakStartRFlag)
				{
					if (ABS(BreakStartR - BreakEndR >= 6))
					{
						RightExitFlag = 1;
						//break;
					}

				}

			}


			if (RightScanFlag && !RightExitFlag)//��������ұ��Ƿ��г��ڣ��������ұ�û���ҵ�����
			{
				LeftExitFlag = 1;//Ĭ�ϳ��������
			}

			else if (LeftScanFlag && !LeftExitFlag)
			{
				RightExitFlag = 1;

			}

		}

	}

}

#endif


#if 1

//��·�Ŀ���
//�ⲿ����

void LoopControl()
{

	static int x, y;


	if ((LeftUpExitFlag || LeftExitFlag) && !(RightUpExitFlag || RightExitFlag) && LoopLeftControlFlag == 0 && LoopRightControlFlag == 0)//���������
	{
		x++;
		if (x >= 3)//�˲�
		{
			x = 0;
			LoopLeftControlFlag = 1;//��ȡ��·��ߵĿ��Ʊ�־λ
		}
	}


	if (!(LeftUpExitFlag || LeftExitFlag) && (RightUpExitFlag || RightExitFlag) && LoopLeftControlFlag == 0 && LoopRightControlFlag == 0)
	{

		y++;
		if (y >= 3)
		{
			LoopRightControlFlag = 1;//��ȡ��·�ұߵĿ��Ʊ�־λ
			y = 0;
		}


	}

}

#endif


#if 1

//����������ڲ���

void LoopRepair()
{
	int i;


	if (LoopRightControlFlag)//�������˳������ұߵĿ����ź�
	{

		for (i = 58; i > 15; i--)
		{

			MiddleLine[i] = RightEdge[i] - Width[i] / 2;

		}


		if (LeftEdge[58] == 0 && LeftEdge[57] == 0 && LeftEdge[56] == 0)//�ײ�����ȫ������

		{

			ClearLoopControlFlag = 1;//����������Ʊ�־

			for (i = 58; i > 15; i--)
			{

				MiddleLine[i] = RightEdge[i] - Width[i] / 2;
			}

		}

		if (ClearLoopControlFlag && LeftEdge[45] != 0 && LeftEdge[46] != 0)//���־
		{
			// gpio_set   (PTB19, 0);
			LoopRightControlFlag = 0;
			LoopLeftControlFlag = 0;
			ClearLoopControlFlag = 0;
			MotivateLoopDlayFlagR = 1;
		}


	}

	//���


	else if (LoopLeftControlFlag)
	{
		for (i = 58; i > 15; i--)
		{
			MiddleLine[i] = LeftEdge[i] + Width[i] / 2;

		}


		if (RightEdge[58] == 80 && RightEdge[57] == 80 && RightEdge[56] == 80)

		{

			ClearLoopControlFlag = 1;

			for (i = 58; i > 15; i--)
			{

				MiddleLine[i] = LeftEdge[i] + Width[i] / 2;
			}

		}



		if (ClearLoopControlFlag && RightEdge[45] != 80 && RightEdge[46] != 80)
		{
			LoopRightControlFlag = 0;
			LoopLeftControlFlag = 0;
			ClearLoopControlFlag = 0;
			MotivateLoopDlayFlagL = 1;//��ʱ��ʼ
		}


	}

}




#endif

//��·���ڲ��ߴ���

void LoopExitRepair()
{

	int i;
	static unsigned char  ClearInLoopLeftInflectionPointFlag = 0;
	static unsigned char  ClearInLoopRightInflectionPointFlag = 0;
	static unsigned char  OpenLoopExitRepairNumR = 0;
	static unsigned char  OpenLoopExitRepairNumL = 0;
	static unsigned char  LastInLoopLeftInflectionPointFlag = 0;
	static unsigned char  LastLastInLoopLeftInflectionPointFlag = 0;
	static unsigned char  LastInLoopRightInflectionPointFlag = 0;
	static unsigned char  LastLastInLoopRightInflectionPointFlag = 0;
	static unsigned char  InLoopDlay = 0;


	if (MotivateLoopDlayFlagL && OpenLoopExitRepairFlagL == 0 && MotivateLoopDlayFlagR == 0)//�ڻ�������,���˻���ʶ���ʮ��ʶ��
	{
		OpenLoopExitRepairNumL++;//����С��ʱ

		if (OpenLoopExitRepairNumL >= 6)//��ʱ����
		{
			OpenLoopExitRepairNumL = 0;
			OpenLoopExitRepairFlagL = 1;//�ڻ�·����򿪳���Ѱ�Һ���

		}

	}

	if (OpenLoopExitRepairFlagL)//���
	{

		LastLastInLoopLeftInflectionPointFlag = LastInLoopLeftInflectionPointFlag;
		LastInLoopLeftInflectionPointFlag = InLoopLeftInflectionPointFlag;


		for (i = 59; i > 20; i--) //ǰ��40��
		{

			if (LeftEdge[i] != 0 && LeftEdge[i - 1] != 0 && LeftEdge[i + 1] != 0)
			{
				if ((LeftEdge[i] - LeftEdge[i + 1] >= 0) && (LeftEdge[i - 1] - LeftEdge[i] <= -1))

				{

					InLoopLeftInflectionPointRow = i - 1;
					InLoopLeftInflectionPointFlag = 1;
					break;

				}
				else

				{

					InLoopLeftInflectionPointRow = 0;

					InLoopLeftInflectionPointFlag = 0;
				}

			}

		}

		if (InLoopLeftInflectionPointFlag = 1 && (InLoopLeftInflectionPointRow >= 25))
		{

			ClearInLoopLeftInflectionPointFlag = 1;

		}

		if (ClearInLoopLeftInflectionPointFlag)
		{
			gpio_set(PTB19, 1);

			for (i = 58; i > 15; i--)
			{

				MiddleLine[i] = LeftEdge[i] + Width[i] / 2;

			}

			InLoopDlay++;//������ʱ�������־

			if (InLoopDlay >= 100)//�������û�û�������־������,ǿ�����־
			{
				gpio_set(PTB19, 0);
				ClearInLoopLeftInflectionPointFlag = 0;
				LastLastInLoopLeftInflectionPointFlag = 0;
				LastInLoopLeftInflectionPointFlag = 0;
				MotivateLoopDlayFlagR = 0;
				MotivateLoopDlayFlagL = 0;
				InLoopDlay = 0;
				OpenLoopExitRepairFlagL = 0;
				OpenLoopExitRepairFlagR = 0;

			}


		}

		if (ClearInLoopLeftInflectionPointFlag && (LastInLoopLeftInflectionPointFlag && LastLastInLoopLeftInflectionPointFlag && !InLoopLeftInflectionPointFlag))
		{
			gpio_set(PTB19, 0);
			ClearInLoopLeftInflectionPointFlag = 0;
			LastLastInLoopLeftInflectionPointFlag = 0;
			LastInLoopLeftInflectionPointFlag = 0;
			MotivateLoopDlayFlagR = 0;
			MotivateLoopDlayFlagL = 0;
			InLoopDlay = 0;
			OpenLoopExitRepairFlagL = 0;
			OpenLoopExitRepairFlagR = 0;
		}

	}

	//�ұ�
	if (MotivateLoopDlayFlagR && OpenLoopExitRepairFlagR == 0 && MotivateLoopDlayFlagL == 0)//�ڻ�������,���˻���ʶ���ʮ��ʶ��
	{

		OpenLoopExitRepairNumR++;//����С��ʱ

		if (OpenLoopExitRepairNumR >= 6)
		{
			OpenLoopExitRepairNumR = 0;
			OpenLoopExitRepairFlagR = 1;
		}

	}



	if (OpenLoopExitRepairFlagR)
	{

		LastLastInLoopRightInflectionPointFlag = LastInLoopRightInflectionPointFlag;
		LastInLoopRightInflectionPointFlag = InLoopRightInflectionPointFlag;

		for (i = 59; i > 20; i--)
		{

			if (RightEdge[i] != 80 && RightEdge[i - 1] != 80 && RightEdge[i + 1] != 80)
			{
				if (RightEdge[i] - RightEdge[i + 1] <= 0 && (RightEdge[i - 1] - RightEdge[i] >= 1))//�ҵ��յ�
				{
					InLoopRightInflectionPointRow = i - 1;

					InLoopRightInflectionPointFlag = 1;
					break;
				}
				else

				{

					InLoopRightInflectionPointRow = 0;

					InLoopRightInflectionPointFlag = 0;
				}
			}

		}

		if (InLoopRightInflectionPointFlag = 1 && (InLoopRightInflectionPointRow >= 25))
		{

			ClearInLoopRightInflectionPointFlag = 1;

		}
		if (ClearInLoopRightInflectionPointFlag)
		{
			gpio_set(PTB19, 1);

			for (i = 58; i > 15; i--)
			{

				MiddleLine[i] = RightEdge[i] - Width[i] / 2;

			}

			InLoopDlay++;//������ʱ�������־

			if (InLoopDlay >= 100)//�������û�û�������־������,ǿ�����־
			{

				gpio_set(PTB19, 0);
				ClearInLoopRightInflectionPointFlag = 0;
				LastInLoopRightInflectionPointFlag = 0;
				LastLastInLoopRightInflectionPointFlag = 0;
				InLoopDlay = 0;
				MotivateLoopDlayFlagR = 0;
				MotivateLoopDlayFlagL = 0;
				OpenLoopExitRepairFlagL = 0;
				OpenLoopExitRepairFlagR = 0;
			}

		}


		if (ClearInLoopRightInflectionPointFlag && (LastInLoopRightInflectionPointFlag && LastLastInLoopRightInflectionPointFlag && (!InLoopRightInflectionPointFlag)))

		{
			gpio_set(PTB19, 0);
			ClearInLoopRightInflectionPointFlag = 0;
			LastInLoopRightInflectionPointFlag = 0;
			LastLastInLoopRightInflectionPointFlag = 0;
			InLoopDlay = 0;
			MotivateLoopDlayFlagR = 0;
			MotivateLoopDlayFlagL = 0;
			OpenLoopExitRepairFlagL = 0;
			OpenLoopExitRepairFlagR = 0;
		}

	}
}





