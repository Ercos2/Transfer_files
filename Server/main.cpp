#include "Server_source.h"
#include "TCP_Sock.h"
#include "UDP_Sock.h"

int main() {
    std::string ip_address;
    std::string path;
    int port;
    
    std::cin >> ip_address;                                                     // � �� ���� ��������, ��� ��������� ������ ������� ���������
    std::cin >> port;
    std::cin >> path;
    while (true) {                                                              // ������ ������ ���������� �������� ����� ���������� �������� �����
        TCP_Sock TCP_sock(ip_address, port);
        TCP_sock.connect();
        TCP_sock.put_data();

        UDP_Sock UDP_sock(ip_address, TCP_sock.get_UDP_struct(), &TCP_sock);
        UDP_sock.get_data(path);
    }
}