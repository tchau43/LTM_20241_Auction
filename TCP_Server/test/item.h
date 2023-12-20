#ifndef ITEM_H
#define ITEM_H

#include "room.h"
/*Item define struct*/
typedef struct item{
    char name[30];
    int owner;
    int current_bid;
    int best_cus;
    int direct_sell_price;
    struct item * next;
}item;
/*Create new item*/
item* create_item_node(char name[], int owner, int start_bid, int direct_sell_price);
/*Add new item to queue*/
void push_item(item** queue, item* it);
/*Pop item from queue
*/
int pop_item(item** queue);
/**
 * Delete item from queue
 * @return :1 if not found, 0 if deleted first one found
*/
int delete_item(item** queue, char name[]);
/**
 * Get item list
 * @param queue: queue that use to get
 * @return : 1 if OK
*/
int addItem(char name[], int start_bid, int direct_sell_price, room* room_join, session sess, int sesit);

#endif