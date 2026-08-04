/*
 * resource_3b4 @ 0x020008c0, complete 284-byte owner through its three-word
 * pool.  The reviewed resource_373/resource_3bf structure is used with this
 * overlay's independently resolved id and geometry tables at 0x0200a684 and
 * 0x0200a69c.
 */
#include "types.h"

u8 *Func_0808a080(s32 slot);
void Func_080091c0(s32 x, s32 y, s32 width, s32 height,
                   s32 localX, s32 localY);
s32 Func_02000244(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);

s32 Func_020008c0(s32 slot)
{
    u8 *world = *(u8 **)0x03001e70;
    u8 *actor = Func_0808a080(slot);
    s16 kind = **(s16 **)(*(u8 **)(actor + 80) + 40);
    s32 *ids = (s32 *)0x0200a684;
    s32 index;
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

    delta = (s32 *)0x0200a69c + index * 4;
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

    Func_080091c0(baseX + tileX, baseY + tileY, width, height, tileX, tileY);
    Func_02000244(0, tileX, tileY, width, height, 255);
    Func_02000244(2, tileX, tileY, width, height, 255);
    return 1;
}
