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
#include "send_msg.h"

#define BUFF_SIZE 1024

/***
 * @function check_auth: Read the account information file and check the state of account
 *
 * @param username: a string to a input username
 *
 * @return: 1 if success
 *          0 if account is banned
 *          -1 if account is not found
 */

int check_auth(char *username)
{
    FILE *fp = fopen("account.txt", "r");
    char line[BUFF_SIZE];
    char check_name[1000];
    int acc_state;
    while (fgets(line, BUFF_SIZE, fp) != NULL)
    {
        sscanf(line, "%s %d", check_name, &acc_state);
        if (!strcmp(username, check_name))
        {
            if (acc_state)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    fclose(fp);
    return -1;
}

/**
 * @function request_handle: handle the request that received from client and send the result code
 *
 * @param conn_sock: a number of connected socket that use to send message
 * @param req: a string use to store request form client
 * @param login_state: a login state of session, 1 if logged in or 0 if not.
 * 
 * @return :1 if success
 *          0 if get an error
 */

int request_handle(int conn_sock, char *req, int *login_state)
{
    char cmd[10];
    sscanf(req, "%s", cmd);
    if (strcmp(cmd, "USER") == 0)
    {
        if (*login_state == 1)
        {
            send_msg(conn_sock, 213);
            return 1;
        }
        char username[1024];
        memset(username, '\0', sizeof(username));
        sscanf(req, "USER %s", username);
        switch (check_auth(username))
        {
        case 1:
            *login_state = 1;
            return send_msg(conn_sock, 110);
        case 0:
            return send_msg(conn_sock, 211);
        case -1:
            return send_msg(conn_sock, 212);
        default:
            break;
        }
    }

    else if (strcmp(cmd, "POST") == 0)
    {
        if (*login_state == 0)
        {
            return send_msg(conn_sock, 221);
        }
        else
        {
            return send_msg(conn_sock, 120);
        }
    }

    else if (strcmp(cmd, "BYE") == 0)
    {
        if (*login_state == 1)
        {
            *login_state = 0;
            return send_msg(conn_sock, 130);
        }
        else
        {
            return send_msg(conn_sock, 221);
        }
    }
    else
        return send_msg(conn_sock, 300);

    return 0;
}