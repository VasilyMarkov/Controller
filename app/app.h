#ifndef __APP_H
#define __APP_H
#include "bsp.h"

typedef void (*sd_callback_t)(const uint8_t*, uint16_t);

void sendData(sd_callback_t);
uint8_t deserialize(uint8_t* buffer, size_t len);
#endif