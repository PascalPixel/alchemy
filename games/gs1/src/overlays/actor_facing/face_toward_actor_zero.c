#include "types.h"

#define SceneActor_FaceTowardActorZero Func_02000aa8

u8 *Func_02002ff0();
s32 Func_02002f42();

/*
 * Complete 40-byte heading update: face the supplied entity towards entity 0,
 * store the resulting angle in its +6 halfword and report zero.
 */
s32 SceneActor_FaceTowardActorZero(u8 *obj)
{
    u8 *p = Func_02002ff0(0);
    *(u16 *)(obj + 6) = (u16)Func_02002f42(
        *(s32 *)(p + 16) - *(s32 *)(obj + 16),
        *(s32 *)(p + 8) - *(s32 *)(obj + 8));
    return 0;
}
