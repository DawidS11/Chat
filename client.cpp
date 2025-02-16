#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

#define IP "127.0.0.1"
#define SERVER_PORT 2048
#define MSG_SIZE 256

void send_msg(int client_socket)
{
    while (true)
    {
        std::cout << "Type: ";
        char msg[MSG_SIZE];
        std::cin.getline(msg, MSG_SIZE);
        std::cout << "\033[A\033[2K" << std::flush;

        if (std::strcmp(msg, "quit") != 0)
        {
            std::cout << "You: " << msg << std::endl;
        }

        send(client_socket, msg, sizeof(msg), 0);
        if (std::strcmp(msg, "quit") == 0)
        {
            std::cout << "You have left the chat." << std::endl;
            close(client_socket);
            exit(0);
        }
    }
}

void recv_msg(int client_socket)
{
    char msg[MSG_SIZE] = "";
    while (true)
    {
        if (recv(client_socket, msg, sizeof(msg), 0) <= 0)
        {
            continue;
        }

        std::cout << "\n\033[A\033[2K";
        std::cout << msg << std::endl;
        std::cout << "Type: ";
        std::fflush(stdout);
    }
}

void error_handling(const int socket, const std::string& msg)
{
    std::cerr << "[" << socket << "] " << msg << std::endl;
    close(socket);
    exit(1);
}

int main ()
{
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        error_handling(client_socket, "Client: socket() error.");
    }

    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    if (connect(client_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
    {
        error_handling(client_socket, "Client: connect() error.");
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