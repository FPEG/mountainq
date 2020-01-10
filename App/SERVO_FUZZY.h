#ifndef _SERVO_FUZZY_H_
#define _SERVO_FUZZY_H_


#include "include.h"
#include "common.h"
extern float32_t Ke,Kec,u,Ka,a;
extern float32_t ABS_ERR,ERR,ERROR_VARIETY; 
extern float32_t P,D,a,b,u,R,Ke,Ke,Ka;

float32_t SERVO_STEER(float32_t err);/*Ä£ºıÔËËãÒıÇæ*/

#endif