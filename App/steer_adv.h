#ifndef __STEER_ADV_H__
#define __STEER_ADV_H__

#define S3010_FTM   FTM2       //FTM1ģ��
#define S3010_CH    FTM_CH0   //ͨ����PTA8
#define S3010_HZ    50       //�����Ƶ��

#define SteerMax 850     //������ֵ
#define SteerMin  700    //�����Сֵ  
#define SteerMidle 768   //�����ֵ

#define SteerSection 68

extern float  LastError;
extern float  Error;


void SteerInit(void);//�����ʼ��
void CalculateError(void);
extern void SteerControl(void);


#endif
