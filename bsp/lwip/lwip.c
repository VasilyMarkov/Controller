#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/udp.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"

void Error_Handler(void);

struct udp_pcb *upcb;
ip_addr_t remote_ip;
u16_t remote_port;

struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
uint8_t IP_ADDRESS[4];
uint8_t NETMASK_ADDRESS[4];
uint8_t GATEWAY_ADDRESS[4];

void udpServer_init(void)
{
	// UDP Control Block structure
   err_t err;

   /* 1. Create a new UDP control block  */
   upcb = udp_new();
   ip_addr_t local_ip;
   u16_t local_port;
   local_port = 1234;
   /* 2. Bind the upcb to the local port */
   IP_ADDR4(&local_ip, 192, 168, 1, 66);

   err = udp_bind(upcb, &local_ip, local_port);  

   /* 3. Set a receive callback for the upcb */
   if(err == ERR_OK)
   {
	  IP4_ADDR(&remote_ip, 192, 168, 1, 111);
	  remote_port = 5678;

   }
   else
   {
	   udp_remove(upcb);
   }
}

void udp_send_data(const char *data, u16_t len)
{
  struct pbuf *p;
  err_t err;

  p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
  if (p != NULL)
  {
    memcpy(p->payload, data, len);
    err = udp_sendto(upcb, p, &remote_ip, remote_port);
    pbuf_free(p);
    if (err != ERR_OK)
    {

    }
  }
}

void lwipInit(void)
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
  
  /* add the network interface (IPv4/IPv6) without RTOS */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

  /* Registers the default network interface */
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

/**
 * ----------------------------------------------------------------------
 * Function given to help user to continue LwIP Initialization
 * Up to user to complete or change this function ...
 * Up to user to call this function in main.c in while (1) of main(void) 
 *-----------------------------------------------------------------------
 * Read a received packet from the Ethernet buffers 
 * Send it to the lwIP stack for handling
 * Handle timeouts if LWIP_TIMERS is set and without RTOS
 * Handle the llink status if LWIP_NETIF_LINK_CALLBACK is set and without RTOS 
 */
void lwipProcess(void)
{
  ethernetif_input(&gnetif);
  sys_check_timeouts();
}

