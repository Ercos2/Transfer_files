#pragma once
#include "Server_source.h"

class TCP_Sock {
    SOCKADDR_IN addr_info;
    int size_addr;
    SOCKET client_sock_listen;
    SOCKET client_sock_connect;

    int file_size;
    std::string file_name;
    int UDP_port;
public:
    TCP_Sock(std::string& ip_address, int& port);
    ~TCP_Sock();

    void connect();
    void put_data();
    void send_ok();
    UDP_Struct* get_UDP_struct() const;
};