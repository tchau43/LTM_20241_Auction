#ifndef ROOM_H
#define ROOM_H

#include<pthread.h>
#include "item.h"
#include "session.h"

typedef struct room{
    char name[30];
    int userNum;
    int userList[30];
    item* item_queue;
    pthread_t time_counter;
}room;

/**
 * Init roomlist
 * @param roomlist: roomlist use to init
 * @param n: number of room
*/
void init_roomlist(room roomlist[], int n);

/**
 * add room to roomlist
 * @param roomlist: list of room
 * @param name:room name 
 * @param n: size of roomlist
 * 
 * @return :0 if Ok,
 *          1 if roomlist full,
 *          2 if room already exist,
 *          3 if user not logged in
*/
int create_room(room roomlist[],int n, char name[], session sess);

/**
 * Join room handle
 * 
 * @param roomlist: list of room
 * @param room_n: number of room
 * @param room_name: name of room want to join
 * @param sess: session of user use to join room
 * @param sesit: iterator that is index of session in session store
 * 
 * @return :0 if OK,
 *          1 if not logged in,
 *          2 if already in room,
 *          3 if room is full,
 *          4 if room is not found
*/
int join_room(room roomlist[], int room_n,char room_name[], session* sess, int sesit);


#endif