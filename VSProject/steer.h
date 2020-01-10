 /* @file       MK60_it.h
 * @brief      山外K60 平台中断服务重定向头文件
 * @author     山外科技
 * @version    v5.0
 * @date       2013-06-26
 */

#ifndef _STEER_H_
#define _STEER_H_


#define SteerFTM      FTM3
#define SteerFTM_CH   FTM_CH1
#define SteerFTM_Hz   50

#define SteerCenture    1455     //舵机中值  越大，方向越向左
#define Steer_section   150
#define SteerLeft_M     (SteerCenture+Steer_section )     //舵机左极限，很关键，限幅防止舵机打死
#define SteerRight_M    (SteerCenture-Steer_section )    //舵机右极限，很关键，限幅防止舵机打


extern float servPWMDuty;
extern float centerErr;
extern float servPram;    //舵机转角比例系数，调整方法：若舵机角度跟随慢增大该值，若舵机角度摆幅很大减小该值
extern float servD  ;



extern void S3010_control();
extern void S3010_init();

#endif