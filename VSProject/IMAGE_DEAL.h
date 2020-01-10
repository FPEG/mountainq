/* @file       VCAN_OV7725_Eagle.h
 * @brief      ӥ��ov7725��������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-09-07
 */
#ifndef _IMAGE_DEAL_H_
#define _IMAGE_DEAL_H_


#define Border_left    1     //ͼ����߽�
#define Border_right   COL-2  //ͼ���ұ߽�
#define Border_top     0     //ͼ���ϱ߽�
#define Border_bottom  ROW-1 //ͼ���±߽�
#define mid_point      COL/2//ͼ���е�


extern uint16 LeftBlack[ROW];
extern uint16 RightBlack[ROW];
extern uint16 BlackLineData[ROW];



extern void halve_image(unsigned char *p_in,unsigned char  *p_out,unsigned char row,unsigned char col);

//-----------------------------19.3.7

extern float kk;
extern int jiaozhenbiao[ROW][COL];
extern int jiaozhen_left[ROW];
extern int jiaozhen_right[ROW];


//���������
extern uint8 NS_LeftBlack[ROW];
extern uint8 NS_RightBlack[ROW];
extern void Left_Tracking();
extern void Right_Tracking();
extern uint8 NS_CenterBlack[ROW];


void jiaozhen_cal(void);

extern void find_L_R_M(void);
extern void image_deal();
extern void mark_stop();
extern void check_and_stop(int num,int flag);
extern void err_deal(int line_start,int line_end);
extern void  Ctrl(void);



extern float center_line_err;



//------------------


extern void LCD_Draw_Midline();
extern uint32 total_time;
extern uint8 jump_point;
extern uint8 stop_car_flag;




#endif