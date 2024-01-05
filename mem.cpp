#include "mem.hpp"

#include "afx/types.h"

namespace afx {
    void* memset(void* mem, char c, usz len) {
        char* m_mem = (char*) mem;

        for (usz i = 0; i < len; i++)
            m_mem[i] = c;

        return mem;
    }
}