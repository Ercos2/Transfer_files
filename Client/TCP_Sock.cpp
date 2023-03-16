#include "TCP_Sock.h"

TCP_Sock::TCP_Sock(std::string& ip_address, int& port) {                                        // настраиваем сокет
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
    server_sock_listen = socket(AF_INET, SOCK_STREAM, 0);
}
TCP_Sock::~TCP_Sock() {                                                                         // деструктор, потому что мы не верим, что оно само разрушится
    closesocket(server_sock_listen);
}

void TCP_Sock::TCP_Sock_connect() {                                                             // подключаемся к серверу
    if (connect(server_sock_listen, (sockaddr*)&addr_info, sizeof(addr_info)) != 0) {
        std::cout << "Accept error.\n";
        exit(1);
    }
    else std::cout << "Connected\n";
}

void TCP_Sock::get_data(std::string file_name, int UDP_port) {                                  // передаём размер файла, имя файла и порт UDP
    std::fstream file;
    std::cout << file_name << "\n";
    file.open(file_name, std::ios_base::in | std::ios_base::binary);

    if (file.is_open()) {
        int file_size = std::experimental::filesystem::file_size(file_name) + 1;

        //std::cout << "name size: " << file_name.size() << "\n";                               // вывод для контроля
        //std::cout << "size: " << file_size << "\n";
        //std::cout << "name: " << file_name << "\n";
        //std::cout << "port: " << UDP_port << "\n";

        if (file_size > 10485761) {                                                             // в тз просили ограничить размер файла 10мб
            std::cout << "the file is too large\n";
            exit(1);
        }

        const int buff_size = 16;
        send(server_sock_listen, std::to_string(file_name.size()).c_str(), buff_size, 0);      //передаём размер названия, чтобы нормально его получить на сервере
        send(server_sock_listen, std::to_string(file_size).c_str(), buff_size, 0);
        send(server_sock_listen, file_name.c_str(), file_name.size() + 10, 0);
        send(server_sock_listen, std::to_string(UDP_port).c_str(), buff_size, 0);

    }
    else std::cout << "Error file open\n";

    file.close();
}

bool TCP_Sock::put_ok(int timer) {                                                               //задаём время ожидания порта, потому что неблокируемый создавать слишком сложно 
    DWORD timeout = timer;
    setsockopt(server_sock_listen, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    std::string message;
    recv(server_sock_listen, (char*)message.c_str(), 100, 0);
    if (message == "ok") return 0;
    else return 1;
}