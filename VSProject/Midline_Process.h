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
//extern int TopE1;//中心线上部分偏差
//extern int TopE2;//中心线下部分偏差
//extern int SubBasePoint;//下部分基准点最大偏差
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
//void GetLMR();//提取中心线
//void LAverageFilter();
//void RAverageFilter();
//void AverageFilter();
//void GetFinalMidLine();//通过左右边缘来获得舵机控制中心线
//void MidLineCompensate();//中心线补偿
//void StoreMidLine();
//void GetSectionParam();
//void GetMidLineVariance();//获取中心线偏差
//void GetSpecialError();//获取特殊中心线偏差
//void GetCrossingMidLine();//获取十字中心线
//void CommonRectificate(unsigned char data[],unsigned char begin,unsigned char end);//曲线拟合
//void SCProcessing();//正十字处理
//void ProcessCrossing();//十字处理
//void CrossingMidFilter();//新提取十字进行滤波
//void MidLineProcess();//中心线处理
//void UseTemMidLine();

#endif