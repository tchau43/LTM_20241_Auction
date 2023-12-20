#ifndef ITEM_M_H
#define ITEM_M_H
/*Item define struct*/
typedef struct item
{
    char name[30];
    int owner;
    int current_bid;
    int best_cus;
    int direct_sell_price;
    struct item *next;
} item;
#endif