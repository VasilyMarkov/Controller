#include "app.h"



void delay(uint32_t ms) {
    ms *= 16800;
    while(ms--) {
        __NOP();
    }
}

void loop() {
    GPIO_ToggleBits(GPIOB, GPIO_Pin_7);
    
    delay(500);
}

void app() {
    while(1) {
        printf("Hello, world%.6f\r\n", 5.432);
        loop();
    };
}

