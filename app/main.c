#include "../bsp/bsp.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include "simple.pb.h"

void serialzie() {
    /* This is the buffer where we will store our message. */
    uint8_t buffer[128];
    size_t message_length;
    bool status;
    
    /* Encode our message */
    {
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
    return 0;
}

int main(void)
{
  boardInit();
  udpServer_init();
  uint32_t cnt = 0;
  char numStr[10];
  
  while (1)
  {
    lwipProcess(); 
    const char *data = "Hello, world!";
    sprintf(numStr, "%d", cnt++);
    char* strCat = strcat(data, numStr);
    // udp_send_data(numStr, strlen(numStr));
    serialzie();
    // printf("Test\r\n");
    GPIO_ToggleBits(GPIOB, GPIO_Pin_14);
    delay(100);
  }

}

void SystemClock_Config()
{
  SystemCoreClock = 180000000;
}

