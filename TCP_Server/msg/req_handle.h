#ifndef REQ_H_H
#define REQ_H_H

#include "../model/session_model.h"
#include "../model/room_model.h"

int request_handle(session sess_store[], int sesit, room room_store[], char* req);

#endif