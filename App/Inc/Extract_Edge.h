#ifndef _EXTRACT_EDGE_H_
#define _EXTRACT_EDGE_H_

 /*���߲��ֺ궨��*/
#define L_LOST            119             //����ض�ʧʱ������������Χ
#define L_EDGE_IN         60              //�����û������ʱ������������Χ
#define L_EDGE_OUT        60              //�����û������ʱ������������Χ
#define L_IN_2_IN         60              //���������ʱ������������Χ
#define L_IN_2_OUT        40              //���������ʱ������������Χ
#define L_OUT_2_IN        40              //���������ʱ������������Χ
#define L_OUT_2_OUT       40              //���������ʱ������������Χ
#define L_OUT_END         20              //��������㶪ʧ�����ж�
#define L_LOOP            20              //����ض�ʧ����������Χ


#define R_LOST            40              //�ұ��ض�ʧʱ������������Χ
#define R_EDGE_IN         60              //�����û������ʱ������������Χ
#define R_EDGE_OUT        60              //�����û������ʱ������������Χ
#define R_IN_2_IN         60              //�ұ�������ʱ������������Χ
#define R_IN_2_OUT        40              //�ұ�������ʱ������������Χ
#define R_OUT_2_IN        40              //�ұ�������ʱ������������Χ
#define R_OUT_2_OUT       40              //�ұ�������ʱ������������Χ
#define R_OUT_END         139             //�ұ������㶪ʧ�����ж�
#define R_LOOP            140             //�ұ��ض�ʧ����������Χ





typedef  struct
{
       int   RightEdge [ROW];//��߽����飬�����һ��Ԫ��60��
       int   LeftEdge  [ROW];//�ұ߽�����
       int   MiddleLine[ROW];

       unsigned char   RowLose[ROW];

       unsigned char   AllLose;//����ͬʱ���ߵļ�¼ֵ
       unsigned char   OnlyLeftLose;//ֻ����߽綪�ߵļ�¼ֵ
       unsigned char   OnlyRightLose;//ֻ���ұ߽綪�ߵļ�¼ֵ
       unsigned char   AllLeftLose;//��߽����ж��߼�¼ֵ
       unsigned char   AllRightLose;//�ұ߽����ж��߼�¼ֵ

       unsigned char   track_Width[ROW];//�������


       unsigned char L_StartLine;                //�������������������  -1
       unsigned char R_StartLine;                //�ұ�����������������  -1
       unsigned char StartLine;

       unsigned char L_EndLine;                //�������������������  -1
       unsigned char R_EndLine;                //�ұ�����������������  -1
       unsigned char EndLine;//��̬�ⶥ��
       unsigned char LastEndLine;//��¼�ϴηⶥ�еļ�¼ֵ

       unsigned char Left_Edge_num;
       unsigned char Right_Edge_num;
}Str_Boundary;

typedef  struct
{
       Site_t  L_down_point;            //�����ʮ���·�ֱ�ǹյ����飨[0]Ϊʮ�ֹյ�������ֵ��[1]Ϊʮ�ֹյ�������ֵ��
       Site_t  L_up_point;              //�����ʮ���Ϸ�ֱ�ǹյ����飨[0]Ϊʮ�ֹյ�������ֵ��[1]Ϊʮ�ֹյ�������ֵ��

       Site_t  R_down_point;            //�ұ���ʮ���·�ֱ�ǹյ����飨[0]Ϊʮ�ֹյ�������ֵ��[1]Ϊʮ�ֹյ�������ֵ��
       Site_t  R_up_point;              //�ұ���ʮ���Ϸ�ֱ�ǹյ����飨[0]Ϊʮ�ֹյ�������ֵ��[1]Ϊʮ�ֹյ�������ֵ��

}Str_Inflection_point;   //�յ�ṹ��



extern float Camera_CenterError;



extern Str_Boundary          Boundary;
extern Str_Inflection_point  Inflection_point;

extern void extract_edge(Str_Boundary  *str,unsigned char Row_START,unsigned char Row_END ,unsigned char Col_START,unsigned char Col_END) ;    //�˺���Ϊ������ȡ������


#endif