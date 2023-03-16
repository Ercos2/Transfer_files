#include "Client_source.h"
#include "TCP_Sock.h"
#include "UDP_Sock.h"

int main() {
    std::string ip_address;
    int TCP_port;

    std::cin >> ip_address;
    std::cin >> TCP_port;
    
    TCP_Sock TCP_sock(ip_address, TCP_port);
    TCP_sock.TCP_Sock_connect();
    std::string file_name;
    int UDP_port;
    int timer;

    std::cin >> UDP_port;
    std::cin >> file_name;
    std::cin >> timer;

    TCP_sock.get_data(file_name, UDP_port);

    UDP_Sock UDP_sock(ip_address, UDP_port, &TCP_sock);
    UDP_sock.UDP_Sock_connect();
    UDP_sock.put_data(file_name, timer);
}