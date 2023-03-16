#include "UDP_Sock.h"

UDP_Sock::UDP_Sock(std::string& ip_address, int& port, TCP_Sock* new_TCP_sock) : TCP_sock(new_TCP_sock) {
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

UDP_Sock::~UDP_Sock() {
    closesocket(server_socket);
}

void UDP_Sock::UDP_Sock_connect() {
    if (bind(server_socket, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
}

void UDP_Sock::message_to_server(int const buffer_size, char* buffer, SOCKADDR_IN& client, int& client_sizeof, FILE* file) {
    if (sendto(server_socket, buffer, buffer_size, 0, (SOCKADDR*)&client, client_sizeof) == SOCKET_ERROR) {
        fclose(file);
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
}

void UDP_Sock::put_data(std::string file_name, int timer) {
    SOCKADDR_IN client;
    int client_sizeof = sizeof(client);
    int const buffer_size = 1024;
    char buffer[buffer_size];

    if (recvfrom(server_socket, buffer, buffer_size, 0, (SOCKADDR*)&client, &client_sizeof) == SOCKET_ERROR) {
        closesocket(server_socket);
        WSACleanup();
        exit(1);
    }
    else {
        FILE* file;
        file = fopen(file_name.c_str(), "rb");
        std::cout << "Filename: " << file_name << std::endl;

        if (file == NULL || file == 0) {
            std::cout << "Couldn't open the file." << std::endl;
            strcpy(buffer, "NOFILE");
            message_to_server(buffer_size, buffer, client, client_sizeof, file);
        }

        fseek(file, 0, SEEK_END);
        int file_size = ftell(file);

        size_t reading_size;
        int part = 0;
        const clock_t begin_time = clock();
        int bytes_sent = 0;

        while ((part * buffer_size) < file_size) {
            fseek(file, (part * buffer_size), SEEK_SET);
            reading_size = fread(buffer, 1, buffer_size, file);
            message_to_server(reading_size, buffer, client, client_sizeof, file);
            
            clock_t timer_time = clock();
            while (clock() - timer_time < timer) {
                if (!TCP_sock->put_ok()) {
                    part++;
                    bytes_sent += reading_size;
                    break;
                }
                //std::cout << "Again " << clock() - timer_time << " < " << timer << "\n";
            }
            //std::cout << "Sent " << part * 1024 << " bytes of " << file_size << " bytes." << "\n";
            std::cout << "Sent " << bytes_sent << " bytes of " << file_size << " bytes." << "\n";
            //std::cout << "Sent " << (int)((double)bytes_sent / (double)file_size * 100) << "% of " << 100 << "%." << "\n";
            //std::cout << "Sent " << part << " parts of " << buffer_size << " bytes." << std::endl;
        }
        std::cout << "Time of sending file: " << float(clock() - begin_time) / CLOCKS_PER_SEC << " seconds." << std::endl;

        strcpy(buffer, "QUIT");
        message_to_server(buffer_size, buffer, client, client_sizeof, file);
        fclose(file);
    }
}