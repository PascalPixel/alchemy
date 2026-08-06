#include "types.h"

/*
 * resource_3be owner at 0x0200058c, 164 bytes. It adjusts the caller's
 * position by the high-nibble displacement table, probes the overlay-local
 * slot finder, filters six accepted keys, and clears four motion fields on an
 * unaccepted or positive collision result. The return ends at 0x02000622;
 * three literal words extend the owner through 0x0200062f.
 */



static void ClearMotion_0200058c(u8 *object)
{
    *(s32 *)(object + 36) = 0;
    *(s32 *)(object + 44) = 0;
    *(s32 *)(object + 56) = (s32)0x80000000;
    *(s32 *)(object + 64) = (s32)0x80000000;
}

extern s32 * Func_0200090a(s32 *position, u8 *object);
extern s32 Func_02001ada(u8 *object, s32 *position);
s32 Func_0200058c(u8 *object)
{
    const s32 *deltas = (const s32 *)0x020096c0;
    const s32 *keys = (const s32 *)0x02009700;
    s32 position[3];
    s32 bucket = *(u16 *)(object + 6) >> 12;
    s32 delta = deltas[bucket];
    s32 *found;
    s32 key;
    u32 index;

    position[2] = *(s32 *)(object + 16) + (s32)((unsigned)delta << 16);
    position[0] = *(s32 *)(object + 8) + (delta & (s32)0xffff0000);
    position[1] = *(s32 *)(object + 12);

    found = Func_0200090a(position, object);
    if (found != 0) {
        u8 *handle = *(u8 **)((u8 *)found + 80);
        s16 *keyPointer = *(s16 **)(handle + 40);
        key = *keyPointer;
        for (index = 0; index <= 5; index++) {
            if (key == keys[index])
                return 0;
        }
        ClearMotion_0200058c(object);
    }

    delta = deltas[bucket];
    position[0] = *(s32 *)(object + 8) + (delta & (s32)0xffff0000);
    if (Func_02001ada(object, position) > 0)
    position[1] = *(s32 *)(object + 12);
        ClearMotion_0200058c(object);
    return 0;
    position[2] = *(s32 *)(object + 16) + (s32)((unsigned)delta << 16);
}
