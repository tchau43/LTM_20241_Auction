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
#include "../room/room.h"
#include "../item/item.h"
#include "../val/response.h"

#include "../bid/bid.h"
#include "../auth/auth_handle.h"
#include "../log/log_writter.h"

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
int request_handle(int sesit, char *req)
{
    char cmd[10];
    sscanf(req, "%s", cmd);
    if (strcmp(cmd, "LOGIN") == 0)
    {
        if (sess_store[sesit].is_loggedin == 1)
        {
            write_to_log(sess_store[sesit].conn_sock, req, ALREADYLOGIN);
            send_code(sess_store[sesit].conn_sock, ALREADYLOGIN);
            return 1;
        }
        char username[1024];
        char password[1024];
        memset(username, '\0', sizeof(username));
        memset(password, '\0', sizeof(password));
        if (sscanf(req, "LOGIN %s %s", username, password) == 2)
        {
            switch (login_handle(username, password))
            {
            case LOGIN_SUCCESS:
                sess_store[sesit].is_loggedin = 1;
                strcpy(sess_store[sesit].username, username);
                write_to_log(sess_store[sesit].conn_sock, req, LOGINOK);
                return send_code(sess_store[sesit].conn_sock, LOGINOK);
            case LG_OTHER_CLIENT:
                write_to_log(sess_store[sesit].conn_sock, req, ACCLOGIN);
                return send_code(sess_store[sesit].conn_sock, ACCLOGIN);
            case INCORRECT_PASSWORD:
                write_to_log(sess_store[sesit].conn_sock, req, WRONG_PASSWORD);
                return send_code(sess_store[sesit].conn_sock, WRONG_PASSWORD);
            case LG_USER_NOT_EXIST:
                write_to_log(sess_store[sesit].conn_sock, req, UNAMENF);
                return send_code(sess_store[sesit].conn_sock, UNAMENF);
            default:
                break;
            }
        }
        write_to_log(sess_store[sesit].conn_sock, req, SYNTAXERR);
        return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
    }
    else if (strcmp(cmd, "SIGNUP") == 0)
    {
        if (sess_store[sesit].is_loggedin == 1)
        {
            write_to_log(sess_store[sesit].conn_sock, req, ALREADYLOGIN);
            send_code(sess_store[sesit].conn_sock, ALREADYLOGIN);
            return 1;
        }
        char username[1024];
        char password[1024];
        memset(username, '\0', sizeof(username));
        memset(password, '\0', sizeof(password));
        if (sscanf(req, "SIGNUP %s %s", username, password) == 2)
        {
            if (signup_handle(username, password))
            {
                write_to_log(sess_store[sesit].conn_sock, req, SIGNUPSUCESS);
                return send_code(sess_store[sesit].conn_sock, SIGNUPSUCESS);
            }
            write_to_log(sess_store[sesit].conn_sock, req, SIGNUPFAIL);
            return send_code(sess_store[sesit].conn_sock, SIGNUPFAIL);
        }
        write_to_log(sess_store[sesit].conn_sock, req, SYNTAXERR);
        return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
    }
    else if (strcmp(cmd, "JOIN") == 0)
    {
        char room_name[1024];
        memset(room_name, '\0', sizeof(room_name));
        char *infor_join = (char *)malloc(BUFF_SIZE);
        if (sscanf(req, "JOIN %s", room_name) == 1)
        {
            switch (join_room(room_name, sesit))
            {
            case USER_NOT_LOGINED_IN:
                write_to_log(sess_store[sesit].conn_sock, req, NOTLOGIN);
                return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
                break;
            case ALREADY_IN_ROOM:
                write_to_log(sess_store[sesit].conn_sock, req, ALREADYINROOM);
                return send_code(sess_store[sesit].conn_sock, ALREADYINROOM);
                break;
            case ROOM_NOT_FOUND:
                write_to_log(sess_store[sesit].conn_sock, req, ROOMNE);
                return send_code(sess_store[sesit].conn_sock, ROOMNE);
                break;
            case FULL_ROOM:
                write_to_log(sess_store[sesit].conn_sock, req, ROOMF);
                return send_code(sess_store[sesit].conn_sock, ROOMF);
                break;
            case ROOM_OK:
                memset(infor_join, '\0', 1024);
                strcat(infor_join, "1020");
                char temp[1000];
                if (room_store[sess_store[sesit].in_room].item_queue == NULL)
                {
                    return send_msg(sess_store[sesit].conn_sock, infor_join);
                }
                sprintf(temp, " %s %d %d",
                        room_store[sess_store[sesit].in_room].item_queue->name,
                        room_store[sess_store[sesit].in_room].item_queue->current_bid,
                        room_store[sess_store[sesit].in_room].item_queue->direct_sell_price);
                strcat(infor_join, temp);
                strcat(infor_join, "\r\n\0");
                write_to_log(sess_store[sesit].conn_sock, req, JOINNOK);
                return send_msg(sess_store[sesit].conn_sock, infor_join);
                break;
            default:
                break;
            }
        }
        write_to_log(sess_store[sesit].conn_sock, req, SYNTAXERR);
        return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
    }

    // Create Room Handle
    else if (strcmp(cmd, "ROOMCR") == 0)
    {
        char room_name[30];
        memset(room_name, '\0', sizeof(room_name));
        if (sscanf(req, "ROOMCR %s", room_name) == 1)
        {
            if (strlen(room_name) <= 0)
            {
                printf("Not found Name\n");
                write_to_log(sess_store[sesit].conn_sock, req, SYNTAXERR);
                return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
            }

            switch (create_room(room_name, sess_store[sesit]))
            {
            case 0:
                write_to_log(sess_store[sesit].conn_sock, req, ROOMCROK);
                return send_code(sess_store[sesit].conn_sock, ROOMCROK);
            case 1:
                write_to_log(sess_store[sesit].conn_sock, req, ROOMLFULL);
                return send_code(sess_store[sesit].conn_sock, ROOMLFULL);
            case 2:
                write_to_log(sess_store[sesit].conn_sock, req, ALREADYEXISTR);
                return send_code(sess_store[sesit].conn_sock, ALREADYEXISTR);
            case 3:
                write_to_log(sess_store[sesit].conn_sock, req, NOTLOGIN);
                return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
            }
        }
        write_to_log(sess_store[sesit].conn_sock, req, SYNTAXERR);
        return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
    }
    else if (strcmp(cmd, "ROOML") == 0)
    {
        if (!sess_store[sesit].is_loggedin)
        {
            write_to_log(sess_store[sesit].conn_sock, req, NOTLOGIN);
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        }
        char list_room[1024];
        memset(list_room, '\0', 1024);
        strcat(list_room, "1050 ");
        for (int i = 0; i < ROOM_NUM; i++)
        {
            if (room_store[i].userNum != -1)
            {
                strcat(list_room, room_store[i].name);
                strcat(list_room, "\n");
            }
        }
        strcat(list_room, "\r\n\0");
        write_to_log(sess_store[sesit].conn_sock, req, GETROOML);
        return send_msg(sess_store[sesit].conn_sock, list_room);
    }
    else if (strcmp(cmd, "OUTROOM") == 0)
    {
        switch (out_room(sesit))
        {
        case 1:
            write_to_log(sess_store[sesit].conn_sock, req, OUTOK);
            return send_code(sess_store[sesit].conn_sock, OUTOK);
        case 2:
            write_to_log(sess_store[sesit].conn_sock, req, NOTLOGIN);
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        case 3:
            write_to_log(sess_store[sesit].conn_sock, req, NOTINROOM);
            return send_code(sess_store[sesit].conn_sock, NOTINROOM);
        }
    }
    else if (strcmp(cmd, "ITEMADD") == 0)
    {
        char item_name[30];
        int stating_bid, direct_sell_price;
        if (sscanf(req, "ITEMADD %s %d %d", item_name, &stating_bid, &direct_sell_price) != 3)
        {
            printf("Agrument not enough\n");
            write_to_log(sess_store[sesit].conn_sock, req, SYNTAXERR);
            return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
        }
        if (stating_bid <= 0 || direct_sell_price <= 0){
            printf("Agrument invalid\n");
            write_to_log(sess_store[sesit].conn_sock, req, SYNTAXERR);
            return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
        }
        switch (addItem(item_name, stating_bid, direct_sell_price, sesit))
        {

        case 0:
            write_to_log(sess_store[sesit].conn_sock, req, ADDITEMOK);
            return send_code(sess_store[sesit].conn_sock, ADDITEMOK);
        case 1:
            write_to_log(sess_store[sesit].conn_sock, req, NOTLOGIN);
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        case 2:
            write_to_log(sess_store[sesit].conn_sock, req, NOTINROOM);
            return send_code(sess_store[sesit].conn_sock, NOTINROOM);
        case 3:
            write_to_log(sess_store[sesit].conn_sock, req, ALREADYEXISTITEM);
            return send_code(sess_store[sesit].conn_sock, ALREADYEXISTITEM);
        default:
            printf("It's a bug\n");
        }
    }
    else if (strcmp(cmd, "ITEMRETRIEVE") == 0)
    {
        char item_name[30];
        if (sscanf(req, "ITEMRETRIEVE %s", item_name) != 1)
        {
            printf("Agrument not invalid\n");
            return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
        }
        switch (retrieveItem(item_name, sesit))
        {
        case 0:
            write_to_log(sess_store[sesit].conn_sock, req, ITEMNE);
            return send_code(sess_store[sesit].conn_sock, ITEMNE);
        case 1:
            write_to_log(sess_store[sesit].conn_sock, req, NOTLOGIN);
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        case 2:
            write_to_log(sess_store[sesit].conn_sock, req, NOTINROOM);
            return send_code(sess_store[sesit].conn_sock, NOTINROOM);
        case 3:
            write_to_log(sess_store[sesit].conn_sock, req, ALREADYAUCITEM);
            return send_code(sess_store[sesit].conn_sock, ALREADYAUCITEM);
        case 4:
            write_to_log(sess_store[sesit].conn_sock, req, RETRIEVEOK);
            return send_code(sess_store[sesit].conn_sock, RETRIEVEOK);
        case 5:
            write_to_log(sess_store[sesit].conn_sock, req, ITEMNO);
            return send_code(sess_store[sesit].conn_sock, ITEMNO);
        }
    }
    else if (strcmp(cmd, "BID") == 0)
    {
        int bid;
        sscanf(req, "BID %d", &bid);
        switch (bidding(sesit, bid))
        {
        case 0:
            // return send_code(sess_store[sesit].conn_sock, BIDOK);
            write_to_log(sess_store[sesit].conn_sock, req, BIDOK);
            return 1;
        case 1:
            write_to_log(sess_store[sesit].conn_sock, req, NOTLOGIN);
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        case 2:
            write_to_log(sess_store[sesit].conn_sock, req, NOTINROOM);
            return send_code(sess_store[sesit].conn_sock, NOTINROOM);
        case 3:
            write_to_log(sess_store[sesit].conn_sock, req, NOITEM);
            return send_code(sess_store[sesit].conn_sock, NOITEM);
        case 4:
            write_to_log(sess_store[sesit].conn_sock, req, NOTHIGHER);
            return send_code(sess_store[sesit].conn_sock, NOTHIGHER);
        default:
            printf("It's a bug\n");
            return send_code(sess_store[sesit].conn_sock, 300);
        }
    }

    else if (strcmp(cmd, "BUYNOW") == 0)
    {
        switch (buynow(sesit))
        {
        case 0:
            return 1;
        case 1:
            write_to_log(sess_store[sesit].conn_sock, req, NOTLOGIN);
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        case 2:
            write_to_log(sess_store[sesit].conn_sock, req, NOTINROOM);
            return send_code(sess_store[sesit].conn_sock, NOTINROOM);
        case 3:
            write_to_log(sess_store[sesit].conn_sock, req, NOITEM);
            return send_code(sess_store[sesit].conn_sock, NOITEM);
        default:
            printf("It's a bug\n");
            return send_code(sess_store[sesit].conn_sock, 300);
            break;
        }
    }
    else if (strcmp(cmd, "LOGOUT") == 0)
    {
        if (!sess_store[sesit].is_loggedin)
        {
            write_to_log(sess_store[sesit].conn_sock, req, NOTLOGIN);
            return send_code(sess_store[sesit].conn_sock, NOTLOGIN);
        }
        out_room(sesit);
        sess_store[sesit].username[0] = '\0';
        sess_store[sesit].is_loggedin = 0;
        write_to_log(sess_store[sesit].conn_sock, req, LOGOUTOK);
        return send_code(sess_store[sesit].conn_sock, LOGOUTOK);
    }
    else
    {
        write_to_log(sess_store[sesit].conn_sock, req, SYNTAXERR);
        return send_code(sess_store[sesit].conn_sock, SYNTAXERR);
    }
    return 0;
}