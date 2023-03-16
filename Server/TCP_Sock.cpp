#include "TCP_Sock.h"

TCP_Sock::TCP_Sock(std::string& ip_address, int& port) {
    WSAData wsad;
    if (WSAStartup(MAKEWORD(2, 1), &wsad) != 0)
    {
        std::cout << "WSAStartup error.\n";
        exit(1);
    }
    memset(&addr_info, 0, sizeof(SOCKADDR_IN));
    size_addr = sizeof(addr_info);
    addr_info.sin_addr.s_addr = inet_addr((char*)ip_address.c_str());
    addr_info.sin_port = htons(port);
    addr_info.sin_family = AF_INET;
    client_sock_listen = socket(AF_INET, SOCK_STREAM, 0);
}
TCP_Sock::~TCP_Sock() {
    closesocket(client_sock_listen);
    closesocket(client_sock_connect);
}

void TCP_Sock::connect() {
    bind(client_sock_listen, (sockaddr*)&addr_info, sizeof(addr_info));
    listen(client_sock_listen, SOMAXCONN);
    client_sock_connect = accept(client_sock_listen, (sockaddr*)&addr_info, &size_addr);

    if (client_sock_connect == 0) {
        std::cout << "Accept error.\n";
        exit(1);
    }
    else std::cout << "Connected\n";
}

void TCP_Sock::put_data() {
    const int buff_size = 16;
    char buff[buff_size];

    recv(client_sock_connect, buff, buff_size, 0);
    int file_name_size = atoi(buff);
    recv(client_sock_connect, buff, buff_size, 0);
    file_size = atoi(buff);
    recv(client_sock_connect, (char*)file_name.c_str(), file_name_size + 10, 0);
    recv(client_sock_connect, buff, buff_size, 0);
    UDP_port = atoi(buff);

    std::cout << "name size: " << file_name_size << "\n";
    std::cout << "size: " << file_size << "\n";
    std::cout << "name: " << file_name << "\n";
    std::cout << "port: " << UDP_port << "\n";
}

void TCP_Sock::send_ok() {
    std::string ok = "ok";
    send(client_sock_connect, (char*)ok.c_str(), 100, 0);
}

UDP_Struct* TCP_Sock::get_UDP_struct() const {
    return new UDP_Struct(file_name, file_size, UDP_port);
}