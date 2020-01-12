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
  Find_Gap,//����Բ�����ģ���Ϊ��ȱ��
  Into_Round,//���Խ���
  Into_Round2,
  In_Round,
  Out_Round,
  Leave_Round,
  Leave_Round2

}RoundProcessTypeDef;

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


extern Str_Round  Round;
extern void Round_Deal();
extern RoundProcessTypeDef RoundProcess;;

#endif