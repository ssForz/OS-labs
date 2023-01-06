#include "config.h"


int main(int argc, const char** argv)
{
    void* context = zmq_ctx_new();
    if (context == NULL) {
        throw runtime_error("Error: Can't initialize context");
    }
    int self_id = atoi(argv[0]);

    void* self_socket = zmq_socket(context, ZMQ_REP);
    if (self_socket == NULL) {
        throw runtime_error("Error: Can't initialize socket");
    }
    const char* self_adr = convert_adr_server(BASE_PORT + self_id).c_str();

    int rc = zmq_bind(self_socket, self_adr);
    assert(rc == 0);
    void* socket_left = NULL;
    void* socket_right = NULL;
    int rm = 0;
    int ex = 0;
    int id_left, id_right;
    while (true) {
        int flag = 0;
        command_type cur_command = DEFAULT;
        int sum = 0;
        int count_args = 0;
        int id_target = 0;
        int* argv;
        int size_arr = 0;
        while (true) {
            rm = 0;
            ex = 0;
            zmq_msg_t piece;
            int get = zmq_msg_init(&piece);
            assert(get == 0);
            get = zmq_msg_recv(&piece, self_socket, 0);
            assert(get != -1);

            switch (count_args) {
                case 0:
                    memcpy(&cur_command, zmq_msg_data(&piece), zmq_msg_size(&piece));
                    break;
                case 1:
                    switch (cur_command) {
                        case CREATE:
                            memcpy(&id_target, zmq_msg_data(&piece), zmq_msg_size(&piece));
                            break;
                        case REMOVE:
                            memcpy(&id_target, zmq_msg_data(&piece), zmq_msg_size(&piece));
                            break;
                        case EXEC:
                            memcpy(&id_target, zmq_msg_data(&piece), zmq_msg_size(&piece));
                            break;
                        case PING:
                            memcpy(&id_target, zmq_msg_data(&piece), zmq_msg_size(&piece));
                            break;
                        default:
                            break;
                    }
                    break;
                case 2:
                    switch (cur_command) {
                        case EXEC:
                            memcpy(&size_arr, zmq_msg_data(&piece), zmq_msg_size(&piece));
                            zmq_msg_close((&piece));
                            if (!zmq_msg_more(&piece)) {
                                break;
                            }

                            argv = (int*)calloc(sizeof(int), size_arr);
                            for (int i = 0; i < size_arr;i++) {
                                get = zmq_msg_init(&piece);
                                assert(get == 0);
                                get = zmq_msg_recv(&piece, self_socket, 0);
                                assert(get != -1);
                                memcpy(&argv[i], zmq_msg_data(&piece), zmq_msg_size(&piece));
                                if (!zmq_msg_more(&piece)) {
                                    break;
                                }
                                zmq_msg_close((&piece));
                            }
                            flag = 1;
                            break;
                        default:
                            break;
                        }
                        break;
                default:
                    throw runtime_error("Error: wrong command received");
                    break;
            }

            zmq_msg_close((&piece));
            count_args++;
            if (flag == 1) {
                break;
            }
            if (!zmq_msg_more(&piece)) {
                break;
            }
        }

        char *reply = (char *)calloc(sizeof(char), 64);
        int replied = 0; 
            if (cur_command == EXIT) {
                if (socket_right != NULL) {
                    send_exit(socket_right);
                }
                if (socket_left != NULL) {
                    send_exit(socket_left);
                }
                break;
            }   
            if (cur_command == CREATE) {
                    int child_id = id_target;
                    if ((child_id > self_id) && socket_right == NULL) {
                        
                        
                       int fork_pid = fork();
                        if (fork_pid == -1) {
                            throw runtime_error("Error: fork problem occured");
                            
                        }
                        if (fork_pid == 0) {
                                create_server_node(child_id);
                                break;
                        }
                            
                        socket_right = zmq_socket(context, ZMQ_REQ);
                        int opt = 0;
                        int rc = zmq_setsockopt(socket_right, ZMQ_LINGER, &opt, sizeof(opt));
                        assert(rc == 0);
                            
                        if (socket_right == NULL) {
                            throw runtime_error("Error: sosocket not created");
                        }
                        rc = zmq_connect(socket_right, convert_adr_client(BASE_PORT + child_id).c_str());
                            
                        assert(rc == 0);
                            
                        const char* fork_pid_str = int_to_str(fork_pid);
                        sprintf(reply, "OK: %s", fork_pid_str);
                        replied = 1;

                    } else if ((child_id < self_id) && socket_left == NULL) {
                        
                        if (socket_left == NULL) {
                            int fork_pid = fork();
                            if (fork_pid == -1) {
                                throw runtime_error("Error: fork problem occured");
                            
                            }
                            if (fork_pid == 0) {
                                create_server_node(child_id);
                                break;
                            }
                            socket_left = zmq_socket(context, ZMQ_REQ);
                            int opt = 0;
                            int rc = zmq_setsockopt(socket_left, ZMQ_LINGER, &opt, sizeof(opt));
                            assert(rc == 0);
                            if (socket_left == NULL) {
                                throw runtime_error("Error: socket not created");
                            }
                            rc = zmq_connect(socket_left, convert_adr_client(BASE_PORT + child_id).c_str());
                            assert(rc == 0);
                            

                            const char* fork_pid_str2 = int_to_str(fork_pid);
                            
                            sprintf(reply, "OK: %s", fork_pid_str2);
                            replied = 1;
                            
                        }
                    }
                    else if ((child_id > self_id) && socket_right != NULL) {
                        
                        
                        send_create(socket_right, child_id);
                        if (availible_receive(socket_right)) {
                            reply = receive(socket_right);
                            if (strcmp(EMPTY_STR, reply) != 0) {
                                replied = 1;
                            }
                        }
                        if (replied == 0) {

                            cout<<"Error: node "<<child_id<<" unavailible"<<endl;
                        }
                    }
                    
                    else if ((child_id < self_id) && socket_left != NULL) {
                        
                        
                        send_create(socket_left, child_id);
                        if (availible_receive(socket_left)) {
                            reply = receive(socket_left);
                            if (strcmp(EMPTY_STR, reply) != 0) {
                                replied = 1;
                            }
                        }
                        if (replied == 0) {

                            cout<<"Error: node "<<child_id<<" unavailible"<<endl;
                        }
                    }
            }
            if (cur_command == REMOVE) {
                
                int remove_id = id_target;
                if (remove_id == self_id) {
                    if (socket_right != NULL) {
                        send_exit(socket_right);
                    }
                    if (socket_left != NULL) {
                        send_exit(socket_left);
                    }
                    
                    
                    sprintf(reply, "Removed %d", remove_id);
                    replied = 1;
                    rm = 1;
                }
                else if (id_target > self_id) {
                    if (socket_right != NULL) {
                        
                        send_remove(socket_right, remove_id);
                        if (availible_receive(socket_right)) {
                            reply = receive(socket_right);
                            if ((strcmp(EMPTY_STR, reply)) != 0) {
                                replied = 1;
                            }

                        }
                        
                    }
                }
                else if (id_target < self_id) {
                    if (socket_left != NULL) {
                        
                        send_remove(socket_left, remove_id);
                        if (availible_receive(socket_left)) {
                            reply = receive(socket_left);
                            if ((strcmp(EMPTY_STR, reply)) != 0) {
                                replied = 1;
                            }
                        }
                        
                    }
                }
            }
            if (cur_command == EXEC) {

                if (id_target == self_id) {
                    for (int i =0; i<size_arr;i++) {
                        sum += argv[i];
                    }
                    sprintf(reply, "OK: %d: sum is %d", id_target, sum);
                    replied = 1;

                }
                if (id_target > self_id) {
                     if (socket_right != NULL) {
                        
                        send_exec(socket_right, id_target, size_arr, argv);
                        if (availible_receive(socket_right)) {
                            reply = receive(socket_right);
                            if ((strcmp(EMPTY_STR, reply)) != 0) {
                                replied = 1;
                            }
                        }
                        
                    }
                }
                if (id_target < self_id) {
                    if (socket_left != NULL) {
                        
                        send_exec(socket_left, id_target, size_arr, argv);
                        if (availible_receive(socket_left)) {
                            reply = receive(socket_left);
                            if ((strcmp(EMPTY_STR, reply)) != 0) {
                                replied = 1;
                            }
                        }
                        
                    }
                }
            }
            if (cur_command == PING) {
                if (id_target == self_id) {
                    sprintf(reply, "OK: 1");
                    replied = 1;
                }
                if (id_target > self_id) {
                     if (socket_right != NULL) {
                        
                        send_ping(socket_right, id_target);
                        if (availible_receive(socket_right)) {
                            reply = receive(socket_right);
                            if ((strcmp(EMPTY_STR, reply)) != 0) {
                                replied = 1;
                            }
                        }
                        
                    }
                }
                if (id_target < self_id) {
                    if (socket_left != NULL) {
                        send_ping(socket_left, id_target);
                        if (availible_receive(socket_left)) {
                            reply = receive(socket_left);
                            if ((strcmp(EMPTY_STR, reply)) != 0) {
                                replied = 1;
                            }
                        }
                        
                    }
                }
            }
        if (replied == 0) {
            reply = EMPTY_STR;
        }
        
        size_t rep_len = strlen(reply) + 1;
        zmq_msg_t create_response;
        int rec = zmq_msg_init(&create_response);
        assert(rec != -1);
        zmq_msg_init_size(&create_response, rep_len);
        memcpy(zmq_msg_data(&create_response), reply, rep_len);
        zmq_msg_send(&create_response, self_socket, 0);
        zmq_msg_close(&create_response);
        if (rm == 1) {
            break;
        }
        if (ex == 1) {
            break;
        }
        
    }
    zmq_close(self_socket);
    zmq_ctx_destroy(context);



}