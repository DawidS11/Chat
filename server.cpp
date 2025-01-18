#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>

#include "values.hpp"

int num_clients = 0;
std::mutex mtx;
std::unordered_map<int, std::string> clients;

void send_to_all(char* name, char* msg, const bool did_quit = false)
{
    std::strcat(name, (did_quit ? " " : ": "));
    std::strcat(name, msg);
    std::cout << "XXX: " << name;
    for (auto it : clients)
    {
        send(it.first, name, sizeof(name), 0);
    }
}

void handle_client(int client_socket)
{
    const char welcome_msg[MSG_SIZE] = "Admin: Welcome! Type quit to leave the chat.";
    send(client_socket, welcome_msg, sizeof(welcome_msg), 0);

    char name[MSG_SIZE];
    if (recv(client_socket, name, sizeof(name), 0) == -1)
    {
        // TODO: Error handling.
        std::cerr << "Server: recv() name error." << std::endl;
        return;
    }
    clients.try_emplace(client_socket, name);
    for (auto it : clients)
    {
        std::cout << it.first << " " << it.second << std::endl;
    }
    std::cout << "Name: " << name << std::endl;
    
    char msg[MSG_SIZE];
    int receive;
    while (true)
    {
        std::cout << "Q " << std::endl;
        receive = recv(client_socket, msg, sizeof(msg), 0);
        if (receive <= 0)
        {
            return;
        }

        if (std::strcmp(msg, "quit") == 0)
        {
            mtx.lock();
            --num_clients;
            clients.erase(client_socket);
            mtx.unlock();
            char quit_msg[MSG_SIZE] = "has left the chat.";
            send_to_all(name, quit_msg, true);
            std::cout << "NUM CLIENTS2: " << num_clients << std::endl;
        }

        send_to_all(name, msg);
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
        mtx.lock();
        ++num_clients;
        mtx.unlock();
        std::cout << "NUM CLIENTS: " << num_clients << std::endl;

        std::thread th(handle_client, client_socket);
        th.detach();
    }
    close(server_socket);
}