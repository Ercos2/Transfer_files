#include "UDP_Sock.h"

UDP_Sock::UDP_Sock(std::string& ip_address, int& port, TCP_Sock* new_TCP_sock) : TCP_sock(new_TCP_sock) {                       // настраиваем сокет
    WSADATA wsaData;
    int winsock_result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (winsock_result != 0) {
        exit(1);
    }

    server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_socket == INVALID_SOCKET) {
        WSACleanup();
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr((char*)ip_address.c_str());
}

UDP_Sock::~UDP_Sock() {                                                                                                         // деструктор с закрытием сокета, чтобы наверняка
    closesocket(server_socket);
}

void UDP_Sock::UDP_Sock_connect() {                                                                                             // подключаемся к серверу
    if (bind(server_socket, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
}

void UDP_Sock::message_to_server(int const buffer_size, char* buffer, SOCKADDR_IN& client, int& client_sizeof, FILE* file) {    //функция для передачи сообщений, чтобы несколько раз не писать эти строки потом
    if (sendto(server_socket, buffer, buffer_size, 0, (SOCKADDR*)&client, client_sizeof) == SOCKET_ERROR) {
        fclose(file);
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
}

void UDP_Sock::put_data(std::string file_name, int timer) {                                                                     // передаём данные на сервер
    SOCKADDR_IN client;
    int client_sizeof = sizeof(client);
    int const buffer_size = 1024;
    char buffer[buffer_size];

    if (recvfrom(server_socket, buffer, buffer_size, 0, (SOCKADDR*)&client, &client_sizeof) == SOCKET_ERROR) {                  // ждём пока сервер сообщит о готовности
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
    else {
        FILE* file;
        file = fopen(file_name.c_str(), "rb");                                                                                  // открываем отправляемый файл
        std::cout << "Filename: " << file_name << std::endl;

        if (file == NULL || file == 0) {
            std::cout << "Couldn't open the file." << std::endl;
            strcpy(buffer, "NOFILE");
            message_to_server(buffer_size, buffer, client, client_sizeof, file);
        }

        fseek(file, 0, SEEK_END);                                                                                               // помещаем файловый указатель в начало
        int file_size = ftell(file);                                                                                            // узнаём размер файла

        size_t reading_size;
        int part = 0;
        const clock_t begin_time = clock();                                                                                     // таймер времени передачи ради интереса
        int bytes_sent = 0;

        while ((part * buffer_size) < file_size) {                                                                              // прерываем цикл, как дойдём до конца файла
            fseek(file, (part * buffer_size), SEEK_SET);                                                                        // помещаем файловый указатель в зависимости от переданного
            reading_size = fread(buffer, 1, buffer_size, file);                                                                 // читаем часть файла и запоминаем, сколько именно
            message_to_server(reading_size, buffer, client, client_sizeof, file);                                               // передаём прочитанное с использованием его размера
            
            clock_t timer_time = clock();                                                                                       // ожидаем подтверждения получения от сервера
            while (clock() - timer_time < timer) {
                if (!TCP_sock->put_ok(timer)) {
                    part++;
                    bytes_sent += reading_size;                                                                                 // я хотел вывод о процессе передачи
                    break;
                }
                //std::cout << "Again " << clock() - timer_time << " < " << timer << "\n";                                      // контроль цикла
            }
            //std::cout << "Sent " << part * 1024 << " bytes of " << file_size << " bytes." << "\n";                            // варианты вывода
            std::cout << "Sent " << bytes_sent << " bytes of " << file_size << " bytes." << "\n";
            //std::cout << "Sent " << (int)((double)bytes_sent / (double)file_size * 100) << "% of " << 100 << "%." << "\n";
            //std::cout << "Sent " << part << " parts of " << buffer_size << " bytes." << std::endl;
        }
        std::cout << "Time of sending file: " << float(clock() - begin_time) / CLOCKS_PER_SEC << " seconds." << std::endl;

        strcpy(buffer, "QUIT");
        message_to_server(buffer_size, buffer, client, client_sizeof, file);                                                    // сообщаем, что файл закончился
        fclose(file);
    }
}