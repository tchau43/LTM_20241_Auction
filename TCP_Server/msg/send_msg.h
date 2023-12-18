#ifndef SENDMSG_H
#define SENDMSG_H

/**
 * @def send_msg: send message to client via sockfd
 *
 * @param sockfd: number of socket that use to send message
 * @param res_code: result code of request
 * 
 * @return :1 if success
 *          0 if get an error
 */
int send_msg(int sockfd, int res_code);

#endif