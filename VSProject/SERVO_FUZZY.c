#include "SERVO_FUZZY.h"

float32_t last_err;
float32_t error_variety=0;//误差变化
float32_t a=0.2,b,u,R = 35;
float32_t ABS_ERR,ERR,ERROR_VARIETY;
float32_t Ke=67.5,Kec=60.5,Ka=0.85;

float32_t SERVO_STEER(float32_t err)/*模糊运算引擎*/
{
       //e------->err误差
       //ec------>误差变化率
       //Ke,Kec自调整因子
       //控制规则：Ke * e + Kec * ec，u =(a + ABS( err ))* err +(b - ABS( err ))* error_variety;

  error_variety = err - last_err;

  u =Ke*(a+Ka*ABS( err )/R)* err + Kec*(1-a-Ka*ABS( err )/R)* error_variety;
  if(u>=165)
  {
    u = 165;
  }
  else if(u<=-165)
  {
    u = -165;
  }

  last_err = err;

  return u;
}