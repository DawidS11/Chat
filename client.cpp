#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

#include "values.hpp"

void send_msg(int client_socket)
{
    char msg[MSG_SIZE];
    while (true)
    {
        std::cout << "Type here1: ";
        std::cin.getline(msg, MSG_SIZE);
        send(client_socket, msg, sizeof(msg), 0);
        if (std::strcmp(msg, "quit") == 0){
            close(client_socket);
            exit(0);
        }
    }
}

void recv_msg(int client_socket)
{
    char msg[MSG_SIZE];
    while (true)
    {
        if (recv(client_socket, msg, sizeof(msg), 0) <= 0)
        {
            continue;
        }

        char backspace_code = 8;
        std::string type_here_msg = "Type here: ";
        int welcome_msg_size = type_here_msg.length() + 1;
        for (int i = 0; i < welcome_msg_size; ++i)
        {
            std::cout << backspace_code;
        }
        std::cout << msg << std::endl;
        std::cout << type_here_msg;
        std::fflush(stdout);
    }
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

    char name[MSG_SIZE];
	std::cout << "Enter your name: ";
	std::cin.getline(name, MSG_SIZE);
	send(client_socket, name, sizeof(name), 0);

    std::thread th_send(send_msg, client_socket);
    std::thread th_recv(recv_msg, client_socket);
    if (th_send.joinable()) th_send.join();
    if (th_recv.joinable()) th_recv.join();

    close(client_socket);
}