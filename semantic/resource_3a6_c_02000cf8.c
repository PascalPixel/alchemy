#include "types.h"

extern u8 *Func_0808a080(s32 actorId);
extern s32 Func_08000100(s32 deltaZ, s32 deltaX);

/* Point an object toward actor zero using their fixed-point X/Z delta. */
s32 Func_02000cf8(u8 *object)
{
    u8 *reference = Func_0808a080(0);
    s32 deltaZ = *(s32 *)(reference + 16) - *(s32 *)(object + 16);
    s32 deltaX = *(s32 *)(reference + 8) - *(s32 *)(object + 8);

    *(s16 *)(object + 6) = (s16)Func_08000100(deltaZ, deltaX);
    return 0;
}
