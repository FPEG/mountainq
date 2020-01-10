#include "include.h"
#include "TFmini_Plus.h"

unsigned char ucRxBuffer[TFMINI_DATA_Len];
char ucRxCnt = 0;
T_TFmini TFmini_Plus ;




void TFmini_Plus_handler(void);


void TFmini_Plus_init()
{
       uart_init(TF_UART,115200);
       set_vector_handler(TF_UART_RX_TX_VECTORn,TFmini_Plus_handler);
       enable_irq(TF_UART_RX_TX_IRQn);
       uart_rx_irq_en(TF_UART);
}



/*******************************************
 TF�ж�
*******************************************/
void TFmini_Plus_handler(void)
{
       uart_getchar(TF_UART,&ucRxBuffer[ucRxCnt++]);
       if(ucRxCnt>1)
       {
	      if (ucRxBuffer[0]!=TFMINT_DATA_Head||ucRxBuffer[1] != TFMINT_DATA_Head) //�ж�����ͷ
	      {
		     ucRxCnt = 0;
		     return;
	      }
	      else
	      {
		     if (ucRxCnt < TFMINI_DATA_Len)
		     {
			    return;
		     }
		     else
		     {
			    memcpy(&TFmini_Plus, &ucRxBuffer[2],8);
			    ucRxCnt = 0;
		     }
	      }
       }

}

