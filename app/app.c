#include "app.h"

void delay(uint32_t ms) {
    ms *= 16800;
    while(ms--) {
        __NOP();
    }
}

void loop() {
    // GPIO_ToggleBits(GPIOB, GPIO_Pin_7);
    
    // delay(500);
}

void app() {
    while(1) {
        // printf("Hello, world%.2f\r\n", 2.71);
        RCC_ClocksTypeDef clk;
        RCC_GetClocksFreq(&clk);
        printf("Status: %.d\r\n", getRegister());
        // printf("VAL: %.d\r\n", SysTick->VAL);
        loop();
    };
}

