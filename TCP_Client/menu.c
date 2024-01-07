#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "send_msg.h"
#include "global_var.h"

#define BUFF_SIZE 1024

void auth_menu()
{
    printf("-------------------------------------------\n");
    printf("%s:\n", APP_NAME);
    printf("1. Log in\n");
    printf("2. Create new account\n");
    printf("3. Exit\n");
    printf("Enter your command from 1 to 3:");
}

void main_menu()
{
    printf("---------------------------------------------\n");
    printf("Welcome to %s\n", APP_NAME);
    printf("1. Create new room\n");
    printf("2. Show room list\n");
    printf("3. Join room\n");
    printf("4. Log out\n");
    printf("5. Exit\n");
    printf("Enter your command from 1 to 5:");
}

void inroom_menu()
{
    printf("---------------------------------------------\n");
    printf("Welcome to %s\n", APP_NAME);
    printf("1. Add new items\n");
    printf("2. Remove item\n");
    printf("3. Bid\n");
    printf("4. Buy out\n");
    printf("5. Out room\n");
    printf("6. Exit\n");
    printf("Enter your command from 1 to 3:");
}

void auth_menu_handle(int conn_sock, char *buff)
{

    memset(buff, '\0', BUFF_SIZE);
    auth_menu();
    int cmd;
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
        fgets(buff + strlen(buff), 30, stdin);
        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';

        buff[strlen(buff)] = ' ';

        // User can only enter 1000 character. If more than 1000, it will be cut
        printf("Enter password:");
        fgets(buff + strlen(buff), 30, stdin);

        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';
        strcat(buff, "\r\n");

        send_msg(conn_sock, buff);

        pthread_mutex_lock(&state_mutex);
        state = -1;
        pthread_mutex_unlock(&state_mutex);
        break;

    case '2':
        strncpy(buff, SIGNUP_CMD, strlen(SIGNUP_CMD));
        buff[strlen(buff)] = ' ';
        while (getchar() != '\n')
        {
        }
        // User can only enter 30 character. If more than 30, it will be cut
        printf("Enter usename:");
        fgets(buff + strlen(buff), 30, stdin);
        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';

        buff[strlen(buff)] = ' ';
        // User can only enter 30 character. If more than 30, it will be cut
        printf("Enter password:");
        fgets(buff + strlen(buff), 30, stdin);

        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';
        strcat(buff, "\r\n");

        send_msg(conn_sock, buff);
        break;
    case '3':
        pthread_mutex_lock(&state_mutex);
        state = 0;
        pthread_mutex_unlock(&state_mutex);
    default:
        break;
    }
}

void main_menu_handle(int conn_sock, char *buff)
{
    memset(buff, '\0', BUFF_SIZE);
    main_menu();
    int cmd;
    cmd = fgetc(stdin);
    switch (cmd)
    {
    case '1':
        strncpy(buff, ROOM_CREATE_CMD, strlen(ROOM_CREATE_CMD));
        buff[strlen(buff)] = ' ';
        while (getchar() != '\n')
        {
        }
        // User can only enter 30 character. If more than 30, it will be cut
        printf("Enter name of room to create:");
        fgets(buff + strlen(buff), 30, stdin);

        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';
        strcat(buff, "\r\n");
        send_msg(conn_sock, buff);
        break;
    case '2':
        strncpy(buff, ROOM_LIST_CMD, strlen(ROOM_LIST_CMD));
        strcat(buff, "\r\n");
        send_msg(conn_sock, buff);
        while (getchar() != '\n')
        {
        }
        break;
    case '3':
        strncpy(buff, JOIN_ROOM_CMD, strlen(JOIN_ROOM_CMD));
        buff[strlen(buff)] = ' ';
        while (getchar() != '\n')
        {
        }
        // User can only enter 30 character. If more than 30, it will be cut
        printf("Enter room's name to join:");
        fgets(buff + strlen(buff), 30, stdin);

        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';
        strcat(buff, "\r\n");
        send_msg(conn_sock, buff);
        pthread_mutex_lock(&state_mutex);
        state = -2;
        pthread_mutex_unlock(&state_mutex);
        break;
    case '4':
        strncpy(buff, LOGOUT_CMD, strlen(LOGOUT_CMD));
        strcat(buff, "\r\n");
        send_msg(conn_sock, buff);
        while (getchar() != '\n')
        {
        }
        pthread_mutex_lock(&state_mutex);
        state = -2;
        pthread_mutex_unlock(&state_mutex);
        break;
    case '5':
        pthread_mutex_lock(&state_mutex);
        state = 0;
        pthread_mutex_unlock(&state_mutex);
        break;
    default:
        break;
    }
}

void in_room_handle(int conn_sock, char *buff)
{
    memset(buff, '\0', BUFF_SIZE);
    inroom_menu();
    int cmd;
    cmd = fgetc(stdin);
    switch (cmd)
    {
    case '1':
        strncpy(buff, ADD_ITEM_CMD, strlen(ADD_ITEM_CMD));
        buff[strlen(buff)] = ' ';
        while (getchar() != '\n')
        {
        }
        // User can only enter 30 character. If more than 30, it will be cut
        printf("Enter item's name:");
        fgets(buff + strlen(buff), 30, stdin);
        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';

        buff[strlen(buff)] = ' ';
        printf("Enter starting price (thousand VND):");
        fgets(buff + strlen(buff), 30, stdin);

        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';
        buff[strlen(buff)] = ' ';
        printf("Enter buy out price (thousand VND):");
        fgets(buff + strlen(buff), 30, stdin);

        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';

        strcat(buff, "\r\n\0");

        send_msg(conn_sock, buff);
        break;
    case '2':
        strncpy(buff, DELETE_ITEM_CMD, strlen(DELETE_ITEM_CMD));
        buff[strlen(buff)] = ' ';
        while (getchar() != '\n')
        {
        }
        // User can only enter 30 character. If more than 30, it will be cut
        printf("Enter item's name to delete:");
        fgets(buff + strlen(buff), 30, stdin);

        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';
        strcat(buff, "\r\n");
        send_msg(conn_sock, buff);
        break;
    case '3':
        strncpy(buff, BID_CMD, strlen(BID_CMD));
        buff[strlen(buff)] = ' ';
        while (getchar() != '\n')
        {
        }
        // User can only enter 30 character. If more than 30, it will be cut
        printf("Enter bid (thousand VND):");
        fgets(buff + strlen(buff), 30, stdin);

        if (buff[strlen(buff) - 1] == '\n' || buff[strlen(buff) - 1] == '\r')
            buff[strlen(buff) - 1] = '\0';
        strcat(buff, "\r\n\0");

        send_msg(conn_sock, buff);
        break;
    case '4':
        strncpy(buff, BUYOUT_CMD, strlen(BUYOUT_CMD));
        strcat(buff, "\r\n");
        send_msg(conn_sock, buff);
        while (getchar() != '\n')
        {
        }
        break;
    case '5':
        strncpy(buff, OUTROOM_CMD, strlen(OUTROOM_CMD));
        strcat(buff, "\r\n");
        send_msg(conn_sock, buff);
        while (getchar() != '\n')
        {
        }
        pthread_mutex_lock(&state_mutex);
        state = -3;
        pthread_mutex_unlock(&state_mutex);
        break;
    case '6':
        pthread_mutex_lock(&state_mutex);
        state = 0;
        pthread_mutex_unlock(&state_mutex);
        break;
    default:
        break;
    }
}