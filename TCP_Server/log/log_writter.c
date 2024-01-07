#include <stdio.h>
#include <time.h>

/**
 * @function write_to_log: Write a log to the log file (log_2020065.txt)
 * 
 * @param cmd: an int that is the command code
 * @param input: a string that user input
 * @param res: an int that is the result of command, 1 for success command esle 0
*/
void write_to_log(int cmd, char* input, int res)
{
    FILE *fp = fopen("log_20200651.txt", "a");
    time_t curtime;
    time(&curtime);
    struct tm tm = *localtime(&curtime);
    char* result;
    if(res){
        result = "+OK";
    }
    else{
        result = "-ERR";
    }
    fprintf(fp, "[%02d/%02d/%d %02d:%02d:%02d] $ %d $ %s $ %s \n",
     tm.tm_mday, tm.tm_mon, tm.tm_year,
      tm.tm_hour, tm.tm_min, tm.tm_sec, 
      cmd, input, result);
    fclose(fp);
}