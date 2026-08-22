#include "types.h"

extern u8 *Func_0200749e(s32 actor);
extern u8 *Func_020074a6(s32 actor);
extern void Func_02007462(s32 flag);
extern void Func_02007474(s32 flag);

void Func_0200271c(void)
{
    u8 *actor0 = Func_0200749e(0);
    s32 raw_x = *(s32 *)(actor0 + 8);
    s32 x;
    s32 z;

    u8 *actor1 = Func_020074a6(0);
    x = raw_x >> 20;
    z = *(s32 *)(actor1 + 16);
    x = x - 34;
    z = z >> 20;

    if ((u32)x <= 1 && z > 40 && z <= 42) {
        Func_02007462(148 << 2);
    } else {
        Func_02007474(148 << 2);
    }
}
