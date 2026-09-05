#include "packet.h"

#include <string.h>

void st_packet_init(
    STPacket* packet,
    STPacketType type
)
{
    if (packet == NULL)
        return;

    memset(packet, 0, sizeof(STPacket));

    packet->type = type;
}

int st_packet_set_data(
    STPacket* packet,
    const void* data,
    int size
)
{
    if (packet == NULL ||
        data == NULL ||
        size < 0 ||
        size > ST_PACKET_MAX_DATA)
    {
        return 0;
    }

    if (size > 0)
    {
        memcpy(
            packet->data,
            data,
            (size_t)size
        );
    }

    packet->size = size;

    return 1;
}

int st_packet_get_data(
    const STPacket* packet,
    void* data,
    int data_size
)
{
    if (packet == NULL ||
        data == NULL ||
        data_size < packet->size)
    {
        return 0;
    }

    if (packet->size > 0)
    {
        memcpy(
            data,
            packet->data,
            (size_t)packet->size
        );
    }

    return packet->size;
}

void st_packet_clear(STPacket* packet)
{
    if (packet == NULL)
        return;

    memset(packet, 0, sizeof(STPacket));
}

int st_packet_size(const STPacket* packet)
{
    if (packet == NULL)
        return 0;

    return packet->size;
}

STPacketType st_packet_type(
    const STPacket* packet
)
{
    if (packet == NULL)
        return ST_PACKET_NONE;

    return packet->type;
}

const char* st_packet_type_name(
    STPacketType type
)
{
    switch (type)
    {
        case ST_PACKET_NONE:
            return "NONE";

        case ST_PACKET_CONNECT:
            return "CONNECT";

        case ST_PACKET_DISCONNECT:
            return "DISCONNECT";

        case ST_PACKET_MESSAGE:
            return "MESSAGE";

        case ST_PACKET_PLAYER_JOIN:
            return "PLAYER_JOIN";

        case ST_PACKET_PLAYER_LEAVE:
            return "PLAYER_LEAVE";

        case ST_PACKET_PLAYER_UPDATE:
            return "PLAYER_UPDATE";

        case ST_PACKET_GAME_EVENT:
            return "GAME_EVENT";

        case ST_PACKET_PING:
            return "PING";

        case ST_PACKET_PONG:
            return "PONG";

        default:
            return "UNKNOWN";
    }
}