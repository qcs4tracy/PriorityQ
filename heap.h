//
//  heap.h
//  PriorityQueue
//
//  Created by QiuChusheng on 9/25/14.
//  Copyright (c) 2014 QiuChusheng. All rights reserved.
//

#ifndef __PriorityQueue__heap__
#define __PriorityQueue__heap__

#include <stdio.h>
#include "array.h"

typedef struct heap_elem_s heap_elem_t;
typedef struct heap_s heap_t;
typedef int (*heap_cmp_t)(int prio1, int prio2);

struct heap_elem_s
{
    int prio;/*priority*/
    void *data;
};

struct heap_s
{
    array_t *array;
    heap_cmp_t cmp;
};

heap_t *create_heap(heap_cmp_t cmp_func, u_int hint);
/*build a heap*/
void build_heap(heap_t *heap);
void insert(heap_t *heap, int prio, void *data);
int delete_max(heap_t *heap, heap_elem_t *ret);

#endif /* defined(__PriorityQueue__heap__) */
