/*TCP Echo Client*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "recv_msg.h"
#include "response.h"
/**
 * @function receive_code: receive message from server, resolve and display it
 *
 * @param client_sock: a number of socket use to receive message
 *
 */
int receive_code(int conn_sock)
{
    char res[CODE_SIZE + 1];
    int received_bytes;
    while (1)
    {
        received_bytes = recv(conn_sock, res, CODE_SIZE, 0);
        if (received_bytes < 0)
            perror("\nError1: ");
        else if (received_bytes == 0)
        {
            printf("Connection closed.\n");
            exit(1);
        }
        else
            break;
    }
    res[received_bytes] = '\0';
    printf("%s\n", res);
    return atoi(res);
}

int receive_anno(int conn_sock, int anno_type)
{
    char *msg = (char *)malloc(BUFF_SIZE);
    int received_bytes = recv(conn_sock, msg, BUFF_SIZE, 0);
    if (received_bytes < 0)
    {
        perror("\nError2: ");
        return 1;
    }
    else if (received_bytes == 0)
    {
        printf("Connection closed.\n");
        exit(0);
    }
    else
    {
        msg[received_bytes] = '\0';
        switch (anno_type)
        {
        case NEWBID:
            new_bid_msg_resolver(msg);
            break;
        case SOLDED:
            sold_msg_resolver(msg);
            break;
        case NEWITEMARRIVED:
            new_item_msg_resolver(msg);
            break;
        case COUNTDOWN:
            countdown_msg_resolver(msg);
            break;
        default:
            printf("Empty announcement\n");
            break;
        }
        return 0;
    }
    return 0;
}

void *msg_signal_handle(void *conn_sock)
{
}

void *recv_msg_handle(void *conn_sock)
{
    int sockfd = *((int *)conn_sock);
    pthread_detach(pthread_self());
    while (1)
    {
        int res_code = receive_code(sockfd);
        char res[BUFF_SIZE];
        int received_bytes;
        switch (res_code)
        {
        case NEWBID:
        case NEWITEMARRIVED:
        case SOLDED:
        case COUNTDOWN:
            if (receive_anno(sockfd, res_code) != 1)
                ;
            break;
            break;
        default:
            res_code_resolver(res_code);
            break;
        }
    }
}