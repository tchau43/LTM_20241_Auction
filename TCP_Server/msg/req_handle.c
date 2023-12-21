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
#include "../val/response.h"

#define BUFF_SIZE 1024

/***
 * @function check_auth: Read the account information file and check the state of account
 *
 * @param username: a string to a input username
 *
 * @return: LOGIN_SUCCESS if success
 *          LG_USER_BLOCK if account is banned
 *          LG_USER_NOT_EXIST if account is not found
 *          WRONG_PASSWORD if input wrong password
 */
enum AuthStatus{
    LOGIN_SUCCESS, 
    LG_USER_BLOCK,
    LG_USER_NOT_EXIST,
    INCORRECT_PASSWORD
};
enum AuthStatus check_auth(char *username, char *password)
{
    FILE *fp = fopen("account.txt", "r");
    char line[BUFF_SIZE];
    char check_name[1000];
    int acc_state;
    char check_password[1000];
    while (fgets(line, BUFF_SIZE, fp) != NULL)
    {
        sscanf(line, "%s %d %s", check_name, &acc_state, &check_password);
        if (!strcmp(username, check_name))
        {
            if (acc_state)
            {
                if(!strcmp(password, check_password)) {
                    return LOGIN_SUCCESS;
                }
                else return INCORRECT_PASSWORD;
            }
            else
            {
                return LG_USER_BLOCK;
            }
        }
    }
    fclose(fp);
    return LG_USER_NOT_EXIST;
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

int request_handle(int conn_sock, char* req, session* sess)
{
    char cmd[10];
    sscanf(req, "%s", cmd);
    if (strcmp(cmd, "LOGIN") == 0)
    {
        if (sess->is_loggedin== 1)
        {
            send_msg(conn_sock, ALREADYLOGIN);
            return 1;
        }
        char username[1024];
        char password[1024];
        memset(username, '\0', sizeof(username));
        sscanf(req, "USER %s PASSWORD %s", username,password);
        
        switch (check_auth(username, password))
        {
        case LOGIN_SUCCESS:
            sess->is_loggedin= 1;
            return send_msg(conn_sock, LOGINOK);
        case LG_USER_BLOCK:
            return send_msg(conn_sock, ACCBLOCK);
        case INCORRECT_PASSWORD:
            return send_msg(conn_sock, WRONG_PASSWORD);
        case LG_USER_NOT_EXIST:
            return send_msg(conn_sock, UNAMENF);
        default:
            break;
        }
    }

    else if (strcmp(cmd, "JOIN_ROOM") == 0)
    {
        
        if (sess->is_loggedin== 0)
        {
            return send_msg(conn_sock, NOTLOGIN);
        }
        else
        {
            send_msg(conn_sock, JOINNOK);
            int room;
            
        }
    }

    else if (strcmp(cmd, "BYE") == 0)
    {
        if (sess->is_loggedin== 1)
        {
            sess->is_loggedin= 0;
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