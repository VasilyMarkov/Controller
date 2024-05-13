#include "app.h"

void app() {

    init_LWIP();
    static uint32_t loop_cnt = 0;
    printf("FLASH->ACR: %d\r\n", FLASH->ACR);
    printf("RCC->CFGR: %d\r\n", RCC->CFGR);
    printf("RCC->CR): %d\r\n", RCC->CR);
    printf("RCC->PLLCFGR: %d\r\n", RCC->PLLCFGR);
    printf("RCC->APB1ENR: %d\r\n", RCC->APB1ENR);
    while(1) {
        process_LWIP();
        // if(loop_cnt == 10000) {
        //     GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
        //     loop_cnt = 0;
        // }
        // loop_cnt++;
        GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
        delay(1000);
    };
}

