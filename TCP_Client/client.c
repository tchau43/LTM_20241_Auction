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

#include "send_msg.h"
#include "recv_msg.h"
#include "menu.h"
#include "global_var.h"

#define BUFF_SIZE 1024
#define CODE_SIZE 4

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
    int conn_sock;
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

    // Set up hearing thread
    pthread_t hear_thread;
    pthread_create(&hear_thread, NULL, &recv_msg_handle, &conn_sock);
    // Communicate with sever
    int window_state = 1;
    do
    {
        switch (state)
        {
        case -1:
        case -2:
            break;
        case 0:
            window_state = 0;
            break;
        case 1:
            auth_menu_handle(conn_sock, buff);
            break;
        case 2:
            main_menu_handle(conn_sock, buff);
            break;
        case 3:
            in_room_handle(conn_sock, buff);
            break;
        case 4:
            while (1)
            {
                memset(buff, '\0', sizeof(buff));
                printf("Enter message:");
                fgets(buff, 1000, stdin);

                if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
                    buff[strlen(buff) - 1] = '\0';
                strcat(buff, "\r\n");
                send_msg(conn_sock, buff);
                while (getchar() != '\n')
                {
                }
            }
            break;
        default:
            break;
        }
    } while (window_state == 1);
    // Step 4: Close socket
    pthread_cancel(hear_thread);
    free(buff);
    close(conn_sock);
    return 0;
}