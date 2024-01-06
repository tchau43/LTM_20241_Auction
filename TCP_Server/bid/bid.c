#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "../val/global_var.h"
#include "../val/response.h"
#include "../item/item.h"
#include "../msg/send_msg.h"

#define EXTENDSTIME 10
#define EXTENDSTIME2 3
#define INCREMENTSTEP 5

void room_anno(int room_it, char *msg, int anno_type);

void *auction_start(void *roomit)
{
    pthread_detach(pthread_self());
    int it = *((int *)roomit);
    char new_item_msg[BUFF_SIZE];
    sprintf(new_item_msg, "%s %s %d %d",
            room_store[it].name,
            room_store[it].item_queue->name,
            room_store[it].item_queue->current_bid,
            room_store[it].item_queue->direct_sell_price);
    room_anno(it, new_item_msg, NEWITEMARRIVED);

    sleep(EXTENDSTIME);
    printf("Auction pharse start for %s\n", (room_store[it].item_queue)->name);
    for (int i = 0; i < 3; i++)
    {
        char cd_msg[BUFF_SIZE];
        sprintf(cd_msg, "%s %s %d %d",
                room_store[it].name,
                room_store[it].item_queue->name,
                room_store[it].item_queue->current_bid,
                i + 1);
        room_anno(it, cd_msg, COUNTDOWN);
        sleep(EXTENDSTIME2);
        // printf("%s : %d\n", (room_store[it].item_queue)->name, (room_store[it].item_queue)->current_bid);
    }
    if (room_store[it].item_queue->best_cus == -1)
    {
        printf("Outbid\n");
    }
    else
    {
        char sold_msg[BUFF_SIZE];
        sprintf(sold_msg, "%s %s %s",
                room_store[it].name,
                sess_store[room_store[it].item_queue->best_cus].username,
                room_store[it].item_queue->name);
        room_anno(it, sold_msg, SOLDED);
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

    char new_bid_msg[BUFF_SIZE];
    sprintf(new_bid_msg, "%s %s %s %d\r\n",
            room_store[sess_store[sesit].in_room].name,
            room_store[sess_store[sesit].in_room].item_queue->name,
            sess_store[sesit].username,
            bid);
    printf("Anno: %s\n", new_bid_msg);
    send_code(sess_store[sesit].conn_sock, BIDOK);
    room_anno(sess_store[sesit].in_room, new_bid_msg, NEWBID);
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
    char sold_msg[BUFF_SIZE];
    sprintf(sold_msg, "%s %s %s",
            room_store[sess_store[sesit].in_room].name,
            sess_store[sesit].username,
            room_store[sess_store[sesit].in_room].item_queue->name);
    room_anno(sess_store[sesit].in_room, sold_msg, SOLDED);
    
    printf("Item sold to %s\n", sess_store[sesit].username);
    pop_item(&(room_store[sess_store[sesit].in_room].item_queue));
    if (room_store[sess_store[sesit].in_room].item_queue != NULL)
        start_auction(sess_store[sesit].in_room);
    send_code(sess_store[sesit].conn_sock, BUYOK);

    return 0;
}

void room_anno(int room_it, char *msg, int anno_type)
{
    if (room_store[room_it].userNum < 0)
        return;
    for (int i = 0; i < ROOM_SIZE; i++)
    {
        if (room_store[room_it].userList[i] >= 0)
        {
            printf("Anno to %s\n", sess_store[room_store[room_it].userList[i]].username);
            send_code(sess_store[room_store[room_it].userList[i]].conn_sock, anno_type);
            send_msg(sess_store[room_store[room_it].userList[i]].conn_sock, msg);
        }
    }
}