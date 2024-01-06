#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>

void send_msg(int conn_sock, char *msg)
{
    int sent_bytes = send(conn_sock, msg, strlen(msg), 0);
    if (sent_bytes < 0)
        perror("\nError: ");
}