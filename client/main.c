#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define PORT 8090
#define BUFF_MAX 256

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Address Ip was not prvided");
        exit(EXIT_FAILURE);
    }

    const char *server_ip = argv[1];

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("SOCKET ERROR");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    if (inet_pton(AF_INET, server_ip, &address.sin_addr.s_addr) <= 0)
    {
        perror("Invalid IP address");
        exit(EXIT_FAILURE);
    }
    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Unable to connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected with %s:%d\n", server_ip, PORT);

    char buffer[BUFF_MAX];
    memset(&buffer, 0, sizeof(buffer));

    ssize_t bytesReceived = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived < 0)
    {
        perror("Error receiving data");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buffer[bytesReceived] = '\0';

    printf("Received message:%s", buffer);

    close(sockfd);
    return 0;
}