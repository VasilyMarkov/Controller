#include "app.h"

void app() {

    init_LWIP();

    while(1) {
        // process_LWIP();
        uint32_t regvalue = 0;
        HAL_ETH_ReadPHYRegister(getEthStruct(), 1, &regvalue);
        delay(500);
        printf("Test: %d\r\n", regvalue);
    };
}

