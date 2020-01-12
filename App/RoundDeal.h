#ifndef __ROUNDDEAL_H__
#define __ROUNDDEAL_H__
#include "common.h"
#include "include.h"

/**
 * \brief //Բ���ṹ��
 */
typedef  struct
{
	unsigned char Round_flag;     //Բ����־λ//�ǻ�Ϊ1
	unsigned char Round_flag_able; //Բ����־λʹ��
	unsigned char Entrance_count;//Բ����ڼ���
	unsigned char Track_Width_forRound[ROW_END];//�������
}Str_Round;
typedef  struct
{
	Site_t  L_down_point;//�����ʮ���·�ֱ�ǹյ����飨[0]Ϊʮ�ֹյ�������ֵ��[1]Ϊʮ�ֹյ�������ֵ��
	Site_t  L_up_point;//�����ʮ���Ϸ�ֱ�ǹյ����飨[0]Ϊʮ�ֹյ�������ֵ��[1]Ϊʮ�ֹյ�������ֵ��
	Site_t  R_down_point;//�ұ���ʮ���·�ֱ�ǹյ����飨[0]Ϊʮ�ֹյ�������ֵ��[1]Ϊʮ�ֹյ�������ֵ��
	Site_t  R_up_point;//�ұ���ʮ���Ϸ�ֱ�ǹյ����飨[0]Ϊʮ�ֹյ�������ֵ��[1]Ϊʮ�ֹյ�������ֵ��

}Str_Inflection_point;   //�յ�ṹ��
extern Str_Round  Round;

extern void Round_Deal();
typedef  struct
{
	//int   RightEdge[ROW];//��߽����飬�����һ��Ԫ��60��
	//int   LeftEdge[ROW];//�ұ߽�����
	//int   MiddleLine[ROW];
	int* RightEdge;
	int* LeftEdge;
	int* MiddleLine;

	//unsigned char   RowLose[ROW];
	unsigned char   *RowLose;

	unsigned char   AllLose;//����ͬʱ���ߵļ�¼ֵ
	unsigned char   OnlyLeftLose;//ֻ����߽綪�ߵļ�¼ֵ
	unsigned char   OnlyRightLose;//ֻ���ұ߽綪�ߵļ�¼ֵ
	unsigned char   AllLeftLose;//��߽����ж��߼�¼ֵ
	unsigned char   AllRightLose;//�ұ߽����ж��߼�¼ֵ

	//unsigned char   track_Width[ROW];//�������
	unsigned char   *track_Width;//�������


	unsigned char L_StartLine;                //���ߵ�һ�λ���ߵ��������������������������  -1
	unsigned char R_StartLine;                //���ߵ�һ�λ���ߵ��������ұ�����������������  -1
	unsigned char StartLine;

	unsigned char L_EndLine;                //�������������������  -1
	unsigned char R_EndLine;                //�ұ�����������������  -1
	unsigned char EndLine;//��̬�ⶥ��
	unsigned char LastEndLine;//��¼�ϴηⶥ�еļ�¼ֵ

	unsigned char Left_Edge_num;
	unsigned char Right_Edge_num;
}Str_Boundary;
//extern RoundProcessTypeDef RoundProcess;;
extern Str_Boundary          Boundary;
extern Str_Inflection_point  Inflection_point;

typedef enum __RoundType
{
	Left_Round = 1,
	Right_Round
}RoundTypeDef;


typedef enum __RoundSize
{
	Small_Round = 1,
	//  Middle_Round,               //�л���������
	Large_Round

}RoundSizeTypeDef;

typedef enum __RoundProcess
{
	Round_Not_Found = 0,
	Find_Round,
	Find_Gap,//����Բ�����ģ���Ϊ��ȱ��
	Into_Round,//���Խ���
	Into_Round2,
	In_Round,
	Out_Round,
	Leave_Round,
	Leave_Round2

}RoundProcessTypeDef;
extern RoundProcessTypeDef RoundProcess;
#endif

