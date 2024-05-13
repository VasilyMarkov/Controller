#include "../bsp/bsp.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include "simple.pb.h"

void MX_GPIO_Init(void)
{
    RCC_AHB1PeriphClockCmd(
        RCC_AHB1Periph_GPIOA | 
        RCC_AHB1Periph_GPIOB | 
        RCC_AHB1Periph_GPIOC | 
        RCC_AHB1Periph_GPIOD | 
        RCC_AHB1Periph_GPIOG, 
        ENABLE
    );

    const uint16_t led_pins = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
    GPIO_InitTypeDef gpio;

    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = led_pins;
    GPIO_Init(GPIOB, &gpio);
    GPIO_ResetBits(GPIOB, led_pins);
}

void MX_USART2_UART_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  GPIO_InitTypeDef gpio;

  const uint16_t usart_pins = GPIO_Pin_5 | GPIO_Pin_6;
  gpio.GPIO_Mode = GPIO_Mode_AF;
  gpio.GPIO_Pin = usart_pins;
  gpio.GPIO_Speed = GPIO_Speed_50MHz;
  gpio.GPIO_OType = GPIO_OType_PP;
  gpio.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &gpio);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

  // NVIC_SetPriority(USART2_IRQn, 0);
  // NVIC_EnableIRQ(USART2_IRQn);

  USART_InitTypeDef usart;
  usart.USART_BaudRate = 115200;
  usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_Init(USART2, &usart);
  USART_Cmd(USART2, ENABLE);  
}

void SystemClock_Config();

void eth_init() {

    RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_ETHMACEN, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_ETHMACRXEN, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_ETHMACTXEN, ENABLE);

    GPIO_InitTypeDef gpio;

    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;

    gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7; 
    GPIO_Init(GPIOA, &gpio);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH); //RMII ref clock
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH); //RMII MDIO
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH); //RMII RX Data Valid
    
    gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_Init(GPIOC, &gpio);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH); //RMII MDC
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH); //RMII RXD0
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH); //RMII RXD1

    gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_13;
    GPIO_Init(GPIOG, &gpio);
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH); //RMII TX enable
    GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH); //RMII TXD0

    gpio.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &gpio);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_ETH); //RMII TXD1

    NVIC_SetPriority(ETH_IRQn, 0);
    NVIC_EnableIRQ(ETH_IRQn);
}

void serialzie() {
    /* This is the buffer where we will store our message. */
    uint8_t buffer[128];
    size_t message_length;
    bool status;
    
    /* Encode our message */
    {
        /* Allocate space on the stack to store the message data.
         *
         * Nanopb generates simple struct definitions for all the messages.
         * - check out the contents of simple.pb.h!
         * It is a good idea to always initialize your structures
         * so that you do not have garbage data from RAM in there.
         */
        SimpleMessage message = SimpleMessage_init_zero;
        
        /* Create a stream that will write to our buffer. */
        pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
        
        /* Fill in the lucky number */
        message.number = 42;
        /* Now we are ready to encode the message! */
        status = pb_encode(&stream, SimpleMessage_fields, &message);
        message_length = stream.bytes_written;
        udp_send_data(buffer, message_length);
        /* Then just check for any errors.. */
        if (!status)
        {
            printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
            return 1;
        }
    }
    
    /* Now we could transmit the message over network, store it in a file or
     * wrap it to a pigeon's leg.
     */

    /* But because we are lazy, we will just decode it immediately. */
    
    // {
    //     /* Allocate space for the decoded message. */
    //     SimpleMessage message = SimpleMessage_init_zero;
        
    //     /* Create a stream that reads from the buffer. */
    //     pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);
        
    //     /* Now we are ready to decode the message. */
    //     status = pb_decode(&stream, SimpleMessage_fields, &message);
        
    //     /* Check for errors... */
    //     if (!status)
    //     {
    //         printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
    //         return 1;
    //     }
        
    //     /* Print the data contained in the message. */
    //     // printf("Project: %s\n", message.project);
    //     // printf("Number: %d\n", message.number);
    // }
    
    return 0;
}

int main(void)
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  SystemClock_Config();
  uint32_t tick = SystemCoreClock/1000 - 1;
  SysTick_Config(tick);
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  eth_init();
  MX_LWIP_Init();

  printf("Starting main loop..\r\n");
  printf("SysClock: %d\r\n:",  SystemCoreClock);
  
  printf("FLASH->ACR: %d\r\n", FLASH->ACR);
  printf("RCC->CFGR :%d\r\n", RCC->CFGR);
  printf("RCC->CR :%d\r\n", RCC->CR);
  printf("RCC->PLLCFGR :%d\r\n", RCC->PLLCFGR);
  printf("RCC->APB1ENR :%d\r\n", RCC->APB1ENR);
  udpServer_init();
  // serialzie();
  while (1)
  {
    MX_LWIP_Process(); // poll for ethernet rx and timer operations.
    const char *data = "Hello, world!";
    udp_send_data(data, strlen(data));
    // serialzie();
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
    delay(500);
  }

}

void SystemClock_Config()
{
  SystemCoreClock = 180000000;
}

