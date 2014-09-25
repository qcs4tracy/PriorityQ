//
//  array.c
//  array
//
//  Created by TracyQCS on 13-7-10.
//  Copyright (c) 2013年 TracyQCS. All rights reserved.
//

#include <stdio.h>
#include <memory.h>
#include "array.h"
#include "palloc.h"

array_t *create_array(mem_pool_t *p, u_int n, size_t size){

    array_t *a;
    a = mem_palloc(p, sizeof(array_t));
    if (a == NULL) {
        return NULL;
    }
    
    a->elts = mem_palloc(p, n * size);
    if (a->elts == NULL) {
        return NULL;
    }
    
    a->nelts = 0;
    a->nalloc = n;
    a->pool = p;
    a->size = size;
    
    return a;
}



void array_destroy(array_t *a){

    mem_pool_t *p;
    p = a->pool;
    
    if ((u_char *)a->elts + a->nalloc * a->size == p->d.last) {
        p->d.last -= a->nalloc * a->size;
    }
    
    if ((u_char *)a + sizeof(array_t) == p->d.last) {
        p->d.last -= sizeof(array_t);
    }

}

void *array_push(array_t *a){
    
    mem_pool_t *p;
    size_t size;
    void *new, *elt;
    
    p = a->pool;
    /* the array is full */
    if (a->nalloc == a->nelts) {
        
        size = a->size * a->nalloc;
        
        if ((u_char *)a->elts + size == p->d.last &&
            (u_char *)a->elts + size + a->size <= p->d.end) {
            
            p->d.last += a->size;
            a->nalloc++;
        } else {
            new = mem_palloc(p, 2*size);
            if (new == NULL) {
                return NULL;
            }
            memcpy(new, a->elts, a->nelts * a->size);
            a->nalloc *= 2;
            a->elts = new;
        }
    }
    
    elt = (u_char *) a->elts + a->nelts*a->size;
    a->nelts++;
    
    return elt;
}



void *array_push_n(array_t *a, u_int n){

    mem_pool_t *p;
    void *new, *elt;
    size_t size;
    u_int m;
    
    size = n * a->size;
    /* not enough empty elt to be allocated*/
    if (a->nalloc < a->nelts + n) {
        
        p = a->pool;
        /* if a is not full but not enough empty elt, this will expand extra (n-a->nalloc+a->nelts)*/
        if ((u_char *)a->elts + a->nalloc*a->size == p->d.last &&
            (u_char *)p->d.last + size <= p->d.end) {
            
            p->d.last += size;
            a->nalloc += n;
            
        } else {
            
            m = a->nalloc < n? n: a->nalloc;
            new = mem_palloc(p, m);
            if (new == NULL) {
                return NULL;
            }
            memcpy(new, a->elts, a->nelts * a->size);
            a->elts = new;
            a->nalloc = m;
            
        }
    }

    elt = (u_char *) a->elts + a->nelts * a->size;
    a->nelts += n;
    
    return elt;

}

void *array_elem(array_t *a, int n)
{
    int n_el = a->nelts;
    if (n > n_el - 1) { return NULL; }
    return a->elts + n * a->size;
    
}




