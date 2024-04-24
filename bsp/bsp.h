#ifndef BSP_H
#define BSP_H
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_exti.h"

#include "misc.h"

void board_init();
uint32_t getRegister();
void transmitPacket();
#endif
