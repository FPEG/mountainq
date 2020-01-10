#ifndef _ROAD_CHECK_H_
#define _ROAD_CHECK_H_

typedef enum
{
       else_road,
       Break_RoadType,
       Barrier_Road,
       Round_Road,
       Ramp          //坡道

} enum_RoadType;

typedef  struct
{
       unsigned char EN_Break_RoadType;   //断路使能标志位
       unsigned char EN_Barrier;         //障碍使能标志位
       unsigned char EN_Ramp ;         //坡道使能标志位
       unsigned char EN_Round;     //圆环使能标志位
       unsigned char EN_Cross;      //十字使能标志位


}Str_RoadEN;    //圆环结构体


extern  enum_RoadType  RoadType;
extern  Str_RoadEN   REN;

extern int  max_speed;
extern int  nor_speed;
extern int  rou_speed;
extern int  Break_speed;
extern int  Bar_speed;



extern void RTRecognition();

#endif