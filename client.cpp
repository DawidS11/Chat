#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "values.hpp"

int main ()
{
    std::cout << "CLIENT" << std::endl;

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        // TODO: Error handling.
        std::cerr << "Client: socket() error." << std::endl;
        close(client_socket);
        exit(1);
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (connect(client_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
    {
        // TODO: Error handling.
        std::cerr << "Client: connect() error." << std::endl;
        close(client_socket);
        exit(1);
    }

    const char* message = "Client -> Server";
    send(client_socket, message, strlen(message), 0);
    std::cout << "Client has sent a message." << std::endl;

    close(client_socket);
}