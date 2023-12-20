#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "session.h"

void init_session_store(session sess_store[], int sess_n){
    for(int i = 0; i<sess_n; i++){
        sess_store[i].conn_sock = -1;
    }
}

int create_new_session(session sess_store[], int sess_n, int conn_sock){
    for(int i=0; i<sess_n; i++){
        if(sess_store[i].conn_sock <0){
            sess_store[i].conn_sock = conn_sock;
            sess_store[i].in_room = -1;
            sess_store[i].is_loggedin = 0;
            memset(sess_store[i].username, '\0', sizeof(sess_store[i].username));
            memset(sess_store[i].buff, '\0', sizeof(sess_store[i].buff));
            printf("session %d\n", i);
            return i;
        }
    }
    return -1;
}