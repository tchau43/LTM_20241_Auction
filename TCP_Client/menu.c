// void communicate(int conn_sock, char *buff)
// {
//     int window_state = 1;
//     do
//     {
//         memset(buff, '\0', sizeof(buff));
//         int cmd;
//         menu_display();
//         cmd = fgetc(stdin);
//         switch (cmd)
//         {
//         case '1':
//             strncpy(buff, LOGIN_CMD, strlen(LOGIN_CMD));
//             buff[strlen(buff)] = ' ';
//             while (getchar() != '\n')
//             {
//             }
//             // User can only enter 1000 character. If more than 1000, it will be cut
//             printf("Enter usename:");
//             fgets(buff + strlen(buff), 1000, stdin);

//             if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
//                 buff[strlen(buff) - 1] = '\0';
//             strcat(buff, "\r\n");

//             send_msg(conn_sock, buff);
//             receive_msg(conn_sock);
//             break;

//         case '2':
//             strncpy(buff, POST_CMD, strlen(POST_CMD));
//             buff[strlen(buff)] = ' ';
//             while (getchar() != '\n')
//             {
//             }
//             // User can only enter 1000 character. If more than 1000, it will be cut
//             printf("Post message:");
//             fgets(buff + strlen(buff), 1000, stdin);

//             if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
//                 buff[strlen(buff) - 1] = '\0';
//             strcat(buff, "\r\n");
//             send_msg(conn_sock, buff);
//             receive_msg(conn_sock);
//             break;

//         case '3':
//             strncpy(buff, LOGOUT_CMD, strlen(LOGOUT_CMD));
//             strcat(buff, "\r\n");
//             send_msg(conn_sock, buff);
//             receive_msg(conn_sock);
//             while (getchar() != '\n')
//             {
//             }
//             break;

//         case '4':
//             window_state = 0;
//             break;
//         default:
//             printf("\nIncorrect command, please try\n");
//             break;
//         }

//     } while (window_state);
// }

#include <stdio.h>

#include "menu.h"

void auth_menu()
{
    printf("-------------------------------------------\n");
    printf("Auction App:\n");
    printf("1. Log in\n");
    printf("2. Create new account\n");
    printf("4. Exit\n");
    printf("Enter your command from 1 to 3:");
}