#include "stm32f4xx_it.h"
#include "ring_buffer.h"

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
    ringbuf_uint8t* printf_buffer =  get_printf_buffer();
    while(!rb_is_empty(printf_buffer)) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
        USART_SendData(USART2, (uint16_t)(rb_get(printf_buffer)));
    }
    USART1->CR1 &= ~USART_CR1_TXEIE;
    GPIO_ToggleBits(GPIOB, GPIO_Pin_7); 
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