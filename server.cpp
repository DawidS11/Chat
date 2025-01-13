#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "values.hpp"

int main()
{
    std::cout << "SERVER" << std::endl;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        // TODO: Error handling.
        std::cerr << "socket() error." << std::endl;
        close(server_socket);
        exit(1);
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
    {
        // TODO: Error handling.
        std::cerr << "bind() error." << std::endl;
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1)
    {
        // TODO: Error handling.
        std::cerr << "listen() error." << std::endl;
        close(server_socket);
        exit(1);
    }
}