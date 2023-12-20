#ifndef ROOM_H
#define ROOM_H

#include<pthread.h>
#include "item_model.h"
#include "session.h"

typedef struct room{
    char name[30];
    int userNum;
    int userList[30];
    item* item_queue;
    pthread_t time_counter;
}room;

#endif