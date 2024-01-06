
enum AuthStatus
{
    LOGIN_SUCCESS,
    LG_OTHER_CLIENT,
    LG_USER_NOT_EXIST,
    INCORRECT_PASSWORD
};
enum AuthStatus login_handle(char *username, char *password);
int check_account_exist();
// void logout_handle();
int signup_handle();