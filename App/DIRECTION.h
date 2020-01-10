#ifndef _DIRECTION_H_
#define _DIRECTION_H_


extern float ke;

extern volatile unsigned char IsStartLine;

float GetSteerError(unsigned char start,unsigned char end,float midpos);
float GetSteerError2(unsigned char start,unsigned char end,float midpos);
void DirectionCtrol();

#endif