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

#include "req_handle.h"
#include "msg_handle.h"

#define BUFF_SIZE 1024
#define DELIMITER "\r\n"

/**
 * @function client_handle: handle receive message from client, recieve message and handle stream, handle request
 *
 * @param conn_sock: number of connected socket that used to communicate with client
 * @param login_state: loggin state of client that connected to conn_sock
 * @param buff: buffer of stream handling for client that connected with conn_sock
 * 
 * @return :1 if handle success
 *          0 if get an error or connection close
 *
 */
int msg_handle(int conn_sock, session* sess)
{
    char *req = (char *)malloc(BUFF_SIZE);
    char tmp[BUFF_SIZE];
    memset(req, '\0', sizeof(req));
    memset(tmp, '\0', sizeof(tmp));

    int received_bytes = recv(conn_sock, req, BUFF_SIZE, 0);
    if (received_bytes < 0)
    {
        perror("\nError4: ");
        return 0;
    }
    else if (received_bytes == 0)
    {
        printf("Connection closed\n");
        return 0;
    }

    req[received_bytes] = '\0';
    while (strlen(req) > 0)
    {
        char *part2 = strstr(req, DELIMITER);
        if (part2 == NULL)
        {
            strcat(sess->buff, req);
            break;
        }
        else
        {
            int part2_n = strlen(part2);
            strncat(sess->buff, req, strlen(req) - part2_n);
            printf("%d: %s\n", conn_sock, sess->buff);
            if (strlen(sess->buff) == 0)
                continue;
            if (!request_handle(conn_sock, sess->buff, sess))
            {
                return 0;
            }
            memset(sess->buff, '\0', sizeof(sess->buff));
            memset(tmp, '\0', sizeof(tmp));
            strcpy(tmp, part2 + 2);
            memset(req, '\0', sizeof(req));
            strcpy(req, tmp);
        }
    }
    free(req);
    return 1;
}
