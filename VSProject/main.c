#include "include.h"


/********************************************
------------------------------------
����汾�� IAR 7.2 or MDK 5.17
Ŀ����ģ� MK60DN512VLL10
============================================
MT9V032���߶��壺
------------------------------------
ģ��ܽ�            ��Ƭ���ܽ�
SDA(51��RX)         E8
SCL(51��TX)         E9
���ж�              C7
���ж�				δʹ�ã���˲���
�����ж�            C2
���ݿ�              C8-C15


============================================

�ֱ�����                160*120
����ͷ�������ÿ��Ե�    SEEKFREE-->h_file-->SEEKFREE_MT9V032.h


*********************************************/

/*!
*  @brief      main����
*  @since      v5.0
*  @note       ɽ������ͷ LCD ����ʵ��
*/
void  main(void)
{
	//gpio_init(PTA4, GPO, 0);
	//ftm_pwm_init(FTM2, FTM_CH0, 50, 78);//C
	SteerInit();
	//ftm_pwm_init(FTM0, FTM_CH4, 10000, 800);//D4���ֵ���
	//ftm_pwm_init(FTM0, FTM_CH6, 10000, 800);//D6���ֵ���
	ftm_pwm_init(FTM0, FTM_CH5, 10000, 800);//D5����ǰ��
	ftm_pwm_init(FTM0, FTM_CH7, 10000, 800);//D7����ǰ��
	ZZF_Init();
	LCD_Init();        //OLED ��Ļ��ʼ��
	enable_irq(PORTA_IRQn);//��ͼ��ɼ��жϣ�
	while (1)
	{
		if (mt9v032_finish_flag)
		{
			//image_buff[60][90]=0;
			//LCD_show_ZZF_image();
			adapt_otsuThreshold(*image_buff, 120, 160, &Threshold);
			ZZF_Half();
			binarization_processing(*image, 60, 80);

			GetBlackEndParam();//��ȡ���߽�ֹ�� 
			SearchCenterBlackline();
			LCD_show_ZZF_image_t(Threshold);
			//if (image_buff[60][80] > 60)
			//{
			//	ftm_pwm_duty(FTM2, FTM_CH0, 70);//��ת
			//}
			//else
			//{
			//	ftm_pwm_duty(FTM2, FTM_CH0, 85);//��ת
			//}
			SteerControl();
			mt9v032_finish_flag = 0;
			enable_irq(PORTC_IRQn);
		}
	}
}



