#ifndef _Break_Road_H_
#define _Break_Road_H_

/*------------------------------�궨��ö��-----------------------------*/



/*----------------------------------------------------------------------*/

/*------------------------------����������------------------------------*/
  typedef  struct
{
   unsigned char Break_Road_Flag;
   unsigned char Break_Road_preFlag; //���жϱ�־λ


}Str_Break_Road;


typedef  struct
{
   unsigned char Barrier_Flag;
   int  Bar_Err;

}Str_Barrier;

typedef enum
{
       No_Bar,
       Find_Bar,
       In_Bar,
       Out_Bar
}Enum_Bar_Process;

typedef enum
{
  Left_Run,
  Right_Run,
  Back_Run
}Enum_BarType;


extern int BDet_distance;
extern Str_Break_Road  Break_Road;
extern Str_Barrier  Barrier;
extern Enum_Bar_Process  Bar_Process; //�ϰ�·�ι��̼���
/*----------------------------------------------------------------------*/

/*------------------------------����������------------------------------*/
extern void Barrier_Deal();
extern void Break_Road_Deal();//��·����
/*----------------------------------------------------------------------*/






#endif


