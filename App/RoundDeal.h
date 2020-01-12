#ifndef __ROUNDDEAL_H__
#define __ROUNDDEAL_H__
#include "common.h"
#include "include.h"

/**
 * \brief //圆环结构体
 */
typedef  struct
{
	unsigned char Round_flag;     //圆环标志位//是环为1
	unsigned char Round_flag_able; //圆环标志位使能
	unsigned char Entrance_count;//圆环入口计数
	unsigned char Track_Width_forRound[ROW_END];//赛道宽度
}Str_Round;
typedef  struct
{
	Site_t  L_down_point;//左边沿十字下方直角拐点数组（[0]为十字拐点行坐标值，[1]为十字拐点列坐标值）
	Site_t  L_up_point;//左边沿十字上方直角拐点数组（[0]为十字拐点行坐标值，[1]为十字拐点列坐标值）
	Site_t  R_down_point;//右边沿十字下方直角拐点数组（[0]为十字拐点行坐标值，[1]为十字拐点列坐标值）
	Site_t  R_up_point;//右边沿十字上方直角拐点数组（[0]为十字拐点行坐标值，[1]为十字拐点列坐标值）

}Str_Inflection_point;   //拐点结构体
extern Str_Round  Round;

extern void Round_Deal();
typedef  struct
{
	//int   RightEdge[ROW];//左边界数组，定义多一个元素60号
	//int   LeftEdge[ROW];//右边界数组
	//int   MiddleLine[ROW];
	int* RightEdge;
	int* LeftEdge;
	int* MiddleLine;

	//unsigned char   RowLose[ROW];
	unsigned char   *RowLose;

	unsigned char   AllLose;//左右同时丢线的记录值
	unsigned char   OnlyLeftLose;//只是左边界丢线的记录值
	unsigned char   OnlyRightLose;//只有右边界丢线的记录值
	unsigned char   AllLeftLose;//左边界所有丢线记录值
	unsigned char   AllRightLose;//右边界所有丢线记录值

	//unsigned char   track_Width[ROW];//赛道宽度
	unsigned char   *track_Width;//赛道宽度


	unsigned char L_StartLine;                //左线第一次获得线的行数，左边沿搜索结束行数置  -1
	unsigned char R_StartLine;                //右线第一次获得线的行数，右边沿搜索结束行数置  -1
	unsigned char StartLine;

	unsigned char L_EndLine;                //左边沿搜索结束行数置  -1
	unsigned char R_EndLine;                //右边沿搜索结束行数置  -1
	unsigned char EndLine;//动态封顶行
	unsigned char LastEndLine;//记录上次封顶行的记录值

	unsigned char Left_Edge_num;
	unsigned char Right_Edge_num;
}Str_Boundary;
//extern RoundProcessTypeDef RoundProcess;;
extern Str_Boundary          Boundary;
extern Str_Inflection_point  Inflection_point;

typedef enum __RoundType
{
	Left_Round = 1,
	Right_Round
}RoundTypeDef;


typedef enum __RoundSize
{
	Small_Round = 1,
	//  Middle_Round,               //中环不考虑了
	Large_Round

}RoundSizeTypeDef;

typedef enum __RoundProcess
{
	Round_Not_Found = 0,
	Find_Round,
	Find_Gap,//发现圆环中心，认为有缺口
	Into_Round,//可以进环
	Into_Round2,
	In_Round,
	Out_Round,
	Leave_Round,
	Leave_Round2

}RoundProcessTypeDef;
extern RoundProcessTypeDef RoundProcess;
#endif

