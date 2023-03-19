#include "UDP_Sock.h"

UDP_Sock::UDP_Sock(std::string& ip_address, UDP_Struct* temp_UDP_struct, TCP_Sock* new_TCP_sock) : 
                    file_name(temp_UDP_struct->file_name), file_size(temp_UDP_struct->file_size), TCP_sock(new_TCP_sock) {      // настраиваем подключение
    WSADATA wsaData;
    int winsock_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (winsock_result != 0)
        exit(1);

    client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_socket == INVALID_SOCKET) {
        WSACleanup();
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(temp_UDP_struct->UDP_port);
    server.sin_addr.s_addr = inet_addr((char*)ip_address.c_str());
}

UDP_Sock::~UDP_Sock() {                                                                                                         // закрываем сокет
    closesocket(client_socket);
}

void UDP_Sock::get_data(std::string path) {                                                                                    // получаем данные
    int server_sizeof = sizeof(server);
    int const buffer_size = 1024;
    char buffer[buffer_size];
    char filename[buffer_size];
    strcpy(filename, (path + "//" + file_name).c_str());                                                                        //задаём имя будущего файла

    strcpy(buffer, "Ready");
    if (sendto(client_socket, buffer, buffer_size, 0, (SOCKADDR*)&server, server_sizeof) == SOCKET_ERROR) {                     // подаём сигнал, что готовы принимать данные
        closesocket(client_socket);
        WSACleanup();
        exit(1);
    }

    int received_size = 0;
    std::vector<char> buffer_vec;                                                                                               // буфер для данных, потому что recvfrom работает странно

    while (true) {
        received_size = recvfrom(client_socket, buffer, buffer_size, 0, (SOCKADDR*)&server, &server_sizeof);                    // запоминаем размер переданных данных
        if (received_size == SOCKET_ERROR) {
            closesocket(client_socket);
            WSACleanup();
            exit(1);
        }
        if (strcmp(buffer, "NOFILE") == 0) {                                                                                    // получаем сигнал, что файла не существует
            std::cout << "The file does not exist on the server." << "\n";
            remove(filename);
            break;
        }
        else if (strcmp(buffer, "QUIT") == 0) {                                                                                 // получаем сигнал, что передача оончена
            std::cout << "Transmission ended by the server." << "\n";
            break;
        }
        for (int a = 0; a < received_size; ++a) {                                                                               // заносим данные в буфер
            buffer_vec.push_back(buffer[a]);
        }
        TCP_sock->send_ok();                                                                                                    // сообщаем, что получили данные
    }
    FILE* file;
    file = fopen(filename, "wb");                                                                                               // создаём файл
    fwrite(buffer_vec.data(), sizeof(char), buffer_vec.size(), file);                                                           // заполняем полученными данными
    fclose(file);                                                                                                               // закрываем файл
}