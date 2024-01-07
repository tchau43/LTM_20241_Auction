#ifndef ITEM_H
#define ITEM_H

#include "../val/global_var.h"
#include "../model/item_model.h"
#include "../model/room_model.h"
#include "../model/session_model.h"
/***
 * Create new item
 * 
 * @param queue: pointer of item queue
 * 
 * @return :1 if success
 *          0 if no item
*/
int pop_item(item** queue);
/**
 * Get item list
 * 
 * @param name: name of item
 * @param start_bid: starting bid
 * @param direct_sell_price: buyout price
 * @param sesit: iterator of session in session storage
 * 
 * @return :0 if OK
 *          1 if not logged in
 *          2 if not in room
 *          3 if item already exist 
*/
int addItem(char name[], int start_bid, int direct_sell_price, int sesit);
int retrieveItem(char item_name[],int sesit);
#endif