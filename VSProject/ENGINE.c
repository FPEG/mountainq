/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,Ұ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�Ұ���ѧ��̳ http://www.chuxue123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����Ұ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��Ұ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      Ұ��K60 ƽ̨������
 * @author     Ұ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */
#include "ENGINE.h"
#include "include.h"


Str_Motor  Motor;

void Speed_Differential_calculation(int temSpeed);
void Speed_Left(float speed_set);
void Speed_Right(float speed_set);
void Open_Loop(int t1,int t2);
void Closed_Loop(float L,float R);





/****************************************
                 ��ʼ��
****************************************/
void Engine_init(void)
{
  ftm_pwm_init(MOTOR_FTM, Motor_CH1,MOTOR_HZ,0);
  ftm_pwm_init(MOTOR_FTM, Motor_CH2,MOTOR_HZ,0);
  ftm_pwm_init(MOTOR_FTM, Motor_CH3,MOTOR_HZ,0);
  ftm_pwm_init(MOTOR_FTM, Motor_CH4,MOTOR_HZ,0);
}



float spppp=50.0 ;
/****************************************
               ����ܿ���
****************************************/
void  Engine()
{
       if(stop_car_flag )
       {

	      if(spppp<20)
	      {
		     spppp=0;
	      }
	      else
	      {
		     spppp=spppp-0.3;
	      }
#if 1

	      Closed_Loop(spppp,spppp) ;

#endif
	   // Open_Loop(0,0);
       }
       else
       {
	      Speed_Differential_calculation(Motor.SetSpeed);
       }

}




/****************************************
                 ǰ��
****************************************/
void Engine_Left(int t1)//�ڲ�����
{
       t1=int16_range_protect(t1,-990,990) ;
       if(t1>=0)     //����
       {
	      ftm_pwm_duty(MOTOR_FTM, Motor_CH1, t1);
	      ftm_pwm_duty(MOTOR_FTM, Motor_CH2, 0);
       }
       else
       {
	      ftm_pwm_duty(MOTOR_FTM, Motor_CH1, 0);
	      ftm_pwm_duty(MOTOR_FTM, Motor_CH2,-t1);
       }
}


void Engine_Right(int t2)//�ڲ�����
{
       t2=int16_range_protect(t2,-990,990) ;
       if(t2>=0)
       {
	      ftm_pwm_duty(MOTOR_FTM, Motor_CH3, t2);
	      ftm_pwm_duty(MOTOR_FTM, Motor_CH4, 0);
       }
       else
       {
	      ftm_pwm_duty(MOTOR_FTM, Motor_CH3, 0);
	      ftm_pwm_duty(MOTOR_FTM, Motor_CH4, -t2);
       }
}
/****************************************
                 ����
****************************************/
void Open_Loop(int t1,int t2)//�ڲ�����
{
       Engine_Left(t1);
       Engine_Right(t2);
}

/****************************************
                 �ջ�
****************************************/
void Closed_Loop(float L,float R)
{
       Speed_Left (L);
       Speed_Right(R);
}



PID  L_Speed_pid={
       0,

      6.0,0.0003, 2.1    //P,I,D


};

PID  R_Speed_pid={
       0,

      6.0,0.0003, 2.1    //P,I,D
};





/****************************************
               ���ٱջ�
****************************************/
void Speed_Differential_calculation(int temSpeed)//ֻ���ڹ���ʱ�ô˲���
{
       float  C_left_speed=0.0,C_right_speed=0.0;//���Ҳ��ٺ��ٶ�
       float  Differential_coefficient=0;  //����ϵ��
       double serve_out=servPWMDuty-SteerCenture;



       if(serve_out>120||serve_out<-120)
	      Differential_coefficient=0.245;
       else if(serve_out<40&&serve_out>-40)
	      Differential_coefficient=0;
       else
	      Differential_coefficient=0.0021*serve_out;



       Differential_coefficient=Float_ABS(Differential_coefficient); //�������ϵ��


       if(serve_out>= 0)
       {
	      C_left_speed  =1.0* temSpeed  *  (1.0-Differential_coefficient);    //�����ٶ�<1
	      C_right_speed =1.0* temSpeed  *  (1.0+Differential_coefficient);    //�����ٶ�>1
       }
       else if(serve_out< 0)
       {
	      C_left_speed  =1.0* temSpeed  *  (1.0+Differential_coefficient);       /////////
	      C_right_speed =1.0* temSpeed  *  (1.0-Differential_coefficient);       //////////
       }
       Closed_Loop(C_left_speed,C_right_speed);
}


