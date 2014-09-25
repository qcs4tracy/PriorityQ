//
//  heap.c
//  PriorityQueue
//
//  Created by QiuChusheng on 9/25/14.
//  Copyright (c) 2014 QiuChusheng. All rights reserved.
//

#include "heap.h"


void heapify(heap_t *heap, int el)
{
    int j, hsize = heap->array->nelts - 1;/*the first element is not used*/
    array_t *a = heap->array;
    heap_elem_t *tmp = (heap_elem_t *)a->elts;
    heap_elem_t swap;
    
    while (el*2 <= hsize) {
        j = 2*el;
        /*right node has larger prio value*/
        if ( (j+1) <= (a->nelts -1) && heap->cmp(tmp[j].prio, tmp[j+1].prio) < 0) { j++; }
        
        if (heap->cmp(tmp[el].prio, tmp[j].prio) > 0) { break; }
        
        /*swap of the two elements*/
        swap = tmp[el];
        tmp[el] = tmp[j];
        tmp[j] = swap;
        
        el = j;
    }

}

int delete_max(heap_t *heap, heap_elem_t *ret)
{
    heap_elem_t *max = (heap_elem_t *)array_elem(heap->array, 1);
    heap_elem_t *last = (heap_elem_t *)array_elem(heap->array, heap->array->nelts - 1);
    
    if (max != NULL) {
        /*copy the fields to element pointed by ret*/
        ret->prio = max->prio;
        ret->data = max->data;
        *max = *last;
        heap->array->nelts--;
        heapify(heap, 1);
        return 0;
    }
    
    return -1;
}


void percolate_up(heap_t *heap, int el)
{
    heap_elem_t *elms = (heap_elem_t *)heap->array->elts;
    int j, prio0;
    if (heap->array->nelts - 1  < el || el <= 0) return;
    //put it on the index 0 element which is unused by heap
    elms[0] = elms[el];
    prio0 = elms[0].prio;
    
    while (el/2 >= 1) {
        
        j = el/2;
        /*the parent of current node is larger than the node, finish*/
        if (heap->cmp(prio0, elms[j].prio) <= 0) break;
        
        elms[el] = elms[j];
        el = j;
    }
    
    elms[el] = elms[0];

}

void insert(heap_t *heap, int prio, void *data)
{
    heap_elem_t *elm = (heap_elem_t *)array_push(heap->array);
    elm->prio = prio;
    elm->data = data;
    percolate_up(heap, heap->array->nelts - 1);
}


heap_t *create_heap(heap_cmp_t cmp_func, u_int hint)
{
    mem_pool_t *pool = mem_create_pool(MEM_POOL_FAIL_THREHOLD);
    heap_t *new = mem_palloc(pool, sizeof(heap_t));
    new->array = create_array(pool, hint + 1, sizeof(heap_elem_t));
    array_push(new->array);
    new->cmp = cmp_func;
    
    return new;
}

/*build a heap*/
void build_heap(heap_t *heap)
{
    int i = (heap->array->nelts - 1) / 2;
    /* [1, nelt-1] would be the interval of heap*/
    for (; i >= 1; i--) {
        heapify(heap, i);
    }
}





