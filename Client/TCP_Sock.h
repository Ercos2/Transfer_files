#pragma once
#include "Client_source.h"

class TCP_Sock {
    SOCKADDR_IN addr_info;
    int size_addr;
    SOCKET server_sock_listen;
public:
    TCP_Sock(std::string& ip_address, int& port);
    ~TCP_Sock();

    void TCP_Sock_connect();
    void get_data(std::string file_name, int UDP_port);
    bool put_ok();
};