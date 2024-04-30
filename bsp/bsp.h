#ifndef BSP_H
#define BSP_H

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
void board_init();
uint32_t getRegister();
void transmitPacket();
uint32_t getSysTick();
void delay(uint32_t);

#endif
