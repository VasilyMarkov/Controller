#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
#include "bsp.h"

#define MAX_DELAY 0xFFFFFFFU
#define INTERVAL 500

static uint32_t EthStatus = 0;
static volatile uint32_t sysTick = 0;
static volatile uint32_t cnt = 0;

void SysTick_Handler(void)
{
    if (cnt == INTERVAL) {
        GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
        cnt = 0;
    }
    sysTick++;
    cnt++;
}   

uint32_t getSysTick() {
    return sysTick;
}

void delay(uint32_t delay) {
    uint32_t tickStart = getSysTick();
    uint32_t wait  = delay;
    if (wait < MAX_DELAY) {
        wait++;
    }
    while((getSysTick() - tickStart) < wait) {}
}

void USART2_IRQHandler()
{

}  

void gpio_init() {

    const uint16_t led_pins = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
    const uint16_t usart_pins = GPIO_Pin_5 | GPIO_Pin_6;

    RCC_AHB1PeriphClockCmd(
        RCC_AHB1Periph_GPIOA | 
        RCC_AHB1Periph_GPIOB | 
        RCC_AHB1Periph_GPIOC | 
        RCC_AHB1Periph_GPIOD | 
        RCC_AHB1Periph_GPIOG, 
        ENABLE
    );

    GPIO_InitTypeDef gpio;

/***************LED_CONF********************/
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = led_pins;
    GPIO_Init(GPIOB, &gpio);
    GPIO_ResetBits(GPIOB, led_pins);
/*******************************************/

/**************UART_CONF********************/
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Pin = usart_pins;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &gpio);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
/*******************************************/

/***************ETH_CONF********************/
    gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7; 
    GPIO_Init(GPIOA, &gpio);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH); //RMII ref clock
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH); //RMII MDIO
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH); //RMII RX Data Valid

    gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOC, &gpio);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH); //RMII MDC
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH); //RMII RXD0
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH); //RMII RXD1

    gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_13;
    GPIO_Init(GPIOG, &gpio);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH); //RMII TX enable
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH); //RMII TXD0

    gpio.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &gpio);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_ETH); //RMII TXD1
/*******************************************/

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

void eth_init() {

}

void board_init() {
    uint32_t tick = SystemCoreClock/1000;
    
    SysTick_Config(tick);
    NVIC_EnableIRQ(SysTick_IRQn);
    __enable_irq(); 
    gpio_init();
    usart_init();
    delay(50); //wait until periph init
    printf("Controller is started...\r\n");
}

// uint32_t getRegister() {
//     return EthStatus;
// }

