#include <stdio.h> 
#include "bsp.h"
#include "stm32f4x7_eth.h"

static uint32_t EthStatus = 0;

void SysTick_Handler(void)
{
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
}   

void USART2_IRQHandler()
{

}  

void sysTick_init(void) {
    uint32_t tick = 10000000;
    SysTick->LOAD = tick; 
    SysTick->VAL = 0; 
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void gpio_init() {

    const uint16_t led_pins = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
    const uint16_t usart_pins = GPIO_Pin_5 | GPIO_Pin_6;

    RCC_AHB1PeriphClockCmd(
        RCC_AHB1Periph_GPIOA | 
        RCC_AHB1Periph_GPIOB | 
        RCC_AHB1Periph_GPIOC | 
        RCC_AHB1Periph_GPIOD | 
        RCC_AHB1Periph_GPIOG, 
        ENABLE
    );

    GPIO_InitTypeDef gpio;

/***************LED_CONF********************/
    GPIO_StructInit(&gpio);
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_Pin = led_pins;
    GPIO_Init(GPIOB, &gpio);
    GPIO_ResetBits(GPIOB, led_pins);
/*******************************************/

/**************UART_CONF********************/
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_Pin = usart_pins;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &gpio);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
/*******************************************/

/***************ETH_CONF********************/
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
/*******************************************/

}

void usart_init() {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    USART_InitTypeDef usart;
    usart.USART_BaudRate = 115200;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &usart);
    USART_Cmd(USART2, ENABLE);
    //NVIC_EnableIRQ(USART2_IRQn);
}

const uint8_t MAC_ADDR[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
const uint8_t PHY_ADDR = 0x00;

#define IP_ADDR0 192
#define IP_ADDR1 168
#define IP_ADDR2 0
#define IP_ADDR3 5

#define NETMASK_ADDR0 255
#define NETMASK_ADDR1 255
#define NETMASK_ADDR2 255
#define NETMASK_ADDR3 0

#define GW_ADDR0 192
#define GW_ADDR1 168
#define GW_ADDR2 0
#define GW_ADDR3 1

static ETH_InitTypeDef ETH_InitStructure;
static ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB], DMATxDscrTab[ETH_TXBUFNB];
static u8 Rx_Buff[ETH_RXBUFNB][ETH_MAX_PACKET_SIZE], Tx_Buff[ETH_TXBUFNB][ETH_MAX_PACKET_SIZE]; 

void eth_init() {
    ETH_DeInit();
    ETH_SoftwareReset();
    while(ETH_GetSoftwareResetStatus()==SET);

    ETH_StructInit(&ETH_InitStructure);
    /*------------------------   MAC   -----------------------------------*/
    ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable ; 
    ETH_InitStructure.ETH_Speed = ETH_Speed_100M;                                     
    ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
    ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;
    ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
    ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable; 
    ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Enable;
    ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Disable;     
    ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
    ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
    ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
    /* Configure ETHERNET */
    uint32_t value = ETH_Init(&ETH_InitStructure, 0);
    /* Initialize Tx Descriptors list: Chain Mode */
    ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
    /* Initialize Rx Descriptors list: Chain Mode  */
    ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

    //Разрешаем прием
    DMARxDscrTab -> Status = ETH_DMARxDesc_OWN;

    // ETH_MACAddressConfig(ETH_MAC_Address1, MAC_ADDR);
    // ETH_MACAddressFilterConfig(ETH_MAC_Address1,ETH_MAC_AddressFilter_SA);
    // ETH_MACAddressPerfectFilterCmd(ETH_MAC_Address1, ENABLE);

    /* Enable MAC and DMA transmission and reception */
    ETH_Start();  
}

void board_init() {
    uint32_t tick = 0x1000000;
    
    SysTick_Config(tick);
    
    NVIC_EnableIRQ(SysTick_IRQn);
    __enable_irq(); 
    gpio_init();
    usart_init();
    eth_init();
    printf("Controller is started...\n");
    // printf("clk: %.d\r\n", tick);

}

uint32_t getRegister() {
    return EthStatus;
}

void transmitPacket() {
    if ((DMATxDscrTab->Status & ETH_DMARxDesc_OWN)==0){   
        GPIO_ToggleBits(GPIOB, GPIO_Pin_7);

        //Сначала отключаем передачу
        ETH_DMATransmissionCmd(DISABLE);

        Tx_Buff[0][20] = 0x01;
        Tx_Buff[0][21] = 0x02;

        //отдаем дескриптор в руки DMA Ethernet
        DMATxDscrTab -> Status = ETH_DMARxDesc_OWN | ETH_DMATxDesc_TCH | ETH_DMATxDesc_TTSE |
                        ETH_DMATxDesc_LS | ETH_DMATxDesc_FS;
                    
                        //разрешаем отправку
        ETH_DMATransmissionCmd(ENABLE);
    }
}