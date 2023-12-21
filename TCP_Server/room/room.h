#ifndef ROOM_H_H
#define ROOM_H_H

#include"../model/room_model.h"
#include"../model/session_model.h"

enum RoomStatus {
    ROOM_OK,
    FULL_ROOM,
    ROOM_ALREADY_EXIST,
    USER_NOT_LOGINED_IN,
    ROOM_NOT_FOUND,
    UNIDENTIFIED
}

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
 * @return : room status 
*/


enum RoomStatus create_room(room roomlist[],int n, char name[], session sess);

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


enum RoomStatus join_room(room roomlist[], int room_n,char room_name[], session* sess, int sesit);

#endif
