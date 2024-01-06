#ifndef ROOM_H
#define ROOM_H

#include<pthread.h>
#include "item_model.h"

#define ROOM_SIZE 30
#define ROOM_NAME_MAX_LENGTH 30

/**
 * Struct describe room
*/
typedef struct room{
    char name[ROOM_NAME_MAX_LENGTH];
    int userNum;
    int userList[ROOM_SIZE];
    item* item_queue;
    pthread_t time_counter;
}room;

/**
 * Struct of data that use to send roomlist
*/
typedef struct room_data {
    char name[ROOM_NAME_MAX_LENGTH];
    int userNum;
} room_data;


#endif