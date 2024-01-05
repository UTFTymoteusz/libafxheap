#include "pool.hpp"

#include "block.hpp"
#include "mem.hpp"

namespace afx {
    template <typename T, typename U>
    constexpr T alfloor(T x, U alignment) {
        return x / alignment;
    }

    template <typename T, typename m, typename M>
    constexpr T inrange(T x, m min, M max) {
        return x >= min && x <= max;
    }

    void afxpool::prepare(usz size) {
        this->_size = size - ALLOC_START;
        this->next  = nullptr;

        memset(this->start(), '\0', _size);

        block* first_block = (block*) start();
        first_block->init(alfloor(this->_size - 1, ALLOC_SIZE));
    }

    void* afxpool::alloc(alloc_t len) {
        if (void* alloc = this->alloc_peaceful(len))
            return alloc;

        if (void* alloc = this->alloc_aggresive(len))
            return alloc;

        return nullptr;
    }

    bool afxpool::free(void* ptr) {
        block* blk;

        if (!(blk = this->get(ptr)))
            return false;

        blk->free = true;

        while (this->contains(blk->next()) && blk->next() && blk->next()->free)
            blk->len = blk->next()->len + 1;

        return true;
    }

    block* afxpool::get(void* ptr) {
        block* blk = (block*) ((u8*) ptr - ALLOC_SIZE);
        return this->contains(blk) ? blk : nullptr;
    }

    void* afxpool::alloc_peaceful(alloc_t len) {
        block* current = (block*) this->start();

        while (this->contains(current)) {
            if (!current->free) {
                current = current->next();
                continue;
            }

            if (current->len == len) {
                current->free = false;
                return current->memory();
            }

            current = current->next();
        }

        return nullptr;
    }

    void* afxpool::alloc_aggresive(alloc_t len) {
        block* current = (block*) this->start();

        while (this->contains(current)) {
            if (!current->free) {
                current = current->next();
                continue;
            }

            if (current->len > len + 1) {
                current->slice(len);
                current->take();

                return current->memory();
            }

            current = current->next();
        }

        return nullptr;
    }
}