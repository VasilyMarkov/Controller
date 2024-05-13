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
#include "ring_buffer.h"

typedef enum  {
  LINK_DOWN,
  LINK_UP 
} status_t;

typedef struct {
  uint8_t link_status;
} lwip_status_t;

void board_init();
uint32_t getRegister();
uint32_t getSysTick();
void delay(uint32_t);
lwip_status_t* get_lwip_status();
ringbuf_uint8t* get_printf_buffer();
#endif
