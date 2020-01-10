#ifndef _EXTRACT_EDGE_H_
#define _EXTRACT_EDGE_H_

 /*搜线部分宏定义*/
#define L_LOST            119             //左边沿丢失时，向右搜索范围
#define L_EDGE_IN         60              //左边沿没有趋势时，向内搜索范围
#define L_EDGE_OUT        60              //左边沿没有趋势时，向外搜索范围
#define L_IN_2_IN         60              //左边沿内倾时，向内搜索范围
#define L_IN_2_OUT        40              //左边沿内倾时，向外搜索范围
#define L_OUT_2_IN        40              //左边沿外倾时，向内搜索范围
#define L_OUT_2_OUT       40              //左边沿外倾时，向外搜索范围
#define L_OUT_END         20              //左边沿外倾丢失结束判断
#define L_LOOP            20              //左边沿丢失环道搜索范围


#define R_LOST            40              //右边沿丢失时，向右搜索范围
#define R_EDGE_IN         60              //左边沿没有趋势时，向内搜索范围
#define R_EDGE_OUT        60              //左边沿没有趋势时，向外搜索范围
#define R_IN_2_IN         60              //右边沿内倾时，向内搜索范围
#define R_IN_2_OUT        40              //右边沿内倾时，向外搜索范围
#define R_OUT_2_IN        40              //右边沿外倾时，向内搜索范围
#define R_OUT_2_OUT       40              //右边沿外倾时，向外搜索范围
#define R_OUT_END         139             //右边沿外倾丢失结束判断
#define R_LOOP            140             //右边沿丢失环道搜索范围





typedef  struct
{
       int   RightEdge [ROW];//左边界数组，定义多一个元素60号
       int   LeftEdge  [ROW];//右边界数组
       int   MiddleLine[ROW];

       unsigned char   RowLose[ROW];

       unsigned char   AllLose;//左右同时丢线的记录值
       unsigned char   OnlyLeftLose;//只是左边界丢线的记录值
       unsigned char   OnlyRightLose;//只有右边界丢线的记录值
       unsigned char   AllLeftLose;//左边界所有丢线记录值
       unsigned char   AllRightLose;//右边界所有丢线记录值

       unsigned char   track_Width[ROW];//赛道宽度


       unsigned char L_StartLine;                //左边沿搜索结束行数置  -1
       unsigned char R_StartLine;                //右边沿搜索结束行数置  -1
       unsigned char StartLine;

       unsigned char L_EndLine;                //左边沿搜索结束行数置  -1
       unsigned char R_EndLine;                //右边沿搜索结束行数置  -1
       unsigned char EndLine;//动态封顶行
       unsigned char LastEndLine;//记录上次封顶行的记录值

       unsigned char Left_Edge_num;
       unsigned char Right_Edge_num;
}Str_Boundary;

typedef  struct
{
       Site_t  L_down_point;            //左边沿十字下方直角拐点数组（[0]为十字拐点行坐标值，[1]为十字拐点列坐标值）
       Site_t  L_up_point;              //左边沿十字上方直角拐点数组（[0]为十字拐点行坐标值，[1]为十字拐点列坐标值）

       Site_t  R_down_point;            //右边沿十字下方直角拐点数组（[0]为十字拐点行坐标值，[1]为十字拐点列坐标值）
       Site_t  R_up_point;              //右边沿十字上方直角拐点数组（[0]为十字拐点行坐标值，[1]为十字拐点列坐标值）

}Str_Inflection_point;   //拐点结构体



extern float Camera_CenterError;



extern Str_Boundary          Boundary;
extern Str_Inflection_point  Inflection_point;

extern void extract_edge(Str_Boundary  *str,unsigned char Row_START,unsigned char Row_END ,unsigned char Col_START,unsigned char Col_END) ;    //此函数为边沿提取主函数


#endif