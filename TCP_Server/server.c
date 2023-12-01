#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "req_handle.h"
#include "msg_handle.h"

#define BACKLOG 20
#define BUFF_SIZE 1024

void processData(char *in, char *out);

int receiveData(int s, char *buff, int size, int flags);

int sendData(int s, char *buff, int size, int flags);

int main(int argc, char* argv[])
{   
    // Get port number
        if (argc != 2)
    {
        printf("Usage: %s [port_number]", argv[0]);
        return 0;
    }
    int port = atoi(argv[1]);

    // Prepare server resoucse
    int maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    fd_set readfds, allset;
    int *login_state;
    login_state = (int *)malloc(FD_SETSIZE * sizeof(int));
    char **connBuff;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    // Set listen socket for server
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("\nError1: ");
        return 0;
    }

    // Bind
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("\nError2: ");
        return 0;
    }

    // Listen from sock
    if (listen(listenfd, BACKLOG) == -1)
    {
        perror("\nError3: ");
        return 0;
    }

    // Preset for client[] and readfds
    maxfd = listenfd;
    maxi = -1;
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        client[i] = -1;
    }
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    // Set up buffer for connected sock, handle stream
    connBuff = (char **)malloc(sizeof(char *) * FD_SETSIZE);
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        connBuff[i] = (char *)malloc(BUFF_SIZE);
    }

    // Running server
    while (1)
    {
        // Set readfds
        readfds = allset;
        nready = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (nready < 0)
        {
            perror("\nError4: ");
            return 0;
        }

        // Handle connection from client
        if (FD_ISSET(listenfd, &readfds))
        {
            clilen = sizeof(cliaddr);
            if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
                perror("\nError5: ");
            else
            {
                printf("You got connection from %s\n", inet_ntoa(cliaddr.sin_addr));
                int it = 0;
                // Add connection sock to client[]
                for (it = 0; it < FD_SETSIZE; it++)
                    if (client[it] < 0)
                    {
                        client[it] = connfd;
                        break;
                    }

                if (it == FD_SETSIZE)
                {
                    printf("\nToo many clients");
                    close(connfd);
                }
                else
                {
                    // Add connected sock to socklist
                    FD_SET(connfd, &allset);
                    memset(connBuff[it], '\0', sizeof(connBuff[it]));
                    login_state[it] = 0;
                    if (!send_msg(connfd, 100))
                    {
                        FD_CLR(connfd, &allset);
                        close(connfd);
                        client[it] = -1;
                    }
                    else
                    {
                        if (connfd > maxfd)
                            maxfd = connfd;
                        if (it > maxi)
                            maxi = it;
                    }
                    if (--nready <= 0)
                        continue;
                }
            }
        }

        // Hearing request from client
        for (int i = 0; i <= maxi; i++)
        {
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &readfds))
            {
                if (!msg_handle(sockfd, login_state + i, connBuff[i]))
                {
                    FD_CLR(sockfd, &allset);
                    close(sockfd);
                    client[i] = -1;
                }
                if (--nready <= 0)
                    break;
            }
        }
    }

    // Free connected sock's BUFFER
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        free(connBuff + i);
    }
    free(login_state);
    free(connBuff);
    return 0;
}
