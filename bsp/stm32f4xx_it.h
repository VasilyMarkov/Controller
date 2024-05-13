#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H
#include "stdint.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void USART2_IRQHandler(void);
void ETH_IRQHandler(void);
uint32_t getSysTick();
void delay(uint32_t);


#endif /* __STM32F4xx_IT_H */

