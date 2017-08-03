#ifndef __SERIAL2_H__
#define __SERIAL2_H__

#include "stm32f4xx.h"
#include "stdio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
#include "String.h"

#define SERIAL2_BUFFER_SIZE 10

//extern char GlobalStringBuffer[100];
//extern char Serial2_RxBuffer[SERIAL2_BUFFER_SIZE];
//extern u16 Serial2_RxReadPos;
//extern u16 Serial2_RxCounter;

void Serial2_Init(void);
void Serial2_NVIC_Config(void);

void USART2_IRQHandler(void);
void Serial2_Buffer_Clear(void);
int16_t Serial2_Buffer_Get(void);
uint16_t Serial2_Buffer_Count(void);


void Serial2_WriteByte(u8 chr);
void Serial2_WriteBytes(u8 *chr, u8 numBytes);
void Serial2_WriteWords(u16 *word, u16 numWords);
void Serial2_Print(char *chrBuf);
void Serial2_Println(char *chrBuf);

#endif /* __SERIAL_H__ */
