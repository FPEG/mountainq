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
//  Middle_Round,               //�л���������
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
       unsigned char Round_flag;     //Բ����־λ
       unsigned char Round_flag_able; //Բ����־λʹ��

       unsigned char Entrance_count;

       unsigned char Track_Width_forRound[ROW_END];//��������

}Str_Round;    //Բ���ṹ��


extern Str_Round  Round;
extern void Round_Deal();
extern RoundProcessTypeDef RoundProcess;;

#endif