#include <iostream>
#include <sys/socket.h>

#include "values.hpp"

int main()
{
    std::cout << "SERVER" << std::endl;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        // TODO: Error handling.
        std::cerr << "socket() error." << std::endl;
        exit(1);
    }
}