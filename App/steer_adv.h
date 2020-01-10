#ifndef __STEER_ADV_H__
#define __STEER_ADV_H__

#define S3010_FTM   FTM2       //FTM1模块
#define S3010_CH    FTM_CH0   //通道号PTA8
#define S3010_HZ    50       //舵机的频率

#define SteerMax 850     //舵机最大值
#define SteerMin  700    //舵机最小值  
#define SteerMidle 768   //舵机中值

#define SteerSection 68

extern float  LastError;
extern float  Error;


void SteerInit(void);//舵机初始化
void CalculateError(void);
extern void SteerControl(void);


#endif
