#ifndef _Cross_Deal_H_
#define _Cross_Deal_H_

/*------------------------------�궨��ö��-----------------------------*/





/*------------------------------����������------------------------------*/
typedef  struct
{
       unsigned char Cross_flag;      //ʮ�ֱ�־λ
       unsigned char Cross_flag_able; //ʮ�ֱ�־λʹ��

       unsigned char Track_Width_forCross[ROW_END];//�������

       unsigned char addR_flag ;
       unsigned char addL_flag ;

       unsigned char Entrance_count;
       unsigned char Gap_count;

}Str_Cross;    //Բ���ṹ��


//ö��
typedef enum
{
       Cro_else ,
       Cro_LeftLean,     //ʮ�������־λ
       Cro_RightLean,    //ʮ�������־λ
       Cro_ZHENG        //��ʮ��־λ
}Enum_CrossType;

typedef enum
{
       Cross_Not_Found,
       Find_Cross,
       In_Cross,
       Out_Cross
}Enum_CrossProcess;
/*------------------------------����������------------------------------*/

extern Str_Cross   Cross;
extern Enum_CrossType  CrossType;
extern void Cross_Deal();//ʮ�ּ��
extern Enum_CrossProcess  CrossProcess;






#endif


