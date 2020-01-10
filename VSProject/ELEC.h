#ifndef _ELEC_h
#define _ELEC_h

#include "include.h"

#define FILTER_TIMES 10
#define Round_SUM    1000
#define ELEC_Plus_Middle 1
#define ELEC_Plus_Left ELEC_Plus_Middle
#define ELEC_Plus_Right ELEC_Plus_Middle
#define ELEC_Plus_Right_Ver ELEC_Plus_Right
#define ELEC_Plus_Left_Ver ELEC_Plus_Left

#define eInto_Left_Round_Offset 50//50
#define eOut_Left_Round_Offset  35//30
#define eInto_Right_Round_Offset -44//52
#define eOut_Right_Round_Offset -35//32

typedef enum __eRoundProcess
{
  eRound_Not_Found=0,
  eFind_Round,//1
  eInto_Round,//2
  eIn_Round,//3
  eOut_Round,//4
  eLeave_Round//5
}eRoundProcessTypeDef;


typedef enum __eRoundType
{
  eNo_Round,
  eLeft_Round,
  eRight_Round
}eRoundTypeDef;

typedef struct _ELEC
{
  uint16 left_hor_ad;//左边横电感
  uint16 left_ver_ad;//左边竖直电感
  uint16 right_hor_ad;//右边横电感
  uint16 right_ver_ad;//右边数值电感
  uint16 Mid_hor_ad;//中间电感
  uint32 Elec_Sum[5];
  float Elec_Data[5][FILTER_TIMES];
  float Lvbojieguo[5];
  float eSum;

}ElecTypeDef;






extern int signO,s,count,type[4];
extern int Ring_state;
extern int ValueOfAD[5];
extern float lvbojieguo[5];
void Push_And_Pull(float *buff,int len,float newdata);
void fillter_middle(ElecTypeDef *eStr);
void s_fillter(void);
void elec_init(void);
void elec(void);
void circle1(void);
void circle2(void);
void get_elec(ElecTypeDef *eStr);
void display(void);
void cc(void);
void Elec_Lose_Protect();
extern void ELEC_Display(void);
extern void ELEC_Display_ADC();





extern uint8  tanshe_flag;
extern uint16 l_hor_max, l_hor_min;
extern uint16 l_ver_max, l_ver_min;
extern uint16 r_hor_max, r_hor_min;
extern uint16 r_ver_max, r_ver_min;
extern int16 turn_angle;
extern uint16 L3_ad,L4_ad,L5_ad; 
extern uint16 left_hor_ad, left_ver_ad;//左边横电感，左边竖电感
extern uint16 right_hor_ad, right_ver_ad;//右边横电感，右边竖电感

extern int16 L3_deal,L4_deal,L5_deal; 
extern int16 left_hor_deal, left_ver_deal;//左边横电感，左边竖电感
extern int16 right_hor_deal, right_ver_deal;//右边横电感，右边竖电感

extern float ELEC_err;
extern int16 position;
#endif
