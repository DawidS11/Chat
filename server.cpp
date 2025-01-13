#include <iostream>
#include <sys/socket.h>

int main()
{
    std::cout << "SERVER" << std::endl;

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
}