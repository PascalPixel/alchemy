#include "types.h"

extern void Func_02001e8e(s32, s32, u8 *);

void Func_02000f6c(u8 *p)
{
    s32 i = 0;

    do {
        Func_02001e8e(i + 11, 0x180000, p);
        p -= 13107;
        i++;
    } while (i <= 4);
}
