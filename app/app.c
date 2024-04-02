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
    volatile uint8_t sendData[8];
    volatile uint8_t bytesToSend = 8;
    // Счетчик отправленных байт
    volatile uint8_t sendDataCounter = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        sendData[i] = i;
    }
    // USART_ITConfig(USART2, USART_IT_TC, ENABLE);
    while(1) {
        USART_SendData(USART2, sendData[sendDataCounter++ % 8]);
        loop();
    };
}

