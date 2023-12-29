#include <stdio.h>
#include <stdio.h>

#include "response.h"

void res_code_resolver(int res_code)
{
    switch (res_code)
    {

    // Fof Login
    case LOGINOK:
        printf("Login successfully\n");
        break;
    case ACCBLOCK:
        printf("Account is banned\n");
        break;
    case UNAMENF:
        printf("Account's not found\n");
        break;
    case ALREADYLOGIN:
        printf("Already Login\n");
        break;

    // For logout
    case NOTLOGIN:
        printf("You are not logged in\n");
        break;
    case LOGOUTOK:
        printf("Logout successfully\n");
        break;

    // For create Room
    case ROOMCROK:
        printf("Create room successfully\n");
        break;
    case ALREADYEXISTR:
        printf("Room is already exist\n");
        break;
    case ROOMLFULL:
        printf("Server's room list is full, try another time\n");
        break;

    // For add item
    case ADDITEMOK:
        printf("Item added successfully\n");
        break;
    case ALREADYEXISTITEM:
        printf("Item is already exist\n");
        break;
    
    //For bidding

    case BIDOK:
        printf("\n");
        break;
    case NOTHIGHER:

    case NOITEM:

    // For buynew
    case BUYOK:

    default:
        printf("Syntax error\n");
        break;
    }
}