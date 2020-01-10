#ifndef _MT9V032_h
#define _MT9V032_h

#include "include.h"
extern   uint8 ffffff;

#define Black 0
#define White 1


#define COL     160//图像宽度   范围1-752     K60采集不允许超过188
#define ROW     120//图像高度	范围1-480

#define COL_START  1
#define COL_END    79
#define ROW_START  1
#define ROW_END    39
//摄像头命令枚举
typedef enum
{
    INIT = 0,               //摄像头初始化命令
    AUTO_EXP,               //自动曝光命令
    EXP_TIME,               //曝光时间命令
    FPS,                    //摄像头帧率命令
    SET_COL,                //图像列命令
    SET_ROW,                //图像行命令
    LR_OFFSET,              //图像左右偏移命令
    UD_OFFSET,              //图像上下偏移命令
    GAIN,                   //图像偏移命令
    CONFIG_FINISH,          //非命令位，主要用来占位计数

    SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
    GET_STATUS,             //获取摄像头配置命令
    GET_VERSION,            //固件版本号命令

    SET_ADDR = 0XFE,        //寄存器地址命令
    SET_DATA                //寄存器数据命令
}CMD;



extern uint8 mt9v032_finish_flag;  //一场图像采集完成标志位
extern uint8 image[ROW/2][COL/2];      //内部使用
extern uint8 image_buff[ROW][COL];      //内部使用


//引脚配置

#define MT9V032_COF_UART    UART5  //配置摄像头所使用到的串口


//--------------------------------------------------------------------------------------------------
//        如果修改了场中断引脚那么注意修改PORTC_IRQn、PORTC_ISFR。
//        比如使用的是PTA这组的引脚那么将这两个修改为PORTA_IRQn、PORTA_ISFR即可
//        且务必在对应的中断函数中调用VSYNC函数
//--------------------------------------------------------------------------------------------------
#define MT9V032_PCLK                PTC2                            	//摄像头像素时钟

#define MT9V032_VSYNC               PTC7                            	//摄像头场同步信号
#define MT9V032_VSYNC_CHANNEL       7                               //MT9V032_VSYNC对应引脚的引脚号 PTC6   -->     6
#define MT9V032_INTERRUPT_NUNBERS   PORTC_IRQn                      //中断编号
#define MT9V032_INTERRUPT_ISFR      PORTC_ISFR                     //中断标志位



//--------------------------------------------------------------------------------------------------
//        摄像头数据接口
//        C_IN_DATA(1)
//        第一个字母C代表着使用C口，可以选择A、B、C、D、E.分别对应A_IN_DATA，B_IN_DATA，C_IN_DATA，D_IN_DATA，E_IN_DATA
//        第二个括号里面的参数1代表着使用的是C8 - C15口,
//		  可以选择0、1、2、3，分别对应的是	该端口的X0-X7，X8-X15，X16-X23，X24-X31(这里x代表的是第一个字母，比如现在的就是C8-C15)
//--------------------------------------------------------------------------------------------------

#define MT9V032_DATAPORT 		PTC_B1_IN//B_IN_DATA(0)	                //DMA数据口

void   mt9v032_cof_uart_interrupt(void);
void   VSYNC(void);
void   camera_init(void);
void   seekfree_sendimg_032(unsigned char row,unsigned char col);
void   row_finished(void);
extern void ZZF_Init();
extern void ZZF_Half();


#endif

