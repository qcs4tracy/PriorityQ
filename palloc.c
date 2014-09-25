//
//  palloc.c
//  array
//
//  Created by TracyQCS on 13-7-10.
//  Copyright (c) 2013年 TracyQCS. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "palloc.h"

static void *mem_palloc_block(mem_pool_t *pool, size_t size);
static void *mem_palloc_large(mem_pool_t *pool, size_t size);

void *mem_alloc(size_t size/*, log_t *log */){
    
    void *p;
    p = malloc(size);
    
    if (p == NULL) {
        return NULL;
    }
    
    return p;
}


void *mem_calloc(size_t size/*, log_t *log */){

    void *p;
    p = malloc(size);
    
    if (p == NULL) {
        return NULL;
    }
    
    memset(p, 0, size);
    
    return p;
}

mem_pool_t *mem_create_pool(size_t size/*, log_t *log */){

    mem_pool_t *p;
    p = mem_alloc(size);
    if(p == NULL){
        return NULL;
    }
    
    
    p->d.last = (u_char *)p + sizeof(mem_pool_t);
    p->d.end = (u_char *)p + size;
    p->d.next = NULL;
    p->d.failed = 0;
    
    size = size - sizeof(mem_pool_t);
    p->max = size < MAX_ALLOC_FROM_POOL? size: MAX_ALLOC_FROM_POOL;
    p->current = p;
    p->large = NULL;
    
    return p;
}


void mem_destroy_pool(mem_pool_t *pool){

    mem_pool_t *p, *n;
    mem_pool_large_t *l;
    
    for (l = pool->large; l; l = l->next) {
        if (l->alloc != NULL) {
            free(l->alloc);
        }
    }
    
    for (p = pool, n = p->d.next; /*void*/; p = n, n = p->d.next) {
        
        free(p);
        
        if (n == NULL) {
            break;
        }
    }

}



void mem_reset_pool(mem_pool_t *pool){

    mem_pool_t *p;
    mem_pool_large_t *l;
    
    /* free the large mem*/
    for (l = pool->large; l; l = l->next) {

        if (l->alloc != NULL) {
            free(l->alloc);
        }
    }
    pool->large = NULL;
    /* reset the pool mem*/
    for (p = pool; p; p = p->d.next) {
        p->d.last = (u_char *)p + sizeof(mem_pool_t);
        p->d.failed = 0;
    }
    
    pool->current = pool;

}


void *mem_palloc(mem_pool_t *pool, size_t size){

    u_char *m;
    mem_pool_t *p;
   
    if (size <= pool->max) {
        
        p = pool->current;
        do {
            m = p->d.last;
            if ((size_t)(p->d.end - m) >= size) {
                p->d.last += size;
                return m;
            }
            
            p = p->d.next;
            
        } while (p);
        
        return mem_palloc_block(pool, size);
    }
    
    return mem_palloc_large(pool, size);

}

static void *mem_palloc_block(mem_pool_t *pool, size_t size){
    
    u_char *m;
    size_t psize;
    mem_pool_t *p, *new, *current;
    
    psize = (size_t)((unsigned long)pool->d.end - (unsigned long)pool);
    m = mem_alloc(psize);
    if (m == NULL) {
        return NULL;
    }
    
    new = (mem_pool_t *) m;
    new->d.end = m + psize;
    m += sizeof(mem_pool_t);
    new->d.last = m + size;
    new->d.failed = 0;
    new->d.next = NULL;
    
    current = pool->current;
    for (p = current; p->d.next; p = p->d.next) {
        if (p->d.failed++ > MEM_POOL_FAIL_THREHOLD) {
            current = p->d.next;
        }
    }
    
    p->d.next = new;
    pool->current = current? current: new;
    
    return m;
}


static void *mem_palloc_large(mem_pool_t *pool, size_t size){
    
    void *p;
    u_int16_t n;
    mem_pool_large_t *large;
    
    p = mem_alloc(size);
    if(p == NULL){
        return NULL;
    }
    
    n = 0;
    for (large = pool->large; large; large = large->next) {
        
        if (large->alloc ==NULL) {
            large->alloc = p;
            return p;
        }
        
        if (n++ > 3) {
            break;
        }
    }
    
    large = mem_palloc(pool, sizeof(mem_pool_large_t));
    if (large == NULL) {
        free(p);
        return NULL;
    }
    
    large->alloc = p;
    large->next = pool->large;
    pool->large = large;
    
    return p;
    
}

u_int mem_pfree(mem_pool_t *pool, void *p){

    mem_pool_large_t *l;
    for (l = pool->large; l; l = l->next) {
        if (l->alloc == p) {
            free(l->alloc);
            l->alloc = NULL;
            return MEM_OK;
        }
    }
    
    return MEM_FAILED;
    
}

void *mem_pcalloc(mem_pool_t *pool, size_t size){

    void *p;
    p = mem_palloc(pool, size);
    if (p) {
        mem_memzero(p, size);
    }
    
    return p;
}

/* palloc without memory alignment */
void *mem_pnalloc(mem_pool_t *pool, size_t size){
    /* now palloc is not align memory */
    return mem_palloc(pool, size);

}




