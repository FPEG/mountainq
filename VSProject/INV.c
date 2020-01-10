#include "include.h"
#include "INV.h"


//rot=[ 8.3026, 2.8477e-12, -668.3553; -2.9995e-13, -12.6121, 884.3799;-3.6189e-14, 0.08971, 1 ]。
//rot=[10.8534, 26.6711, -423.5712; -0.31229, 30.7797, -251.1947; -0.018005, 0.69711, 1 ];

#define INV_A  10.8534
#define INV_B  27
#define INV_C  -423.5712
#define INV_D  1
#define INV_E  30.7797
#define INV_F  -251.1947
#define INV_G  0
#define INV_H  0.69711




Site_xy get_inv_img(int16 xxx ,int16 yyy)	//逆透视（xxx 为图像行，yyy 为列）
{
       Site_xy temp;
       xxx++;
       yyy++;
       temp.y = (INV_A*yyy+INV_B*xxx+INV_C)/(INV_G*yyy+INV_H*xxx+1);
       temp.x = (INV_D*yyy+INV_E*xxx+INV_F)/(INV_G*yyy+INV_H*xxx+1);
       return temp;
}

/******************************************************************************
*****************************************************/

Site_xy1 get_invinv_img(double xxx,double yyy) //反逆透视（xxx 为实际行，yyy 为列）
{
       Site_xy1 temp;
       double uSrcImg,vSrcImg;//反逆透视坐标
       uSrcImg =(INV_B*INV_F - INV_C*INV_E + INV_E*yyy - INV_B*xxx - INV_F*INV_H*yyy + INV_C*INV_H*xxx)/(INV_A*INV_E - INV_B*INV_D + INV_D*INV_H*yyy - INV_E*INV_G*yyy - INV_A*INV_H*xxx + INV_B*INV_G*xxx);
       vSrcImg =-(INV_A*INV_F - INV_C*INV_D + INV_D*yyy - INV_A*xxx - INV_F*INV_G*yyy + INV_C*INV_G*xxx)/(INV_A*INV_E - INV_B*INV_D + INV_D*INV_H*yyy - INV_E*INV_G*yyy - INV_A*INV_H*xxx + INV_B*INV_G*xxx);

       temp.x=(int)round(vSrcImg-1); temp.y=(int)round(uSrcImg-1);
       return temp;
}

