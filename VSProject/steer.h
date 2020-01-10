 /* @file       MK60_it.h
 * @brief      ɽ��K60 ƽ̨�жϷ����ض���ͷ�ļ�
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-06-26
 */

#ifndef _STEER_H_
#define _STEER_H_


#define SteerFTM      FTM3
#define SteerFTM_CH   FTM_CH1
#define SteerFTM_Hz   50

#define SteerCenture    1455     //�����ֵ  Խ�󣬷���Խ����
#define Steer_section   150
#define SteerLeft_M     (SteerCenture+Steer_section )     //������ޣ��ܹؼ����޷���ֹ�������
#define SteerRight_M    (SteerCenture-Steer_section )    //����Ҽ��ޣ��ܹؼ����޷���ֹ�����


extern float servPWMDuty;
extern float centerErr;
extern float servPram;    //���ת�Ǳ���ϵ��������������������Ƕȸ����������ֵ��������ǶȰڷ��ܴ��С��ֵ
extern float servD  ;



extern void S3010_control();
extern void S3010_init();

#endif