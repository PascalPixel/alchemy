#include "types.h"

#define SceneActor_FaceActorZero Func_02000cf8

/* Point an object toward actor zero using their fixed-point X/Z delta. */
extern u8 * Func_020029bc(s32 no);
extern s32 Func_0200293e(s32 dz, s32 dx);
s32 SceneActor_FaceActorZero(u8 *obj)
{
    u8 *target = Func_020029bc(0);
    s32 dz = *(s32 *)(target + 16) - *(s32 *)(obj + 16);
    s32 dx = *(s32 *)(target + 8) - *(s32 *)(obj + 8);

    *(s16 *)(obj + 6) = (s16)Func_0200293e(dz, dx);
    return 0;
}
