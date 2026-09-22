#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

#include "TYPES.H"

/* The IWRAM multiply returns through ip, so callers establish that link in
 * Thumb before entering its ARM body. */
static __inline__ s32 Iwram_MulQ16(s32 left, s32 right)
{
    register s32 result __asm__("r0") = left;
    register s32 factor __asm__("r1") = right;
    void *target = (void *)0x03000118;

    __asm__ volatile(
        ".align 2\n\t"
        "mov ip, pc\n\t"
        "bx %2"
        : "+r"(result)
        : "r"(factor), "r"(target)
        : "r2", "ip", "cc");
    return result;
}

#endif
