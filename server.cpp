#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>

#define BACKLOG_LISTEN 10
#define SERVER_PORT 2048
#define MSG_SIZE 256

int num_clients = 0;
std::mutex mtx;
std::unordered_map<int, std::string> clients;

void send_to_all(char* name, char* msg, const int client_socket, const bool did_quit = false)
{
    std::string msg_to_send = name;
    msg_to_send += (did_quit ? " " : ": ");
    msg_to_send += msg;
    for (auto it : clients)
    {
        if (it.first != client_socket)
        {
            send(it.first, msg_to_send.c_str(), msg_to_send.length() + 1, 0);
        }
    }
}

void send_welcome_msg(const int client_socket)
{
    std::string welcome_msg = "Admin: Welcome " + clients[client_socket] + "! Type \"quit\" to leave the chat.";
    send(client_socket, welcome_msg.c_str(), welcome_msg.length() + 1, 0);

    std::string has_joined_msg = clients[client_socket] + " has joined the chat.";
    for (auto it : clients)
    {
        if (it.first != client_socket)
        {
            send(it.first, has_joined_msg.c_str(), has_joined_msg.length() + 1, 0);
        }
    }
}

void error_handling(const int socket, const std::string& msg, const bool do_close = true, const bool do_exit = true)
{
    std::cerr << "[" << socket << "] " << msg << std::endl;
    if (do_close)
    {
        close(socket);
    }
    if (do_exit)
    {
        exit(1);
    }
}

void handle_client(int client_socket)
{
    char name[MSG_SIZE];
    if (recv(client_socket, name, sizeof(name), 0) == -1)
    {
        error_handling(client_socket, "Server: recv() name error.", false, false);
        return;
    }
    
    if (!clients.try_emplace(client_socket, name).second)
    {
        const std::string& error_msg = "Server: the socket " + std::to_string(client_socket) + " is already being used.";
        error_handling(client_socket, error_msg, false, false);
        return;
    }
    
    send_welcome_msg(client_socket);

    char msg[MSG_SIZE];
    int receive;
    while (true)
    {
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
            send_to_all(name, quit_msg, client_socket, true);
            std::cout << "num clients: " << num_clients << std::endl;
        }
        else
        {
            send_to_all(name, msg, client_socket);
        }
    }
}


int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        error_handling(server_socket, "Server: socket() error.");
        std::cerr << "Server: socket() error." << std::endl;
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
    {
        error_handling(server_socket, "Server: bind() error.");
    }

    if (listen(server_socket, BACKLOG_LISTEN) == -1)
    {
        error_handling(server_socket, "Server: listen() error.");
    }

    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket;
    while (true)
    {
        client_socket = accept(server_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
        if (client_socket == -1)
        {
            error_handling(server_socket, "Server: accept() error.");
        }
        mtx.lock();
        ++num_clients;
        std::cout << "num clients: " << num_clients << std::endl;
        mtx.unlock();

        std::thread th(handle_client, client_socket);
        th.detach();
    }
    close(server_socket);
}