#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "item.h"
void *timecounter(void *a)
{
    sleep(10);
    printf("thread end\n");
}

int main()
{
    item* queue = NULL;
    push_item(&queue, create_item_node("lmao2", 2, 123, 456));
    push_item(&queue, create_item_node("lmao3", 2, 123, 456));
    push_item(&queue, create_item_node("lmao4", 2, 123, 456));
    push_item(&queue, create_item_node("lmao5", 2, 123, 456));
    delete_item(&queue, "lmao3");
    pop_item(&queue);
    item *it = queue;
    while (it != NULL)
    {
        printf("%s ||  ", it->name);
        it = it->next;
    }
}