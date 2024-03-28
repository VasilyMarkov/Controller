#include "bsp.h"

const uint16_t LEDS = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
const uint16_t LED[4] = {GPIO_Pin_0, GPIO_Pin_7, GPIO_Pin_14};

void board_init() {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef gpio;
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = LEDS;
    GPIO_Init(GPIOB, &gpio);

    GPIO_SetBits(GPIOB, LEDS);
}


