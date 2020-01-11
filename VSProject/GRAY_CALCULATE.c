#include "include.h"
#include "GRAY_CALCULATE.h"

unsigned char Threshold = 100;



/***************************************************************
二值化

* 函数名称：binarization_processing
* 功能说明：二值化
* 参数说明：
* 函数返回：voiold
* 修改时间：2018年3月7日
* 备 注：
***************************************************************/

void binarization_processing(unsigned char* p, uint16 row, uint16 col)
{
	for (int i = 0; i < row; i++)//100
	{
		for (int j = 0; j < col; j++)//150
		{
			if (*(p + i * col + j) >= Threshold)
			{
				*(p + i * col + j) = 255;
			}
			else
			{
				*(p + i * col + j) = Black;
			}
		}
	}
}



/***************************************************************
大津法  9ms 偶尔 3ms

* 函数名称：uint8 otsuThreshold(uint8 Image[ROW][COL], uint16 col, uint16 row)
* 功能说明：获取图像的灰度信息 取最佳阈值
* 参数说明：
* 函数返回：void uint8 threshold
* 修改时间：2018年3月7日
* 备 注：
***************************************************************/
uint8 otsuThreshold(uint8 * image, uint16 col, uint16 row)
{
#define GrayScale 256
	uint16 width = col;
	uint16 height = row;
	int pixelCount[GrayScale];
	float pixelPro[GrayScale];
	int i, j, pixelSum = width * height;
	uint8 threshold = 0;
	uint8* data = image;  //指向像素数据的指针
	for (i = 0; i < GrayScale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//统计灰度级中每个像素在整幅图像中的个数
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
		}
	}

	//计算每个像素在整幅图像中的比例
	float maxPro = 0.0;
	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
		if (pixelPro[i] > maxPro)
		{
			maxPro = pixelPro[i];
		}
	}

	//遍历灰度级[0,255]
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < GrayScale; i++)     // i作为阈值
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < GrayScale; j++)
		{
			if (j <= i)   //背景部分
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //前景部分
			{
				w1 += pixelPro[j];
				u1tmp += j * pixelPro[j];
			}
		}
		u0 = u0tmp / w0;
		u1 = u1tmp / w1;
		u = u0tmp + u1tmp;
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			threshold = i;
		}
	}

	return threshold;
}



/***************************************************************
简化大津法  1.01ms  计算量大时 1.5ms    计算量小时0.7ms

* 函数名称：uint8 otsuThreshold(uint8 Image[ROW][COL], uint16 col, uint16 row)
* 功能说明：获取图像的灰度信息 取最佳阈值
* 参数说明：
* 函数返回：void uint8 threshold
* 修改时间：2018年3月7日
* 备 注：
***************************************************************/
unsigned char adapt_otsuThreshold(uint8 * image, uint16 col, uint16 row, unsigned char* threshold)   //注意计算阈值的一定要是原图像
{
#define GrayScale 256
	uint16 width = col;
	uint16 height = row;
	int pixelCount[GrayScale];
	float pixelPro[GrayScale];
	int i, j, pixelSum = width * height / 4;
	//uint8 threshold = 0;
	uint8* data = image;  //指向像素数据的指针
	for (i = 0; i < GrayScale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	uint32 gray_sum = 0;
	//统计灰度级中每个像素在整幅图像中的个数
	for (i = 0; i < height; i += 2)
	{
		for (j = 0; j < width; j += 2)
		{
			pixelCount[(int)data[i * width + j]]++;  //将当前的点的像素值作为计数数组的下标
			gray_sum += (int)data[i * width + j];       //灰度值总和
		}
	}

	//计算每个像素值的点在整幅图像中的比例

	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;

	}

	//遍历灰度级[0,255]
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;



	w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
	for (j = 0; j < GrayScale; j++)
	{

		w0 += pixelPro[j];  //背景部分每个灰度值的像素点所占比例之和   即背景部分的比例
		u0tmp += j * pixelPro[j];  //背景部分 每个灰度值的点的比例 *灰度值

		w1 = 1 - w0;
		u1tmp = gray_sum / pixelSum - u0tmp;

		u0 = u0tmp / w0;              //背景平均灰度
		u1 = u1tmp / w1;              //前景平均灰度
		u = u0tmp + u1tmp;            //全局平均灰度
		deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
		if (deltaTmp > deltaMax)
		{
			deltaMax = deltaTmp;
			*threshold = j;
		}
		if (deltaTmp < deltaMax)
		{
			*threshold += 0;
			break;
		}
	}
	return *threshold;
}


/***************************************************************
迭代法    一次900us

* 函数名称：iteration
* 功能说明：获取图像的灰度信息 取最佳阈值
* 参数说明：
* 函数返回：void uint8 threshold
* 修改时间：2018年3月7日
* 备 注：
**************************************************************/
unsigned char iteration(unsigned char* p, unsigned char row, unsigned char col, unsigned char iteration_time, unsigned char* BlackThres)
{
	//unsigned char  BlackThres=100;
	uint16_t i = 0, j = 0, N0 = 0, N1 = 0;
	float T0, T1, T2, T_center;
	uint32_t S0 = 0, S1 = 0;
	T2 = *BlackThres;
	for (int k = 0; k < iteration_time; k++)
	{
		S0 = 0; N0 = 0; S1 = 0; N1 = 0;
		for (i = 0; i < row; i++)
		{
			for (j = 1; j < col; j++)
			{
				if (*(p + i * col + j) < T2)
				{
					S0 += *(p + i * col + j);
					N0++;
				}
				else
				{
					S1 += *(p + i * col + j);
					N1++;
				}
			}
		}
		T0 = S0 / N0;
		T1 = S1 / N1;
		T_center = (T0 + T1) / 2;
		T2 = T_center;
		*BlackThres = (unsigned char)T2;
	}
	return  0;
}

