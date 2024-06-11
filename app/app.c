#include "app.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include "simple.pb.h"
#include "quaternion.pb.h"

#define SER_BUF_SIZE 128
#define DESER_BUF_SIZE 128

typedef void (*sd_callback_t)(const uint8_t*, uint16_t);

uint8_t ser_buffer[SER_BUF_SIZE];
uint8_t des_buffer[SER_BUF_SIZE];

static Quaternion quaternion = {0,0,0,0};

uint8_t deserialize(uint8_t* buffer, size_t len) {
    bool status = 0;

    pb_istream_t stream = pb_istream_from_buffer(buffer, len);
    
    status = pb_decode(&stream, Quaternion_fields, &quaternion);
    
    if (!status)
    {
        printf("Decoding failed: %s\r\n", PB_GET_ERROR(&stream));
        return 1;
    }
    
}

uint8_t serialize(uint8_t* buffer, size_t* len) {
    bool status = 0;
    
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, &len);

    status = pb_encode(&stream, Quaternion_fields, &quaternion);
    
    *len = stream.bytes_written;

    if (!status)
    {
        printf("Encoding failed: %s\r\n", PB_GET_ERROR(&stream));
        return 1;
    }
        
    return 0;
}

void commandHandler() {

}

void sendData(sd_callback_t send_data_callback) {
    size_t ser_len = 0;
    serialize(ser_buffer, &ser_len);
    send_data_callback(ser_buffer, ser_len);
}

void app() {
    lwip_status_t* lwip_status = getLwipStatus();
    if (lwip_status->udp_packet_rdy == PACKET_RDY) {

        lwip_status->udp_packet_rdy = PACKET_NOT_RDY;
        udp_receive_buffer_t* udp_buffer = getUdpReceiveBuffer();
        deserialize(udp_buffer->buf, udp_buffer->len);
        sendData(udp_send_data);
        printf("Quaternion: w: %.2f, x: %.2f, y: %.2f, z: %.2f\r\n", 
            quaternion.w, 
            quaternion.x, 
            quaternion.y, 
            quaternion.z
        );

    }
}