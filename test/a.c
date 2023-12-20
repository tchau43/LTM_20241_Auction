#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "item.h"
#include "room.h"

#define ROOM_LIST_SIZE 3

void *timecounter(void *a)
{
    sleep(10);
    printf("thread end\n");
}

int main()
{
    int cmd;
    room *roomlist = (room *)malloc(sizeof(room) * ROOM_LIST_SIZE);
    init_roomlist(roomlist, ROOM_LIST_SIZE);

    session sess;
    sess.is_loggedin = 1;
    sess.in_room = -1;
    sess.conn_sock = 1;
    strcpy(sess.username, "abc\0");

    session sess2;
    sess2.is_loggedin = 1;
    sess2.in_room = -1;
    sess2.conn_sock = 1;
    strcpy(sess2.username, "abc\0");

    printf("%d\n", create_room(roomlist, ROOM_LIST_SIZE, "lmao1", sess));
    printf("%d\n", create_room(roomlist, ROOM_LIST_SIZE, "lmao2", sess));

    printf("%d\n", join_room(roomlist, ROOM_LIST_SIZE,"lmao1", &sess, 2));   
    printf("%d\n", join_room(roomlist, ROOM_LIST_SIZE,"lmao1", &sess2, 2)); 

    printf("%d\n", addItem("item1", 100, 1000, &roomlist[sess.in_room], sess, 2));
    printf("%d\n", addItem("item2", 100, 1000, &roomlist[sess.in_room], sess, 2));
    printf("%d\n", addItem("item3", 100, 1000, &roomlist[sess.in_room], sess, 2));
    showAllItemInRoom(roomlist[0]);
}

void showAllItemInRoom(room r){
    item *i = r.item_queue;
    while(i!=NULL){
        printf("%s ||", i->name);
        i = i->next;
    }
    printf("\n");

}