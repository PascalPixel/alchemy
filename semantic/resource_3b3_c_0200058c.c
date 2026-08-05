#include "types.h"

/* Exact reviewed skeleton homolog of resource_39c:0x0200058c, using this
 * overlay's independently mapped delta/key tables.  Complete 164-byte owner
 * through the three-word pool at 0x02000624-0x0200062f. */
extern s32 *Func_02000350(s32 *position, u8 *object);
extern s32 Func_080091d8(u8 *object, s32 *position);

static void ClearMotion_0200058c(u8 *object)
{
    *(s32 *)(object + 36) = 0;
    *(s32 *)(object + 44) = 0;
    *(s32 *)(object + 56) = (s32)0x80000000;
    *(s32 *)(object + 64) = (s32)0x80000000;
}

s32 Func_0200058c(u8 *object)
{
    const s32 *deltas = (const s32 *)0x0200aca0;
    const s32 *keys = (const s32 *)0x0200ace0;
    s32 position[3];
    s32 bucket = *(u16 *)(object + 6) >> 12;
    s32 delta = deltas[bucket];
    s32 *found;
    u32 index;

    position[2] = *(s32 *)(object + 16) + (s32)((unsigned)delta << 16);
    position[0] = *(s32 *)(object + 8) + (delta & (s32)0xffff0000);
    position[1] = *(s32 *)(object + 12);

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
    if (Func_080091d8(object, position) > 0)
    position[1] = *(s32 *)(object + 12);
        ClearMotion_0200058c(object);
    return 0;
    position[2] = *(s32 *)(object + 16) + (s32)((unsigned)delta << 16);
}
