#ifndef ALCHEMY_IWRAM_CALL_H
#define ALCHEMY_IWRAM_CALL_H

#include "TYPES.H"

/* The IWRAM routines return through ip, so callers establish that link in
 * Thumb before entering their ARM bodies. The routine is the last argument:
 * GCC evaluates arguments right to left, so its address is loaded before the
 * operands are computed, as every call site in the ROM shows. */
static __inline__ s32 Iwram_Call2(s32 left, s32 right, void *routine)
{
    register s32 result __asm__("r0") = left;
    register s32 factor __asm__("r1") = right;

    __asm__ volatile(
        ".align 2\n\t"
        "mov ip, pc\n\t"
        "bx %2"
        : "+r"(result)
        : "r"(factor), "r"(routine)
        : "r2", "ip", "cc");
    return result;
}

#define Iwram_MulQ16(left, right) Iwram_Call2((left), (right), (void *)0x03000118)

#endif
