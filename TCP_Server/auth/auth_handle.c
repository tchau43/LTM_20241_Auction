#include<stdio.h>

void login_handle(){

}

void logout_handle(){

}

void signup_handle(char *username){
    if(check_account_exist(username)){
        return;
    }
    

}

int check_account_exist(char *username){
    FILE *fp = fopen("account.txt", "r");
    char line[1024];
    char check_name[1000];
    while (fgets(line, 1024, fp) != NULL)
    {
        sscanf(line, "%s", check_name);
        if (!strcmp(username, check_name))
        {
            return 1;
        }
    }
    fclose(fp);
    return 0;
}