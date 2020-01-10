#ifndef _GRAY_CALCULATE_H_
#define _GRAY_CALCULATE_H_


#define GrayScale 256   //´ó½ò·¨ÏñËØ

extern int  G_yuzhi;
extern uint8 Threshold;
extern uint8 otsuThreshold(uint8 *image, uint16 col,uint16 row);
extern void binarization_processing1();
extern void binarization_processing(unsigned char *p,uint16 row,uint16 col);
extern unsigned char iteration(unsigned char *p,unsigned char row,unsigned char col,unsigned char iteration_time,unsigned char *BlackThres) ;
extern uint8  adapt_otsuThreshold(uint8 *image, uint16 col, uint16 row,unsigned char *threshold);
#endif