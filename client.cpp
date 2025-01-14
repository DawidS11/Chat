#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

#include "values.hpp"

void send_msg(int client_socket)
{
    std::string message;
    while (true)
    {
        std::cout << "You: ";
        std::getline(std::cin, message);
        if (message == "Quit"|| message == "quit"){
            close(client_socket);
            exit(0);
        }
        send(client_socket, message.c_str(), message.length() + 1, 0);
    }
}

void recv_msg(int client_socket)
{
    char msg[MSG_SIZE];
    while (true)
    {
        if (recv(client_socket, msg, sizeof(msg), 0) <= 0)
        {
            // TODO: Error handling.
            //std::cerr << "Client: recv() error." << std::endl;
            //close(client_socket);
            //exit(1);
            continue;
        }
        std::cout << "C: " << msg << std::endl;
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

    /*char msg[MSG_SIZE];
    if (recv(client_socket, msg, sizeof(msg), 0) != 0)
    {
        std::cout << "S: " << msg << std::endl;
    }*/

    //std::string message;
    //std::getline(std::cin, message);
    //std::cin >> message;
    //send(client_socket, message.c_str(), message.length() + 1, 0);
    //std::cout << "C: My name is " << message << std::endl;

    std::string client_name = "client1";

    std::thread th_send(send_msg, client_socket);
    std::thread th_recv(recv_msg, client_socket);
    if (th_send.joinable()) th_send.join();
    if (th_recv.joinable()) th_recv.join();

    close(client_socket);
}