#include "webserv.hpp"
#include <iostream> 

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        std::cerr << "Failed to socket()" << std::endl;
        return (1);
    }
    
    sockaddr_in serverAddress;
    std::memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Failed to bind()" << std::endl;
        close(serverSocket);
        return (1);
    }

    if (listen(serverSocket, 5) == -1)
    {
        std::cerr << "Failed to listen()" << std::endl;
        close(serverSocket);
        return (1);
    }

    int clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == -1)
    {
        std::cerr << "Failed to accept()" << std::endl;
        close(serverSocket);
        return (1);
    }
    
    char buffer[BUFFER_SIZE] = {0};
    std::memset(buffer, 0, BUFFER_SIZE);
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;


    const char* http_response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 12\r\n"
    "Connection: close\r\n"
    "\r\n"
    "Hello World!";

    send(clientSocket, http_response, std::strlen(http_response), 0);

    close(clientSocket);
    close(serverSocket);
}