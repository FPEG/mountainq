/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */


#include "include.h"
#include "MT9V032.h"


void zPORTC_IRQHandler();
void zDMA0_IRQHandler();
void UART5_RX_TX_IRQHandler();


void ZZF_Init()
{
  set_vector_handler(UART5_RX_TX_VECTORn ,UART5_RX_TX_IRQHandler);    //把 uart3_handler 函数添加到中断向量表，不需要我们手动调用
  set_vector_handler(DMA0_VECTORn , zDMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler
  set_vector_handler(PORTC_VECTORn , zPORTC_IRQHandler);
  //port_init(PTC2, ALT1 | DMA_FALLING | PULLDOWN );         //PCLK，下降沿触发，下拉电阻
  //DMA PTXn触发源默认上升沿触发传输，若需修改，则初始化后调用 port_init 配置DMA 触发方式初始化后，需要调用 DMA_EN 来实现/****/来自dma.c
  //port_init(PTC7, ALT1 | IRQ_RISING  | PULLDOWN | PF);     //场中断，上升沿触发中断，下拉电阻，带滤波
  camera_init();
}

uint8 ffffff=0;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      PROTC中断执行函数
//  @return     void
//  @since      v1.0
//  Sample usage:               当A口启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void zPORTC_IRQHandler(void)
{
  //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
  uint8  n = 0;    //引脚号
  //PTA29
  n = 7;
  if(PORTC_ISFR & (1 << n))           //PTA6触发中断
  {
     PORTC_ISFR = (1 << n);        //写1清中断标志位


    //使用我们编写的宏定义清除发生中断的引脚
    //PORTA_FLAG_CLR(A1);
    VSYNC();
  }
}


void zDMA0_IRQHandler(void)
{

  DMA_IRQ_CLEAN(DMA_CH0);
  row_finished();
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART3中断执行函数
//  @return     void
//  @since      v1.0
//  Sample usage:               当UART3启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void UART5_RX_TX_IRQHandler(void)
{
  UARTn_e uratn = UART5;
  if(UART_S1_REG(UARTN[uratn]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
  {
    //用户需要处理接收数据
    mt9v032_cof_uart_interrupt();
  }
  if(UART_S1_REG(UARTN[uratn]) & UART_S1_TDRE_MASK )  //发送数据寄存器空
  {
    //用户需要处理发送数据
  }

}


uint8   image[ROW/2][COL/2];      //图像数组
uint8   image_buff[ROW][COL];      //图像数组
uint8   receive[3];
uint8   receive_num = 0;
uint8   uart_receive_flag = 1;

//需要配置到摄像头的数据
int16 MT9V032_CFG[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
                              //一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
    {EXP_TIME,          300}, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
    {FPS,               100},  //图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
    {SET_COL,           COL}, //图像列数量        范围1-752     K60采集不允许超过188
    {SET_ROW,           ROW}, //图像行数量        范围1-480
    {LR_OFFSET,         0},   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {UD_OFFSET,         0},   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {GAIN,              32},  //图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度


    {INIT,              0}    //摄像头开始初始化
};

//从摄像头内部获取到的配置数据
int16 GET_CFG[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},   //自动曝光设置
    {EXP_TIME,          0},   //曝光时间
    {FPS,               0},   //图像帧率
    {SET_COL,           0},   //图像列数量
    {SET_ROW,           0},   //图像行数量
    {LR_OFFSET,         0},   //图像左右偏移量
    {UD_OFFSET,         0},   //图像上下偏移量
    {GAIN,              0},   //图像增益
};


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头串口中断函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//  @note       该函数在ISR文件 串口3中断程序被调用
//-------------------------------------------------------------------------------------------------------------------
void mt9v032_cof_uart_interrupt(void)
{
    uart_getchar(MT9V032_COF_UART,&receive[receive_num]);;
    receive_num++;

    if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
    if(3 == receive_num)
    {
        receive_num = 0;
        uart_receive_flag = 1;
    }

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头内部配置信息
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart3
//-------------------------------------------------------------------------------------------------------------------
void get_config(void)
{
	uint16 temp, i;
    uint8  send_buffer[4];

    for(i=0; i<CONFIG_FINISH-1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = GET_STATUS;
        temp = GET_CFG[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;

        uart_putbuff(MT9V032_COF_UART,send_buffer,4);

        //等待接受回传数据
        while(!uart_receive_flag);
        uart_receive_flag = 0;

        GET_CFG[i][1] = receive[1]<<8 | receive[2];

    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头固件版本
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart3
//-------------------------------------------------------------------------------------------------------------------
uint16 get_version(void)
{
    uint16 temp;
    uint8  send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = GET_STATUS;
    temp = GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;

    uart_putbuff(MT9V032_COF_UART,send_buffer,4);

    //等待接受回传数据
    while(!uart_receive_flag);
    uart_receive_flag = 0;

    return ((uint16)(receive[1]<<8) | receive[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      单独设置摄像头曝光时间
//  @param      light   设置曝光时间越大图像越亮，摄像头收到后会根据分辨率及FPS计算最大曝光时间如果设置的数据过大，那么摄像头将会设置这个最大值
//  @return     uint16  当前曝光值，用于确认是否正确写入
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint16 set_exposure_time(uint16 light)
{
	uint16 temp;
    uint8  send_buffer[4];

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_EXP_TIME;
    temp = light;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;

    uart_putbuff(MT9V032_COF_UART,send_buffer,4);

    //等待接受回传数据
    while(!uart_receive_flag);
    uart_receive_flag = 0;

    temp = receive[1]<<8 | receive[2];
    return temp;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      对摄像头内部寄存器进行写操作
//  @param      addr    摄像头内部寄存器地址
//  @param      data    需要写入的数据
//  @return     uint16  寄存器当前数据，用于确认是否写入成功
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint16 set_mt9v032_reg(uint8 addr, uint16 data)
{
	uint16 temp;
    uint8  send_buffer[4];

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_ADDR;
    temp = addr;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;

    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    systick_delay_ms(10);

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_DATA;
    temp = data;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;

    uart_putbuff(MT9V032_COF_UART,send_buffer,4);

    //等待接受回传数据
    while(!uart_receive_flag);
    uart_receive_flag = 0;

    temp = receive[1]<<8 | receive[2];
    return temp;

}

uint16 version;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头初始化
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void camera_init(void)
{
    //设置参数    具体请参看使用手册
    uint16 temp, i;
    uint8  send_buffer[4];


    uart_init (MT9V032_COF_UART,9600);	//初始换串口 配置摄像头
    uart_rx_irq_en(MT9V032_COF_UART);


    //等待摄像头上电初始化成功
    systick_delay_ms(1000);
    uart_receive_flag = 0;

    //开始配置摄像头并重新初始化
    for(i=0; i<CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = MT9V032_CFG[i][0];
        temp = MT9V032_CFG[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;

        uart_putbuff(MT9V032_COF_UART,send_buffer,4);
        systick_delay_ms(2);
    }
    //等待摄像头初始化成功
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    while((0xff != receive[1]) || (0xff != receive[2]));
    //以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
    //利用set_exposure_time函数单独配置的曝光数据不存储在eeprom中


    //获取配置便于查看配置是否正确
    get_config();


    //摄像头采集初始化
    DisableInterrupts;
    //DMA通道0初始化，PTC18触发源(默认上升沿)，源地址为C_IN_DATA(1)(PTC8-PTC15)，目的地址为：image，每次传输1Byte 传输完毕保持目的地址
    dma_portx2buff_init(DMA_CH0, (void *)&MT9V032_DATAPORT, (void *)image_buff, PTC2, DMA_BYTE1, COL*ROW, DADDR_KEEPON);
    port_init(PTC2, ALT1 | DMA_RISING | PULLDOWN);  	//PCLK  触发源设置
    DMA_DIS(DMA_CH0);                                     		//禁用DMA通道
    DMA_IRQ_CLEAN(DMA_CH0);                               		//清除通道传输中断标志位
    DMA_IRQ_EN(DMA_CH0);                                  		//允许DMA通道中断
    DMA_EN(DMA_CH0);  							//使能DMA
    disable_irq(PORTC_IRQn);                     //关闭PTC的中断
    //port_init(C7, ALT1 | IRQ_FALLING | PULLDOWN);      			//行中断
    port_init(PTC7, ALT1 | IRQ_FALLING | PULLDOWN| PF);    //场中断，下降沿触发中断、下拉
    set_irq_priority(PORTC_IRQn,1);              // 中断优先级
    enable_irq (PORTC_IRQn);
    EnableInterrupts;
}



uint32  now_row;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头场中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void VSYNC(void)
{
  dma_repeat(DMA_CH0,(void *)&MT9V032_DATAPORT,(void *)image_buff[0],COL*ROW);
}


uint8   mt9v032_finish_flag = 0;      //一场图像采集完成标志位
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032摄像头DMA完成中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void row_finished(void)
{
  disable_irq(PORTC_IRQn);
  mt9v032_finish_flag = 1;
  //一副图像从采集开始到采集结束耗时3.33MS左右(50FPS、188*120分辨率)
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风摄像头图像发送至上位机查看图像
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart2
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_032(unsigned char row,unsigned char col)
{
       uart_putchar(UART2,0x00);
       uart_putchar(UART2,0xff);
       uart_putchar(UART2,0x01);
       uart_putchar(UART2,0x01);//发送命令
       uart_putbuff(UART2, (uint8_t *)image, row*col);  //发送图像

}
