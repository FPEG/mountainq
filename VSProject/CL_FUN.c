/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,
 * All rights reserve
 * @date       		2017-11-6
 ********************************************************************************************************************/
#include "CL_FUN.h"


float Float_ABS(float x)
{
       if(x<0)
	      return -x;
       else
	      return x;
}
//-------------------------------------------------------------------------//
/******** 限幅保护 *********/
float float_range_protect( float duty, float min, float max ) //限幅保护
{
    if( duty > max )
    {
        return max;
    }
    else if( duty <= min )
    {
        return min;
    }
    else
    {
        return duty;
    }
}

/******** 限幅保护 *********/
int32 int32_range_protect( int32  duty, int32  min, int32  max ) //限幅保护
{
    if( duty > max )
    {
        return max;
    }
    else if( duty <= min )
    {
        return min;
    }
    else
    {
        return duty;
    }
}


/******** 限幅保护 *********/
int16 int16_range_protect( int16  duty, int16  min, int16  max ) //限幅保护
{
    if( duty > max )
    {
        return max;
    }
    else if( duty <= min )
    {
        return min;
    }
    else
    {
        return duty;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      绝对值函数
//  @param      dat				需要求绝对值的数
//  @return     int				返回绝对值
//  @since      v1.0
//  Sample usage:				dat = myabs(dat);//将dat变成正数
//-------------------------------------------------------------------------------------------------------------------
int  myabs(int dat)
{
    if(dat>=0)  return dat;
    else        return -dat;
}

int sswr1(float fdx)//四舍五入
{
       float T_fdx;
       int res;
       if(fdx>=0)
       {
	      T_fdx=fdx-(int)(fdx);
	      if((T_fdx-0.5)>=0)
	      {res=(int)(fdx)+1;}
	      else
		     res=(int)(fdx);
       }
       else
       {
	      fdx=-fdx;
	      T_fdx=fdx-(int)(fdx);
	      if((T_fdx-0.5)>=0)
	      {res=(int)(fdx)+1;}
	      else
		     res=(int)(fdx);
	      res=-res;
       }
       return res;
}


//拨码开关
//定义 DSW 编号对应的管脚
PTXn_e DSW_PTxn[DSW_MAX] = {PTB2, PTB1, PTB0, PTA29};

void dsw_init(DSW_e dsw)
{
    if(dsw < DSW_MAX)   //初始化 对应端口
    {
        gpio_init(DSW_PTxn[dsw], GPI, 0);
        port_init_NoALT(DSW_PTxn[dsw], PULLUP);     //保持复用不变，仅仅改变配置选项
    }

    else                //初始化 全部端口
    {
        dsw = DSW_MAX;

        while(dsw--)
        {
            gpio_init(DSW_PTxn[dsw], GPI, 0);
            port_init_NoALT(DSW_PTxn[dsw], PULLUP); //保持复用不变，仅仅改变配置选项
        }
    }
}

DSW_STATUS_e dsw_get(DSW_e dsw)
{
    if(gpio_get(DSW_PTxn[dsw]) == DSW_ON)
    {
        return DSW_ON;
    }
    return DSW_OFF;
}

