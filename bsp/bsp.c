#include "misc.h"
#include "bsp.h"
#include "lwip.h"

#define MAX_DELAY 0xFFFFFFFU
#define INTERVAL 500

static uint32_t EthStatus = 0;
static volatile uint32_t sysTick = 0;
static volatile uint32_t cnt = 0;

void SysTick_Handler(void)
{
    if (cnt == INTERVAL) {
        GPIO_ToggleBits(GPIOB, GPIO_Pin_0);
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

void TIM2_IRQHandler()
{
    GPIO_ToggleBits(GPIOB, GPIO_Pin_7);
    printf("Tim\r\n");
    printf("Tim\r\n");
    printf("Tim\r\n");
    printf("Tim\r\n");
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

void USART2_IRQHandler()
{

}  

void gpio_init() {

    const uint16_t led_pins = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;

    RCC_AHB1PeriphClockCmd(
        RCC_AHB1Periph_GPIOA | 
        RCC_AHB1Periph_GPIOB | 
        RCC_AHB1Periph_GPIOC | 
        RCC_AHB1Periph_GPIOD | 
        RCC_AHB1Periph_GPIOG, 
        ENABLE
    );
    GPIO_InitTypeDef gpio;

    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = led_pins;
    GPIO_Init(GPIOB, &gpio);
    GPIO_ResetBits(GPIOB, led_pins);
}

void usart_init() {

    GPIO_InitTypeDef gpio;

    const uint16_t usart_pins = GPIO_Pin_5 | GPIO_Pin_6;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Pin = usart_pins;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &gpio);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_InitTypeDef usart;
    usart.USART_BaudRate = 115200;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &usart);
    USART_Cmd(USART2, ENABLE);    
}

void tim_init() {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef tim2;

    tim2.TIM_Prescaler = 9000-1;
    tim2.TIM_CounterMode = TIM_CounterMode_Up;
    tim2.TIM_Period = 5000;
    tim2.TIM_ClockDivision = TIM_CKD_DIV1;
    
    TIM_TimeBaseInit(TIM2, &tim2);
    
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void dma_init() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
    DMA_InitTypeDef dma;
    dma.DMA_Channel = DMA_Channel_0;
    dma.DMA_DIR = DMA_DIR_MemoryToMemory;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Normal;
    dma.DMA_Priority = DMA_Priority_Low;
    dma.DMA_FIFOMode = DMA_FIFOMode_Enable;
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_Init(DMA2_Stream0, &dma);
}

void eth_init() {

    RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_ETHMACEN, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_ETHMACRXEN, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_ETHMACTXEN, ENABLE);

    GPIO_InitTypeDef gpio;

    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;

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

    NVIC_SetPriority(ETH_IRQn, 0);
    NVIC_EnableIRQ(ETH_IRQn);
}


void board_init() {
    uint32_t tick = SystemCoreClock/1000 - 1;
    SysTick_Config(tick);
    NVIC_EnableIRQ(SysTick_IRQn);
    __enable_irq();
    gpio_init();
    usart_init();
    tim_init();
    // dma_init();
    eth_init();
    init_LWIP();
    TIM_Cmd(TIM2, ENABLE);
    printf("Controller is started...\r\n");
}


