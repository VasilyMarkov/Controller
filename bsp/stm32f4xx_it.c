#include "stm32f4xx_it.h"
#include "lwip/ethernetif.h"

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

void TIM7_IRQHandler()
{
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update); 

    if(getLwipStatus()->link_status == LINK_UP) {
        GPIO_ToggleBits(GPIOB, GPIO_Pin_7); 
    }
    else {
        GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    }
}

void USART2_IRQHandler(void)
{

}

void ETH_IRQHandler(void)
{
  HAL_ETH_IRQHandler(getEthStruct());
}

