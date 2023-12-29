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

#include "response.h"
#include "send_msg.h"

#define BUFF_SIZE 1024
#define RES_SIZE 4

#define LOGIN_CMD "USER"
#define POST_CMD "POST"
#define LOGOUT_CMD "BYE"

int conn_sock;

/**
 * @function receive_msg: receive message from server, resolve and display it
 *
 * @param client_sock: a number of socket use to receive message
 *
 */
int receive_msg(int conn_sock)
{
    char res[RES_SIZE + 1];
    int received_bytes = recv(conn_sock, res, RES_SIZE, 0);
    if (received_bytes < 0)
        perror("\nError1: ");
    else if (received_bytes == 0)
    {
        printf("Connection closed.\n");
    }
    else
    {
        res[received_bytes] = '\0';
        printf("%s\n", res);
    }
    return atoi(res);
}

void msg_signal_handle(int signal)
{
    int res_code = receive_msg(conn_sock);
    char res[BUFF_SIZE];
    int received_bytes;
    switch (res_code)
    {
    case NEWBEST:
        received_bytes = recv(conn_sock, res, BUFF_SIZE, 0);
        if (received_bytes < 0)
            perror("\nError1: ");
        else if (received_bytes == 0)
        {
            printf("Connection closed.\n");
        }
        else
        {
            res[received_bytes] = '\0';
            printf("%s\n", res);
        }
        break;

    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <server_ip> <port_number>\n", argv[0]);
        return 0;
    }

    int port_num = atoi(argv[2]);
    if (port_num == 0)
    {
        printf("Wrong port number\n");
        return 0;
    }

    char *ip_adress = argv[1];
    if (inet_addr(ip_adress) == -1)
    {
        printf("Wrong ip_address\n");
        return 0;
    }
    char *buff = (char *)malloc(BUFF_SIZE + 1);
    struct sockaddr_in server_addr; /* server's address information */

    // Step 1: Construct socket
    if ((conn_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("\nError2: ");
        exit(EXIT_FAILURE);
    }

    // Step 2: Specify server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);
    server_addr.sin_addr.s_addr = inet_addr(ip_adress);
    // Step 3: Request to connect server
    if (connect(conn_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("\nError3: ");
        exit(EXIT_FAILURE);
    }

    // Receive comfirm connection from server
    char conn_res[4];
    int received_bytes = recv(conn_sock, conn_res, 4, 0);
    if (received_bytes < 0)
        perror("\nError4: ");
    else if (received_bytes == 0)
    {
        printf("Connection closed.\n");
    }
    else
    {
        if (atoi(conn_res) != 100)
        {
            printf("Cannot connect to server\n");
            return 0;
        }
        else
        {
            printf("Conntected to server\n");
        }
    }

    // Set up signal driven io
    signal(SIGIO, msg_signal_handle);
    fcntl(conn_sock, F_SETOWN, getpid());
    int flags = fcntl(conn_sock, F_GETFL, 0);
    fcntl(conn_sock, F_SETFL, flags | O_ASYNC | O_NONBLOCK);

    // Communicate with sever
    while (1)
    {
        memset(buff, '\0', sizeof(buff));
        printf("Enter message:");
        fgets(buff, 1000, stdin);

        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';
        strcat(buff, "\r\n");
        send_msg(conn_sock, buff);
        while (getchar()!='\n')
        {
        }
        
    }
    // Step 4: Close socket
    free(buff);
    close(conn_sock);
    return 0;
}