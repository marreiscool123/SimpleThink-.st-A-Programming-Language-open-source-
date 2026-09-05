#ifndef ST_PACKET_H
#define ST_PACKET_H

#define ST_PACKET_MAX_DATA 1024

typedef enum STPacketType
{
    ST_PACKET_NONE = 0,
    ST_PACKET_CONNECT,
    ST_PACKET_DISCONNECT,
    ST_PACKET_MESSAGE,
    ST_PACKET_PLAYER_JOIN,
    ST_PACKET_PLAYER_LEAVE,
    ST_PACKET_PLAYER_UPDATE,
    ST_PACKET_GAME_EVENT,
    ST_PACKET_PING,
    ST_PACKET_PONG
} STPacketType;

typedef struct STPacket
{
    STPacketType type;
    int size;
    unsigned char data[ST_PACKET_MAX_DATA];
} STPacket;

void st_packet_init(
    STPacket* packet,
    STPacketType type
);

int st_packet_set_data(
    STPacket* packet,
    const void* data,
    int size
);

int st_packet_get_data(
    const STPacket* packet,
    void* data,
    int data_size
);

void st_packet_clear(STPacket* packet);

int st_packet_size(const STPacket* packet);

STPacketType st_packet_type(
    const STPacket* packet
);

const char* st_packet_type_name(
    STPacketType type
);

#endif