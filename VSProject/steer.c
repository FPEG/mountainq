/*!
 *@file       VCAN_OV7725_Eagle.c
 * @brief      ӥ��ov7725��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-07
 */
#include "include.h"
#include "steer.h"

float centerErr=0;
float centerErr_history[2];
float servPWMDuty;                     //���PWMռ�ձ�


float servPram =        3.7;    //���ת�Ǳ���ϵ��������������������Ƕȸ����������ֵ��������ǶȰڷ��ܴ��С��ֵ
float servD    =        2;




/*******************************************************************************
                         s3010�����ʼ��
*******************************************************************************/
void S3010_init()
{
  //ftm_pwm_init(SteerFTM,SteerFTM_CH,SteerFTM_Hz,SteerCenture) ; //��ʼ�� FTM3_CH0ΪƵ��50Hz��PWM��ռ�ձ�Ϊ7.2%/FTM0_PRECISON
}


/*******************************************************************************
                          s3010�������
*******************************************************************************/
void s_control(uint32 tt)//������ t�ǽǶ�
{
  //ftm_pwm_duty(SteerFTM,SteerFTM_CH,tt) ;
}


/*******************************************************************************
                            ƫ��������
*******************************************************************************/
void Offset_processing()//ƫ��������
{
//       switch(RoadType)//Break_Road)
//       {
//	    case else_road:
//	      centerErr=Camera_CenterError;break;
//	    case Break_RoadType:  //��·
//	      centerErr=ELEC_err/2.2;      break;
//	    case Barrier_Road:    //·��
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



