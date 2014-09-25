//
//  main.c
//  PriorityQueue
//
//  Created by QiuChusheng on 9/25/14.
//  Copyright (c) 2014 QiuChusheng. All rights reserved.
//

#include <stdio.h>
#include "heap.h"

#define HINT 10
int gt(int n1, int n2) { return n1 - n2; }
int lt(int n1, int n2) { return n2 - n1; }

int main(int argc, char *argv[])
{
    heap_t *heap;
    array_t *a;
    heap_elem_t *h1, h2;
    int i;
    heap = create_heap(gt, HINT);
    a = heap->array;
    h1 = array_push_n(a, HINT);
    
    for (i = 0; i < HINT; i++) {
        h1[i].prio = HINT - i;
        h1[i].data = NULL;
    }
    
    build_heap(heap);
    
    insert(heap, HINT+1, NULL);
    
    for (i = 0; i < HINT; i++) {
        delete_max(heap, &h2);
        printf("h2.prio=%d\n", h2.prio);
    }

    return 0;
}