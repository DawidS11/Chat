#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <thread>

#include "values.hpp"

int num_clients = 0;

void handle_client(int client_socket)
{
    std::cout << "XXXXX1" << std::endl;
    char msg[MSG_SIZE];

    int receive;
    std::cout << "XXXXX2" << std::endl;
    while (true)
    {
        receive = recv(client_socket, msg, sizeof(msg), 0);
        if (receive <= 0)
        {
            // TODO: Error handling.
            //std::cerr << "Server: recv() error." << std::endl;
            //close(client_socket);
            return;
        }

        std::cout << "C: " << msg << "-" << std::endl;
    }
}


int main()
{
    std::cout << "SERVER" << std::endl;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        // TODO: Error handling.
        std::cerr << "Server: socket() error." << std::endl;
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
        std::cerr << "Server: bind() error." << std::endl;
        close(server_socket);
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1)
    {
        // TODO: Error handling.
        std::cerr << "Server: listen() error." << std::endl;
        close(server_socket);
        exit(1);
    }

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket;
    while (true)
    {
        client_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
        if (client_socket == -1)
        {
            // TODO: Error handling.
            std::cerr << "Server: accept() error." << std::endl;
            close(server_socket);
            exit(1);
        }
        std::cout << "Cout: Client connected." << std::endl;
        ++num_clients;

        /*const char* message = "What is your name?";
        send(client_socket, message, strlen(message) + 1, 0);
        std::cout << "S: " << message << std::endl;*/

        std::thread th(handle_client, client_socket);
        th.detach();

        close(client_socket);
        close(server_socket);

        //break; // temporary
    }
}