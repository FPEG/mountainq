#include "include.h"
#include "PIT.h"




int  encoder_left_speed=0,encoder_right_speed=0;
Str_Encoder Encoder={0};

void Pulse_acquire_init();
void PIT0_IRQHandler();

/*******************************************************************************
                                 编码器初始化
*******************************************************************************/
void encoder_PIT_INIT(void)
{
  Pulse_acquire_init();  //正交解码通道初始化
  pit_init_ms(PIT0,5);   //5ms采集一次中断
  set_vector_handler(PIT0_VECTORn,PIT0_IRQHandler);    //设置中断复位函数为 PIT0_VECTORn, 在commom.h和misc.h里找
  enable_irq(PIT0_IRQn);   //使能pit0中断
}

/*******************************************************************************
                              编码器FTM初始化
*******************************************************************************/
void Pulse_acquire_init(void)//内部初始化调用
{
  ftm_quad_init(FTM1);
  ftm_quad_init(FTM2);
}


/*******************************************************************************
                              编码器速度采集
*******************************************************************************/
void Pulse_acquire(void)    //中断函数中调用
{
  encoder_left_speed = ftm_quad_get(FTM2);
  encoder_right_speed=-ftm_quad_get(FTM1);//右轮反向转
  ftm_quad_clean(FTM1);
  ftm_quad_clean(FTM2);

  Encoder.Pulse_Num=(encoder_left_speed +encoder_right_speed)/4;//取寄存器的值
  Motor.ActualSpeed=(encoder_left_speed +encoder_right_speed)/2;
#if 0
       Encoder.PulseAccumulate+=Encoder.Pulse_Num;// 正交解码测脉冲

       Encoder.trip_distance_mm=Encoder.PulseAccumulate/5;  //保持好这次测得的总数，10ms一共测得的脉冲

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
                             pit0速度采集中断函数
*******************************************************************************/
//float spppp=50 ;


int NS_Speed=60;

uint8 speed_start_flag=0;
uint8 speed_start_num=0;
void PIT0_IRQHandler()   //pit0中断函数
{
   PIT_Flag_Clear(PIT0); //清中断标志位

   Pulse_acquire();

   if(speed_start_flag==0)
   {
	speed_start_num++;
   }
   else
   {
	  Engine();//  电机总控制
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
编码器测行走路程

* 函数名称：distance_judge(int e_distance)
* 功能说明：编码器测行走路程
* 参数说明：e_distance要走的距离
* 函数返回：char
* 修改时间：
* 备 注：
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

/*—————————————————*以上编码器*——————————————————*/




/*———————————————*以下pit计时中断*—————————————————*/
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