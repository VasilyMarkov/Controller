#include "bsp.h"
#include <stdio.h> 
// int _write(int fd, char* ptr, int len) {
//     int i= 0;
//     while(i < len) {
//         while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); 
//         USART_SendData(USART2, *ptr++);
//         i++;
//     }
    
//     return len;
// }

void SysTick_Handler(void)
{
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
}   

void USART2_IRQHandler()
{

}  

void sysTick_init(void) {
    uint32_t tick = 10000000;
    SysTick->LOAD = tick; 
    SysTick->VAL = 0; 
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void gpio_init() {

    const uint16_t led_pins = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
    const uint16_t usart_pins = GPIO_Pin_5 | GPIO_Pin_6;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    GPIO_InitTypeDef gpio;

    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = led_pins;
    GPIO_Init(GPIOB, &gpio);
    GPIO_ResetBits(GPIOB, led_pins);

    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Pin = usart_pins;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &gpio);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
}

void usart_init() {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_InitTypeDef usart;
    usart.USART_BaudRate = 115200;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &usart);
    USART_Cmd(USART2, ENABLE);
    //NVIC_EnableIRQ(USART2_IRQn);
}

void ethernet_init() {
    ETH_InitTypeDef ETH_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx | RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);
}

void board_init() {
    // uint32_t tick = 16777216;
    uint32_t tick = 0x1000000;
    
    SysTick_Config(tick);
    
    NVIC_EnableIRQ(SysTick_IRQn);
    __enable_irq(); 
    gpio_init();
    usart_init();
    // ETH_BSP_Config();
    printf("Controller is started...\n");
    // printf("clk: %.d\r\n", tick);

}


