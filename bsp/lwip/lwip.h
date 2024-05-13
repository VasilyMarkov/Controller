#ifndef __mx_lwip_H
#define __mx_lwip_H

#include "ethernetif.h"

void udpServer_init(void);
void udp_send_data(const char *data, u16_t len);
void MX_LWIP_Init(void);
void MX_LWIP_Process(void);

#endif /*__ mx_lwip_H */
