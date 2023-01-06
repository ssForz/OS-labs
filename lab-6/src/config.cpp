#include "config.h"

void create_server_node(int id)
{
    const char* arg = SERVER_PATH;
    const char* arg0 = int_to_str(id);
    execl(arg, arg0, NULL);
}

void send_create(void* socket, int id)
{
    command_type cmd = CREATE;
    zmq_msg_t command;
    zmq_msg_init_size(&command, sizeof(cmd));
    memcpy(zmq_msg_data(&command), &cmd, sizeof(cmd));
    zmq_msg_send(&command, socket, ZMQ_SNDMORE);
    zmq_msg_close(&command);
    
    zmq_msg_t id_msg;
    zmq_msg_init_size(&id_msg, sizeof(id));
    memcpy(zmq_msg_data(&id_msg), &id, sizeof(id));
    zmq_msg_send(&id_msg, socket, 0);
    zmq_msg_close(&id_msg);

}

void send_remove(void* socket, int id)
{
    command_type cmd = REMOVE;
    zmq_msg_t command;
    zmq_msg_init_size(&command, sizeof(cmd));
    memcpy(zmq_msg_data(&command), &cmd, sizeof(cmd));
    zmq_msg_send(&command, socket, ZMQ_SNDMORE);
    zmq_msg_close(&command);
    
    zmq_msg_t id_msg;
    zmq_msg_init_size(&id_msg, sizeof(id));
    memcpy(zmq_msg_data(&id_msg), &id, sizeof(id));
    zmq_msg_send(&id_msg, socket, 0);
    zmq_msg_close(&id_msg);
}

void send_exec(void* socket, int id, int size_arr, int* argv)
{
    command_type cmd = EXEC;
    zmq_msg_t command;
    zmq_msg_init_size(&command, sizeof(cmd));
    memcpy(zmq_msg_data(&command), &cmd, sizeof(cmd));
    zmq_msg_send(&command, socket, ZMQ_SNDMORE);
    zmq_msg_close(&command);

    zmq_msg_t id_msg;
    zmq_msg_init_size(&id_msg, sizeof(id));
    memcpy(zmq_msg_data(&id_msg), &id, sizeof(id));
    zmq_msg_send(&id_msg, socket, ZMQ_SNDMORE);
    zmq_msg_close(&id_msg);

    zmq_msg_t size_arr_msg;
    zmq_msg_init_size(&size_arr_msg, sizeof(size_arr));
    memcpy(zmq_msg_data(&size_arr_msg), &size_arr, sizeof(size_arr));
    zmq_msg_send(&size_arr_msg, socket, ZMQ_SNDMORE);
    zmq_msg_close(&size_arr_msg);
    for (int i = 0; i < size_arr; i++) {
        zmq_msg_t arg;
        zmq_msg_init_size(&arg, sizeof(argv[i]));
        memcpy(zmq_msg_data(&arg), &argv[i], sizeof(argv[i]));
        if (i == size_arr - 1) {
            zmq_msg_send(&arg, socket, 0);
        }
        if (i != size_arr - 1) {
            zmq_msg_send(&arg, socket, ZMQ_SNDMORE);
        }
        zmq_msg_close(&size_arr_msg);
    }

}

bool availible_receive(void *socket) {
    zmq_pollitem_t items[1] = {{socket, 0, ZMQ_POLLIN, 0}};
    int rc = zmq_poll(items, 1, REQUEST_TIMEOUT);
    assert(rc != -1);
    if (items[0].revents & ZMQ_POLLIN)
        return true;
    return false;
}



char* receive(void* socket)
{
    zmq_msg_t reply;
    zmq_msg_init(&reply);
    zmq_msg_recv(&reply, socket, 0);
    size_t result_size = zmq_msg_size(&reply);

    char* result = (char*)calloc(sizeof(char), result_size + 1);
    memcpy(result, zmq_msg_data(&reply), result_size);
    zmq_msg_close(&reply);

    return result;
}

void send_exit(void *socket)
{
    command_type cmd = EXIT;
    zmq_msg_t command_msg;
    zmq_msg_init_size(&command_msg, sizeof(cmd));
    memcpy(zmq_msg_data(&command_msg), &cmd, sizeof(cmd));
    zmq_msg_send(&command_msg, socket, 0);
    zmq_msg_close(&command_msg);
}

void send_ping(void* socket, int id)
{
    command_type cmd = PING;
    zmq_msg_t command;
    zmq_msg_init_size(&command, sizeof(cmd));
    memcpy(zmq_msg_data(&command), &cmd, sizeof(cmd));
    zmq_msg_send(&command, socket, ZMQ_SNDMORE);
    zmq_msg_close(&command);
    
    zmq_msg_t id_msg;
    zmq_msg_init_size(&id_msg, sizeof(id));
    memcpy(zmq_msg_data(&id_msg), &id, sizeof(id));
    zmq_msg_send(&id_msg, socket, 0);
    zmq_msg_close(&id_msg);
}
