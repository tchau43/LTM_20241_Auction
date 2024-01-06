#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include <pthread.h>

#include "../model/room_model.h"
#include "../model/session_model.h"

#define ROOM_NUM 30

extern pthread_mutex_t room_mutex;
extern session *sess_store;
extern room *room_store;

#endif