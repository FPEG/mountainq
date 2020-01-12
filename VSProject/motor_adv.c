#include  "common.h"
#include "include.h"
#include "motor_adv.h"
float MaxMean = 0;
void MotorInit()
{
	//ftm_pwm_init(FTM0, FTM_CH4, 10000, 800);//D4右轮倒车
	//ftm_pwm_init(FTM0, FTM_CH6, 10000, 800);//D6左轮倒车
	ftm_pwm_init(FTM0, FTM_CH5, 10000, 1000);//D5右轮前进
	ftm_pwm_init(FTM0, FTM_CH7, 10000, 1000);//D7左轮前进
}
void MotorControl()
{
	LCD_Show_Number3(100, 1, BlackEndLL);
	LCD_Show_Number3(100, 2, BlackEndL);
	LCD_Show_Number3(100, 3, BlackEndML);
	LCD_Show_Number3(100, 4, BlackEndM);
	//LCD_Show_Number3(100, 5, BlackEndMR);
	//LCD_Show_Number3(100, 6, BlackEndR);
	//LCD_Show_Number3(100, 7, BlackEndRR);
	//float avg = (BlackEndLL + BlackEndL + BlackEndML + BlackEndM + BlackEndMR + BlackEndR + BlackEndRR) / 7;
	//float d = ()
	float32_t p[7];
	p[0] = BlackEndLL;
	p[1] = BlackEndL;
	p[2] = BlackEndML;
	p[3] = BlackEndM;
	p[4] = BlackEndMR;
	p[5] = BlackEndR;
	p[6] = BlackEndRR;
	float32_t var = 0;
	float32_t mean = 0;
	float32_t min = 0;
	float32_t max = 0;
	uint32_t index = 0;
	arm_var_f32(p, 7, &var);
	OLED_PrintFloat(80, 7, var);
	arm_max_f32(p, 7, &max,&index);
	arm_min_f32(p, 7, &min,&index);
	arm_mean_f32(p, 7, &mean);
	OLED_PrintFloat(80, 6, max-mean);
	OLED_PrintFloat(80, 5, max-min);
	MaxMean = max - mean;
/*
	if(MaxMean>40)
	{
		ftm_pwm_duty(FTM0, FTM_CH5, 1000);//D5右轮前进
		ftm_pwm_duty(FTM0, FTM_CH7, 1000);//D5右轮前进
		return;
	}
*/
	if (BlackEndM > 50)
	{
		ftm_pwm_duty(FTM0, FTM_CH5, 680);//D5右轮前进
		ftm_pwm_duty(FTM0, FTM_CH7, 680);//D7左轮前进
	}
	else
	{
		ftm_pwm_duty(FTM0, FTM_CH5, 750);//D5右轮前进
		ftm_pwm_duty(FTM0, FTM_CH7, 750);//D7左轮前进
	}
}
