#include "include.h"


/********************************************
------------------------------------
软件版本： IAR 7.2 or MDK 5.17
目标核心： MK60DN512VLL10
============================================
MT9V032接线定义：
------------------------------------
模块管脚            单片机管脚
SDA(51的RX)         E8
SCL(51的TX)         E9
场中断              C7
行中断				未使用，因此不接
像素中断            C2
数据口              C8-C15


============================================

分辨率是                160*120
摄像头参数设置可以到    SEEKFREE-->h_file-->SEEKFREE_MT9V032.h


*********************************************/

/*!
*  @brief      main函数
*  @since      v5.0
*  @note       山外摄像头 LCD 测试实验
*/
void  main(void)
{
	//gpio_init(PTA4, GPO, 0);
	//ftm_pwm_init(FTM2, FTM_CH0, 50, 78);//C
	SteerInit();
	//ftm_pwm_init(FTM0, FTM_CH4, 10000, 800);//D4右轮倒车
	//ftm_pwm_init(FTM0, FTM_CH6, 10000, 800);//D6左轮倒车
	ftm_pwm_init(FTM0, FTM_CH5, 10000, 800);//D5右轮前进
	ftm_pwm_init(FTM0, FTM_CH7, 10000, 800);//D7左轮前进
	ZZF_Init();
	LCD_Init();        //OLED 屏幕初始化
	enable_irq(PORTA_IRQn);//开图像采集中断；
	while (1)
	{
		if (mt9v032_finish_flag)
		{
			//image_buff[60][90]=0;
			//LCD_show_ZZF_image();
			adapt_otsuThreshold(*image_buff, 120, 160, &Threshold);
			ZZF_Half();
			binarization_processing(*image, 60, 80);

			GetBlackEndParam();//获取黑线截止行 
			SearchCenterBlackline();
			LCD_show_ZZF_image_t(Threshold);
			//if (image_buff[60][80] > 60)
			//{
			//	ftm_pwm_duty(FTM2, FTM_CH0, 70);//右转
			//}
			//else
			//{
			//	ftm_pwm_duty(FTM2, FTM_CH0, 85);//左转
			//}
			SteerControl();
			mt9v032_finish_flag = 0;
			enable_irq(PORTC_IRQn);
		}
	}
}



