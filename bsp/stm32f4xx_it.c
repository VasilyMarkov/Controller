#include "stm32f4xx_it.h"

static volatile uint32_t sysTick = 0;

void TIM7_IRQHandler()
{
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update); 

    if(get_lwip_status()->link_status == LINK_UP) {
        GPIO_ToggleBits(GPIOB, GPIO_Pin_7); 
    }
    else {
        GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    }
}

void USART2_IRQHandler()
{

}  

void SysTick_Handler(void)
{
    static uint32_t cnt = 0;
    if (cnt == 500) {
        GPIO_ToggleBits(GPIOB, GPIO_Pin_0);
        cnt = 0;
    }
    sysTick++;
    cnt++;
}   

uint32_t getSysTick() {
    return sysTick;
}