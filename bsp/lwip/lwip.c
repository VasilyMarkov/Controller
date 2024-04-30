#include "lwip.h"
#include "lwip/init.h"
// #include "lwip/netif.h"
// #include "lwip/opt.h"
// #include "lwip/mem.h"
// #include "lwip/memp.h"
// #include "netif/etharp.h"
// #include "lwip/timeouts.h"
// #include "ethernetif.h"

void Error_Handler(void);

// /* DHCP Variables initialization ---------------------------------------------*/
// uint32_t DHCPfineTimer = 0;
// uint32_t DHCPcoarseTimer = 0;

// struct netif gnetif;
// ip4_addr_t ipaddr;
// ip4_addr_t netmask;
// ip4_addr_t gw;


void init_LWIP(void)
{
  /* Initilialize the LwIP stack without RTOS */
  lwip_init();

  // /* IP addresses initialization with DHCP (IPv4) */
  // ipaddr.addr = 0;
  // netmask.addr = 0;
  // gw.addr = 0;

  // /* add the network interface (IPv4b/IPv6) without RTOS */
  // netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  // /* Registers the default network interface */
  // netif_set_default(&gnetif);

  // if (netif_is_link_up(&gnetif))
  // {
  //   /* When the netif is fully configured this function must be called */
  //   netif_set_up(&gnetif);
  // }
  // else
  // {
  //   /* When the netif link is down this function must be called */
  //   netif_set_down(&gnetif);
  // }

  /* Start DHCP negotiation for a network interface (IPv4) */
  //dhcp_start(&gnetif);


}

void process_LWIP(void)
{

  // ethernetif_input(&gnetif);
  
  // sys_check_timeouts();

}

