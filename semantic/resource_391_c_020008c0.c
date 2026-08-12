/* resource_391 0x020008c0-0x020009db: kind-selected rectangle redraw. */
#include "types.h"

u8 *Func_0808a080(s32 slot);
void Func_080091c0(s32 x, s32 z, s32 width, s32 depth, s32 globalX, s32 globalZ);
s32 Func_02000244(s32 layer, s32 x, s32 z, s32 width, s32 depth, s32 value);

s32 Func_020008c0(s32 slot)
{
    u8 *runtime = *(u8 **)0x03001e70;
    u8 *object = Func_0808a080(slot);
    s16 kind = **(s16 **)(*(u8 **)(object + 80) + 40);
    s32 *keys = (s32 *)0x0200ada8;
    s32 *delta;
    s32 index;
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
    s32 width;
    s32 depth;
    s32 tileX;
    s32 tileZ;
    s32 globalX;
    s32 globalZ;

    for (index = 0; index < 6; index++) if (kind == keys[index]) break;
    if (index == 6) return 0;

    delta = (s32 *)0x0200adc0 + index * 4;
    z0 = delta[1] < 0 ? -delta[1] : delta[1];
    z1 = delta[3] < 0 ? -delta[3] : delta[3];
    depth = (z0 + z1) >> 4;
    x0 = delta[0] < 0 ? -delta[0] : delta[0];
    x1 = delta[2] < 0 ? -delta[2] : delta[2];
    tileX = (*(s32 *)(object + 8) + (delta[0] << 16)) >> 20;
    tileZ = (*(s32 *)(object + 16) + (delta[1] << 16)) >> 20;
    width = (x0 + x1) >> 4;
    globalX = (*(s32 *)(runtime + 316) >> 20) + tileX;
    globalZ = (*(s32 *)(runtime + 320) >> 20) + tileZ;

    Func_080091c0(tileX, tileZ, width, depth, globalX, globalZ);
    Func_02000244(0, tileX, tileZ, width, depth, 255);
    Func_02000244(2, tileX, tileZ, width, depth, 255);
    return 1;
}
