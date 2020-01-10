#ifndef _PIT_H_
#define _PIT_H_

typedef struct
{
       int Pulse_Num;
       int Pulse_Accumulate;
       int trip_distance_mm;
       int trip_distance_m;

       int trip_distance_flag;
       int trip_distance_start;
       int trip_distance_get;
}Str_Encoder;

//extern Str_Encoder Encoder;

extern int NS_Speed;

extern int  encoder_left_speed,encoder_right_speed;
extern void encoder_PIT_INIT(void);

extern char distance_judge(int e_distance);
#endif