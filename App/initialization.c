#include "include.h"
#include "initialization.h"


void Threshold_get();
void IAB() ;

void initialization(void)
{
/************************ 配置 K60 的优先级  ***********************/
    NVIC_SetPriorityGrouping(3);            //设置优先级分组,4bit 抢占优先级,没有亚优先级

    NVIC_SetPriority(PORTC_IRQn,0);         //配置优先级
    NVIC_SetPriority(DMA0_IRQn,0);          //配置优先级
    NVIC_SetPriority(PIT0_IRQn,1);         //配置优先级
    NVIC_SetPriority(UART2_RX_TX_IRQn,2);          //配置优先级

    /***********************摄像头相关中断初始化****************************/
#if 1
       ZZF_Init();
       //Threshold_get();//采集一副图像测阈值

       /***********************摄像头相关中断初始化结束************************/
       /***********************关总中断****************************************/
       DisableInterrupts;
       /***********************电机初始化*******************************************/
       Engine_init();     //电机初始化
       /***********************舵机初始化*******************************************/
       S3010_init();      //舵机初始化
       /***********************电感采集初始化*******************************************/
       elec_init();
       adc_init(ADC0_SE16);//电池采集
       /***********************板载led初始化*******************************************/
       //led_init(LED0);
       led_init(LED_MAX);
	//遥控
       gpio_init (PTE0, GPI,0);    //初始化 PTE26管脚为输出
       gpio_init (PTE1, GPI,0);    //初始化 PTE26管脚为输出
       gpio_init (PTE2, GPI,0);    //初始化 PTE26管脚为输出
       gpio_init (PTE3, GPI,0);    //初始化 PTE26管脚为输出
	/***********************显示器初始化*******************************************/
       LCD_Init();        //OLED 屏幕初始化
       lcd_init();//tft初始化
       dsp_single_colour(BLACK); //tft清屏
	/***********************蜂鸣器初始化*******************************************/
	BuzzerInit();
	/***********************拨码开关*******************************************/
       dsw_init(DSW_MAX); //拨码开关
       /*************************按键初始化********************************************/
       key_init(KEY_MAX);
       /************************界面设置*********************************************/
       GUI();


       	/*圆环路宽补线*/
       for(int Round_row=ROW_END;Round_row>=ROW_START;Round_row--)
       {
	      Round.Track_Width_forRound[Round_row]=(int)(4.0+Round_row*1.85); //计算  圆环路宽
       }
	/*******权重******/
       for(int i=ROW_END;i>ROW_START;i--)
       {
	      //LineWeight[i]=-0.0125*(i-30.0)*(i-30.0)+5.0; //计算  权重
	      //if(LineWeight[i]<0)
		   //  LineWeight[i]=0;
       }
       /**///搬走

       /*************************测速相关初始化*******************************************/
       encoder_PIT_INIT();//编码器初始化
#endif
       /*************************TF测距初始化*******************************************/
       TFmini_Plus_init();

       IAB();//再次开始图像采集；
       //lptmr_delay_ms(2000);
       /*************************开总中断********************************************/
       EnableInterrupts;
}



void Threshold_get()
{
       while(!mt9v032_finish_flag);//等待图像采集
       //Threshold =otsuThreshold(image_buff[0],COL,ROW);
       //adapt_otsuThreshold(image_buff[0],160,80,&Threshold);  //自适应大津法
       //Threshold= yuzhiget();
       mt9v032_finish_flag = 0;
       //大津法取阈值
}

void IAB()//Image acquisition begins
{
       enable_irq(PORTA_IRQn);
}