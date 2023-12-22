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

#define BUFF_SIZE 1024
#define ROOM_NUM 30

/***
 * @function check_auth: Read the account information file and check the state of account
 *
 * @param username: a string to a input username
 *
 * @return: 1 if success
 *          0 if account is banned
 *          -1 if account is not found
 *
 *
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

int request_handle(session sess_store[], int sesit, room roomstore[], char *req)
{
    char cmd[10];
    sscanf(req, "%s", cmd);
    if (strcmp(cmd, "LOGIN") == 0)
    {
        return send_msg(sess_store[sesit].conn_sock, 300);
    }

    else if (strcmp(cmd, "JOIN") == 0)
    {
        return send_msg(sess_store[sesit].conn_sock, 300);
    }

    // Create Room Handle
    else if (strcmp(cmd, "ROOMCR") == 0)
    {
        char room_name[30];
        sscanf(req, "ROOMCR %s", room_name);
        if (strlen(room_name) <= 0){
            printf("Not found Name\n");
            return send_msg(sess_store[sesit].conn_sock, SYNTAXERR);
        }

        switch (create_room(roomstore, ROOM_NUM, room_name, sess_store[sesit]))
        {
        case 0:
            return send_msg(sess_store[sesit].conn_sock, ROOMCROK);
        case 1:
            return send_msg(sess_store[sesit].conn_sock, ROOMLFULL);
        case 2:
            return send_msg(sess_store[sesit].conn_sock, ALREADYEXISTR);
        case 3:
            return send_msg(sess_store[sesit].conn_sock, NOTLOGIN);
        }
    }

    else if (strcmp(cmd, "JOIN") == 0)
    {
        return send_msg(sess_store[sesit].conn_sock, 300);
    }

    else if (strcmp(cmd, "ITEMADD") == 0)
    {
        char item_name[30];
        int stating_bid, direct_sell_price;
        if (sscanf(req, "ITEMADD %s %d %d", item_name, &stating_bid, &direct_sell_price) != 3)
        {   
            printf("Agrument not enough\n");
            return send_msg(sess_store[sesit].conn_sock, SYNTAXERR);
        }
        switch (addItem(item_name, stating_bid, direct_sell_price, roomstore, sess_store[sesit], sesit))
        {
        case 0:
            return send_msg(sess_store[sesit].conn_sock, ADDITEMOK);
        case 1:
            return send_msg(sess_store[sesit].conn_sock, NOTLOGIN);
        case 2:
            return send_msg(sess_store[sesit].conn_sock, NOTINROOM);
        case 3:
            return send_msg(sess_store[sesit].conn_sock, ALREADYEXISTITEM);
        default:
            printf("It's a bug\n");
            return send_msg(sess_store[sesit].conn_sock, 300);
        }
    }

    else if (strcmp(cmd, "BYE") == 0)
    {
        return send_msg(sess_store[sesit].conn_sock, 300);
    }
    else
        return send_msg(sess_store[sesit].conn_sock, 300);

    return 0;
}