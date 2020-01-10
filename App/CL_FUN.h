#ifndef _SEEKFREE_FUN_h
#define _SEEKFREE_FUN_h

#include "include.h"




//拨码开关端口枚举
typedef enum
{
    DSW_0,  //
    DSW_1,  //
    DSW_2,  //
    DSW_3,  //

    DSW_MAX,

} DSW_e;

//拨码开关状态枚举
typedef enum
{
    DSW_OFF  =   1,    //拨码开关关闭时对应电平
    DSW_ON   =   0,    //拨码开关打开时对应电平

} DSW_STATUS_e;


/************外部接口函数************/
void            dsw_init        (DSW_e dsw);    //初始化DSW端口
DSW_STATUS_e    dsw_get         (DSW_e dsw);    //检测DSW状态


int   myabs(int dat);
void  my_delay(long t);
float limit(float x, uint16 y);
int16 limit_ab(int16 x, int16 a, int16 b);

extern int32 int32_range_protect( int32  duty, int32  min, int32  max ) ;
extern int16 int16_range_protect( int16  duty, int16  min, int16  max );
extern   float float_range_protect( float duty, float min, float max );
extern  float Float_ABS(float x) ;





#endif
