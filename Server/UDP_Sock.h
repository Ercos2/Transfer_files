#pragma once
#include "Server_source.h"
#include "TCP_Sock.h"

class UDP_Sock {
    SOCKET client_socket;
    SOCKADDR_IN server;
    TCP_Sock* TCP_sock;
    std::string file_name;
    int file_size;
public:
    UDP_Sock(std::string& ip_address, UDP_Struct* temp_UDP_struct, TCP_Sock* new_TCP_sock);

    void get_data(std::string path);
};