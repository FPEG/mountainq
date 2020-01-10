#ifndef _FIT_CURVES_H_
#define _FIT_CURVES_H_

/*------------------------------宏定义枚举-----------------------------*/
#define TFMINI_DATA_Len  9
#define TFMINT_DATA_Head 0x59
#define	TF_UART			UART2
#define	TF_UART_RX_TX_VECTORn	UART2_RX_TX_VECTORn
#define TF_UART_RX_TX_IRQn	UART2_RX_TX_IRQn



/*----------------------------------------------------------------------*/

/*------------------------------变量声明区------------------------------*/
typedef struct
{
       short Dist;
       short Strength;
       short temperature;
       unsigned char CheckSum;

}T_TFmini;

extern T_TFmini TFmini_Plus ;
/*----------------------------------------------------------------------*/

/*------------------------------函数声明区------------------------------*/

extern void TFmini_Plus_init();

/*----------------------------------------------------------------------*/






#endif


