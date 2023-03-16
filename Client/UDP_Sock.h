#pragma once
#include "Client_source.h"
#include "TCP_Sock.h"

class UDP_Sock {
    SOCKET server_socket;
    SOCKADDR_IN server;
    TCP_Sock* TCP_sock;
public:
    UDP_Sock(std::string& ip_address, int& port, TCP_Sock* new_TCP_sock);
    ~UDP_Sock();

    void UDP_Sock_connect();
    void message_to_server(int const buffer_size, char* buffer, SOCKADDR_IN& client, int& client_sizeof, FILE* file);
    void put_data(std::string file_name, int timer);
};
