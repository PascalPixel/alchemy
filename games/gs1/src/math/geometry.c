#include "types.h"
#include "fixed_math.h"

s32 FixedPoint_Ratio(s32 numerator, s32 denominator);
extern const u16 Data_08007676[];

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
            table = Data_08007676;
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

s32 Math_IntegerSqrt(s32 value)
{
    s32 trial;
    s32 remainder;
    s32 bit;
    s32 result;

    remainder = value;
    result = 0;
    bit = 0xf;
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

s32 FixedSqrt(s32 value)
{
    s32 (*root)(s32) = (s32 (*)(s32))0x030001d8;

    return (s32)((u32)root(value) << 8);
}
