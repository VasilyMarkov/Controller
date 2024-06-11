#include "../bsp/bsp.h"
#include "app.h"

int main(void)
{
  boardInit();
  udpServer_init();
  while (1)
  {
    lwipProcess(); 
    app();
  }

}

void SystemClock_Config()
{
  SystemCoreClock = 180000000;
}

