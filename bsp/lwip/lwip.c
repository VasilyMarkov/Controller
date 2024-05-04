#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/udp.h"
#include "netif/etharp.h"
#include "lwip/timeouts.h"
#include "ethernetif.h"

void Error_Handler(void);

struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

err_t udp_client_init();

void init_LWIP(void)
{
  IP_ADDRESS[0] = 192;
  IP_ADDRESS[1] = 168;
  IP_ADDRESS[2] = 1;
  IP_ADDRESS[3] = 66;
  NETMASK_ADDRESS[0] = 255;
  NETMASK_ADDRESS[1] = 255;
  NETMASK_ADDRESS[2] = 0;
  NETMASK_ADDRESS[3] = 0;
  GATEWAY_ADDRESS[0] = 0;
  GATEWAY_ADDRESS[1] = 0;
  GATEWAY_ADDRESS[2] = 0;
  GATEWAY_ADDRESS[3] = 0;

  /* Initilialize the LwIP stack without RTOS */
  lwip_init();

  IP4_ADDR(&ipaddr, IP_ADDRESS[0], IP_ADDRESS[1], IP_ADDRESS[2], IP_ADDRESS[3]);
  IP4_ADDR(&netmask, NETMASK_ADDRESS[0], NETMASK_ADDRESS[1] , NETMASK_ADDRESS[2], NETMASK_ADDRESS[3]);
  IP4_ADDR(&gw, GATEWAY_ADDRESS[0], GATEWAY_ADDRESS[1], GATEWAY_ADDRESS[2], GATEWAY_ADDRESS[3]);

  /* add the network interface (IPv4b/IPv6) without RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  // /* Registers the default network interface */
  netif_set_default(&gnetif);

  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called */
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

}

void process_LWIP()
{
  ethernetif_input(&gnetif);
  // sys_check_timeouts();
}

