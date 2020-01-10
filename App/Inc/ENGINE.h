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
//#define Ki  0.01     //小鱼8
//#define Kd  1


//建立电机数据结构体
typedef struct
{
       int   ActualSpeed;      //定义实际值
       int   SetSpeed;
       int   MotorPwmAdd;//电机增量
       int   MotorPwmOut;//电机输出pwm值
       int   SpeedSet;//初始速度
       int   SpeedError;
       int   SpeedLastError;
       int   SpeedPerError;
       int   BlackEndM;//速度给定的前瞻
}Str_Motor;

extern Str_Motor  Motor;

/*定义结构体和公用体*/

typedef struct

{
  float setpoint;       //设定值

  float P;     //比例系数

  float I;      //积分系数

  float D;    //微分系数

  float P_out;     //比例输出

  float I_out;      //积分输出

  float D_out;    //微分输出

  float deviation[3];

  float Uk;

  float lasterror;     //前一拍偏差

  float preerror;     //前两拍偏差

  float deadband;     //死区

  float result;      //输出值

  float deltadiff;              /*微分增量*/

  int integral;          /*积分累计量*/

  float gama;                   /*微分先行滤波系数*/



  float lastPv;                 /*上一拍的过程测量值*/

  float lastDeltaPv;            /*上一拍的过程测量值增量*/

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
//extern float ActualSpeed;      //定义实际值
//extern float integral_zuo;
//extern float integral_you;
//extern uint8 duzhuan_c;
//
//
//extern int16 SetSpeed;	//定义设定值
//extern int16 Corner_speed;
//extern int16 Corner_speed1;
//extern uint8 speed_gear;         //速度档位
//
//
//
extern void Speed_Control(int Speed_set);
extern void Engine_init(void);
extern void  Engine();
extern void Speed_Differential_calculation(int temSpeed);


#endif