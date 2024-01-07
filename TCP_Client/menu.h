#ifndef MENU_H
#define MENU_H

#define LOGIN_CMD "LOGIN"
#define SIGNUP_CMD "SIGNUP"

#define LOGOUT_CMD "LOGOUT"
#define ROOM_LIST_CMD "ROOML"
#define ROOM_CREATE_CMD "ROOMCR"
#define JOIN_ROOM_CMD "JOIN"

#define ADD_ITEM_CMD "ITEMADD"
#define DELETE_ITEM_CMD "ITEMRETRIEVE"
#define BID_CMD "BID"
#define BUYOUT_CMD "BUYNOW"
#define OUTROOM_CMD "OUTROOM"

void auth_menu();
void main_menu();
void inroom_menu();
void auth_menu_handle(int conn_sock, char* buff);
void main_menu_handle(int conn_sock, char* buff);
void in_room_handle(int conn_sock, char* buff);

#endif