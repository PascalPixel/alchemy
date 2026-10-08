#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "iwram_call.h"
#include "fixed_math.h"


void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position)
{
    *position++ += Iwram_MulQ16(radius, Trig_Sin(angle + 0x4000));
    position++;
    *position += Iwram_MulQ16(radius, Trig_Sin(angle));
}

s32 ArcTan2(s32 x, s32 y)
{
    const u16 *table;
    s32 value;
    s32 ratio;
    s32 result;
    u32 bit;

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

        ratio = Iwram_UnsignedDivide(value << 8, ratio);
        result = 0x4000;
        if (ratio <= 0xFB6A) {
            table = Math_ArcTanTable;
            result = 0;
            bit = 0x2000;

            value = *table;
            table -= 0x40;
            if (ratio > value) {
                result += bit;
                table += 0x80;
            }
            value = *table;
            bit >>= 1;
            table -= 0x20;
            if (ratio > value) {
                result += bit;
                table += 0x40;
            }
            value = *table;
            bit >>= 1;
            table -= 0x10;
            if (ratio > value) {
                result += bit;
                table += 0x20;
            }
            value = *table;
            bit >>= 1;
            table -= 8;
            if (ratio > value) {
                result += bit;
                table += 0x10;
            }
            value = *table;
            bit >>= 1;
            table -= 4;
            if (ratio > value) {
                result += bit;
                table += 8;
            }
            value = *table;
            bit >>= 1;
            table -= 2;
            if (ratio > value) {
                result += bit;
                table += 4;
            }
            value = *table;
            table--;
            if (ratio > value) {
                result += 0x80;
                table += 2;
            }
            value = *table;
            if (ratio > value)
                result += 0x40;
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
    return Iwram_Sqrt(value) << 8;
}


s32 Runtime_GetLowTableAddress(void)
{
    return (s32)System_BasicColorPalette;
}
