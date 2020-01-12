#include "common.h"
#include "include.h"
#include "TrackType.h"


/*********define for CrossConduct**********/
unsigned char  CrossFlag = 0;//十字标志
unsigned char  LastCrossFlag = 0;//记录上上次是否是十字
unsigned char  LastLastCrossFlag = 0;//记录上一次是否是十字
unsigned char  CrossNumberFalg = 0;//记录，代表已经进入了十字
unsigned char  CloseLoopFlag = 0;
/*
 * 不用的
 */
/*********define for RecognitionObstacle**********/
unsigned char  ObstacleLfetRowStart = 0;
unsigned char  ObstacleLfetColStart = 0;
unsigned char  ObstacleLfetOkFlag = 0;
unsigned char  ObstacleRightRowStart = 0;
unsigned char  ObstacleRightColStart = 0;
unsigned char  ObstacleRightOkFlag = 0;


/**
 * \brief 
 */
void NormalCrossConduct(void)
{
	unsigned char i;

	LastLastCrossFlag = LastCrossFlag;//记录上上次是否是十字
	LastCrossFlag = CrossFlag;//记录上一次是否是十字
	CrossFlag = 0;//清零


	if ((AllLose >= 15))//左右同时丢线//左右同时丢线行数大于15//15这个参数感觉要改//15是用来判断十字还是圆环的标志，圆环0，十字1
	{
		CrossFlag = 1;//标记十字    
	}
	if (CrossFlag)
	{


		if ((BlackEndM >= BlackEndL) && (BlackEndM >= BlackEndR))//中间白线最多 
		{
			for (i = 58; i > 20; i--)
			{
				MiddleLine[i] = 40;//直走    
			}
		}
		else if ((BlackEndL > BlackEndM) && (BlackEndL > BlackEndR))//看到左边白的最多，车体偏右了，往左跑  
		{
			for (i = 58; i > 20; i--)
			{
				MiddleLine[i] = 30;
			}
		}
		else if ((BlackEndR > BlackEndM) && (BlackEndR > BlackEndL))//看到右边白的最多，车体偏左了，往右跑
		{
			for (i = 58; i > 20; i--)
			{

				MiddleLine[i] = 50;
			}
		}
	}


	IsOrNotCrossLoop();

}



/**
 * \brief //判断是不是进去了十字回环，如果，则要关闭环道识别，否则斜出十字是，会误判成环道
 */
void IsOrNotCrossLoop(void)
{
	static char CrossLoopErrorNum = 0;
	static unsigned char CloseLoopNum = 0;
	static unsigned char CrossErrorClose = 0;//记录多久没有遇到偏差大于10//？我认为是用单双记录出环入环
	unsigned char  ErrorCloseFlag = 0;

	ErrorCloseFlag = 0;

	if (LastLastCrossFlag && LastCrossFlag && CrossFlag == 0)//前两次是十字，本次不是十字//进入十字
	{
		CrossNumberFalg++;//记录，代表已经进入了十字
	}
	/*
	 * 进入了圆环
	 */
	if (CrossNumberFalg == 1)
	{
		CrossErrorClose++;//为出圆环准备

		if (CrossErrorClose > 8)//？？进了多次环
		{
			ErrorCloseFlag = 1;
			CrossErrorClose = 0;

		}
		/*
		 * 看不懂
		 */
		if (ABS(Error) >= 10 && !ErrorCloseFlag && LastLine >= 20)//避免十字接直道，关闭了环道
		{
			CrossLoopErrorNum++;//连续记录偏大于10

		}
		else
		{
			CrossLoopErrorNum = 0;

		}

		/*
		 * 看不懂
		 */
		if (CrossLoopErrorNum >= 4 && CloseLoopFlag == 0)//连续4次偏差大于10
		{
			CloseLoopFlag = 1;//关掉圆环处理
			CrossLoopErrorNum = 0;
		}
	}


	/*
	 * 圆环内多次绕圈处理
	 * 我们应该不需要
	 */
	if (CloseLoopFlag)//开始计算关闭了环道处理的时间
	{

		CloseLoopNum++;
		gpio_set(PTB19, 1);

		if (CloseLoopNum >= 100)//如果时间太长还没遇到十字出口，强制打开环道
		{
			CloseLoopFlag = 0;
			CloseLoopNum = 0;
			LastLastCrossFlag = 0;
			LastCrossFlag = 0;
			CrossNumberFalg = 0;//开圆环处理
			gpio_set(PTB19, 0);
		}

	}

	if (CrossNumberFalg >= 2 && CloseLoopFlag)
	{
		CloseLoopFlag = 0;//开圆环处理
		LastLastCrossFlag = 0;
		LastCrossFlag = 0;
		CrossNumberFalg = 0;
		CloseLoopNum = 0;
		gpio_set(PTB19, 0);
	}
}


