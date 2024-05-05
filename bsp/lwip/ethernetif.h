#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

#include "lwip/err.h"
#include "lwip/netif.h"
#include "stm32f4xx_hal_eth.h"
#include "bsp.h"

err_t ethernetif_init(struct netif *netif);

void ethernetif_input(struct netif *netif);
void ethernetif_update_config(struct netif *netif);
void ethernetif_notify_conn_changed(struct netif *netif);
void check_link_status(struct netif* netif);
u32_t sys_jiffies(void);
u32_t sys_now(void);
ETH_HandleTypeDef* getEthStruct();
#endif

