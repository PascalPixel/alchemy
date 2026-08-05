#include "types.h"

extern s32 *Func_02000350(s32 *position, u8 *object);
extern s32 Func_080091d8(u8 *object, s32 *position);

static void ClearMotion_0200058c(u8 *object)
{
    *(s32 *)(object + 36) = 0;
    *(s32 *)(object + 44) = 0;
    *(s32 *)(object + 56) = (s32)0x80000000;
    *(s32 *)(object + 64) = (s32)0x80000000;
}

/* Complete 164-byte collision-motion owner.  The two local tables and the
 * direction-step table were independently recovered from the pool at 0624. */
s32 Func_0200058c(u8 *object)
{
    const s32 *deltas = (const s32 *)0x02009e48;
    const s32 *keys = (const s32 *)0x02009e88;
    s32 position[3];
    s32 bucket = *(u16 *)(object + 6) >> 12;
    s32 delta = deltas[bucket];
    s32 *found;
    u32 index;

    position[0] = *(s32 *)(object + 8) + (delta & (s32)0xffff0000);
    position[1] = *(s32 *)(object + 12);
    position[2] = *(s32 *)(object + 16) + (s32)((unsigned)delta << 16);
    found = Func_02000350(position, object);
    if (found != 0) {
        u8 *handle = *(u8 **)((u8 *)found + 80);
        s16 key = **(s16 **)(handle + 40);
        for (index = 0; index <= 5; index++) {
            if (key == keys[index])
                return 0;
        }
        ClearMotion_0200058c(object);
    }

    delta = deltas[bucket];
    position[0] = *(s32 *)(object + 8) + (delta & (s32)0xffff0000);
    position[1] = *(s32 *)(object + 12);
    position[2] = *(s32 *)(object + 16) + (s32)((unsigned)delta << 16);
    if (Func_080091d8(object, position) > 0)
        ClearMotion_0200058c(object);
    return 0;
}
