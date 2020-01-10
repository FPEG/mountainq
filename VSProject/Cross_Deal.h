#ifndef _Cross_Deal_H_
#define _Cross_Deal_H_

/*------------------------------宏定义枚举-----------------------------*/





/*------------------------------变量声明区------------------------------*/
typedef  struct
{
       unsigned char Cross_flag;      //十字标志位
       unsigned char Cross_flag_able; //十字标志位使能

       unsigned char Track_Width_forCross[ROW_END];//赛道宽度

       unsigned char addR_flag ;
       unsigned char addL_flag ;

       unsigned char Entrance_count;
       unsigned char Gap_count;

}Str_Cross;    //圆环结构体


//枚举
typedef enum
{
       Cro_else ,
       Cro_LeftLean,     //十字左倾标志位
       Cro_RightLean,    //十字右倾标志位
       Cro_ZHENG        //正十字志位
}Enum_CrossType;

typedef enum
{
       Cross_Not_Found,
       Find_Cross,
       In_Cross,
       Out_Cross
}Enum_CrossProcess;
/*------------------------------函数声明区------------------------------*/

extern Str_Cross   Cross;
extern Enum_CrossType  CrossType;
extern void Cross_Deal();//十字检测
extern Enum_CrossProcess  CrossProcess;






#endif


