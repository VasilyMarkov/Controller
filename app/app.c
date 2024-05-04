#include "app.h"

void app() {

    init_LWIP();

    while(1) {
        process_LWIP();
        // GPIO_ToggleBits(GPIOB, GPIO_Pin_7);
        delay(400);
    };
}

