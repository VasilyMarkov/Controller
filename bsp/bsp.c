#include "misc.h"
#include "bsp.h"

void gpio_init() 
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


void usart_init() 
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

void board_init() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    SystemInit();
    SystemCoreClock = 180000000;
    gpio_init();
    usart_init();
    printf("Starting main loop..\r\n");
    printf("SysClock: %d\r\n:",  SystemCoreClock);

    printf("FLASH->ACR: %d\r\n", FLASH->ACR);
    printf("RCC->CFGR :%d\r\n", RCC->CFGR);
    printf("RCC->CR :%d\r\n", RCC->CR);
    printf("RCC->PLLCFGR :%d\r\n", RCC->PLLCFGR);
    printf("RCC->APB1ENR :%d\r\n", RCC->APB1ENR);
    MX_LWIP_Init();
}