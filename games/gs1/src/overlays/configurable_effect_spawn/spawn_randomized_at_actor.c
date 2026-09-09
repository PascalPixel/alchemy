#include "types.h"

extern volatile s32 Data_03001e40;

void Func_02003fba(s32 actor, s32 mode);
void Func_02003fc4(s32 actor, s32 mode);
s32 Func_02003fc2(void);
s32 Func_02003fd6(void);
void Func_02001ab4(s32 x, s32 z, s32 y, s32 vx, s32 vy, s32 vz, u32 flags, void *descriptor);

#define SceneEffect_SpawnRandomizedAtActor Func_02000f4c

s32 SceneEffect_SpawnRandomizedAtActor(s32 actor)
{
    u8 descriptor[40];
    u8 *d;
    s32 flags;
    s32 draw;
    s32 x;
    s32 y;

    if ((Data_03001e40 & 2) != 0) {
        Func_02003fba(actor, 1);
    } else {
        Func_02003fc4(actor, 2);
    }

    flags = Data_03001e40 & 3;
    if (flags != 0) {
        return 0;
    }
    d = descriptor;
    *(s32 *)(d + 8) = 0x4ccc;
    *(s32 *)(d + 12) = 0x4ccc;
    *(s32 *)(d + 4) = 5;
    draw = Func_02003fc2();
    x = *(s32 *)(actor + 8) + ((((u32)(draw * 7) >> 16) - 3) << 16);
    draw = Func_02003fd6();
    y = *(s32 *)(actor + 16) + ((((u32)(draw * 7) >> 16) - 3) << 16);
    Func_02001ab4(x, *(s32 *)(actor + 12) + 0x100000, y, 0, flags, flags,
                  0x90001, d);
    return 0;
}
