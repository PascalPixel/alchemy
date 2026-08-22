#include "types.h"




/* Point an object toward actor zero using their fixed-point X/Z delta. */
extern u8 * Func_020029bc(s32 actorId);
extern s32 Func_0200293e(s32 deltaZ, s32 deltaX);
s32 Func_02000cf8(u8 *object)
{
    u8 *reference = Func_020029bc(0);
    s32 deltaZ = *(s32 *)(reference + 16) - *(s32 *)(object + 16);
    s32 deltaX = *(s32 *)(reference + 8) - *(s32 *)(object + 8);

    *(s16 *)(object + 6) = (s16)Func_0200293e(deltaZ, deltaX);
    return 0;
}
