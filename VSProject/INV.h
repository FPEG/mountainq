#ifndef _INV_H_
#define _INV_H_


typedef struct
{
       double x;
       double y;
}Site_xy;	//���帡���ͽṹ��

typedef struct
{
       int16 x;
       int16 y;
}Site_xy1;	//�������ͽṹ��ͼ��תʵ�ʺ���

extern Site_xy get_inv_img(int16 xxx ,int16 yyy);


#endif