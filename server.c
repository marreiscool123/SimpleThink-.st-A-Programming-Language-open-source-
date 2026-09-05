#include "server.h"

#include <string.h>

int st_server_init(
    STServer* server,
    unsigned short port
)
{
    int i;

    if (server == NULL)
        return 0;

    memset(server, 0, sizeof(STServer));

    server->port = port;
    server->running = 0;
    server->client_count = 0;

    server->socket.handle = INVALID_SOCKET;
    server->socket.connected = 0;

    for (i = 0; i < ST_SERVER_MAX_CLIENTS; i++)
    {
        server->clients[i].socket.handle =
            INVALID_SOCKET;

        server->clients[i].socket.connected = 0;
        server->clients[i].active = 0;
        server->clients[i].id = i;
    }

    return 1;
}

void st_server_shutdown(STServer* server)
{
    int i;

    if (server == NULL)
        return;

    st_server_stop(server);

    for (i = 0; i < ST_SERVER_MAX_CLIENTS; i++)
    {
        if (server->clients[i].active)
        {
            st_network_close(
                &server->clients[i].socket
            );

            server->clients[i].active = 0;
        }
    }

    server->client_count = 0;
}

int st_server_start(STServer* server)
{
    if (server == NULL)
        return 0;

    if (!st_network_socket(&server->socket))
        return 0;

    if (!st_network_bind(
        &server->socket,
        server->port
    ))
    {
        st_network_close(&server->socket);
        return 0;
    }

    if (!st_network_listen(
        &server->socket,
        ST_SERVER_MAX_CLIENTS
    ))
    {
        st_network_close(&server->socket);
        return 0;
    }

    server->running = 1;

    return 1;
}

void st_server_stop(STServer* server)
{
    if (server == NULL)
        return;

    if (server->socket.handle != INVALID_SOCKET)
    {
        st_network_close(&server->socket);
    }

    server->running = 0;
}

int st_server_accept(STServer* server)
{
    int i;

    if (server == NULL || !server->running)
        return -1;

    for (i = 0; i < ST_SERVER_MAX_CLIENTS; i++)
    {
        if (!server->clients[i].active)
        {
            if (st_network_accept(
                &server->socket,
                &server->clients[i].socket
            ))
            {
                server->clients[i].active = 1;
                server->clients[i].id = i;
                server->client_count++;

                return i;
            }

            return -1;
        }
    }

    return -1;
}

int st_server_send(
    STServer* server,
    int client_id,
    const void* data,
    int size
)
{
    if (server == NULL ||
        data == NULL ||
        client_id < 0 ||
        client_id >= ST_SERVER_MAX_CLIENTS)
    {
        return 0;
    }

    if (!server->clients[client_id].active)
        return 0;

    return st_network_send(
        &server->clients[client_id].socket,
        data,
        size
    );
}

int st_server_broadcast(
    STServer* server,
    const void* data,
    int size
)
{
    int i;
    int sent_count = 0;

    if (server == NULL || data == NULL || size <= 0)
        return 0;

    for (i = 0; i < ST_SERVER_MAX_CLIENTS; i++)
    {
        if (server->clients[i].active)
        {
            if (st_network_send(
                &server->clients[i].socket,
                data,
                size
            ) > 0)
            {
                sent_count++;
            }
        }
    }

    return sent_count;
}

int st_server_receive(
    STServer* server,
    int client_id,
    void* buffer,
    int buffer_size
)
{
    if (server == NULL ||
        buffer == NULL ||
        client_id < 0 ||
        client_id >= ST_SERVER_MAX_CLIENTS)
    {
        return 0;
    }

    if (!server->clients[client_id].active)
        return 0;

    return st_network_receive(
        &server->clients[client_id].socket,
        buffer,
        buffer_size
    );
}

void st_server_disconnect(
    STServer* server,
    int client_id
)
{
    if (server == NULL ||
        client_id < 0 ||
        client_id >= ST_SERVER_MAX_CLIENTS)
    {
        return;
    }

    if (!server->clients[client_id].active)
        return;

    st_network_close(
        &server->clients[client_id].socket
    );

    server->clients[client_id].active = 0;

    if (server->client_count > 0)
        server->client_count--;
}

int st_server_is_running(
    const STServer* server
)
{
    if (server == NULL)
        return 0;

    return server->running;
}

int st_server_client_count(
    const STServer* server
)
{
    if (server == NULL)
        return 0;

    return server->client_count;
}