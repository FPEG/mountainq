#ifndef __function_library_H__
#define __function_library_H__


#include   "include.h"

#define buzzer_pin   PTA9
#define KEY1    PTA25
#define KEY2    PTA26
#define KEY3    PTA27
#define KEY4    PTA28

extern uint8 key_flag;


typedef enum __SetMode
{
  Speed_Set=0,
  PID_Set,
  Berrier_Set,
  Round_Set
}SetModeTypeDef;

extern void Wait_before_start(void);
extern void  find_loop();
extern void buzzer_test(void);
extern void XB_reset_check();
extern void XB_reset_check();
extern void lcd_show_setting();
extern void GUI();
extern uint8 erkey_get(PTXn_e ptxn)  ;

extern int16  Bar_Left_para;//左转向参数
extern int16  Bar_Right_para;//右转向参数
extern float servPram;
extern float servD;
#endif  //__MK60_IT_H__

