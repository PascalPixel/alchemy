#include "types.h"

extern s32 *Func_0200ba7e(s32);
extern s32 Func_0200ba3e(s32);
extern u8 *Func_0200ba90(s32);
extern u8 *Func_0200ba98(s32);
extern u8 Value_0000087a;

void Func_02005a94(void)
{
    s32 *g = Func_0200ba7e(0);
    u8 *q;
    if (Func_0200ba3e((s32)&Value_0000087a) != 0)
        q = Func_0200ba90(21);
    else
        q = Func_0200ba98(20);
    if (q != 0) {
        if (g[3] > 0xc80000)
            q[0x23] = 3;
        else
            q[0x23] = 1;
    }
}
