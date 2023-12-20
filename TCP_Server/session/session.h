#ifndef SESS_HANDLE_H
#define SESS_HANDLE_H

#include "../model/session_model.h"
/**
 * Init session store
 * 
 * @param sess_store: session storage
 * @param sess_n: max size of session in storage
*/

void init_session_store(session sess_store[], int sess_n);

int create_new_session(session sess_store[], int sess_n, int conn_sock);

#endif