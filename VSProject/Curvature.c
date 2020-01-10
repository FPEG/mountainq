#include "Curvature.h"
#include "include.h"

Point2D curve1[DD];//存放曲线点的数组
Point2D cp1[4];


/****************************************
*  功能说明：计算出t时候的点值（0<t<1）
*  参数说明：输入四个已知点（首尾和两个控制点）的横、纵坐标；t时（0<t<1）
*  函数返回：输出t时候的曲线点
*  修改时间：2018-1-23
*  备    注：
cp 在此是四个元素的数组:
cp[0] 为起点，或上图中的 P0
cp[1] 为第一控制点，或上图中的 P1
cp[2] 为第二控制点，或上图中的 P2
cp[3] 为结束点，或上图中的 P3
t 为参数值，0 <= t <= 1
****************************************/

Point2D PointOnCubicBezier( Point2D* cp, float t ) //
{
    float ax, bx, cx;
    float ay, by, cy;
    float tSquared, tCubed;
    Point2D result;
    /* 计算多项式系数 */
    cx = 3.0 * ( cp[1].x - cp[0].x );
    bx = 3.0 * ( cp[2].x - cp[1].x ) - cx;
    ax = cp[3].x - cp[0].x - cx - bx;
    cy = 3.0 * ( cp[1].y - cp[0].y );
    by = 3.0 * ( cp[2].y - cp[1].y ) - cy;
    ay = cp[3].y - cp[0].y - cy - by;
    /* 计算t位置的点值 */
    tSquared = t * t;
    tCubed = tSquared * t;
    result.x = ( int )( ( ax * tCubed ) + ( bx * tSquared ) + ( cx * t ) + cp[0].x );
    result.y = ( int )( ( ay * tCubed ) + ( by * tSquared ) + ( cy * t ) + cp[0].y );
    return result;
}

/* ComputeBezier 以控制点 cp 所产生的曲线点，填入 Point2D 结构数组。
调用方必须分配足够的空间以供输出，<sizeof(Point2D) numberOfPoints> */
void ComputeBezier( Point2D* cp, int numberOfPoints, Point2D* curve )
{
    float dt;
    int i;
    dt = 1.0 / ( numberOfPoints - 1 );
    for( i = 0; i < numberOfPoints; i++ )
    {
        curve[i] = PointOnCubicBezier( cp, i * dt );
    }
}



/****************************************
*  功能说明：输入三个曲线上的点获得曲线数点（存放在数组curve1[]中）
*  参数说明：输入三个已知点（首尾和中间）的横、纵坐标；t时（0<t<1）
*  函数返回：无
*  修改时间：2018-1-23
*  备    注：（调用该函数即可获得贝塞尔曲线点坐标）
****************************************/
void Get_Bezier( int xx1, int yy1, int xx2, int yy2, int xx3, int yy3,int xx4,int yy4 ) //（调用该函数即可获得贝塞尔曲线点坐标）
{
    cp1[0].x = xx1;
    cp1[0].y = yy1;
    cp1[1].x = xx2;
    cp1[1].y = yy2;
    cp1[2].x = xx3;
    cp1[2].y = yy3;
    cp1[3].x = xx4;
    cp1[3].y = yy4;
    ComputeBezier( cp1, DD, curve1 );
}