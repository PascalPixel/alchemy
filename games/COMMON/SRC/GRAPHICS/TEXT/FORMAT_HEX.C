#include "TYPES.H"

void Text_FormatHex(u32 value, u32 count, u8 *output) {
    u8 digits[8];
    u32 index;

    if (count > 5)
        count = 5;

    for (index = 0; index != count; index++) {
        u32 digit = value & 15;
        if (digit <= 9)
            digits[index] = digit + '0';
        else
            digits[index] = digit + 'A' - 10;
        value >>= 4;
    }

    for (index = count - 1; (s32)index >= 0; index--)
        *output++ = digits[index];
}
