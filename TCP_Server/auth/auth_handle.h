
enum AuthStatus
{
    LOGIN_SUCCESS,
    LG_USER_BLOCK,
    LG_USER_NOT_EXIST,
    INCORRECT_PASSWORD
};
enum AuthStatus login_handle();
int check_account_exist();
// void logout_handle();
int signup_handle();