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

#define BUFF_SIZE 1024
#define RES_SIZE 3

#define LOGIN_CMD "USER"
#define POST_CMD "POST"
#define LOGOUT_CMD "BYE"

void menu_display()
{
    printf("-------------------------------------------\n");
    printf("Quan ly tai khoan:\n");
    printf("1. Log in\n");
    printf("2. Post message\n");
    printf("3. Log out\n");
    printf("4. Exit\n");
    printf("Enter your command from 1 to 4:");
}

/**
 * @function res_code_resolver: resolve a result code receive from server and print a message it meaning
 *
 * @param res_code: result code receive from server
 */

void res_code_resolver(int res_code)
{
    switch (res_code)
    {
    case 110:
        printf("Login successfully\n");
        break;
    case 211:
        printf("Account is banned\n");
        break;
    case 212:
        printf("Account's not found\n");
        break;
    case 213:
        printf("Already Login\n");
        break;
    case 120:
        printf("Post successfully\n");
        break;
    case 221:
        printf("You are not logged in\n");
        break;
    case 130:
        printf("Logout successfully\n");
        break;
    default:
        printf("Syntax error\n");
        break;
    }
}

/**
 * @function receive_msg: receive message from server, resolve and display it
 *
 * @param client_sock: a number of socket use to receive message
 *
 */

void receive_msg(int conn_sock)
{
    char res[RES_SIZE + 1];
    int received_bytes = recv(conn_sock, res, RES_SIZE, 0);
    if (received_bytes < 0)
        perror("\nError: ");
    else if (received_bytes == 0)
    {
        printf("Connection closed.\n");
    }
    else
    {
        res[received_bytes] = '\0';
        res_code_resolver(atoi(res));
    }
}

/**
 * @function send_msg: send message to server
 *
 * @param conn_sock: connection socket number
 * @param msg: message want to send
 */

void send_msg(int conn_sock, char *msg)
{
    int sent_bytes = send(conn_sock, msg, strlen(msg), 0);
    if (sent_bytes < 0)
        perror("\nError: ");
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
    int conn_sock;
    char *buff = (char *)malloc(BUFF_SIZE + 1);
    struct sockaddr_in server_addr; /* server's address information */

    // Step 1: Construct socket
    if ((conn_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }

    // Step 2: Specify server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);
    server_addr.sin_addr.s_addr = inet_addr(ip_adress);
    // Step 3: Request to connect server
    if (connect(conn_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0)
    {
        perror("\nError: ");
        exit(EXIT_FAILURE);
    }

    // Receive comfirm connection from server
    char conn_res[4];
    int received_bytes = recv(conn_sock, conn_res, 4, 0);
    if (received_bytes < 0)
        perror("\nError: ");
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
    // Communicate with sever
    int window_state = 1;
    do
    {
        memset(buff, '\0', sizeof(buff));
        int cmd;
        menu_display();
        cmd = fgetc(stdin);
        switch (cmd)
        {
        case '1':
            strncpy(buff, LOGIN_CMD, strlen(LOGIN_CMD));
            buff[strlen(buff)] = ' ';
            while (getchar() != '\n')
            {
            }
            // User can only enter 1000 character. If more than 1000, it will be cut
            printf("Enter usename:");
            fgets(buff + strlen(buff), 1000, stdin);

            if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
                buff[strlen(buff) - 1] = '\0';
            strcat(buff, "\r\n");

            send_msg(conn_sock, buff);
            receive_msg(conn_sock);
            break;

        case '2':
            strncpy(buff, POST_CMD, strlen(POST_CMD));
            buff[strlen(buff)] = ' ';
            while (getchar() != '\n')
            {
            }
             // User can only enter 1000 character. If more than 1000, it will be cut
            printf("Post message:");
            fgets(buff + strlen(buff), 1000, stdin);

            if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
                buff[strlen(buff) - 1] = '\0';
            strcat(buff, "\r\n");
            send_msg(conn_sock, buff);
            receive_msg(conn_sock);
            break;

        case '3':
            strncpy(buff, LOGOUT_CMD, strlen(LOGOUT_CMD));
            strcat(buff, "\r\n");
            send_msg(conn_sock, buff);
            receive_msg(conn_sock);
            while (getchar() != '\n')
            {
            }
            break;

        case '4':
            window_state = 0;
            break;
        default:
            printf("\nIncorrect command, please try\n");
            break;
        }

    } while (window_state);
    // Step 4: Close socket
    free(buff);
    close(conn_sock);
    return 0;
}