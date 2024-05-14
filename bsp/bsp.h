#ifndef BSP_H
#define BSP_H

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_flash.h"
#include "lwip/lwip.h"

typedef enum  {
  LINK_DOWN,
  LINK_UP 
} status_t;

typedef struct {
  uint8_t link_status;
} lwip_status_t;

void boardInit();
lwip_status_t* getLwipStatus();

#endif