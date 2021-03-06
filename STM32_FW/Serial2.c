/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Serial2.h"

#define BUFFER_SIZE	100
uint8_t receive2Buffer[BUFFER_SIZE];
uint16_t buffer2WritePos = 0;
uint16_t buffer2ReadPos = 0;

void Serial2_Init(void)
{
	USART_InitTypeDef USART_InitStructure;

	Serial2_NVIC_Config();

	  /* USARTx configured as follow:
	        - BaudRate = 115200 baud
	        - Word Length = 8 Bits
	        - One Stop Bit
	        - No parity
	        - Hardware flow control disabled (RTS and CTS signals)
	        - Receive and transmit enabled
	  */
	  USART_InitStructure.USART_BaudRate = 115200;
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  USART_InitStructure.USART_Parity = USART_Parity_No;
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	  GPIO_InitTypeDef GPIO_InitStructure;

	  /* Enable GPIO clock */
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	  /* Enable UART clock */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	  /* Connect PXx to USARTx_Tx*/
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

	  /* Connect PXx to USARTx_Rx*/
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	  /* Configure USART Tx as alternate function  */
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* Configure USART Rx as alternate function  */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* USART configuration */
	  USART_Init(USART2, &USART_InitStructure);

	  /* Enable USART */
	  USART_Cmd(USART2, ENABLE);

	  /* Enable interrupt */
	  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);



	  Serial_Println("");
	  Serial_Println("");
}

void Serial2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USART2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  This function handles USART2 global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
  char ch;

   if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
   {
     ch = USART_ReceiveData(USART2);       // get received character
 	/*if ((bufferReadPos == bufferWritePos) && (bufferReadPos > (BUFFER_SIZE-100))) {
 		Serial_Buffer_Clear();

 	}
     if (bufferWritePos < BUFFER_SIZE) {
     	receiveBuffer[bufferWritePos] = ch;
     	bufferWritePos++;
     } else {
    	Serial_Buffer_Clear();
     	//while (1); // error - FOR DEBUG
     }*/
     if ((buffer2WritePos != (buffer2ReadPos-1))&&(buffer2WritePos < BUFFER_SIZE))
     {
      	receive2Buffer[buffer2WritePos++] = ch;
     }
   }
}

void Serial2_Buffer_Clear(void)
{
	uint16_t i;
	buffer2WritePos = 0;
	buffer2ReadPos = 0;
}

int16_t Serial2_Buffer_Get(void)
{
	if (buffer2ReadPos != buffer2WritePos)
	{
		return receive2Buffer[buffer2ReadPos++];
	} else {
		return -1;
	}
}

uint16_t Serial2_Buffer_Count(void)
{
	return (buffer2WritePos-buffer2ReadPos);
}


void Serial2_WriteByte(u8 chr)
{
	  USART_SendData(USART2, chr);

	  /* Loop until the end of transmission */
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}
}

void Serial2_WriteBytes(u8 *chr, u8 numBytes)
{
	u8 i;
	for (i = 0; i < numBytes; i++)
	{
		  USART_SendData(USART2, chr[i]);

		  /* Loop until the end of transmission */
		  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}
	}
}

void Serial2_WriteWords(u16 *word, u16 numWords)
{
	u16 i;
	for (i = 0; i < numWords; i++)
	{
		  USART_SendData(USART2, (word[i] & 0x00FF));

		  /* Loop until the end of transmission */
		  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}

		  USART_SendData(USART2, ((word[i] & 0xFF00) >> 8));

		  /* Loop until the end of transmission */
		  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}
	}
}

void Serial2_Print(char *chrBuf)
{
	u8 i = 0;
	while (chrBuf[i] != 0)
	{
		  USART_SendData(USART2, chrBuf[i]);

		  /* Loop until the end of transmission */
		  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}

		  i++;
	}
}

void Serial2_Println(char *chrBuf)
{
	u8 i = 0;
	while (chrBuf[i] != 0)
	{
		  USART_SendData(USART2, chrBuf[i]);
		  /* Loop until the end of transmission */
		  while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) {}

		  i++;
	}

	USART_SendData(USART2, 0x0A);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}

	USART_SendData(USART2, 0x0D);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}
}
