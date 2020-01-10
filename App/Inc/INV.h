#ifndef _INV_H_
#define _INV_H_


typedef struct
{
       double x;
       double y;
}Site_xy;	//定义浮点型结构体

typedef struct
{
       int16 x;
       int16 y;
}Site_xy1;	//定义整型结构体图像转实际函数

extern Site_xy get_inv_img(int16 xxx ,int16 yyy);


#endif