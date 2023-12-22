#ifndef SESS_HANDLE_H
#define SESS_HANDLE_H

#include "../model/session_model.h"
#include "../val/global_var.h"
/**
 * Init session store
 * 
 * @param sess_store: session storage
 * @param sess_n: max size of session in storage
*/

void init_session_store();

int create_new_session(int conn_sock);

#endif