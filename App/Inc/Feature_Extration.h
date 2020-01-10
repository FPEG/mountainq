#ifndef _FEATURE_EXTRACTION_H_
#define _FEATURE_EXTRACTION_H_



typedef  struct
{
       unsigned char BlackEndL ;
       unsigned char BlackEndM ;
       unsigned char BlackEndR ;
       unsigned char BlackEndMax ;
       unsigned char BlackEndLMR ;

       unsigned char LEndFlag ;//左截至标志
       unsigned char MEndFlag ;//中·截至标志
       unsigned char REndFlag ;//右截至标志

}Str_BlackEnd;


typedef  struct
{
       unsigned int  total_time ;
       unsigned char break_point_num ;
       unsigned char many_bp_row;
       unsigned char ZebraLine_Flag ;
       unsigned char ZebraLine_Pos ;
}Str_ZebraLine;//斑马线

extern Str_BlackEnd  BlackEnd;

extern float Slope_Calculate(uint8 begin,uint8 end,int *p);
extern void Mark_ZebraLine(unsigned int Z_TIME); //600
extern void GetBlackEndParam();
//extern void SCProcessing();
//extern unsigned char P_LeftBlack[ROW];
//extern unsigned char P_RightBlack[ROW];
//extern unsigned char Crossingpoint_start;
//extern unsigned char Crossingpoint_end;

extern void CommonRectificate(int data[],unsigned char begin,unsigned char end);
//#define L_BlackEnd 1
//#define R_BlackEnd 2
//#define M_BlackEnd 0
//#define Angle      3

//extern unsigned char WhiteRow;
//extern unsigned char LWhiteRow;
//extern unsigned char RWhiteRow;
//extern unsigned char RoadType;
//extern uint8 Initial_line ;
//#define All_Black 1
//#define All_White 2
//#define None 0
//
//extern unsigned char BlackEndL;
extern unsigned char BlackEndM;
//extern unsigned char BlackEndR;
//extern unsigned char BlackEndMax;
//extern unsigned char BlackEndLMR;
//extern unsigned char g_Derict;
//extern unsigned char LeftBlack_A[ROW];
//extern unsigned char RightBlack_A[ROW];
//extern unsigned char BlackLineData_A[ROW];
//
//extern signed char TripPointPos[ROW];
//extern signed char SubValue[ROW];
//extern float EPerCount;
//extern int Excursion;
//
//extern uint8 LeftEdge_abort;
//extern uint8 RightEdge_abort;

#endif