/* resource_391 0x02000608-0x020008bf: staged actor move/redraw family. */
#include "types.h"
u8 *Func_0808a080(s32);
void Func_0808a090(s32,s32,s32); void Func_0808a100(s32,s32);
void Func_0808a010(s32); void Func_0808a0e0(s32,s32,s32);
void Func_0808a0e8(s32); void Func_08009080(u8 *,s32);
void Func_080f9010(s32); void Func_08009150(u8 *,s32,s32,s32);
void Func_08009158(u8 *); void Func_080091c0(s32,s32,s32,s32,s32,s32);
void Func_0808a5e8(void);
s32 Func_02000244(s32,s32,s32,s32,s32,s32);

void Func_02000608(s32 row, s32 slot, s32 x, s32 elevation, s32 y,
                   void (*callback)(void))
{
    u8 *runtime = *(u8 **)0x03001e70;
    u8 *lead = Func_0808a080(0);
    s32 direction = *(u16 *)(lead + 6) >> 12;
    u8 *object = Func_0808a080(slot);
    s32 *delta = (s32 *)0x0200adc0 + row * 4;
    s32 a0 = delta[0] < 0 ? -delta[0] : delta[0];
    s32 a1 = delta[1] < 0 ? -delta[1] : delta[1];
    s32 a2 = delta[2] < 0 ? -delta[2] : delta[2];
    s32 a3 = delta[3] < 0 ? -delta[3] : delta[3];
    s32 width = (a0 + a2) >> 4, depth = (a1 + a3) >> 4;
    s32 originalX = *(s32 *)(object + 8) + (delta[0] << 16);
    s32 originalZ = *(s32 *)(object + 16) + (delta[1] << 16);
    s32 oldTileX = originalX >> 20, oldTileZ = originalZ >> 20;
    s32 moveX, moveZ, packed, tileX, tileZ, globalX, globalZ;

    *(s32 *)(object + 48) = 0x8000;
    *(s32 *)(object + 52) = 0x1999;
    Func_02000244(0, oldTileX, oldTileZ, width, depth, 0);
    Func_0808a090(0, 0x8000, 0x1999);
    Func_0808a100(0, 8);
    Func_0808a010(15);
    moveX = x - originalX; if (moveX < 0) moveX += 0x1ffff;
    moveZ = y - originalZ; if (moveZ < 0) moveZ += 0x1ffff;
    Func_0808a0e0(0, moveX >> 17, moveZ >> 17);
    lead = Func_0808a080(0);
    *(u32 *)(lead + 108) = 0x020082a9;
    Func_0808a010(4);
    if ((u32)(direction - 6) <= 7) Func_08009080(object, 3);
    else Func_08009080(object, 2);
    Func_080f9010(0xef);
    Func_08009150(object, x, elevation, y);
    Func_0808a0e8(0);
    Func_0808a100(0, 2);
    Func_0808a090(0, 0x4ccc, 0x1999);
    packed = ((s32 *)0x0200ad68)[direction];
    Func_0808a0e0(0, (s16)(packed >> 16) / 2, (s16)packed / 2);
    if (callback) callback();
    Func_0808a0e8(0);
    Func_0808a100(0, 1);
    lead = Func_0808a080(0);
    *(u32 *)(lead + 108) = 0;
    Func_08009158(object);
    Func_080f9010(0x120);
    Func_080f9010(0xd5);
    *(s32 *)(object + 8) = x; *(s32 *)(object + 16) = y;
    *(s32 *)(object + 36) = 0; *(s32 *)(object + 44) = 0;
    Func_08009080(object, 1);
    tileX = (x + (delta[0] << 16)) >> 20;
    tileZ = (y + (delta[1] << 16)) >> 20;
    globalX = (*(s32 *)(runtime + 316) >> 20) + tileX;
    globalZ = (*(s32 *)(runtime + 320) >> 20) + tileZ;
    Func_080091c0(tileX, tileZ, width, depth, globalX, globalZ);
    Func_02000244(0, tileX, tileZ, width, depth, 255);
    Func_02000244(2, tileX, tileZ, width, depth, 255);
    oldTileX = originalX >> 20; oldTileZ = originalZ >> 20;
    Func_080091c0((*(s32 *)(runtime + 316) >> 20) + oldTileX,
                  (*(s32 *)(runtime + 320) >> 20) + oldTileZ,
                  width, depth, oldTileX, oldTileZ);
    Func_02000244(2, oldTileX, oldTileZ, width, depth, 0);
    Func_0808a5e8();
}
