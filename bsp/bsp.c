#include "misc.h"
#include "bsp.h"

#define kHz_1 1000

static lwip_status_t lwip_status = {.link_status = LINK_DOWN};

lwip_status_t* getLwipStatus() {
    return &lwip_status;
}

void sysInit() { 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    FLASH_PrefetchBufferCmd(ENABLE);
    SystemCoreClock = 180000000;
    uint32_t tick = SystemCoreClock/kHz_1 - 1;
    SysTick_Config(tick);
    NVIC_EnableIRQ(SysTick_IRQn);
}

void gpioInit() 
{
    RCC_AHB1PeriphClockCmd(
        RCC_AHB1Periph_GPIOA | 
        RCC_AHB1Periph_GPIOB | 
        RCC_AHB1Periph_GPIOC | 
        RCC_AHB1Periph_GPIOD | 
        RCC_AHB1Periph_GPIOG, 
        ENABLE
    );

    const uint16_t led_pins = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
    GPIO_InitTypeDef gpio;

    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = led_pins;
    GPIO_Init(GPIOB, &gpio);
    GPIO_ResetBits(GPIOB, led_pins);
}

void uartInit(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

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

  // NVIC_SetPriority(USART2_IRQn, 0);
  // NVIC_EnableIRQ(USART2_IRQn);

  USART_InitTypeDef usart;
  usart.USART_BaudRate = 115200;
  usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART2, &usart);
  USART_Cmd(USART2, ENABLE);  
}


void tim_init() {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    TIM_TimeBaseInitTypeDef tim7;

    TIM_TimeBaseStructInit(&tim7);

    tim7.TIM_Prescaler = 9000-1;
    tim7.TIM_CounterMode = TIM_CounterMode_Up;
    tim7.TIM_Period = 2000;
    tim7.TIM_ClockDivision = TIM_CKD_DIV1;
    
    TIM_TimeBaseInit(TIM7, &tim7);
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

    TIM_UpdateRequestConfig(TIM7, TIM_UpdateSource_Regular);

    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    NVIC_SetPriority(TIM7_IRQn, 0);
    NVIC_EnableIRQ(TIM7_IRQn);

    TIM_Cmd(TIM7, ENABLE);
}

void ethInit() {

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

void boardInit() {
    sysInit();
    gpioInit();
    uartInit();
    ethInit();
    tim_init();
    printf("Starting main loop..\r\n");
    lwipInit();
}