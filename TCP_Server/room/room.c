#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "room.h"

void init_roomlist(room roomlist[], int n)
{
    for (int i = 0; i < n; i++)
    {
        roomlist[i].item_queue = NULL;
        roomlist[i].userNum = -1;
        for (int j = 0; j < 30; j++)
        {
            roomlist[i].userList[j] = -1;
        }
    }
    printf("Init room\n");
}

int findRoom(room roomlist[], int n, char room_name[])
{
    for (int i = 0; i < n; i++)
    {
        if (!strcmp(roomlist[i].name, room_name))
        {
            return i;
        }
    }
    return -1;
}

enum RoomStatus create_room(room roomlist[], int n, char name[], session sess)
{
    if (!sess.is_loggedin)
        return USER_NOT_LOGINED_IN;
    if (findRoom(roomlist, n, name) != -1)
        return ROOM_ALREADY_EXIST;

    for (int i = 0; i < n; i++)
    {
        if (roomlist[i].userNum == -1)
        {
            roomlist[i].userNum = 0;
            strncpy(roomlist[i].name, name, 30);
            printf("Create success\n");
            return ROOM_OK;

        }
    }
    return FULL_ROOM;
}

enum RoomStatus join_room(room roomlist[], int room_n, char room_name[], session* sess, int sesit)
{
    if (!sess->is_loggedin)
    {
        return USER_NOT_LOGINED_IN;
    }

    if (sess->in_room != -1)
    {
        return ROOM_ALREADY_EXIST;
    }

    int it = findRoom(roomlist, room_n, room_name);
    if (it == -1)
        return ROOM_NOT_FOUND;

    if (roomlist[it].userNum == 3)
        return FULL_ROOM;

    for (int i = 0; i < 3; i++)
    {
        if (roomlist[it].userList[i] == -1)
        {
            roomlist[it].userNum++;
            roomlist[it].userList[i] = sesit;
            sess->in_room = it;
            printf("Join success:%d|%d\n", roomlist[it].userNum, it);
            return ROOM_OK;
        }
    }

    return UNIDENTIFIED;
}
