#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"

const uint16_t LEDS = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
const uint16_t LED[4] = {GPIO_Pin_0, GPIO_Pin_7, GPIO_Pin_14};

void init();
void loop();

void delay();
