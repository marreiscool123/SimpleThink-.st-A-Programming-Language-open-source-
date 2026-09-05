#ifndef ST_NETWORK_H
#define ST_NETWORK_H

#include <winsock2.h>

typedef struct STNetwork
{
    int initialized;
} STNetwork;

typedef struct STSocket
{
    SOCKET handle;
    int connected;
} STSocket;

int st_network_init(STNetwork* network);
void st_network_shutdown(STNetwork* network);

int st_network_is_initialized(const STNetwork* network);

int st_network_socket(STSocket* socket);
void st_network_close(STSocket* socket);

int st_network_connect(
    STSocket* socket,
    const char* address,
    unsigned short port
);

int st_network_bind(
    STSocket* socket,
    unsigned short port
);

int st_network_listen(
    STSocket* socket,
    int backlog
);

int st_network_accept(
    STSocket* server,
    STSocket* client
);

int st_network_send(
    STSocket* socket,
    const void* data,
    int size
);

int st_network_receive(
    STSocket* socket,
    void* buffer,
    int buffer_size
);

int st_network_is_connected(
    const STSocket* socket
);

#endif