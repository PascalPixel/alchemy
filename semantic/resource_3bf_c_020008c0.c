/*
 * resource_3bf @ 0x020008c0 (284 bytes through its three-word pool).
 *
 * Maps the actor's nested kind through a six-entry id table, derives a
 * rectangle from the corresponding displacement row, redraws it on layers
 * zero and two, and returns whether a mapping was found.  This is the reviewed
 * resource_373 control flow with resource_3bf's independently resolved tables
 * at 0x0200df18 and 0x0200df30.
 */
#include "types.h"





extern u8 * Func_02005f22(s32 slot);
extern void Func_02005f70(s32 x, s32 y, s32 width, s32 height,
                   s32 localX, s32 localY);
extern s32 Func_02000bf0(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);
extern s32 Func_02000c02(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);
s32 Func_020008c0(s32 slot)
{
    u8 *world = *(u8 **)0x03001e70;
    u8 *actor = Func_02005f22(slot);
    s16 kind = **(s16 **)(*(u8 **)(actor + 80) + 40);
    s32 *ids = (s32 *)0x0200df18;
    u32 index;
    s32 *delta;
    s32 dx0;
    s32 dy0;
    s32 dx1;
    s32 dy1;
    s32 width;
    s32 height;
    s32 tileX;
    s32 tileY;
    s32 baseX;
    s32 baseY;

    for (index = 0; index < 6; index++) {
        if (kind == ids[index])
            break;
    }
    if (index == 6)
        return 0;

    delta = (s32 *)0x0200df30 + index * 4;
    dx0 = delta[0] < 0 ? -delta[0] : delta[0];
    dy0 = delta[1] < 0 ? -delta[1] : delta[1];
    dx1 = delta[2] < 0 ? -delta[2] : delta[2];
    dy1 = delta[3] < 0 ? -delta[3] : delta[3];
    width = (dx0 + dx1) >> 4;
    height = (dy0 + dy1) >> 4;
    tileX = (*(s32 *)(actor + 8) + (delta[0] << 16)) >> 20;
    tileY = (*(s32 *)(actor + 16) + (delta[1] << 16)) >> 20;
    baseX = *(s32 *)(world + 316) >> 20;
    baseY = *(s32 *)(world + 320) >> 20;

    Func_02005f70(baseX + tileX, baseY + tileY, width, height, tileX, tileY);
    Func_02000bf0(0, tileX, tileY, width, height, 255);
    Func_02000c02(2, tileX, tileY, width, height, 255);
    return 1;
}
