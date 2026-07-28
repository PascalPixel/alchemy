#include "types.h"

s16 *Func_08004970(s32 arg0);
void Func_08017c8c(u16 *, s32, s32, s32);
void Func_08002df0(u16 *);

void Func_0801e940(u8 *text, s32 arg1, u32 x, u32 y)
{
    u16 *buffer = Func_08004970(0x200);
    u16 *output = buffer;

    while (*text != 0) {
        *output = *text;
        text++;
        output++;
    }
    *output = 0;
    x >>= 3;
    y >>= 3;
    Func_08017c8c(buffer, arg1, x, y);
    Func_08002df0(buffer);
}
