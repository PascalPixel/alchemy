#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "global_cells.h"

/* math/random_16.c */
u32 Random16(void)
{
    s32 value;
    value = (*(s32 *)ADDR_03001CB4 * 0x41c64e6d) + 0x3039;
    *(s32 *)ADDR_03001CB4 = value;
    return (u32)(value << 8) >> 16;
}

/* math/arctan2.c */
s32 FixedPoint_Ratio(s32 numerator, s32 denominator);
extern const u16 gRom[];

u16 ArcTan2(s32 x, s32 y)
{
    const u16 *table;
    s32 value;
    s32 ratio;
    s32 result;

    if (x == 0) {
        result = 0;
    } else if (y == 0) {
        result = 0x4000;
    } else {
        ratio = y;
        if (ratio < 0)
            ratio = -ratio;
        value = x;
        if (value < 0)
            value = -value;

        ratio = FixedPoint_Ratio(value << 8, ratio);
        result = 0x4000;
        if (ratio <= 0xFB6A) {
            table = gRom;
            result = 0;

            value = *table;
            table -= 0x40;
            if (ratio > value) {
                result = 0x2000;
                table += 0x80;
            }
            value = *table;
            table -= 0x20;
            if (ratio > value) {
                result |= 0x1000;
                table += 0x40;
            }
            value = *table;
            table -= 0x10;
            if (ratio > value) {
                result |= 0x800;
                table += 0x20;
            }
            value = *table;
            table -= 8;
            if (ratio > value) {
                result |= 0x400;
                table += 0x10;
            }
            value = *table;
            table -= 4;
            if (ratio > value) {
                result |= 0x200;
                table += 8;
            }
            value = *table;
            table -= 2;
            if (ratio > value) {
                result |= 0x100;
                table += 4;
            }
            value = *table;
            table--;
            if (ratio > value) {
                result |= 0x80;
                table += 2;
            }
            value = *table;
            if (ratio > value)
                result |= 0x40;
        }
    }

    if (y < 0)
        result = 0x8000 - result;
    if (x < 0)
        result = -result;

    return (u16)result;
}

/* math/integer_sqrt.c */
s32 Math_IntegerSqrt(s32 value)
{
    s32 trial;
    s32 remainder;
    s32 bit;
    s32 result;

    remainder = value;
    result = 0;
    bit = 0xF;
    do {
        trial = (result << (bit + 1)) + (1 << (bit * 2));
        if (trial <= remainder) {
            result |= 1 << bit;
            remainder -= trial;
        }
        bit -= 1;
    } while (bit >= 0);
    return result;
}

/* math/fixed_sqrt.c */
/*
 * Fixed-point square root: hand the value to the relocated routine at
 * 0x030001d8 and shift its result up by eight.
 */

/*
 * Sys_SetMode names a `bx rN` slot, so the call is indirect through the
 * register that slot selects; the trailing argument is the callee address
 * loaded into that register, not a parameter of the callee.
 */

/*
 * The callee takes one argument and returns one. Callers feed it a sum of
 * squares and use the result as a length, which reads as a square root but
 * is not established.
 */
s32 FixedSqrt(s32 value, s32 unused1, s32 unused2)
{
    return (s32)((u32)Sys_SetMode(value, unused1, unused2, 0x030001D8) << 8);
}
