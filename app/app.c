#include "app.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include "simple.pb.h"

#define SER_BUF_SIZE 128
uint8_t ser_buffer[SER_BUF_SIZE];

uint8_t serialize(uint8_t* buffer, size_t* len) {
    size_t message_length = 0;
    bool status = 0;
    static uint32_t cnt = 0;
    SimpleMessage message = SimpleMessage_init_zero;
    
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    
    message.number = cnt++;

    status = pb_encode(&stream, SimpleMessage_fields, &message);
    *len = stream.bytes_written;

    if (!status)
    {
        printf("Encoding failed: %s\n", PB_GET_ERROR(&stream));
        return 1;
    }
        
    return 0;
}

void sendData(sd_callback_t send_data_callback) {
    if (get_dbg_uart() == NULL) 
        return;
    size_t ser_len = 0;
    serialize(ser_buffer, &ser_len);
    send_data_callback(ser_buffer, ser_len);
}