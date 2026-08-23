#ifndef ALCHEMY_ALIGNED_ALLOC_H
#define ALCHEMY_ALIGNED_ALLOC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ALCHEMY_CACHE_LINE_SIZE 64

/**
 * Computes memory offset aligned to the specified power-of-two boundary.
 */
static inline uintptr_t alchemy_align_forward(uintptr_t ptr, size_t alignment)
{
    uintptr_t p = ptr;
    uintptr_t a = (uintptr_t)alignment;
    uintptr_t modulo = p & (a - 1);
    if (modulo != 0) {
        p += (a - modulo);
    }
    return p;
}

#ifdef __cplusplus
}
#endif

#endif /* ALCHEMY_ALIGNED_ALLOC_H */
