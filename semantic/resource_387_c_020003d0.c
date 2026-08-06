#include "types.h"

/* resource_387 actor-10 tile-23 transition owner, 108 bytes through its pool. */







static s32 Fixed20_020003d0(s32 value)
{
    if (value < 0) {
        value += 0x0fffff;
    }
    return value >> 20;
}

extern u8 * Func_0200151c(s32 actor);
extern void Func_02001512(s32 frames);
extern u8 * Func_02001538(s32 actor);
extern u8 * Func_02001544(s32 actor);
extern u8 * Func_02001550(s32 actor);
extern void Func_020014d6(u8 *actor, s32 mode);
extern void Func_020014e0(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);
extern void Func_02001536(s32 flagId);
void Func_020003d0(void)
{
    s32 tileX = Fixed20_020003d0(*(s32 *)(Func_0200151c(10) + 8));
    u8 *actor;

    if (tileX != 23) {
        return;
    }

    Func_02001512(10);
    actor[35] = 2;
    actor = Func_02001538(10);
    actor = Func_02001544(10);
    actor[85] = 0;
    actor = Func_02001550(10);
    Func_020014d6(actor, 0);
    Func_020014e0(54, 17, 1, 1, tileX, 17);
    Func_02001536(0x863);
}
