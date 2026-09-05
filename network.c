#include "network.h"

#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int st_network_init(STNetwork* network)
{
    WSADATA data;

    if (network == NULL)
        return 0;

    if (WSAStartup(
        MAKEWORD(2, 2),
        &data
    ) != 0)
    {
        network->initialized = 0;
        return 0;
    }

    network->initialized = 1;

    return 1;
}

void st_network_shutdown(STNetwork* network)
{
    if (network == NULL)
        return;

    if (network->initialized)
    {
        WSACleanup();
        network->initialized = 0;
    }
}

int st_network_is_initialized(
    const STNetwork* network
)
{
    if (network == NULL)
        return 0;

    return network->initialized;
}

int st_network_socket(STSocket* st_socket)
{
    if (st_socket == NULL)
        return 0;

    st_socket->handle = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (st_socket->handle == INVALID_SOCKET)
    {
        st_socket->connected = 0;
        return 0;
    }

    st_socket->connected = 0;

    return 1;
}

void st_network_close(STSocket* socket)
{
    if (socket == NULL)
        return;

    if (socket->handle != INVALID_SOCKET)
    {
        closesocket(socket->handle);
        socket->handle = INVALID_SOCKET;
    }

    socket->connected = 0;
}

int st_network_connect(
    STSocket* socket,
    const char* address,
    unsigned short port
)
{
    struct sockaddr_in server_address;

    if (socket == NULL ||
        address == NULL ||
        socket->handle == INVALID_SOCKET)
    {
        return 0;
    }

    memset(
        &server_address,
        0,
        sizeof(server_address)
    );

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(
        AF_INET,
        address,
        &server_address.sin_addr
    ) != 1)
    {
        return 0;
    }

    if (connect(
        socket->handle,
        (struct sockaddr*)&server_address,
        sizeof(server_address)
    ) == SOCKET_ERROR)
    {
        return 0;
    }

    socket->connected = 1;

    return 1;
}

int st_network_bind(
    STSocket* socket,
    unsigned short port
)
{
    struct sockaddr_in address;

    if (socket == NULL ||
        socket->handle == INVALID_SOCKET)
    {
        return 0;
    }

    memset(
        &address,
        0,
        sizeof(address)
    );

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (bind(
        socket->handle,
        (struct sockaddr*)&address,
        sizeof(address)
    ) == SOCKET_ERROR)
    {
        return 0;
    }

    return 1;
}

int st_network_listen(
    STSocket* socket,
    int backlog
)
{
    if (socket == NULL ||
        socket->handle == INVALID_SOCKET)
    {
        return 0;
    }

    if (backlog <= 0)
        backlog = 5;

    if (listen(
        socket->handle,
        backlog
    ) == SOCKET_ERROR)
    {
        return 0;
    }

    return 1;
}

int st_network_accept(
    STSocket* server,
    STSocket* client
)
{
    SOCKET client_socket;

    if (server == NULL ||
        client == NULL ||
        server->handle == INVALID_SOCKET)
    {
        return 0;
    }

    client_socket = accept(
        server->handle,
        NULL,
        NULL
    );

    if (client_socket == INVALID_SOCKET)
    {
        return 0;
    }

    client->handle = client_socket;
    client->connected = 1;

    return 1;
}

int st_network_send(
    STSocket* socket,
    const void* data,
    int size
)
{
    int sent;
    int total;

    if (socket == NULL ||
        data == NULL ||
        size <= 0 ||
        !socket->connected)
    {
        return 0;
    }

    total = 0;

    while (total < size)
    {
        sent = send(
            socket->handle,
            (const char*)data + total,
            size - total,
            0
        );

        if (sent == SOCKET_ERROR ||
            sent == 0)
        {
            socket->connected = 0;
            return 0;
        }

        total += sent;
    }

    return total;
}

int st_network_receive(
    STSocket* socket,
    void* buffer,
    int buffer_size
)
{
    int received;

    if (socket == NULL ||
        buffer == NULL ||
        buffer_size <= 0 ||
        !socket->connected)
    {
        return 0;
    }

    received = recv(
        socket->handle,
        (char*)buffer,
        buffer_size,
        0
    );

    if (received == 0)
    {
        socket->connected = 0;
        return 0;
    }

    if (received == SOCKET_ERROR)
    {
        return 0;
    }

    return received;
}

int st_network_is_connected(
    const STSocket* socket
)
{
    if (socket == NULL)
        return 0;

    return socket->connected;
}