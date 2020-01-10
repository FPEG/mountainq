#include "Curvature.h"
#include "include.h"

Point2D curve1[DD];//������ߵ������
Point2D cp1[4];


/****************************************
*  ����˵���������tʱ��ĵ�ֵ��0<t<1��
*  ����˵���������ĸ���֪�㣨��β���������Ƶ㣩�ĺᡢ�����ꣻtʱ��0<t<1��
*  �������أ����tʱ������ߵ�
*  �޸�ʱ�䣺2018-1-23
*  ��    ע��
cp �ڴ����ĸ�Ԫ�ص�����:
cp[0] Ϊ��㣬����ͼ�е� P0
cp[1] Ϊ��һ���Ƶ㣬����ͼ�е� P1
cp[2] Ϊ�ڶ����Ƶ㣬����ͼ�е� P2
cp[3] Ϊ�����㣬����ͼ�е� P3
t Ϊ����ֵ��0 <= t <= 1
****************************************/

Point2D PointOnCubicBezier( Point2D* cp, float t ) //
{
    float ax, bx, cx;
    float ay, by, cy;
    float tSquared, tCubed;
    Point2D result;
    /* �������ʽϵ�� */
    cx = 3.0 * ( cp[1].x - cp[0].x );
    bx = 3.0 * ( cp[2].x - cp[1].x ) - cx;
    ax = cp[3].x - cp[0].x - cx - bx;
    cy = 3.0 * ( cp[1].y - cp[0].y );
    by = 3.0 * ( cp[2].y - cp[1].y ) - cy;
    ay = cp[3].y - cp[0].y - cy - by;
    /* ����tλ�õĵ�ֵ */
    tSquared = t * t;
    tCubed = tSquared * t;
    result.x = ( int )( ( ax * tCubed ) + ( bx * tSquared ) + ( cx * t ) + cp[0].x );
    result.y = ( int )( ( ay * tCubed ) + ( by * tSquared ) + ( cy * t ) + cp[0].y );
    return result;
}

/* ComputeBezier �Կ��Ƶ� cp �����������ߵ㣬���� Point2D �ṹ���顣
���÷���������㹻�Ŀռ��Թ������<sizeof(Point2D) numberOfPoints> */
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
*  ����˵�����������������ϵĵ����������㣨���������curve1[]�У�
*  ����˵��������������֪�㣨��β���м䣩�ĺᡢ�����ꣻtʱ��0<t<1��
*  �������أ���
*  �޸�ʱ�䣺2018-1-23
*  ��    ע�������øú������ɻ�ñ��������ߵ����꣩
****************************************/
void Get_Bezier( int xx1, int yy1, int xx2, int yy2, int xx3, int yy3,int xx4,int yy4 ) //�����øú������ɻ�ñ��������ߵ����꣩
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