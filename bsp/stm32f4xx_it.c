#include "stm32f4xx_it.h"
#include "lwip/ethernetif.h"

// extern ETH_HandleTypeDef heth;

static volatile uint32_t sys_tick = 0;

void SysTick_Handler(void)
{
  sys_tick++;
}

uint32_t getSysTick() {
  return sys_tick;
}

#define MAX_DELAY 0xFFFFFFFFU

void delay(uint32_t delay) {
    uint32_t tickStart = getSysTick();
    uint32_t wait  = delay;
    if (wait < MAX_DELAY) {
        wait++;
    }
    while((getSysTick() - tickStart) < wait) {}
}

void USART2_IRQHandler(void)
{

}

void ETH_IRQHandler(void)
{
  HAL_ETH_IRQHandler(getEthStruct());
}

