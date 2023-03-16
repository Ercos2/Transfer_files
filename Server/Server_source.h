#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

struct UDP_Struct {
    std::string file_name;
    const int file_size;
    int UDP_port;
    UDP_Struct(std::string new_file_name, int new_file_size, int new_UDP_port) : 
                file_name(new_file_name), file_size(new_file_size), UDP_port(new_UDP_port) {}
};