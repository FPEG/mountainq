#ifndef __STEER_ADV_H__
#define __STEER_ADV_H__

#define S3010_FTM   FTM1       //FTM1ģ��
#define S3010_CH    FTM_CH0   //ͨ����PTA8
#define S3010_HZ    50       //�����Ƶ��

#define SteerMax 565     //������ֵ
//todo �����ֵ����Сֵ�ǲ��Ƿ���
#define SteerMin  405    //�����ֵ  
#define SteerMidle 480   //�����Сֵ

extern float  LastError;
extern float  Error;


void SteerInit(void);//�����ʼ��
void CalculateError(void);
extern void SteerControl(void);


#endif
