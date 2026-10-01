/*
 * Draft: Map_GetScreenRelativePosition, ported from its ☀️ twin with the map
 * work from its heap slot. Score 60: the listing loads the 0xffff0000 mask
 * right after the map work, before adding the origin offset; this C loads
 * it after.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

struct Thing {
    u8 filler0[8];
    s32 field8;
    u8 filler12[4];
    s32 field16;
};

/* Where an object stands on screen, relative to the camera's origin in the
   map work; -1 and zeroes when it is off screen. */
s32 Map_GetScreenRelativePosition(struct Thing *obj, s32 *out)
{
    u8 *state = (u8 *)Ram_HeapSlots->map_work;
    s32 *org = (s32 *)(state + 228);
    s32 a;
    s32 b;
    s32 x;
    s32 y;

    a = org[0] & 0xffff0000;
    b = org[1] & 0xffff0000;
    x = obj->field8 - a;
    y = obj->field16 - b;
    if ((u32)(x + 0x001fffff) <= 0x012ffffe && y > 0 && y < 0xe00000) {
        *out++ = x >> 16;
        *out = y >> 16;
        return 0;
    }
    *out++ = 0;
    *out = 0;
    return -1;
}