#if ObstacleOpen  //如果开启了障碍识别

//障碍识别

void RecognitionObstacle(void)
{
	int i, j;
	static unsigned char ObstacleLfetRowNum = 0;
	static unsigned char ObstacleLfetNum = 0;
	unsigned char  ObstacleLeftFlag = 0;
	unsigned char ObstacleLeftLFlag = 0;
	static unsigned char ScanColStartLeft = 0;//扫描的起始列
	static unsigned char ScanColEndLeft = 0;//扫描的终止列

	static unsigned char ObstacleRightRowNum = 0;
	static unsigned char ObstacleRightNum = 0;
	unsigned char   ObstacleRightFlag = 0;
	unsigned char  ObstacleRightRFlag = 0;
	static unsigned char ScanColStartRight = 0;//扫描的起始列
	static unsigned char ScanColEndRight = 0;//扫描的终止列


	ScanColStartLeft = 0;
	ScanColEndLeft = 0;
	ObstacleLeftFlag = 0;
	ObstacleLeftLFlag = 0;
	ObstacleLfetRowStart = 0;
	ObstacleLfetColStart = 0;
	ObstacleLfetRowNum = 0;

	ScanColStartRight = 0;
	ScanColEndRight = 0;
	ObstacleRightFlag = 0;
	ObstacleRightRFlag = 0;
	ObstacleRightRowStart = 0;
	ObstacleRightColStart = 0;
	ObstacleRightRowNum = 0;
	//ObstacleLfetOkFlag=0;

	if (ABS(Error) <= 6 && AllLose <= 1 && LastLine <= 12 && !CrossFlag && LoopRightControlFlag == 0 && LoopLeftControlFlag == 0 && LeftLose < 4 && RightLose <= 4)

	{
		for (i = 58; i > 20; i--)
		{
			if (LeftEdge[i] != 0)//如果当行的左边线存在
			{
				if (ABS(LeftEdge[i + 1] - LeftEdge[i]) >= 3)//左边界跳变
				{

					if (LeftEdge[i + 1] != 0)
					{
						ScanColStartLeft = LeftEdge[i + 1];//用前一行作为扫描的起始列

					}

					else
					{

						ScanColStartLeft = LeftEdge[56];

					}

				}

				else
				{

					ScanColStartLeft = LeftEdge[i];

				}

			}

			else//如果当行左边线不存在
			{
				ScanColStartLeft = LeftEdge[i + 1];//用前一行作为扫描的起始列
			}


			ScanColEndLeft = MiddleLine[i];

			for (j = ScanColStartLeft; j < ScanColEndLeft; j++)//往左边寻找障碍物
			{
				if (img[i][j] == White_Point && img[i][j + 1] == Black_Point)//找到了跳变
				{
					ObstacleLeftFlag = 1;

					break;

				}

			}

			unsigned char jj = j + 1;

			if (ObstacleLeftFlag)
			{

				while (jj < ScanColEndLeft)
				{
					if (img[i][jj] == Black_Point && img[i][jj + 1] == White_Point)//找到又一个跳变
					{
						ObstacleLeftLFlag = 1;

						break;//退出while
					}

					jj++;

				}


			}

			if (ObstacleLeftLFlag && ObstacleLeftFlag)
			{

				ObstacleLfetRowNum++;

				if (ObstacleLfetRowNum == 1)
				{
					ObstacleLfetRowStart = i;
					ObstacleLfetColStart = j + 1;
					//ObstacleLfetRowNum=0;
				}
			}


		}

		if (ObstacleLfetRowNum >= 4 && ObstacleLeftLFlag && ObstacleLeftFlag)//大于4行满足条件，认定这是左边有障碍
		{

			ObstacleLfetNum++;

			if (ObstacleLfetNum >= 3)
			{

				ObstacleLfetOkFlag = 1;
				ObstacleLfetNum = 0;
				ObstacleLfetRowNum = 0;
				gpio_set(PTB19, 1);

			}
		}
		else
		{
			// ObstacleLfetOkFlag=0;
			ObstacleLfetNum = 0;
			gpio_set(PTB19, 0);

		}


		//右边障碍 

		if (!ObstacleLfetOkFlag)  //如果没有找到左边障碍物，再寻找右边有没有障碍物
		{


			for (i = 58; i > 20; i--)
			{
				if (RightEdge[i] != 80)//如果当行的右边线存在
				{
					if (ABS(RightEdge[i + 1] - RightEdge[i]) >= 3)//右边界跳变
					{

						if (RightEdge[i + 1] != 80)
						{
							ScanColStartRight = RightEdge[i + 1];//用前一行作为扫描的起始列

						}

						else
						{

							ScanColStartRight = RightEdge[56];

						}

					}

					else
					{

						ScanColStartRight = RightEdge[i];

					}

				}

				else//如果当行左边线不存在
				{
					ScanColStartRight = RightEdge[i + 1];//用前一行作为扫描的起始列
				}


				ScanColEndRight = MiddleLine[i];

				for (j = ScanColStartRight; j > ScanColEndRight; j--)//往左边寻找障碍物
				{
					if (img[i][j] == White_Point && img[i][j - 1] == Black_Point)//找到了跳变
					{
						ObstacleRightFlag = 1;

						break;

					}

				}

				unsigned char jj = j - 1;

				if (ObstacleRightFlag)
				{

					while (jj > ScanColEndRight)
					{
						if (img[i][jj] == Black_Point && img[i][jj - 1] == White_Point)//找到又一个跳变
						{
							ObstacleRightRFlag = 1;

							break;//退出while
						}

						jj--;

					}


				}

				if (ObstacleRightFlag && ObstacleRightRFlag)
				{

					ObstacleRightRowNum++;

					if (ObstacleRightRowNum == 1)
					{
						ObstacleLfetRowStart = i;
						ObstacleLfetColStart = j + 1;
						//ObstacleLfetRowNum=0;
					}
				}


			}

			if (ObstacleRightRowNum >= 4 && ObstacleRightFlag && ObstacleRightRFlag)//大于4行满足条件，认定这是左边有障碍
			{

				ObstacleRightNum++;

				if (ObstacleRightNum >= 3)
				{

					ObstacleRightOkFlag = 1;
					ObstacleRightNum = 0;
					ObstacleRightRowNum = 0;
					gpio_set(PTB19, 1);

				}
			}
			else
			{
				// ObstacleLfetOkFlag=0;
				ObstacleRightNum = 0;
				gpio_set(PTB19, 0);

			}


		}


	}




	ObstacleRepair();

}



//障碍物边线处理

void ObstacleRepair()
{
	int i;
	static unsigned char ObstacleLfetControlDelay = 0;

	if (ObstacleLfetOkFlag && !ObstacleRightOkFlag)
	{
		for (i = 59; i >= LastLine; i--)
		{

			MiddleLine[i] = (int)(RightEdge[i] - Width[i] * 0.2);

		}

		ObstacleLfetControlDelay++;

		if (ObstacleLfetControlDelay >= 20)
		{
			ObstacleLfetControlDelay = 0;
			ObstacleLfetOkFlag = 0;
		}


	}

	if (ObstacleRightOkFlag && !ObstacleLfetOkFlag)
	{
		for (i = 59; i >= LastLine; i--)
		{

			MiddleLine[i] = (int)(LeftEdge[i] + Width[i] * 0.2);

		}

		ObstacleLfetControlDelay++;

		if (ObstacleLfetControlDelay >= 20)
		{
			ObstacleLfetControlDelay = 0;
			ObstacleRightOkFlag = 0;
		}


	}

}

#endif












