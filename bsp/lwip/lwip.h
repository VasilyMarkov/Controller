#ifndef __mx_lwip_H
#define __mx_lwip_H

#include "ethernetif.h"

#define UDP_RX_BUFFER_SIZE 128

typedef struct {
  size_t len;
  char buf[UDP_RX_BUFFER_SIZE];
} udp_receive_buffer_t;

void udpServer_init(void);
void udp_send_data(const char *data, u16_t len);
void lwipInit(void);
void lwipProcess(void);
udp_receive_buffer_t* getUdpReceiveBuffer();
#endif 
