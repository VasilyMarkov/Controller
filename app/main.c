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
    sendData(udp_send_data);
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
    delay(100);
  }

}

void SystemClock_Config()
{
  SystemCoreClock = 180000000;
}

