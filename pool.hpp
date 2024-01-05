#pragma once

#include "afx/types.h"

namespace afx {
    constexpr usz ALLOC_SIZE  = 16; // Allocation size granularity.
    constexpr usz ALLOC_START = 64; // Offset from the pool class to start slicing at.

    typedef u32 alloc_t; // Length in ALLOC_SIZE.

    class afxpool;
    struct block;

    /**
     * @brief A pool of heap memory.
     */
    class afxpool {
        public:
        afxpool* next;

        /**
         * @brief Prepares the pool for use.
         * Keep in mind that the actual length of available memory will be slightly less,
         * as the pool class is treated as a header of the memory region.
         * @param size Total size of the allocated memory region.
         */
        void prepare(usz size);

        /**
         * @brief Attempts to allocate memory.
         * @param len Allocation size in chunks.
         */
        void* alloc(alloc_t len);

        /**
         * @brief Attempts to free memory.
         * @param len Allocation size in chunks.
         */
        bool free(void* ptr);

        /**
         * @brief Tries to get a block descriptor from a memory pointer.
         * This method will return nullptr when the resulting block is not contained by
         * the pool.
         * @return Pointer to the block or nullptr.
         */
        block* get(void* ptr);

        private:
        // proc::lock _lock;
        usz _size;

        void* alloc_peaceful(alloc_t len);
        void* alloc_aggresive(alloc_t len);

        /**
         * @brief Returns the pointer to the region available for allocations.
         * @return Pointer to the memory.
         */
        u8* start() {
            return (u8*) this + ALLOC_START;
        }

        bool contains(block* block) {
            return ((u8*) block >= (u8*) this) &&
                   ((u8*) block < (u8*) this + ALLOC_START + _size);
        }
    };
}