#include<stdio.h>
#include<string.h>
#include"auth_handle.h"
#define BUFF_SIZE 1024

enum AuthStatus login_handle(char *username, char *password)
{
    FILE *fp = fopen("account.txt", "r");
    char line[BUFF_SIZE];
    char check_name[1000];
    int acc_state;
    char check_password[1000];
    while (fgets(line, BUFF_SIZE, fp) != NULL)
    {
        sscanf(line, "%s %d %s", check_name, &acc_state, check_password);
        if (!strcmp(username, check_name))
        {
            if (acc_state)
            {
                if (!strcmp(password, check_password))
                {
                    return LOGIN_SUCCESS;
                }
                else
                    return INCORRECT_PASSWORD;
            }
            else
            {
                return LG_USER_BLOCK;
            }
        }
    }
    fclose(fp);
    return LG_USER_NOT_EXIST;
}
// void logout_handle(){

// }
int check_account_exist(char *username)
{
    FILE *fp = fopen("account.txt", "r");
    char line[BUFF_SIZE];
    char check_name[1000];
    while (fgets(line, BUFF_SIZE, fp) != NULL)
    {
        sscanf(line, "%s", check_name);
        if (!strcmp(username, check_name))
            return 1;
    }
    fclose(fp);
    return 0;
}

int signup_handle(char *username, char *password)
{
    if (check_account_exist(username))
    {
        return 0;
    }
    else
    {
        FILE *fp = fopen("account.txt", "a");
        if (fp == NULL)
        {
            perror("Error opening file");
            return -1; // Return an error code
        }

        fprintf(fp, "%s 1 %s\n", username, password);
        fclose(fp);
        return 1;
    }
}