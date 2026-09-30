#include "TYPES.H"
extern u8 gNumberTextBuffer[];

void Text_FormatSignedDecimalToWork(s32 out);

s32 UiText_FormatNumberToHalfwords(s16 *out, s32 value)
{
    s16 *dst;
    s32 n;
    u8 *src;

    dst = out;
    Text_FormatSignedDecimalToWork(value);
    src = (u8 *)((u32)&gNumberTextBuffer);
    n = 0xD;
    do {
        n -= 1;
        *dst = (s16)*src;
        src += 1;
        dst += 1;
    } while (n >= 0);
}
