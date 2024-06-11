#ifndef BSP_H
#define BSP_H

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_hal_eth.h"
#include "lwip/lwip.h"

typedef enum  {
  LINK_DOWN,
  LINK_UP,
  PACKET_RDY, 
  PACKET_NOT_RDY 
} status_t;

typedef struct {
  uint8_t link_status;
  uint8_t udp_packet_rdy;
} lwip_status_t;

void boardInit();
lwip_status_t* getLwipStatus();
USART_TypeDef* get_dbg_uart();
void uart_send_data(const uint8_t*, uint8_t);
#endif