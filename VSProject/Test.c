#include "include.h"
#include "Test.h"


#if  1//BuzzerOpen //��������˷������궨��

//��������ʼ������
void BuzzerInit()
{
       gpio_init (PTA9, GPO, 0);//��������ʼ��
}

void BuzzerTest(unsigned char flag)
{
       unsigned char Flag=0;
       Flag=flag;
       if(Flag)
       {
	      BuzzerRing;
       }
       else
       {
	      BuzzerQuiet;
       }
}

#endif


#if LedOpen

//Led��ʼ��
void LedInit()
{
   gpio_init (PTC14, GPO,0);//����
   gpio_init (PTE24, GPO,0);//����
   gpio_init (PTB22, GPO,0);//����
   gpio_init (PTA12, GPO,0);//����
}

//�����Ͻǵ�ledȥ���Ա�־λ
void  LeftUpLedTest(unsigned char flag)
{

 unsigned char Flag=0;
  Flag=0;
  Flag=flag;
  if(Flag)
  {
    LeftUpLedBright;
  }
  else
  {
    LeftUpLedDark;
  }
}

//�����½ǵ�ledȥ���Ա�־λ

void LeftUnderLedTest(unsigned char flag)
{

 unsigned char Flag=0;
  Flag=0;
  Flag=flag;
  if(Flag)
  {
    LeftUnderLedBright;
  }
  else
  {
    LeftUnderLedDark;
  }
}

//�����Ͻǵ�ledȥ���Ա�־λ
void RightUpLedTest(unsigned char flag)
{

 unsigned char Flag=0;
  Flag=0;
  Flag=flag;
  if(Flag)
  {
    RightUpLedBright;
  }
  else
  {
   RightUpLedDark;
  }
}

//�����½ǵ�ledȥ���Ա�־λ

void RightUnderLedTest(unsigned char flag)
{

 unsigned char Flag=0;
  Flag=0;
  Flag=flag;
  if(Flag)
  {
    RightUnderLedBright;
  }
  else
  {
    RightUnderLedDark;
  }
}

#endif