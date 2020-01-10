#ifndef __CURVATURE_H__
#define __CURVATURE_H__

typedef struct
{
int x; //float��Ϊint
int y;
} Point2D;

Point2D PointOnCubicBezier( Point2D* cp, float t );
void ComputeBezier( Point2D* cp, int numberOfPoints, Point2D* curve );
void Get_Bezier( int xx1, int yy1, int xx2, int yy2, int xx3, int yy3,int xx4,int yy4 );
#define DD 20//����������  ����������ߵ����
extern Point2D curve1[DD];
extern void Get_Bezier( int xx1, int yy1, int xx2, int yy2, int xx3, int yy3,int xx4,int yy4 );
#endif
