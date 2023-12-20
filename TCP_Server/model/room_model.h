#ifndef ROOM_H
#define ROOM_H

#include<pthread.h>
#include "item_model.h"

#define ROOM_SIZE 30

typedef struct room{
    char name[30];
    int userNum;
    int userList[ROOM_SIZE];
    item* item_queue;
    pthread_t time_counter;
}room;

#endif