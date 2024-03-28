#include "app.h"

const uint16_t leds = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
const uint16_t leds_arr[4] = {GPIO_Pin_0, GPIO_Pin_7, GPIO_Pin_14};

void delay(uint32_t ms) {
    ms *= 3360;
    while(ms--) {
        __NOP();
    }
}

void loop() {
    static uint32_t counter = 0;

    ++counter;

    GPIO_ResetBits(GPIOB, leds);
    GPIO_SetBits(GPIOB, leds_arr[counter % 3]);

    delay(250);
}

void app() {
    
    while(1) {
        loop();
    };
}

