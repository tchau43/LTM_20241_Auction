#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "../val/global_var.h"
#include "../item/item.h"

#define EXTENDSTIME 10
#define EXTENDSTIME2 3
#define INCREMENTSTEP 5

void *auction_start(void *roomit)
{
    int it = *((int *)roomit);
    sleep(EXTENDSTIME);
    printf("Auction pharse start for %s\n", (room_store[it].item_queue)->name);
    for (int i = 0; i < 3; i++)
    {
        sleep(EXTENDSTIME2);
        printf("%s : %d\n", (room_store[it].item_queue)->name, (room_store[it].item_queue)->current_bid);
    }
    if (room_store[it].item_queue->best_cus == -1)
    {
        printf("Outbid\n");
    }
    else
    {
        printf("Item sold to %s\n", sess_store[room_store[it].item_queue->best_cus].username);
    }
    pthread_mutex_lock(&room_mutex);
    pop_item(&(room_store[it].item_queue));
    pthread_mutex_unlock(&room_mutex);

    if (room_store[it].item_queue != NULL)
        auction_start(roomit);
    else
        pthread_exit(NULL);
}

void start_auction(int roomit)
{

    int *it = malloc(sizeof(int));
    if (it == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for thread argument\n");
        exit(EXIT_FAILURE);
    }

    *it = roomit;
    printf("Auction start for %s in room %d\n", (room_store[*it].item_queue)->name, *it);
    pthread_create(&(room_store[roomit].time_counter), NULL, &auction_start, it);
}

int bidding(int sesit, int bid)
{
    if (!sess_store[sesit].is_loggedin)
        return 1;
    if (sess_store[sesit].in_room == -1)
        return 2;
    if (room_store[sess_store[sesit].in_room].item_queue == NULL)
        return 3;
    if (bid - INCREMENTSTEP < room_store[sess_store[sesit].in_room].item_queue->current_bid)
        return 4;
    pthread_cancel(room_store[sess_store[sesit].in_room].time_counter);
    room_store[sess_store[sesit].in_room].item_queue->best_cus = sesit;
    room_store[sess_store[sesit].in_room].item_queue->current_bid = bid;
    start_auction(sess_store[sesit].in_room);
    printf("Item %s get new bid from %s :%d",
           room_store[sess_store[sesit].in_room].item_queue->name,
           sess_store[sesit].username,
           bid);
    return 0;
}

int buynow(int sesit)
{
    if (!sess_store[sesit].is_loggedin)
        return 1;
    if (sess_store[sesit].in_room == -1)
        return 2;
    if (room_store[sess_store[sesit].in_room].item_queue == NULL)
        return 3;
    pthread_cancel(room_store[sess_store[sesit].in_room].time_counter);
    printf("Item sold to %s\n", sess_store[sesit].username);
    pop_item(&(room_store[sess_store[sesit].in_room].item_queue));
    if (room_store[sess_store[sesit].in_room].item_queue != NULL)
        start_auction(sess_store[sesit].in_room);
    return 0;
}