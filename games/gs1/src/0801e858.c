#include "types.h"

s32 Func_08002df0(s16 *);
s16 *Func_08004970(s32 arg0);
s32 Func_08017aa4(s16 *, s32, s32, s32);

void Func_0801e858(u8 *text, s32 arg1, s32 arg2, s32 arg3)
{
    s16 *buffer;
    s16 *output;
    u8 *input;

    input = text;
    buffer = Func_08004970(0x200);
    output = buffer;
    if (*input != 0) {
        do {
            *output = (s16)*input;
            input++;
            output++;
        } while (*input != 0);
    }
    *output = 0;
    Func_08017aa4(buffer, arg1, arg2, arg3);
    Func_08002df0(buffer);
}
