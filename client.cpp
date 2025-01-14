#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

#include "values.hpp"

void send_msg(int client_socket)
{

}

void recv_msg(int client_socket)
{

}

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
    server_addr.sin_addr.s_addr = inet_addr(IP);

    if (connect(client_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
    {
        // TODO: Error handling.
        std::cerr << "Client: connect() error." << std::endl;
        close(client_socket);
        exit(1);
    }

    char msg[MSG_SIZE];
    if (recv(client_socket, msg, sizeof(msg), 0) != 0)
    {
        std::cout << "S: " << msg << std::endl;
    }

    const char* message = "client1";
    send(client_socket, message, strlen(message), 0);
    std::cout << "C: " << message << std::endl;

    std::thread th_send(send_msg, client_socket);
    std::thread th_recv(recv_msg, client_socket);
    
    th_send.join();
    th_recv.join();

    close(client_socket);
}