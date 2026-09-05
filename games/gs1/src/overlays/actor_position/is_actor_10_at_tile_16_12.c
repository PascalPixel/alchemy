#include "types.h"

extern s32 *Func_0200158e(s32);

s32 SceneActor_IsActor10AtTile16x12(void)
{
    s32 *p = Func_0200158e(10);
    s32 z = p[4];
    s32 x;
    s32 cz;
    s32 cx;

    if (z < 0) {
        z += 0x000FFFFF;
    }
    x = p[2];
    cz = z >> 20;
    if (x < 0) {
        x += 0x000FFFFF;
    }
    cx = x >> 20;
    if (cx == 16 && cz == 12) {
        return 1;
    }
    return 0;
}