/******************�����ٶ�PID����*************************/
void Speed_Left1(float speed_set)
{
       L_Speed_pid.deviation[2]=L_Speed_pid.deviation[1];
       L_Speed_pid.deviation[1]=L_Speed_pid.deviation[0];
       L_Speed_pid.deviation[0]=1.0*(speed_set-encoder_left_speed);

       L_Speed_pid.P_out=L_Speed_pid.P*(L_Speed_pid.deviation[0]-R_Speed_pid.deviation[1]);
       L_Speed_pid.I_out=L_Speed_pid.I*L_Speed_pid.deviation[0];
       L_Speed_pid.D_out=L_Speed_pid.D*(L_Speed_pid.deviation[0]-2*L_Speed_pid.deviation[1]+L_Speed_pid.deviation[2]);

       L_Speed_pid.Uk+=L_Speed_pid.P_out+ L_Speed_pid.I_out+ L_Speed_pid.D_out;

       L_Speed_pid.Uk=float_range_protect(L_Speed_pid.Uk,-1000,1000) ;
       Engine_Left((int)L_Speed_pid.Uk);
}



/******************�����ٶ�PID����*************************/
void Speed_Right1(float speed_set)
{
       R_Speed_pid.deviation[2]=R_Speed_pid.deviation[1];
       R_Speed_pid.deviation[1]=R_Speed_pid.deviation[0];
       R_Speed_pid.deviation[0]=1.0*(speed_set-encoder_right_speed);

       R_Speed_pid.P_out=R_Speed_pid.P*(R_Speed_pid.deviation[0]-R_Speed_pid.deviation[1]);
       R_Speed_pid.I_out=R_Speed_pid.I*R_Speed_pid.deviation[0];
       R_Speed_pid.D_out=R_Speed_pid.D*(R_Speed_pid.deviation[0]-2*R_Speed_pid.deviation[1]+R_Speed_pid.deviation[2]);

       R_Speed_pid.Uk+=R_Speed_pid.P_out+ R_Speed_pid.I_out+ R_Speed_pid.D_out;

       R_Speed_pid.Uk=float_range_protect(R_Speed_pid.Uk,-1000,1000) ;
       Engine_Right((int)R_Speed_pid.Uk);
}

/******************�����ٶ�PID����*************************/
void Speed_Left(float speed_set)
{
       L_Speed_pid.deviation[2]=L_Speed_pid.deviation[1];
       L_Speed_pid.deviation[1]=L_Speed_pid.deviation[0];
       L_Speed_pid.deviation[0]=1.0*(speed_set-encoder_left_speed);

       L_Speed_pid.P_out=L_Speed_pid.P*L_Speed_pid.deviation[0];

       L_Speed_pid.integral+=L_Speed_pid.deviation[0];
       if(L_Speed_pid.integral>5000)
	      L_Speed_pid.integral=5000;
       if(L_Speed_pid.integral<-5000)
	      L_Speed_pid.integral=-5000;


       L_Speed_pid.I_out=L_Speed_pid.I*L_Speed_pid.integral;
       L_Speed_pid.D_out=L_Speed_pid.D*(L_Speed_pid.deviation[0]-L_Speed_pid.deviation[1]);

       L_Speed_pid.Uk+=L_Speed_pid.P_out+ L_Speed_pid.I_out+ L_Speed_pid.D_out;

       L_Speed_pid.Uk=float_range_protect(L_Speed_pid.Uk,-1000,1000) ;
       Engine_Left((int)L_Speed_pid.Uk);
}



/******************�����ٶ�PID����*************************/
void Speed_Right(float speed_set)
{
       R_Speed_pid.deviation[2]=R_Speed_pid.deviation[1];
       R_Speed_pid.deviation[1]=R_Speed_pid.deviation[0];
       R_Speed_pid.deviation[0]=1.0*(speed_set-encoder_right_speed);

       R_Speed_pid.P_out=R_Speed_pid.P*(R_Speed_pid.deviation[0]);
       R_Speed_pid.integral+=R_Speed_pid.deviation[0];
       if(R_Speed_pid.integral>5000)
	      R_Speed_pid.integral=5000;
       if(R_Speed_pid.integral<-5000)
	      R_Speed_pid.integral=-5000;

       R_Speed_pid.I_out=R_Speed_pid.I*R_Speed_pid.integral;
       R_Speed_pid.D_out=R_Speed_pid.D*(R_Speed_pid.deviation[0]-R_Speed_pid.deviation[1]);

       R_Speed_pid.Uk+=R_Speed_pid.P_out+ R_Speed_pid.I_out+ R_Speed_pid.D_out;

       R_Speed_pid.Uk=float_range_protect(R_Speed_pid.Uk,-1000,1000) ;
       Engine_Right((int)R_Speed_pid.Uk);
}

