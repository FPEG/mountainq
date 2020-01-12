#ifndef _ROAD_CHECK_H_
#define _ROAD_CHECK_H_

typedef enum
{
       else_road,
       Break_RoadType,
       Barrier_Road,
       Round_Road,
       Ramp          //�µ�

} enum_RoadType;

typedef  struct
{
       unsigned char EN_Break_RoadType;   //��·ʹ�ܱ�־λ
       unsigned char EN_Barrier;         //�ϰ�ʹ�ܱ�־λ
       unsigned char EN_Ramp ;         //�µ�ʹ�ܱ�־λ
       unsigned char EN_Round;     //Բ��ʹ�ܱ�־λ
       unsigned char EN_Cross;      //ʮ��ʹ�ܱ�־λ


}Str_RoadEN;    //Բ���ṹ��


extern  enum_RoadType  RoadType;
extern  Str_RoadEN   REN;

extern int  max_speed;
extern int  nor_speed;
extern int  rou_speed;
extern int  Break_speed;
extern int  Bar_speed;



extern void RTRecognition();

#endif