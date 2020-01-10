#ifndef _FIT_CURVES_H_
#define _FIT_CURVES_H_

/*------------------------------�궨��ö��-----------------------------*/
#define TFMINI_DATA_Len  9
#define TFMINT_DATA_Head 0x59
#define	TF_UART			UART2
#define	TF_UART_RX_TX_VECTORn	UART2_RX_TX_VECTORn
#define TF_UART_RX_TX_IRQn	UART2_RX_TX_IRQn



/*----------------------------------------------------------------------*/

/*------------------------------����������------------------------------*/
typedef struct
{
       short Dist;
       short Strength;
       short temperature;
       unsigned char CheckSum;

}T_TFmini;

extern T_TFmini TFmini_Plus ;
/*----------------------------------------------------------------------*/

/*------------------------------����������------------------------------*/

extern void TFmini_Plus_init();

/*----------------------------------------------------------------------*/






#endif


