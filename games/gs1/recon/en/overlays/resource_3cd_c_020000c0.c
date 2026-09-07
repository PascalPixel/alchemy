#include "types.h"

extern s32 Func_02000942(u16 *);
extern void Func_0200017e(u16, s32);

void Func_020000c0(s32 arg)
{
    u16 list[16];
    u16 *p;
    s32 n;

    n = Func_02000942(list);
    if (n > 0) {
        s32 count = n;
        p = list;
        do {
            Func_0200017e(*p++, arg);
        } while (--count != 0);
    }
}
