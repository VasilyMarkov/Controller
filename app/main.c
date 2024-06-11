#include "../bsp/bsp.h"
#include "app.h"

int main(void)
{
  boardInit();
  udpServer_init();
  uint32_t cnt = 0;
  char numStr[10];
  
  while (1)
  {
    lwipProcess(); 
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
    if (getLwipStatus()->udp_packet_rdy == PACKET_RDY) {
      getLwipStatus()->udp_packet_rdy = PACKET_NOT_RDY;
      udp_receive_buffer_t* udp_buffer = getUdpReceiveBuffer();
      deserialize(udp_buffer->buf, udp_buffer->len);
    }
  }

}

void SystemClock_Config()
{
  SystemCoreClock = 180000000;
}

