/* resource_393 0x020008c0-0x020009db: redraw one staged actor's footprint. */
#include "types.h"

u8 *Func_0808a080(s32 slot);
void Func_080091c0(s32 worldX, s32 worldZ, s32 width, s32 height,
                   s32 tileX, s32 tileZ);
s32 Func_02000244(s32 layer, s32 tileX, s32 tileZ, s32 width, s32 height,
                   s32 alpha);

s32 Func_020008c0(s32 slot)
{
    u8 *world = *(u8 **)0x03001e70;
    u8 *actor = Func_0808a080(slot);
    s16 kind = **(s16 **)(*(u8 **)(actor + 80) + 40);
    s32 *ids = (s32 *)0x02008f50;
    u32 index;
    s32 *delta;
    s32 dx0;
    s32 dz0;
    s32 dx1;
    s32 dz1;
    s32 width;
    s32 height;
    s32 tileX;
    s32 tileZ;
    s32 baseX;
    s32 baseZ;

    for (index = 0; index < 6; index++) {
        if (kind == ids[index])
            break;
    }
    if (index == 6)
        return 0;

    delta = (s32 *)0x02008f68 + index * 4;
    dz0 = delta[1] < 0 ? -delta[1] : delta[1];
    dz1 = delta[3] < 0 ? -delta[3] : delta[3];
    height = (dz0 + dz1) >> 4;
    dx0 = delta[0] < 0 ? -delta[0] : delta[0];
    dx1 = delta[2] < 0 ? -delta[2] : delta[2];
    tileX = (*(s32 *)(actor + 8) + (delta[0] << 16)) >> 20;
    width = (dx0 + dx1) >> 4;
    tileZ = (*(s32 *)(actor + 16) + (delta[1] << 16)) >> 20;
    baseX = *(s32 *)(world + 316) >> 20;
    baseZ = *(s32 *)(world + 320) >> 20;

    Func_080091c0(baseX + tileX, baseZ + tileZ, width, height, tileX, tileZ);
    Func_02000244(0, tileX, tileZ, width, height, 255);
    Func_02000244(2, tileX, tileZ, width, height, 255);
    return 1;
}
