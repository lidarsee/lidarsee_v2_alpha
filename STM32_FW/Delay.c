/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "Delay.h"

__IO uint32_t TimingDelay;
uint32_t MillisCounter = 0;

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
	if (TimingDelay) TimingDelay--;
	MillisCounter++;

}

void Delay_Ms(__IO uint32_t nTime)
{ 

}

uint32_t Millis(void)
{
    return MillisCounter;
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}