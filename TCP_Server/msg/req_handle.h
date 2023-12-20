#ifndef REQ_H_H
#define REQ_H_H

#include "../model/session_model.h"

int request_handle(int conn_sock, char* req, session* sess);

#endif