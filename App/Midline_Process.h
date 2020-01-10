#ifndef _Midline_Process_H_
#define _Midline_Process_H_

#include "Extract_Edge.h"


extern float LineWeight[ROW_END+1];


extern float Getrouteoffset(unsigned char start,unsigned char end,float midpos) ;
extern float Getrouteoffset2(unsigned char start,unsigned char end,float midpos);
extern int ExtractCenterline(Str_Boundary  *str);


//extern int StableNumbers;
//extern int StableNumbers2;
//extern double MidLineVariance;
//extern int TopE1;//�������ϲ���ƫ��
//extern int TopE2;//�������²���ƫ��
//extern int SubBasePoint;//�²��ֻ�׼�����ƫ��
//extern float Mid_K1;
//extern float Mid_K2;
//extern int MidLineExcursion;
//extern int CompensateCount;
//extern unsigned char IsCrossing;
//extern unsigned char ValidLine[60];
//extern int32 left_speed,right_speed,direction,speed;
//void SERVO_DIANJI_GO(uint32 t1,uint32 t2);
//void black_deal(uint8 mid,uint8* imgb);
//
//void GetLMR();//��ȡ������
//void LAverageFilter();
//void RAverageFilter();
//void AverageFilter();
//void GetFinalMidLine();//ͨ�����ұ�Ե����ö������������
//void MidLineCompensate();//�����߲���
//void StoreMidLine();
//void GetSectionParam();
//void GetMidLineVariance();//��ȡ������ƫ��
//void GetSpecialError();//��ȡ����������ƫ��
//void GetCrossingMidLine();//��ȡʮ��������
//void CommonRectificate(unsigned char data[],unsigned char begin,unsigned char end);//�������
//void SCProcessing();//��ʮ�ִ���
//void ProcessCrossing();//ʮ�ִ���
//void CrossingMidFilter();//����ȡʮ�ֽ����˲�
//void MidLineProcess();//�����ߴ���
//void UseTemMidLine();

#endif