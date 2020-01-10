#include "include.h"
#include "PIT.h"




int  encoder_left_speed=0,encoder_right_speed=0;
Str_Encoder Encoder={0};

void Pulse_acquire_init();
void PIT0_IRQHandler();

/*******************************************************************************
                                 ��������ʼ��
*******************************************************************************/
void encoder_PIT_INIT(void)
{
  Pulse_acquire_init();  //��������ͨ����ʼ��
  pit_init_ms(PIT0,5);   //5ms�ɼ�һ���ж�
  set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);    //�����жϸ�λ����Ϊ PIT0_VECTORn, ��commom.h��misc.h����
  enable_irq(PIT0_IRQn);   //ʹ��pit0�ж�
}

/*******************************************************************************
                              ������FTM��ʼ��
*******************************************************************************/
void Pulse_acquire_init(void)//�ڲ���ʼ������
{
  ftm_quad_init(FTM1);
  ftm_quad_init(FTM2);
}


/*******************************************************************************
                              �������ٶȲɼ�
*******************************************************************************/
void Pulse_acquire(void)    //�жϺ����е���
{
  encoder_left_speed = ftm_quad_get(FTM2);
  encoder_right_speed=-ftm_quad_get(FTM1);//���ַ���ת
  ftm_quad_clean(FTM1);
  ftm_quad_clean(FTM2);

  Encoder.Pulse_Num=(encoder_left_speed +encoder_right_speed)/4;//ȡ�Ĵ�����ֵ
  Motor.ActualSpeed=(encoder_left_speed +encoder_right_speed)/2;
#if 0
       Encoder.PulseAccumulate+=Encoder.Pulse_Num;// �������������

       Encoder.trip_distance_mm=Encoder.PulseAccumulate/5;  //���ֺ���β�õ�������10msһ����õ�����

       if(Encoder.trip_distance_mm>1000)
       {
	      Encoder.PulseAccumulate=Encoder.PulseAccumulate-5000;
	      Encoder.trip_distance_m++;

       }
       LCD_Print16(70,5,trip_distance_mm);
       LCD_Print16(30,5,trip_distance_m);
#endif
       if(Encoder.trip_distance_flag==1&&Encoder.trip_distance_start==0)
       {
	      Encoder.trip_distance_start=1;
       }
       if(Encoder.trip_distance_start==1)
       {
	      Encoder.trip_distance_get+=Encoder.Pulse_Num;
       }
       if (Encoder.trip_distance_flag==0&&Encoder.trip_distance_start==1)
       {
	    Encoder.trip_distance_start=0;
       }
}

/*******************************************************************************
                             pit0�ٶȲɼ��жϺ���
*******************************************************************************/
//float spppp=50 ;


int NS_Speed=60;

uint8 speed_start_flag=0;
uint8 speed_start_num=0;
void PIT0_IRQHandler()   //pit0�жϺ���
{
   PIT_Flag_Clear(PIT0); //���жϱ�־λ

   Pulse_acquire();

   if(speed_start_flag==0)
   {
	speed_start_num++;
   }
   else
   {
	  Engine();//  ����ܿ���
   }
   if(speed_start_num>200)
   {
	speed_start_flag=1;
	speed_start_num=0;
   }


   //Speed_Differential_calculation(NS_Speed);


   //SERVO_DIANJI_closed_loop(50,50);
   //  speed_set_zuo(30);

 // S3010_control();
   //steer_control(servPWMDuty);
}


 /***************************************************************
������������·��

* �������ƣ�distance_judge(int e_distance)
* ����˵����������������·��
* ����˵����e_distanceҪ�ߵľ���
* �������أ�char
* �޸�ʱ�䣺
* �� ע��
***************************************************************/
char distance_judge(int e_distance)
{
       if(e_distance>0&&Encoder.trip_distance_flag==0)
       {
	      Encoder.trip_distance_flag=1;
       }
       if(Encoder.trip_distance_get>e_distance)
       { Encoder.trip_distance_flag=0;
//	      LCD_Print16(50,5,Encoder.trip_distance_flag);
//	      LCD_Print16(50,6,Encoder.trip_distance_get);

	      Encoder.trip_distance_get=0;

	      return 1 ;
       }
       else
       {
//	      LCD_Print16(50,5,Encoder.trip_distance_flag);
//	      LCD_Print16(50,6,Encoder.trip_distance_get);
	      return 0 ;
       }
}

/*����������������������������������*���ϱ�����*������������������������������������*/




/*������������������������������*����pit��ʱ�ж�*����������������������������������*/
#if 0
void PIT1_IRQHandler()
{
  PIT_Flag_Clear(PIT1);
}

void PIT3_IRQHandler()
{
  PIT_Flag_Clear(PIT3);
}

int le_time=0;
void PIT2_IRQHandler()
{
  PIT_Flag_Clear(PIT2);
}
#endif