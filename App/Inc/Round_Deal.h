#ifndef _Round_Deal_H_
#define _Round_Deal_H_


typedef enum __RoundType
{
  Left_Round=1,
  Right_Round
}RoundTypeDef;


typedef enum __RoundSize
{
  Small_Round=1,
//  Middle_Round,               //中环不考虑了
  Large_Round

}RoundSizeTypeDef;

typedef enum __RoundProcess
{
  Round_Not_Found=0,
  Find_Round,
  Find_Gap,
  Into_Round,
  Into_Round2,
  In_Round,
  Out_Round,
  Leave_Round,
  Leave_Round2

}RoundProcessTypeDef;

typedef  struct
{
       unsigned char Round_flag;     //圆环标志位
       unsigned char Round_flag_able; //圆环标志位使能

       unsigned char Entrance_count;

       unsigned char Track_Width_forRound[ROW_END];//赛道宽度

}Str_Round;    //圆环结构体


extern Str_Round  Round;
extern void Round_Deal();
extern RoundProcessTypeDef RoundProcess;;

#endif