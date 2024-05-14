#ifndef __mx_lwip_H
#define __mx_lwip_H

#include "ethernetif.h"

void udpServer_init(void);
void udp_send_data(const char *data, u16_t len);
void lwipInit(void);
void lwipProcess(void);

#endif 
