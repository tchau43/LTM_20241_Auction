#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

struct sockaddr_in clientAddr;
socklen_t clientAddrLen = sizeof(clientAddr);

/**
 * @function write_to_log: Write a log to the log file (log_2020065.txt)
 *
 * @param cmd: an int that is the command code
 * @param input: a string that user input
 * @param res: an int that is the result of command, 1 for success command esle 0
 */
void write_to_log(int conn_sock, char *req, int res)
{
    FILE *fp = fopen("log.txt", "a");
    time_t curtime;
    time(&curtime);
    struct tm tm = *localtime(&curtime);
    char clientIP[INET_ADDRSTRLEN];
    if (getpeername(conn_sock, (struct sockaddr *)&clientAddr, &clientAddrLen) == 0)
    {
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
    }
    else
    {
        perror("getpeername");
    }

    char *result;
    if ((res / 1000) % 10 == 1)
    {
        result = "+OK";
    }
    else
    {
        result = "-ERR";
    }
    fprintf(fp, "[%02d/%02d/%d %02d:%02d:%02d] $ %s $ %s $ %s $ %d \n",
            tm.tm_mday, tm.tm_mon, tm.tm_year,
            tm.tm_hour, tm.tm_min, tm.tm_sec,
            clientIP, req, result, res);
    fclose(fp);
}