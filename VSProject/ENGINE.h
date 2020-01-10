/*!
 *     COPYRIGHT NOTICE
 *
 *     All rights reserved.
 *
 */
#ifndef _ENGINE_H_
#define _ENGINE_H_



#define MOTOR_HZ    10000//(10*1000)
#define MOTOR_FTM   FTM0
#define Motor_CH1   FTM_CH6
#define Motor_CH2   FTM_CH5
#define Motor_CH3   FTM_CH4
#define Motor_CH4   FTM_CH3




//#define Kp  3.5
//#define Ki  0.01     //С��8
//#define Kd  1


//����������ݽṹ��
typedef struct
{
       int   ActualSpeed;      //����ʵ��ֵ
       int   SetSpeed;
       int   MotorPwmAdd;//�������
       int   MotorPwmOut;//������pwmֵ
       int   SpeedSet;//��ʼ�ٶ�
       int   SpeedError;
       int   SpeedLastError;
       int   SpeedPerError;
       int   BlackEndM;//�ٶȸ�����ǰհ
}Str_Motor;

extern Str_Motor  Motor;

/*����ṹ��͹�����*/

typedef struct

{
  float setpoint;       //�趨ֵ

  float P;     //����ϵ��

  float I;      //����ϵ��

  float D;    //΢��ϵ��

  float P_out;     //�������

  float I_out;      //�������

  float D_out;    //΢�����

  float deviation[3];

  float Uk;

  float lasterror;     //ǰһ��ƫ��

  float preerror;     //ǰ����ƫ��

  float deadband;     //����

  float result;      //���ֵ

  float deltadiff;              /*΢������*/

  int integral;          /*�����ۼ���*/

  float gama;                   /*΢�������˲�ϵ��*/



  float lastPv;                 /*��һ�ĵĹ��̲���ֵ*/

  float lastDeltaPv;            /*��һ�ĵĹ��̲���ֵ����*/

}PID;

//extern uint8 count_daozhuan;
//extern uint8 speed_check_flag1;
//extern uint8 paodao ;
//
//
//#define hang  46
//extern uint8 engine;
//extern uint8 engine_key;
//extern int32 integral;
//extern int init_speed_left;
//extern int init_speed_right;
//extern float ActualSpeed;      //����ʵ��ֵ
//extern float integral_zuo;
//extern float integral_you;
//extern uint8 duzhuan_c;
//
//
//extern int16 SetSpeed;	//�����趨ֵ
//extern int16 Corner_speed;
//extern int16 Corner_speed1;
//extern uint8 speed_gear;         //�ٶȵ�λ
//
//
//
extern void Speed_Control(int Speed_set);
extern void Engine_init(void);
extern void  Engine();
extern void Speed_Differential_calculation(int temSpeed);


#endif