#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Error opening a socket\n %d", errno);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(8090);

    if (bind(sockfd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Error bind:");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 10) < 0)
    {
        printf("Error listen");
        exit(EXIT_FAILURE);
    }

    int clientfd = accept(sockfd, 0, 0);

    // stdin = 0
    struct pollfd fds[2] = {
        {
            0,
            POLLIN,
            0,
        },
        {sockfd,
         POLLIN,
         0}};

    const char *welcome_msg = "Hello from the server!\n";

    char buff[256] = {0};

    poll(fds, 2, 5000);

    for (;;)
    {

        if (fds[0].revents & POLLIN)
        {
            read(0, buff, 255);
            send(sockfd, buff, 255, 0);
            send(clientfd, welcome_msg, strlen(welcome_msg), 0);
        }
        else if (fds[1].revents & POLLIN)
        {
            if (recv(clientfd, buff, 255, 0) == 0)
            {
                return 0;
            };
        }
        printf("%s\n", buff);
    }

    return 0;
}