#pragma once

#include "afx/types.h"
#include "pool.hpp"

namespace afx {
    constexpr u64 BLOCK_SANITY = 0x2A3B4C5D6E7F8182; // The value to use in block->sanity.

    struct block;

    struct block {
        u64     sanity; // Sanity check value.
        alloc_t len;    // Length of allocation, not counting the struct itself.
        bool    free;   // Marks whether the block is taken or free.

        /**
         * @brief Fills out the block with required data.
         * @param len Total allocation length in ALLOC_SIZE chunks, not counting the
         * struct itself.
         */
        void init(alloc_t len) {
            this->sanity = BLOCK_SANITY;
            this->len    = len;
            this->free   = true;
        }

        /**
         * @brief Returns a pointer to the next block.
         * This method does not perform any boundary checking.
         * @return Pointer to the next block.
         */
        block* next() {
            return (block*) ((u8*) this + ALLOC_SIZE + len * ALLOC_SIZE);
        }

        /**
         * @brief Returns a pointer to the memory area of a block.
         * @return Pointer to the memory.
         */
        void* memory() {
            return (void*) ((u8*) this + ALLOC_SIZE);
        }

        /**
         * @brief Returns a pointer to the memory area of a block.
         * @param offset Offset in ALLOC_SIZEs.
         * @return Pointer to the memory.
         */
        void* memory(alloc_t offset) {
            return (void*) ((u8*) this + ALLOC_SIZE + offset * ALLOC_SIZE);
        }

        /**
         * @brief Slices the block into two blocks.
         * The second slice will be the remainder of the first block after slicing.
         * @param len Length of the first slice.
         * @return Pointer to the next block.
         */
        block* slice(alloc_t len) {
            block* next = (block*) this->memory(len);

            next->init(this->len - (len + 1));

            this->len = len;

            return this->next();
        }

        /**
         * @brief Marks the block as taken.
         */
        constexpr void take() {
            this->free = false;
        }

        /**
         * @brief Marks the block as free.
         */
        constexpr void release() {
            this->free = true;
        }
    };

    static_assert(sizeof(block) <= 16);
}