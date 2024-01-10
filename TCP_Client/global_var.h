#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include<pthread.h>

#define APP_NAME "AUC(I)TION"

extern pthread_mutex_t state_mutex;

// State of menu
/**
 * -1 block, wait hear_thread answer
 *  0 exit
 *  1 auth menu
 *  2 main menu
 *  3 inroom menu
 *  4 for testing program
 * */
extern int state;

#endif