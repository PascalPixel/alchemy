#include "types.h"

extern void Func_02004560(u8 *, s32);
extern void Func_02004580(u8 *, s32);
extern s32 Func_020045c0(s32);
extern void Func_020045d2(s32);
extern u8 Value_000002f1;

s32 Func_02000314(u8 *o)
{
    s32 z;
    Func_02004560(o, 0);
    Func_02004580(o, 10);
    {
        u8 *t = o + 0x59;
        z = 0;
        *t = z;
    }
    if (Func_020045c0(0x8A0) != 0) {
        Func_020045d2((s32)&Value_000002f1);
        *(s32 *)(o + 8) = z;
        *(s32 *)(o + 12) = z;
    }
    return 0;
}
