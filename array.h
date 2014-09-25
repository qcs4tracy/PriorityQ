//
//  array.h
//  array
//
//  Created by TracyQCS on 13-7-10.
//  Copyright (c) 2013年 TracyQCS. All rights reserved.
//

#ifndef array_array_h
#define array_array_h

#include "palloc.h"

typedef struct array_s array_t;

struct array_s {
    void *elts;
    u_int nelts;
    size_t size;
    u_int nalloc;
    mem_pool_t *pool;
};

array_t *create_array(mem_pool_t *p, u_int n, size_t size);
void array_destroy(array_t *a);
void *array_push(array_t *a);
void *array_push_n(array_t *a, u_int n);
void *array_elem(array_t *a, int n);

static inline u_int array_init(array_t *array, mem_pool_t *pool, u_int n, size_t size)
{
    /*
     * set "array->nelts" before "array->elts", otherwise MSVC thinks
     * that "array->nelts" may be used without having been initialized
     */
    
    array->nelts = 0;
    array->size = size;
    array->nalloc = n;
    array->pool = pool;
    
    array->elts = mem_palloc(pool, n * size);
    if (array->elts == NULL) {
        return MEM_FAILED;
    }
    
    return MEM_OK;
}

#endif

