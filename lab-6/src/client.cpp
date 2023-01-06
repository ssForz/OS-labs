#include "config.h"



using namespace std;

#define CLIENT_ROOT_ID 20



int main()
{
    cout<<"Using client root "<<CLIENT_ROOT_ID<<" as default"<<endl;
    Tree* system;
    system = createTree(CLIENT_ROOT_ID);
    void *context = zmq_ctx_new();
    if (context == NULL) {
        throw runtime_error("Error: Can't initialize context");
    }

    void* socket_left = NULL;
    void* socket_right = NULL;
    int ex = 0;
    while (true) {
        command_type cur_command = get_command();
        string child_id_str;
        int child_id;
        string remove_id_str;
        int remove_id;
        int exec_id;
        int ping_id;
        string ping_id_str;
        string exec_id_str;
        int* argv;
        int size_arr;
        char *reply = (char *)calloc(sizeof(char), 64);
        switch(cur_command) {
            case PRINT:
                printTree(system,0);
                break;
            case CREATE:
                child_id_str = unitread();
                child_id = atoi(child_id_str.c_str());
                if (child_id <= 0) {
                    cout<<"Error: invalid id"<<endl;
                    break;
                }
                if (existNode(system, child_id)) {
                    cout<<"Error: already exists"<<endl;
                    break;
                }
                system = createNode(system, child_id);
                if (child_id > CLIENT_ROOT_ID) {
                    if (socket_right == NULL) {
                        int fork_pid = fork();
                        if (fork_pid == -1) {
                            throw runtime_error("Error: fork problem occured");
                            break;
                        }
                        if (fork_pid == 0) {
                            create_server_node(child_id);
                        }
                        
                        socket_right = zmq_socket(context, ZMQ_REQ);

                        cout<<"OK: "<<fork_pid<<endl;
                        int opt = 0;
                        int rc = zmq_setsockopt(socket_right, ZMQ_LINGER, &opt, sizeof(opt));
                        assert(rc == 0);
                        if (socket_right == NULL) {
                            throw runtime_error("Error: socket not created");
                        }
                        rc = zmq_connect(socket_right, convert_adr_client(BASE_PORT + child_id).c_str());
                        assert(rc == 0);
                        break;
                    }
                }
                if (child_id < CLIENT_ROOT_ID) {
                    if (socket_left == NULL) {
                        int fork_pid = fork();
                        if (fork_pid == -1) {
                            throw runtime_error("Error: fork problem occured");
                            break;
                        }
                        if (fork_pid == 0) {
                            create_server_node(child_id);
                        }
                        
                        socket_left = zmq_socket(context, ZMQ_REQ);

                        cout<<"OK: "<<fork_pid<<endl;
                        int opt = 0;
                        int rc = zmq_setsockopt(socket_left, ZMQ_LINGER, &opt, sizeof(opt));
                        assert(rc == 0);
                        if (socket_left == NULL) {
                            throw runtime_error("Error: socket not created");
                        }
                        rc = zmq_connect(socket_left, convert_adr_client(BASE_PORT + child_id).c_str());
                        assert(rc == 0);
                        break;
                    }
                }
                if (child_id > CLIENT_ROOT_ID) {
                    if (socket_right != NULL) {
                        int replied = 0;
                        send_create(socket_right, child_id);
                        if (availible_receive(socket_right)) {
                            reply = receive(socket_right);
                            if (strcmp(EMPTY_STR, reply) != 0) {
                                replied = 1;
                                cout<<reply<<endl;
                            }
                        }
                        if (replied == 0) {
                            cout<<"Error: node "<<child_id<<" unavailible"<<endl;
                        }
                        break;
                    }
                }
                if (child_id < CLIENT_ROOT_ID) {
                    if (socket_left != NULL) {
                        int replied = 0;
                        send_create(socket_left, child_id);
                        if (availible_receive(socket_left)) {
                            reply = receive(socket_left);
                            if (strcmp(EMPTY_STR, reply) != 0) {
                                replied = 1;
                                cout<<reply<<endl;
                            }
                        }
                        if (replied == 0) {
                            cout<<"Error: node "<<child_id<<" unavailible"<<endl;
                        }
                        break;
                    }
                }
                break;

            case REMOVE:
                remove_id_str = unitread();
                remove_id = atoi(remove_id_str.c_str());
                if (remove_id <= 0) {
                    cout<<"Error: invalid id"<<endl;
                    break;
                }
                if (!existNode(system, remove_id)) {
                    cout<<"Error: Not found"<<endl;
                    break;
                }
                if (CLIENT_ROOT_ID == remove_id) {
                    cout<<"Error: can't delete manager root"<<endl;
                    break;
                }
                system = deleteNode(system, remove_id);
                if (remove_id > CLIENT_ROOT_ID) {
                    int replied = 0;
                        send_remove(socket_right, remove_id);
                        if (availible_receive(socket_right)) {
                            reply = receive(socket_right);
                            if (strcmp(EMPTY_STR, reply) != 0) {
                                replied = 1;
                                cout<<reply<<endl;
                            }
                        }
                        if (replied == 0) {
                            cout<<"Error: node "<<child_id<<" unavailible"<<endl;
                        }
                        break;
                }
                else if (remove_id < CLIENT_ROOT_ID) {
                    int replied = 0;
                        send_remove(socket_left, remove_id);
                        if (availible_receive(socket_left)) {
                            reply = receive(socket_left);
                            if (strcmp(EMPTY_STR, reply) != 0) {
                                replied = 1;
                                cout<<reply<<endl;
                            }
                        }
                        if (replied == 0) {
                            cout<<"Error: node "<<child_id<<" unavailible"<<endl;
                        }
                        break;
                }
                break;
            case EXEC:
                exec_id_str = unitread();
                exec_id = atoi(exec_id_str.c_str());
                cin>>size_arr;
                argv = (int*)calloc(sizeof(int), size_arr);
                for (int i = 0; i < size_arr; i++) {
                    cin>>argv[i];
                }
                if (exec_id <= 0) {
                    cout<<"Error: invalid id"<<endl;
                    break;
                }
                if (!existNode(system, exec_id)) {
                    cout<<"Error: Not found"<<endl;
                    break;
                }
                if (CLIENT_ROOT_ID == exec_id) {
                    cout<<"Error: it is a manager root"<<endl;
                    break;
                }
                if (exec_id > CLIENT_ROOT_ID) {
                    int replied = 0;
                        send_exec(socket_right, exec_id, size_arr, argv);
                        if (availible_receive(socket_right)) {
                            reply = receive(socket_right);
                            if (strcmp(EMPTY_STR, reply) != 0) {
                                replied = 1;
                                cout<<reply<<endl;
                            }
                        }
                        if (replied == 0) {
                            cout<<"Error: node "<<exec_id<<" unavailible"<<endl;
                        }
                        break;
                }
                else if (exec_id < CLIENT_ROOT_ID) {
                    int replied = 0;
                        send_exec(socket_left, exec_id, size_arr, argv);
                        if (availible_receive(socket_left)) {
                            reply = receive(socket_left);
                            if (strcmp(EMPTY_STR, reply) != 0) {
                                replied = 1;
                                cout<<reply<<endl;
                            }
                        }
                        if (replied == 0) {
                            cout<<"Error: node "<<exec_id<<" unavailible"<<endl;
                        }
                        break;
                }
                break;

            case PING:
                ping_id_str = unitread();
                ping_id = atoi(ping_id_str.c_str());
                if (ping_id <= 0) {
                    cout<<"Error: invalid id"<<endl;
                    break;
                }
                if (!existNode(system, ping_id)) {
                    cout<<"Error: Not found"<<endl;
                    break;
                }
                if (CLIENT_ROOT_ID == ping_id) {
                    cout<<"Error: it is a manager root"<<endl;
                    break;
                }
                if (ping_id > CLIENT_ROOT_ID) {
                    int replied = 0;
                        send_ping(socket_right, ping_id);
                        if (availible_receive(socket_right)) {
                            reply = receive(socket_right);
                            if (strcmp(EMPTY_STR, reply) != 0) {
                                replied = 1;
                                cout<<reply<<endl;
                            }
                        }
                        if (replied == 0) {
                            cout<<"OK: 0"<<endl;
                        }
                        break;
                }
                else if (ping_id < CLIENT_ROOT_ID) {
                    int replied = 0;
                        send_ping(socket_left, ping_id);
                        if (availible_receive(socket_left)) {
                            reply = receive(socket_left);
                            if (strcmp(EMPTY_STR, reply) != 0) {
                                replied = 1;
                                cout<<reply<<endl;
                            }
                        }
                        if (replied == 0) {
                            cout<<"OK: 0"<<endl;
                        }
                        break;
                }
                break;


            case EXIT:
                if (socket_right != NULL) {
                    send_exit(socket_right);
                }
                if (socket_left != NULL) {
                    send_exit(socket_left);
                }
                ex = 1;
                break;
        }
        if (ex == 1) {
            break;
        }
        free(reply);

    }
    zmq_close(socket_right);
    zmq_close(socket_left);
    zmq_ctx_destroy(context);
}