/****************************************
                 �ջ�����
****************************************/

/*******************************************************************/

PID  Speed_pid={
       0,

       0.2,   0.2,  0.2,    //P,I,D

       0,0,0,0,0,0
};

void Speed_pid_calculation1(float speed_set);
void Speed_Differential_calculation1(float temSpeed);



/******************�ٶ�PID����*************************/
void Speed_pid_calculation1(float speed_set)
{
       Speed_pid.deviation[2]=Speed_pid.deviation[1];
       Speed_pid.deviation[1]=Speed_pid.deviation[0];
       Speed_pid.deviation[0]=1.0*(speed_set-Motor.ActualSpeed);

//      Speed_pid.integral+=Speed_pid.deviation[0];
//     if(Speed_pid.integral>2000)
//Speed_pid.integral=2000;

       Speed_pid.P_out=Speed_pid.P*(Speed_pid.deviation[0]-Speed_pid.deviation[1]);
       Speed_pid.I_out=Speed_pid.I*Speed_pid.deviation[0];
       Speed_pid.D_out=Speed_pid.D*(Speed_pid.deviation[0]-2*Speed_pid.deviation[1]+Speed_pid.deviation[2]);

       Speed_pid.Uk+=Speed_pid.P_out+ Speed_pid.I_out+ Speed_pid.D_out;
}


void Speed_Differential_calculation1(float temSpeed)//
{
       int16  C_left_speed=0,C_right_speed=0;//���Ҳ��ٺ��ٶ�
       float  Differential_coefficient=0;  //����ϵ��
       double serve_out=servPWMDuty-SteerCenture;

       if(serve_out>100||serve_out<-100)
	      Differential_coefficient=0.00110*serve_out;
       else if(serve_out<30&&serve_out>-30)
	      Differential_coefficient=0;
       else
	      Differential_coefficient=0.0008*serve_out;


       Differential_coefficient=Float_ABS(Differential_coefficient); //�������ϵ��

       if(serve_out>= 0)
       {
	      C_left_speed  = (int32)(temSpeed  *  (1-Differential_coefficient));    //�����ٶ�<1
	      C_right_speed = (int32)(temSpeed  *  (1+Differential_coefficient));    //�����ٶ�>1
       }
       else if(serve_out< 0)
       {
	      C_left_speed  = (int32)(temSpeed  *  (1+Differential_coefficient));       /////////
	      C_right_speed  = (int32)(temSpeed *  (1-Differential_coefficient));       //////////
       }
       Open_Loop(C_left_speed,C_right_speed);
}



/****************************************

****************************************/
void Speed_Control(int Speed_set)
{
       if(0)
       {
	      Speed_Differential_calculation(Speed_set);
       }
       else
       {
	      Speed_pid_calculation1(Speed_set);
	      Speed_Differential_calculation1(Speed_pid.Uk);
       }
}



#if 0
/**************************************************************
�ж�ƫ��ĵȼ�,У׼ƫ��
**************************************************************/
float S_deviation_grade_judgment(int deviation)
{
    int num=1;
    float A;
    num = ABS(deviation/10);
    switch(num)
    {
    case 0:
      A=0;
      break;
    case 1:
      A=0;
      break;
    case 2:
      A=0;
      break;
    case 3:
      A=0;
      break;
    case 4:
      A=0.0001;
      break;
    case 5:
      A=0.0001;
      break;
    case 6:
      A=0.0001;
      break;
    case 7:
      A=0.0027;//0.003
      break;
    default:
      A=0;
    }
    return A;
}


float speed_k=0;//�����speed_k<0.2469
double serve_out=0;
void Differential_calculation(int temSpeed)//ֻ���ڹ���ʱ�ô˲���
{
  float j=0.00085;
  serve_out=servPWMDuty-SteerCenture;
  if(serve_out>100||serve_out<-100)
    speed_k=0.00110*serve_out;
  else if(serve_out<30&&serve_out>-30)
    speed_k=0;
  else
    speed_k=j*serve_out;
      speed_k=Float_ABS(speed_k);
  if(serve_out>= 0)
  {
    C_left_speed  = (int32)(temSpeed  *  (1-speed_k));    //�����ٶ�<1
    C_right_speed = (int32)(temSpeed  *  (1+speed_k));    //�����ٶ�>1
  }
  else if(serve_out< 0)
  {
    C_left_speed  = (int32)(temSpeed  *  (1+speed_k));       /////////
    C_right_speed  = (int32)(temSpeed *  (1-speed_k));       //////////
  }
  //SEND(speed_k,0,serve_out,0);
}
#endif