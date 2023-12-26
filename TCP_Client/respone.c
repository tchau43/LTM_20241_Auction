#include <stdio.h>
#include <stdio.h>

#include "response.h"

void res_code_resolver(int res_code)
{
    switch (res_code)
    {
    case 110:
        printf("Login successfully\n");
        break;
    case 211:
        printf("Account is banned\n");
        break;
    case 212:
        printf("Account's not found\n");
        break;
    case 213:
        printf("Already Login\n");
        break;
    case 120:
        printf("Post successfully\n");
        break;
    case 221:
        printf("You are not logged in\n");
        break;
    case 130:
        printf("Logout successfully\n");
        break;
    default:
        printf("Syntax error\n");
        break;
    }
}