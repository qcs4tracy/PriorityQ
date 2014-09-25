//
//  palloc.h
//  array
//
//  Created by TracyQCS on 13-7-10.
//  Copyright (c) 2013年 TracyQCS. All rights reserved.
//

#ifndef array_palloc_h
#define array_palloc_h

#define MEM_POOL_DEFAUT_SIZE (16 * 1024);
#define MAX_ALLOC_FROM_POOL ((4 * 1024) - 1)
#define MEM_POOL_FAIL_THREHOLD 4

#define MEM_OK 0x0000001
#define MEM_FAILED 0x00000002
#define mem_memzero(p, size) memset(p, 0, size)

typedef unsigned char u_char;
typedef unsigned int u_int;

typedef struct mem_pool_s mem_pool_t;
typedef struct mem_pool_data_s mem_pool_data_t;
typedef struct mem_pool_large_s mem_pool_large_t;

struct mem_pool_data_s {
    mem_pool_t *next;
    u_char *last;
    u_char *end;
    u_int failed;
    
};

struct mem_pool_s {
    mem_pool_data_t d;
    size_t max;
    mem_pool_large_t *large;
    mem_pool_t *current;
    /*
    log_t *log;
     */
};

struct mem_pool_large_s {
    void *alloc;
    mem_pool_large_t *next;
};

void *mem_alloc(size_t size/*, log_t *log */);
void *mem_calloc(size_t size/*, log_t *log */);

mem_pool_t *mem_create_pool(size_t size/*, log_t *log */);
void mem_destroy_pool(mem_pool_t *pool);
void mem_reset_pool(mem_pool_t *pool);

void *mem_palloc(mem_pool_t *pool, size_t size);
void *mem_pnalloc(mem_pool_t *pool, size_t size);
void *mem_pcalloc(mem_pool_t *pool, size_t size);
/* void *mem_pmemalign(mem_pool_t *pool, size_t size, size_t alignment); */
u_int mem_pfree(mem_pool_t *pool, void *p);

#endif
