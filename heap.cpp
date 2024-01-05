#include "afx/heap.h"

#include "block.hpp"
#include "mem.hpp"
#include "pool.hpp"

#define alfit(x, alignment) ((x + alignment - 1) / alignment)

namespace afx {
    pool_t* heap_pool(void* space, usz size) {
        afxpool* pool = (afxpool*) space;

        pool->prepare(size);

        return (pool_t*) pool;
    }

    void* heap_malloc(pool_t* pool, usz size) {
        afxpool* internal_pool = (afxpool*) pool;
        alloc_t  alloc_size    = alfit(size, ALLOC_SIZE);
        void*    memory        = internal_pool->alloc(alloc_size);

        return memset(memory, '\0', alloc_size * ALLOC_SIZE);
    }

    bool heap_free(pool_t* pool, void* mem) {
        afxpool* internal_pool = (afxpool*) pool;

        return internal_pool->free(mem);
    }

    usz heap_size(pool_t* pool, void* mem) {
        afxpool* internal_pool = (afxpool*) pool;
        block*   blk           = internal_pool->get(mem);

        if (!blk)
            return 0;

        return blk->len * ALLOC_SIZE;
    }

    pool_t* heap_getnext(pool_t* pool) {
        afxpool* internal_pool = (afxpool*) pool;

        return (pool_t*) internal_pool->next;
    }

    void heap_setnext(pool_t* pool, pool_t* next) {
        afxpool* internal_pool = (afxpool*) pool;

        internal_pool->next = (afxpool*) next;
    }
}