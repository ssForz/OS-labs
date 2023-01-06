#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "interface.h"
#include "tree.h"

#define SERVER_PATH "./server"

void create_server_node(int id);

void send_create(void* socket, int id);

void send_exec(void* socket, int id, int size_arr, int* argv);

void send_remove(void* socket, int id);

void send_exit(void *socket);

void send_heartbit(void *socket, unsigned int time);

bool availible_receive(void* socket);

void send_ping(void* socket, int id);

char* receive(void* socket);

#endif