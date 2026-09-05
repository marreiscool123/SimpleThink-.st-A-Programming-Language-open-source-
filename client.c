#include "client.h"

#include <string.h>

int st_client_init(STClient* client)
{
    if (client == NULL)
        return 0;

    memset(client, 0, sizeof(STClient));

    client->socket.handle = INVALID_SOCKET;
    client->socket.connected = 0;
    client->connected = 0;

    return 1;
}

void st_client_shutdown(STClient* client)
{
    if (client == NULL)
        return;

    st_client_disconnect(client);

    memset(client, 0, sizeof(STClient));

    client->socket.handle = INVALID_SOCKET;
}

int st_client_connect(
    STClient* client,
    const char* address,
    unsigned short port
)
{
    if (client == NULL || address == NULL)
        return 0;

    if (client->connected)
        st_client_disconnect(client);

    if (!st_network_socket(&client->socket))
        return 0;

    if (!st_network_connect(
        &client->socket,
        address,
        port
    ))
    {
        st_network_close(&client->socket);
        return 0;
    }

    strncpy(
        client->address,
        address,
        sizeof(client->address) - 1
    );

    client->address[sizeof(client->address) - 1] = '\0';

    client->port = port;
    client->connected = 1;

    return 1;
}

void st_client_disconnect(STClient* client)
{
    if (client == NULL)
        return;

    if (client->socket.handle != INVALID_SOCKET)
    {
        st_network_close(&client->socket);
    }

    client->connected = 0;
}

int st_client_send(
    STClient* client,
    const void* data,
    int size
)
{
    if (client == NULL ||
        data == NULL ||
        size <= 0 ||
        !client->connected)
    {
        return 0;
    }

    return st_network_send(
        &client->socket,
        data,
        size
    );
}

int st_client_receive(
    STClient* client,
    void* buffer,
    int buffer_size
)
{
    if (client == NULL ||
        buffer == NULL ||
        buffer_size <= 0 ||
        !client->connected)
    {
        return 0;
    }

    return st_network_receive(
        &client->socket,
        buffer,
        buffer_size
    );
}

int st_client_is_connected(
    const STClient* client
)
{
    if (client == NULL)
        return 0;

    return client->connected;
}

const char* st_client_address(
    const STClient* client
)
{
    if (client == NULL)
        return NULL;

    return client->address;
}

unsigned short st_client_port(
    const STClient* client
)
{
    if (client == NULL)
        return 0;

    return client->port;
}