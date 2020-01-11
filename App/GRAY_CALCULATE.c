#include "include.h"
#include "GRAY_CALCULATE.h"

unsigned char Threshold = 100;



/***************************************************************
��ֵ��

* �������ƣ�binarization_processing
* ����˵������ֵ��
* ����˵����
* �������أ�voiold
* �޸�ʱ�䣺2018��3��7��
* �� ע��
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
���  9ms ż�� 3ms

* �������ƣ�uint8 otsuThreshold(uint8 Image[ROW][COL], uint16 col, uint16 row)
* ����˵������ȡͼ��ĻҶ���Ϣ ȡ�����ֵ
* ����˵����
* �������أ�void uint8 threshold
* �޸�ʱ�䣺2018��3��7��
* �� ע��
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
	uint8* data = image;  //ָ���������ݵ�ָ��
	for (i = 0; i < GrayScale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	//ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			pixelCount[(int)data[i * width + j]]++;  //������ֵ��Ϊ����������±�
		}
	}

	//����ÿ������������ͼ���еı���
	float maxPro = 0.0;
	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;
		if (pixelPro[i] > maxPro)
		{
			maxPro = pixelPro[i];
		}
	}

	//�����Ҷȼ�[0,255]
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
	for (i = 0; i < GrayScale; i++)     // i��Ϊ��ֵ
	{
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
		for (j = 0; j < GrayScale; j++)
		{
			if (j <= i)   //��������
			{
				w0 += pixelPro[j];
				u0tmp += j * pixelPro[j];
			}
			else   //ǰ������
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
�򻯴��  1.01ms  ��������ʱ 1.5ms    ������Сʱ0.7ms

* �������ƣ�uint8 otsuThreshold(uint8 Image[ROW][COL], uint16 col, uint16 row)
* ����˵������ȡͼ��ĻҶ���Ϣ ȡ�����ֵ
* ����˵����
* �������أ�void uint8 threshold
* �޸�ʱ�䣺2018��3��7��
* �� ע��
***************************************************************/
unsigned char adapt_otsuThreshold(uint8 * image, uint16 col, uint16 row, unsigned char* threshold)   //ע�������ֵ��һ��Ҫ��ԭͼ��
{
#define GrayScale 256
	uint16 width = col;
	uint16 height = row;
	int pixelCount[GrayScale];
	float pixelPro[GrayScale];
	int i, j, pixelSum = width * height / 4;
	//uint8 threshold = 0;
	uint8* data = image;  //ָ���������ݵ�ָ��
	for (i = 0; i < GrayScale; i++)
	{
		pixelCount[i] = 0;
		pixelPro[i] = 0;
	}

	uint32 gray_sum = 0;
	//ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
	for (i = 0; i < height; i += 2)
	{
		for (j = 0; j < width; j += 2)
		{
			pixelCount[(int)data[i * width + j]]++;  //����ǰ�ĵ������ֵ��Ϊ����������±�
			gray_sum += (int)data[i * width + j];       //�Ҷ�ֵ�ܺ�
		}
	}

	//����ÿ������ֵ�ĵ�������ͼ���еı���

	for (i = 0; i < GrayScale; i++)
	{
		pixelPro[i] = (float)pixelCount[i] / pixelSum;

	}

	//�����Ҷȼ�[0,255]
	float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;



	w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
	for (j = 0; j < GrayScale; j++)
	{

		w0 += pixelPro[j];  //��������ÿ���Ҷ�ֵ�����ص���ռ����֮��   ���������ֵı���
		u0tmp += j * pixelPro[j];  //�������� ÿ���Ҷ�ֵ�ĵ�ı��� *�Ҷ�ֵ

		w1 = 1 - w0;
		u1tmp = gray_sum / pixelSum - u0tmp;

		u0 = u0tmp / w0;              //����ƽ���Ҷ�
		u1 = u1tmp / w1;              //ǰ��ƽ���Ҷ�
		u = u0tmp + u1tmp;            //ȫ��ƽ���Ҷ�
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
������    һ��900us

* �������ƣ�iteration
* ����˵������ȡͼ��ĻҶ���Ϣ ȡ�����ֵ
* ����˵����
* �������أ�void uint8 threshold
* �޸�ʱ�䣺2018��3��7��
* �� ע��
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

