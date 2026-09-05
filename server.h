#ifndef ST_SERVER_H
#define ST_SERVER_H

#include "network.h"

#define ST_SERVER_MAX_CLIENTS 32

typedef struct STServerClient
{
    STSocket socket;
    int active;
    int id;
} STServerClient;

typedef struct STServer
{
    STSocket socket;
    STServerClient clients[ST_SERVER_MAX_CLIENTS];

    unsigned short port;
    int running;
    int client_count;
} STServer;

int st_server_init(STServer* server, unsigned short port);
void st_server_shutdown(STServer* server);

int st_server_start(STServer* server);
void st_server_stop(STServer* server);

int st_server_accept(STServer* server);

int st_server_send(
    STServer* server,
    int client_id,
    const void* data,
    int size
);

int st_server_broadcast(
    STServer* server,
    const void* data,
    int size
);

int st_server_receive(
    STServer* server,
    int client_id,
    void* buffer,
    int buffer_size
);

void st_server_disconnect(
    STServer* server,
    int client_id
);

int st_server_is_running(
    const STServer* server
);

int st_server_client_count(
    const STServer* server
);

#endif