#ifndef __TRACKTYPE_H__
#define __TRACKTYPE_H__

#include "common.h"
#include "include.h"

#define ObstacleOpen  0//关闭障碍

extern unsigned char  CrossFlag;//十字标志
extern unsigned char CloseLoopFlag;



void IsOrNotCrossLoop(void);
extern void RecognitionObstacle(void);
extern void ObstacleRepair();
extern void NormalCrossConduct(void);

#endif