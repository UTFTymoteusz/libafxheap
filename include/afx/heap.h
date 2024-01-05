#pragma once

#include "afx/types.h"

#ifdef __cplusplus
namespace afx {
#endif
    struct pool {};
    typedef struct pool pool_t;

    /**
     * @brief Prepares a space of memory as a heap pool.
     * This function requires a preallocated size of memory.
     * @param space Pointer to a space of memory.
     * @param size  Size of the space.
     * @return Pointer to a heap pool or NULL on failure.
     */
    export pool_t* heap_pool(void* space, usz size);

    /**
     * @brief Allocates memory in a pool.
     * @param pool Pool to allocate memory from.
     * @param size Size of allocation
     * @return Pointer to the block of memory.
     */
    export void* heap_malloc(pool_t* pool, usz size);

    /**
     * @brief Frees memory in a pool.
     * @param pool Pool to free memory from.
     * @param mem  Pointer to the area of memory.
     * @return True if the free operation succedeed.
     */
    export bool heap_free(pool_t* pool, void* mem);

    /**
     * @brief Returns the size of a memory allocation.
     * @param pool Pool of the memory.
     * @param mem  Pointer to the area of memory.
     * @return Acqual size of the allocated memory.
     */
    export usz heap_size(pool_t* pool, void* mem);

    /**
     * @brief Gets the next pool in a chain.
     * @param pool Pool.
     * @return Pointer to the next pool or NULL.
     */
    export pool_t* heap_getnext(pool_t* pool);

    /**
     * @brief Sets the next pool in a chain.
     * @param pool Pool.
     * @param next Next pool.
     */
    export void heap_setnext(pool_t* pool, pool_t* next);
#ifdef __cplusplus
}
#endif
