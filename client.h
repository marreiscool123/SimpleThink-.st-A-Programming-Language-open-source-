#ifndef ST_CLIENT_H
#define ST_CLIENT_H

#include "network.h"

typedef struct STClient
{
    STSocket socket;
    char address[64];
    unsigned short port;
    int connected;
} STClient;

int st_client_init(STClient* client);
void st_client_shutdown(STClient* client);

int st_client_connect(
    STClient* client,
    const char* address,
    unsigned short port
);

void st_client_disconnect(STClient* client);

int st_client_send(
    STClient* client,
    const void* data,
    int size
);

int st_client_receive(
    STClient* client,
    void* buffer,
    int buffer_size
);

int st_client_is_connected(
    const STClient* client
);

const char* st_client_address(
    const STClient* client
);

unsigned short st_client_port(
    const STClient* client
);

#endif