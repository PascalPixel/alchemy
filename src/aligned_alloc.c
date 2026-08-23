#include "alchemy/aligned_alloc.h"

void* alchemy_arena_alloc_aligned(void* current_ptr, size_t size, size_t alignment)
{
    if (!current_ptr || size == 0) {
        return NULL;
    }

    /* Default to 64-byte cache line alignment if unspecified or invalid */
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
        alignment = ALCHEMY_CACHE_LINE_SIZE;
    }

    uintptr_t raw_addr = (uintptr_t)current_ptr;
    uintptr_t aligned_addr = alchemy_align_forward(raw_addr, alignment);

    return (void*)aligned_addr;
}
