#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFF_SIZE 1024

/**
 * @def send_msg: send message to client via sockfd
 *
 * @param sockfd: number of socket that use to send message
 * @param res_code: result code of request
 * 
 * @return :1 if success
 *          0 if get an error
 */

int send_code(int sockfd, int res_code)
{
    char buff[4];
    int sent_bytes, received_bytes;

    memset(buff, '\0', sizeof(buff));
    sprintf(buff, "%d", res_code);

    sent_bytes = send(sockfd, &buff, 4, 0);
    if (sent_bytes < 0)
    {
        perror("\nError6:");
        return 0;
    }
    printf("Sent: %s\n", buff);
    return 1;
}

int send_msg(int sockfd, char *msg){
    char buff[BUFF_SIZE];
    strcpy(buff, msg);
    strcat(buff, "\r\n\0");
    int sent_bytes = send(sockfd, &buff, strlen(buff), 0);
    if (sent_bytes < 0)
    {
        perror("\nError6:");
        return 0;
    }
    printf("Sent: %s\n", buff);
    return 1;
}