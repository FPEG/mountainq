/*!
 *@file       VCAN_OV7725_Eagle.c
 * @brief      鹰眼ov7725驱动代码
 * @author     山外科技
 * @version    v5.0
 * @date       2013-09-07
 */
#include "include.h"
#include "steer.h"

float centerErr=0;
float centerErr_history[2];
float servPWMDuty;                     //舵机PWM占空比


float servPram =        3.7;    //舵机转角比例系数，调整方法：若舵机角度跟随慢增大该值，若舵机角度摆幅很大减小该值
float servD    =        2;




/*******************************************************************************
                         s3010舵机初始化
*******************************************************************************/
void S3010_init()
{
  //ftm_pwm_init(SteerFTM,SteerFTM_CH,SteerFTM_Hz,SteerCenture) ; //初始化 FTM3_CH0为频率50Hz的PWM，占空比为7.2%/FTM0_PRECISON
}


/*******************************************************************************
                          s3010舵机控制
*******************************************************************************/
void s_control(uint32 tt)//舵机打角 t是角度
{
  //ftm_pwm_duty(SteerFTM,SteerFTM_CH,tt) ;
}


/*******************************************************************************
                            偏移量处理
*******************************************************************************/
void Offset_processing()//偏移量处理
{
//       switch(RoadType)//Break_Road)
//       {
//	    case else_road:
//	      centerErr=Camera_CenterError;break;
//	    case Break_RoadType:  //断路
//	      centerErr=ELEC_err/2.2;      break;
//	    case Barrier_Road:    //路障
//	      centerErr=Barrier.Bar_Err;      break;
//	    default:
//	      centerErr=Camera_CenterError;
//       }

       centerErr_history[1] = centerErr_history[0];
       centerErr_history[0] = centerErr;
       servPWMDuty=SteerCenture+servPram*centerErr_history[0]+servD*(centerErr_history[0]-centerErr_history[1]);

       if(servPWMDuty>SteerLeft_M )
	      servPWMDuty =SteerLeft_M ;
       else if(servPWMDuty<SteerRight_M )
	      servPWMDuty=SteerRight_M;
}

void S3010_control()
{
  Offset_processing();
  s_control((uint32)servPWMDuty);
}



