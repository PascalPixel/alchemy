#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"

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
    s32 (*root)(s32) = Iwram_Sqrt;

    return (s32)((u32)root(value) << 8);
}

extern const u8 System_BasicColorPalette[];

s32 Runtime_GetLowTableAddress(void)
{
    return (s32)System_BasicColorPalette;
}
