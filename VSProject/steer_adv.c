#include "common.h"
#include "include.h"
#include "steer_adv.h"
/*
 * 这是一个PD控制，由于偏转不会“漏水”，所以不用i来消除稳态误差
 */
 /*********define for SteerControl**********/

float  KP = 0.0;//舵机方向比例系数，影响舵机的打角范围
float  KD = 16.4;//10//7.5//舵机方向微分系数,影响舵机的打角反应
float  SteerPwmAdd = 0.0;//舵机pwm增量
float  Error;//偏差值
float  LastError;//上次的偏差
float  WeightSum = 0;
float  CenterMeanValue = 0;
float  CenterSum = 0;
float  J = 0.0290;//调节p和偏差的关系，越大，作用越强
float  BasicP = 3.0; //基本的P值/*  相应速度*/
uint32 SteerPwm = 0, LastSteerSwm = 0;//舵机的pwm值和上次舵机的pwm值

//加权平均，权值的选取
#if 1
//float Weight[60] = {
//					0,0,0,0,0,0,0,0,0,0,     //0-9行，基本用不到
//					0,0,0,0,0,0,2,2,2,2,  //0-19行，基本用不到
//
//				   1.8,1.8,1.8,1.8,1.8,1.3,1.3,1.3,1.3,1.3,//20-29行
//
//				   //1.05,1.05,1.05,1.05,1.05,2,2,2,2,2,
//
//				   2,2,2,2,2,2.0,2.0,2.5,2.5,2.5,//30-39行
//
//				   2.1,2.1,2.1,2.1,2.1,2,1,2,1,2,//40-49行
//				  1,0,0,0,0, };//最近十行*///不压线，用于弯道
float Weight[60] = {
0,		0,		0,		0,		0,		0,		0,		0,		0,		0,		//0-9行，基本用不到
0,		0,		0,		0,		0,		0,		2,		2,		2,		2,		//0-19行，基本用不到
1.8,	1.8,	1.8,	1.8,	1.8,	1.3,	1.3,	1.3,	1.3,	1.3,	//20-29行
2,		2,		2,		2,		2,		2.0,	2.0,	2.5,	2.5,	2.5,	//30-39行
2.1,	2.1,	2.1,	2.1,	2.1,	2,		1,		2,		1,		2,		//40-49行
1,		0,		0,		0,		0,		0,		0,		0,		0,		0		//最近十行*///不压线，用于弯道
};
#endif

#if 0
float Weight[60] = {
					 0,0,0,0,0,0,0,0,0,0,              //0-9行，基本用不到
					 0,0,0,0,0,1.5,1,1.5,1,             //0-19行，基本用不到
					 2.5,1,2.2,1,2.5,1,2,2,1,2,            //20-29行
					 1,3,2,2,1,2,1,2,2,1,              //30-39行
					 3,1,2,3,3,2,3,1,1,             //40-49行
					 1,1,1,1,1,2,1,1,0,0, };          //最近十行
#endif

/******************************************************
 *函数名：SteerInit
 *
 *功能：舵机初始化
 *
 * 入口参数：无
 *
 *返回参数：无
 *
 * 作者：XGL
 *
 * 日期：2016-3-01(已测试)
 *******************************************************/
void SteerInit(void)//舵机初始化
{
	ftm_pwm_init(FTM1, FTM_CH0, 50, SteerMidle);
}
#if 1



/******************************************************
 *函数名：CalculateError
 *
 *功能：计算误差
 *
 * 入口参数：无
 *
 *返回参数：无
 *
 * 作者：XGL
 *
 * 日期：2016-3-01(已测试)
 *******************************************************/
void CalculateError(void)

{
	int i;
	CenterSum = 0;
	CenterMeanValue = 0;
	WeightSum = 0;
	for (i = 57; i > LastLine; i--)

	{
		/*
		 * 所有中点带权相加及所有权值相加，根据近大远小加权
		 */
		CenterSum += MiddleLine[i] * Weight[i];
		WeightSum += Weight[i];
	}
	/*
	 * //算出加权平均后中线的值
	 */
	if (WeightSum != 0)
	{
		CenterMeanValue = (CenterSum / WeightSum);
	}
	LastError = Error;
	/*
	 * // 一场图像偏差值
	 */
	Error = (40 - CenterMeanValue);
	/*
	 * //偏差限幅
	 */
	if (Error >= 30.0) Error = 30.0;
	if (Error <= -30.0)	Error = -30.0;
	/*
	 * //动态二次p模型
	 */
	KP = BasicP + (Error * Error) * J;
	/*
	 * //p值限幅
	 */
	if (KP >= 11) KP = 11;
}

#endif                




/******************************************************
 *函数名：SteerControl
 *
 *功能：舵机控制
 *
 * 入口参数：无
 *
 *返回参数：无
 *
 * 作者：XGL
 *
 * 日期：2016-3-01(已测试)
 *******************************************************/
void SteerControl(void)
{
	CalculateError();
	SteerPwmAdd = (KP * Error) + KD * (Error - LastError);//舵机的pd控制器
	/*
	 * 限幅
	 */
	if (SteerPwmAdd >= 120) SteerPwmAdd = 120;
	if (SteerPwmAdd <= -120) SteerPwmAdd = -120;
	SteerPwm = (uint32)(SteerPwmAdd + SteerMidle);
	/*
	 * //限幅
	 */
	if (SteerPwm >= SteerMax)SteerPwm = SteerMax;
	if (SteerPwm <= SteerMin)SteerPwm = SteerMin;
	/*
	 * //舵机pwm更新
	*/
	ftm_pwm_duty(FTM1, FTM_CH0, SteerPwm);
	/*
	 * //记录pwm值
	 */
	LastSteerSwm = SteerPwm;
}










