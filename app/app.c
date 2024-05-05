#include "app.h"

void app() {

    init_LWIP();

    while(1) {
        process_LWIP();
        delay(400);
    };
}

