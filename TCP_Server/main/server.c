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

#include "../msg/req_handle.h"
#include "../msg/msg_handle.h"
#include "../msg/send_msg.h"
#include "../session/session.h"
#include "../room/room.h"
#include "../val/global_var.h"

#define BACKLOG 20
#define BUFF_SIZE 1024

int main(int argc, char *argv[])
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
    int nready;
    fd_set readfds, allset;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    room_store = (room *)malloc(sizeof(room) * ROOM_NUM);
    sess_store = (session *)malloc(sizeof(session) * FD_SETSIZE);

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
    init_session_store();
    init_room_store();
    maxfd = listenfd;
    maxi = -1;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

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
                int index = create_new_session(connfd);
                if (index < 0)
                {
                    printf("Too many client\n");
                    close(connfd);
                }
                else
                {
                    FD_SET(connfd, &allset);
                    if (!send_msg(connfd, 100))
                    {
                        FD_CLR(connfd, &allset);
                        close(connfd);
                        sess_store[index].conn_sock = -1;
                    }
                    else
                    {
                        if (maxfd < connfd)
                            maxfd = connfd;

                        if (index > maxi)
                            maxi = index;
                        printf("%d\n", maxi);
                    }
                    if (--nready <= 0)
                        continue;
                }
            }
        }

        // Hearing request from client
        for (int i = 0; i <= maxi; i++)
        {

            if ((sockfd = sess_store[i].conn_sock) < 0)
                continue;
            if (FD_ISSET(sockfd, &readfds))
            {

                if (!msg_handle(i))
                {
                    FD_CLR(sockfd, &allset);
                    close(sockfd);
                    sess_store[i].conn_sock = -1;
                }
                if (--nready <= 0)
                    break;
            }
        }
    }


    free(sess_store);
    free(room_store);
    return 0;
}
