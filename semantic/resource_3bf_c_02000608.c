/*
 * resource_3bf @ 0x02000608 (696 bytes through the pool at 0x020008bc).
 *
 * Six-argument staged actor movement/effect owner.  It derives a footprint
 * from the selected four-word displacement row, temporarily moves actor and
 * camera state, redraws the old and new occupied rectangles, and restores the
 * normal callbacks.  The control flow and all 31 calls match the independently
 * reviewed resource_373 owner, while this overlay's displacement/orientation
 * tables were resolved independently as 0x0200df30 and 0x0200ded8.  The sixth
 * argument is invoked through this overlay's generic call-through veneer.
 */
#include "types.h"

u8 *Func_0808a080(s32 slot);
void Func_0808a090(s32 id, s32 value, s32 callback);
void Func_0808a100(s32 id, s32 mode);
void Func_0808a010(s32 frames);
void Func_0808a0e0(s32 id, s32 x, s32 y);
void Func_0808a0e8(s32 id);
void Func_08009080(u8 *actor, s32 mode);
void Func_080f9010(s32 sound);
void Func_08009150(u8 *actor, s32 x, s32 elevation, s32 y);
void Func_08009158(u8 *actor);
void Func_080091c0(s32 x, s32 y, s32 width, s32 height,
                   s32 localX, s32 localY);
void Func_0808a5e8(void);
s32 Func_02000244(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);

void Func_02000608(s32 row, s32 actorSlot, s32 targetX, s32 elevation,
                   s32 targetY, void (*callback)(void))
{
    u8 *world = *(u8 **)0x03001e70;
    u8 *lead = Func_0808a080(0);
    s32 direction = *(u16 *)(lead + 6) >> 12;
    u8 *actor = Func_0808a080(actorSlot);
    s32 *delta = (s32 *)0x0200df30 + row * 4;
    s32 dx0 = delta[0] < 0 ? -delta[0] : delta[0];
    s32 dy0 = delta[1] < 0 ? -delta[1] : delta[1];
    s32 dx1 = delta[2] < 0 ? -delta[2] : delta[2];
    s32 dy1 = delta[3] < 0 ? -delta[3] : delta[3];
    s32 width = (dx0 + dx1) >> 4;
    s32 height = (dy0 + dy1) >> 4;
    s32 oldX = *(s32 *)(actor + 8) + (delta[0] << 16);
    s32 oldY = *(s32 *)(actor + 16) + (delta[1] << 16);
    s32 oldTileX = oldX >> 20;
    s32 oldTileY = oldY >> 20;
    s32 moveX;
    s32 moveY;
    s32 packed;
    s32 targetTileX;
    s32 targetTileY;
    s32 baseX;
    s32 baseY;

    *(s32 *)(actor + 48) = 0x8000;
    *(s32 *)(actor + 52) = 0x1999;
    Func_02000244(0, oldTileX, oldTileY, width, height, 0);
    Func_0808a090(0, 0x8000, 0x1999);
    Func_0808a100(0, 8);
    Func_0808a010(15);

    moveX = targetX - oldX;
    if (moveX < 0)
        moveX += 0x1ffff;
    moveX >>= 17;
    moveY = targetY - oldY;
    if (moveY < 0)
        moveY += 0x1ffff;
    moveY >>= 17;
    Func_0808a0e0(0, moveX, moveY);

    lead = Func_0808a080(0);
    *(u32 *)(lead + 108) = 0x020082a9;
    Func_0808a010(4);
    if ((u32)(direction - 6) <= 7)
        Func_08009080(actor, 3);
    else
        Func_08009080(actor, 2);
    Func_080f9010(0xef);
    Func_08009150(actor, targetX, elevation, targetY);
    Func_0808a0e8(0);
    Func_0808a100(0, 2);
    Func_0808a090(0, 0x4ccc, 0x1999);

    packed = ((s32 *)0x0200ded8)[direction];
    Func_0808a0e0(0, (s16)(packed >> 16) / 2, (s16)packed / 2);
    if (callback)
        callback();
    Func_0808a0e8(0);
    Func_0808a100(0, 1);

    lead = Func_0808a080(0);
    *(u32 *)(lead + 108) = 0;
    Func_08009158(actor);
    Func_080f9010(0x120);
    Func_080f9010(0xd5);
    *(s32 *)(actor + 8) = targetX;
    *(s32 *)(actor + 16) = targetY;
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    Func_08009080(actor, 1);

    targetTileX = (targetX + (delta[0] << 16)) >> 20;
    targetTileY = (targetY + (delta[1] << 16)) >> 20;
    baseX = *(s32 *)(world + 316) >> 20;
    baseY = *(s32 *)(world + 320) >> 20;
    Func_080091c0(targetTileX, targetTileY, width, height,
                  baseX + targetTileX, baseY + targetTileY);
    Func_02000244(0, targetTileX, targetTileY, width, height, 255);
    Func_02000244(2, targetTileX, targetTileY, width, height, 255);

    oldTileX = oldX >> 20;
    oldTileY = oldY >> 20;
    Func_080091c0(baseX + oldTileX, baseY + oldTileY, width, height,
                  oldTileX, oldTileY);
    Func_02000244(2, oldTileX, oldTileY, width, height, 0);
    Func_0808a5e8();
}
