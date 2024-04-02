#include "bsp.h"


#define PLL_M      4
#define PLL_N      168

/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      7




void SysTick_Handler(void)
{
    //GPIO_ToggleBits(GPIOB, GPIO_Pin_0);
}   

void USART2_IRQHandler()
{

}  

void sysTick_init(void) {
    uint32_t tick = SystemCoreClock/2 - 1;
    SysTick->LOAD = tick; 
    SysTick->VAL = tick; 
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void systemClock_init(void) {
    RCC_DeInit(); // Reset RCC configuration to default values

    RCC_HSEConfig(RCC_HSE_ON); // Enable HSE (High Speed External) clock
    if (RCC_WaitForHSEStartUp() == SUCCESS) { // Wait for HSE clock to stabilize
        RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q); // Configure PLL
        RCC_PLLCmd(ENABLE); // Enable PLL
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); // Wait for PLL to stabilize

        
        FLASH->ACR = FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); // Set PLL as system clock
        while (RCC_GetSYSCLKSource() != RCC_CFGR_SWS_PLL); // Wait for PLL to become system clock
    }
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

void board_init() {

    SystemInit();
    //systemClock_init();
    sysTick_init();
    //__enable_irq(); 
    gpio_init();
    usart_init();

}


