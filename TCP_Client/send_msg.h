#ifndef SEND_H
#define SEND_H

/**
 * @function send_msg: send message to server
 *
 * @param conn_sock: connection socket number
 * @param msg: message want to send
 */
void send_msg(int conn_sock, char* msg);

#endif