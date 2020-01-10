#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include  "common.h"

/*
 * Include �û��Զ����ͷ�ļ�
 */
#include  "MK60_wdog.h"
#include  "MK60_gpio.h"     //IO�ڲ���
#include  "MK60_uart.h"     //����
#include  "MK60_SysTick.h"
#include  "MK60_lptmr.h"    //�͹��Ķ�ʱ��(��ʱ)
#include  "MK60_i2c.h"      //I2C
#include  "MK60_spi.h"      //SPI
#include  "MK60_ftm.h"      //FTM
#include  "MK60_pit.h"      //PIT
#include  "MK60_rtc.h"      //RTC
#include  "MK60_adc.h"      //ADC
#include  "MK60_dac.h"      //DAC
#include  "MK60_dma.h"      //DMA
#include  "MK60_FLASH.h"    //FLASH
#include  "MK60_can.h"      //CAN
#include  "MK60_sdhc.h"     //SDHC
#include  "MK60_usb.h"      //usb


#include  "VCAN_KEY.H"          //KEY
#include  "VCAN_MMA7455.h"      //������ٶ�MMA7455
#include  "VCAN_key_event.h"    //������Ϣ����
#include  "VCAN_UI_VAR.h"
#include  "VCAN_LED.H"
#include  "TFmini_Plus.h"




#include "initialization.h"


#include "LCD12864.h"
#include "LCD12864lib.h"
#include "UART.h"
#include "PIT.h"



#include  "BreakRoad.h"
#include  "SEEKFREE_18TFT.h"
#include  "font.h"
#include  "Display.h"

#include  "function library.h"
#include  "CL_FUN.h"
//����ͷ�ɼ�����
#include "MT9V032.h"
#include "IMAGE_DEAL.h"

#include "Midline_Process.h"
#include "GRAY_CALCULATE.h"

//#include "Extract_Edge.h"      //�߽���ȡ
//#include "Feature_Extration.h"  //������ȡ
#include "steer_adv.h"
#include "findtrack.h"
#include "Round_Deal.h"
#include "Cross_Deal.h"
#include "ROAD_CHECK.h"
#include "INV.h"

//ELEC�ɼ�
#include "ELEC.h"
//����
#include "ENGINE.h"
#include "steer.h"

#include "Test.h"


  //������
 #include "MPU6050.h"
 #include "My_I2C.h"
 #include "inv_mpu.h"

#endif  //__INCLUDE_H__
