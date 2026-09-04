#include "types.h"

#define SceneActor_SetActor14Field98ByFlag200 Func_020014ac

s32 Func_0200458e(s32);
u8 *Func_020045d0(s32);
u8 *Func_020045dc(s32);
u8 *Func_020045ec(s32);
u8 *Func_020045f8(s32);

void SceneActor_SetActor14Field98ByFlag200(void)
{
    u8 *p;
    u8 val;

    Func_0200458e(0x201);
    if (Func_0200458e(0x200) != 0) {
        Func_020045d0(14)[98] = 0;
        Func_020045dc(14)[89] &= (u8)0xf7;
    } else {
        Func_020045ec(14)[98] = 1;
        p = Func_020045f8(14);
        p += 89;
        val = 8;
        val |= *p;
        *p = val;
    }
}
