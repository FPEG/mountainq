#ifndef __TEST_H__
#define __TEST_H__

#define BuzzerOpen  1 //�򿪷�����

#define LedOpen     1 //��Led

extern void BuzzerInit();
extern void LedInit();
extern void BuzzerTest(unsigned char flag);
extern void LeftUpLedTest(unsigned char flag);
extern void LeftUnderLedTest(unsigned char flag);
extern void RightUnderLedTest(unsigned char flag);
extern void RightUpLedTest(unsigned char flag);


#define  BuzzerRing           gpio_set   (PTA9, 1) //��������
#define  BuzzerQuiet          gpio_set   (PTA9, 0) //����������

#define  LeftUpLedBright      gpio_set   (PTC14, 1) //���ϵ�Led��
#define  LeftUpLedDark        gpio_set   (PTC14, 0) //���ϵ�Led��

#define  LeftUnderLedBright   gpio_set   (PTE24, 1) //���µ�Led��
#define  LeftUnderLedDark     gpio_set   (PTE24, 0) //���µ�Led��

#define  RightUnderLedBright  gpio_set   (PTA12, 1) //���µ�Led��
#define  RightUnderLedDark    gpio_set   (PTA12, 0) //���µ�Led��

#define  RightUpLedBright     gpio_set   (PTB22, 1) //���ϵ�Led��
#define  RightUpLedDark       gpio_set   (PTB22, 0) //���ϵ�Led��


#endif