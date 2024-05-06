#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
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
    bzero(&address, sizeof(address));

    address.sin_family = AF_INET;   // Typ adresu
    address.sin_port = htons(PORT); // Port (przekonwertowany do big-endian)

    if (inet_pton(AF_INET, server_ip, &address.sin_addr.s_addr) <= 0)
    {
        perror("Invalid IP address");
        exit(EXIT_FAILURE);
    }
    if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Nie udało się połączyć z serwerem");
        exit(EXIT_FAILURE);
    }

    printf("Połączono z %s:%d\n", server_ip, PORT);

    // Możesz tutaj dodać dalsze interakcje z serwerem...
    // ...

    // Zamknięcie gniazda po zakończeniu pracy
    close(sockfd);

    return 0;
}