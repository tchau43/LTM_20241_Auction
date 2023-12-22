#include<stdlib.h>
#include<string.h>

#include "item.h"

item* create_item_node(char name[], int owner, int start_bid, int direct_sell_price){
    item* node = (item*)malloc(sizeof(item));
    strcpy(node->name, name);
    node->owner = owner;
    node->current_bid = start_bid;
    node->best_cus = -1;
    node->direct_sell_price = direct_sell_price;
    node->next = NULL;
    return node;
}
/*Add new item to queue*/
void push_item(item** queue, item* it){
    if(*queue == NULL){
        *queue = it;
    }
    else{
        push_item(&((*queue)->next), it);
    }
}
/*Delete item from queue*/
int delete_item(item** queue, char name[]){
    item* current = *queue;
    item* prev = NULL;
    while(current != NULL){
        if(strcmp(current->name, name) ==0){
            if(prev != NULL){
                prev->next = current->next;
            }
            else{
                (*queue) = current->next;
            }
            free(current);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    return 1;
}

int pop_item(item** queue){
   return delete_item(queue, (*queue)->name);
}

int find_item(item* queue, char item_name[]){
    item* i = queue;
    while(i!=NULL){
        if(strcmp(i->name, item_name) == 0)
            return 1;
        i = i->next;
    }
    return 0;
}

int addItem(char name[], int start_bid, int direct_sell_price, int sesit){
    if(!sess_store[sesit].is_loggedin)
        return 1;
    if(sess_store[sesit].in_room == -1)
        return 2;
    if(find_item(room_store[sess_store[sesit].in_room].item_queue, name))
        return 3;
    item* new_item = create_item_node(name, sesit, start_bid, direct_sell_price);
    push_item(&(room_store[sess_store[sesit].in_room].item_queue), new_item);
    return 0;
}